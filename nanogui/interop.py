"""
Infrastructure for real-time rendering and display.

Interactive renderers are ideally implemented as two independent loops:

1. a producer thread that computes images as fast as possible.

2. a consumer thread that displays these images in a user interface.

These two loops may run at very different rates. The following properties are
desirable:

- The producer should never slow down the user interface: UI elements must
  always remain snappy. The view should reuse the previously shown frame
  without blocking when a new one is not available (e.g., when the producer
  is slower than the consumer).

- The user interface should never slow down the producer. When the producer
  generates updates faster than the UI can draw them (e.g., > 60 FPS), the
  producer should be able to render without delay, e.g., to accumulate
  Monte Carlo samples and reduce noise.

- Producers on compute accelerators (CUDA, Metal, etc.) process work on an
  asynchronous queue. The system should limit how far the producer can run
  ahead of the CPU to avoid high latency when responding to UI events (e.g., a
  camera view change following a mouse event).

- Data handoff should use efficient non-blocking synchronization (fences) to
  avoid hazards. In particular, the consumer must not read from a frame before
  the producer's write has finished (RAW), and the producer must not overwrite
  a frame still being used by the consumer (WAR).

- It should be possible to safely drain the pool of frames and replace them
  with a different display resolution, e.g., when the user has resized the
  window.

This module provides reusable infrastructure to connect a producer and consumer
while addressing these goals. It consists of the following classes:

1. ``FrameStream``: coordinates a fixed number of slots representing reusable
    storage exchanged between producer and consumer. It abstracts over details
    such as compute frameworks and storage representations.

2. ``FrameSink``: abstracts over the low-level details of storage and handoff
   of frames, completion events, and frame pacing.

The module provides two ``FrameSink`` implementations:

* ``UploadSink`` is a compatibility option for producers that
  provide frames as CPU-accessible, C-contiguous arrays (e.g. NumPy).

* ``ZeroCopySink`` is a more efficient option for producers that can write
  directly into textures created by NanoGUI.

Below is a minimal ``UploadSink`` viewer that streams procedurally generated
NumPy frames of a moving circle:

```python
import threading, time
import numpy as np
import nanogui as ng
from nanogui.interop import FrameStream, UploadSink

def render(size, t):
    w, h = int(size[0]), int(size[1])
    y, x = np.mgrid[0:h, 0:w]
    s = min(w, h)
    cx = w * 0.5 + 0.3 * s * np.cos(t)
    cy = h * 0.5 + 0.3 * s * np.sin(t)
    img = np.zeros((h, w, 4), np.float16)
    img[..., 3] = 1.0
    img[(x - cx) ** 2 + (y - cy) ** 2 < (0.1 * s) ** 2, :3] = (0.4, 0.7, 1.0)
    return img

class Viewer(ng.Screen):
    def __init__(self, size):
        super().__init__(size=size)
        self.render_pass = ng.RenderPass(color_targets=[self])
        self.quad = ng.TexturedQuad(self.render_pass)
        self.stream = FrameStream(UploadSink(), size)
        self.worker = threading.Thread(target=self.producer)
        self.worker.start()

    def producer(self):
        while self.stream.active:
            if self.stream.wait_if_reconfiguring():
                continue
            frame = render(self.stream.size, time.perf_counter())
            self.stream.submit(frame)

    def draw_contents(self):
        with self.stream.present(self.render_pass) as texture:
            if texture is not None:
                self.quad.set_texture(texture)
                self.quad.draw()
        self.redraw()

if __name__ == "__main__":
    ng.init()
    viewer = Viewer(size=ng.Vector2i(640, 480))
    viewer.set_visible(True)
    ng.run()
    viewer.stream.active = False
    viewer.worker.join()
    del viewer
    ng.shutdown()
```
"""

from __future__ import annotations

import collections
import contextlib
import threading
from time import perf_counter
from collections.abc import Callable, Iterator
from typing import Any

import nanogui as ng

__all__ = [
    "FrameStream",
    "FrameSink",
    "ZeroCopySink",
    "UploadSink",
]


class FrameStream:
    """
    ``FrameStream`` coordinates the handoff of frames between a producer and
    consumer.

    All backend-specific details are handled by a provided ``FrameSink``,
    which exposes a fixed number of frame slots. The ``FrameStream`` refers
    to them by index and decides when a frame can move between one of the
    following states:

    * ``free``: available to the producer
    * ``in flight``: the producer has started writing to the frame.
    * ``ready``: completed and waiting for the next ``present()``
    * ``displayed``: the texture most recently handed to NanoGUI

    In the following, *frame object* refers to the producer's output (e.g., a
    NumPy array) that will later be written to a *texture*.

    Regular usage involves the following steps:

    1. An asynchronous producer calls ``pace()``, which blocks until at most
       ``max_in_flight - 1`` of its earlier renders are still outstanding, and
       then renders a frame. Synchronous producers skip this step.
    2. The producer calls ``submit(frame)`` with the finished frame object.
    3. If a slot is free the stream reserves it. Otherwise it drops the frame.
    4. The sink writes the frame object into that slot's texture. The write may
       be asynchronous, so the slot first enters the ``in flight`` state.
    5. ``present()`` (consumer side) polls the in-flight writes. The newest
       complete slot is marked ``ready``, superseding prior ones.
    6. If a ready slot exists, ``present()`` promotes it to ``displayed`` and
       yields its NanoGUI texture inside the render pass. Otherwise it reuses
       the ``displayed`` slot, and step 7 does not apply.
    7. The slot displaced by step 6 returns to the free pool once the
       consumer's reads of it have retired.

    The stream also carries an application-defined ``state`` object plus a
    monotonically increasing generation counter, which together form a one-way
    channel from the UI to the producer, e.g. for camera/view parameters.
    In this case, an input handler on the UI thread calls ``set_state()``,
    which bumps the generation, and the producer reads ``state()`` to pick
    up the change. The object is passed by reference and should not be
    modified following ``set_state()``.

    Performance statistics are available in the ``producer_rate`` and
    ``consumer_rate`` meters. On each side, ``interval()`` and ``rate()``
    report the smoothed frame time and rate. ``producer_rate.busy()`` further
    reports the host time spent producing a frame: the span from the end of
    ``pace()`` (or of the previous ``submit()``) to the end of ``submit()``,
    which excludes pacing and reconfiguration waits and, for asynchronous
    producers, the device-side rendering they overlap.

    The following functions may only be called from a specific context:

    * Consumer: ``present()``, ``resize()``, ``set_state()``.
    * Producer: ``state()``, ``pace()``, ``submit()``,
      ``wait_if_reconfiguring()``.
    """

    _sink: FrameSink                  # storage + handoff adapter
    _n: int                           # total number of frame slots
    _max_in_flight: int               # cap on outstanding producer renders
    size: ng.Vector2i                 # current frame size, in pixels
    active: bool                      # producer loop flag; clear to stop
    _state: tuple[Any, int]           # (app state, generation), swapped whole
    producer_rate: ng.RateMeter       # producer frame stats, see class docs
    consumer_rate: ng.RateMeter       # display frame stats, see class docs
    _produce_start: float             # start of the current produce step
    _free: list[int]                  # slots available to the producer
    _ready: int                       # newest completed slot (consumer), or -1
    _displayed: int                   # slot on screen (consumer), or -1
    _stale: ng.Texture | None         # last texture of the pre-resize storage
    _writing: collections.deque[int]  # slots with an in-flight write (FIFO)
    _pending: collections.deque       # (fence, slot) awaiting read retire
    _fence_pool: list[ng.Fence]       # recycled consumer fences
    _pace_events: list | None         # producer completion events (async only)
    _pace_index: int                  # submit() count; cursor into _pace_events
    _reconfiguring: bool              # set by resize() to park the producer
    _barrier: threading.Barrier       # park/resume rendezvous, see resize()

    def __init__(self, sink: FrameSink, size: ng.Vector2i, state: Any = None,
                 max_in_flight: int = 2) -> None:
        """Create a stream that hands frames from ``sink`` to NanoGUI.

        Args:
            sink: A ``FrameSink`` owning the per-slot storage and handoff.
            size: Initial frame size in pixels.
            state: Optional initial application state. See the class docstring
                and ``state()``; pass ``None`` if the producer needs no UI input.
            max_in_flight: Number of renders that ``pace()`` allows an
                asynchronous producer to have outstanding at once. The default
                of 2 overlaps device rendering with enqueuing the next frame.
        """
        self._sink = sink
        self._max_in_flight = max_in_flight

        # Number of slots to be allocated by the sink: ``max_in_flight`` plus
        # a ready slot, the displayed slot, and typically one guarded by a
        # consumer fence until its reads retire.
        self._n = max_in_flight + 3
        self.size = ng.Vector2i(size)
        sink.resize(self.size, self._n)

        self._ready = -1
        self._displayed = -1
        self._stale = None
        self._free = list(range(self._n))
        self._writing = collections.deque()

        self.active = True
        self._state = (state, 0)

        # Keep track of per-frame statistics on each end
        self.producer_rate = ng.RateMeter()
        self.consumer_rate = ng.RateMeter()
        self._produce_start = perf_counter()

        self._pending = collections.deque()
        self._fence_pool = []

        # Ring of completion events for pacing
        factory = sink.event_factory()
        self._pace_events = None if factory is None else \
            [factory() for _ in range(max_in_flight)]
        self._pace_index = 0

        self._reconfiguring = False
        self._barrier = threading.Barrier(2)

    def set_state(self, state: Any) -> None:
        """
        Replace the application state (e.g., the camera view) and increment the
        associated generation counter.
        """
        # Only the consumer thread writes _state, and the producer picks up the
        # packed tuple with a single atomic attribute read. No lock is needed.
        self._state = (state, self._state[1] + 1)

    def state(self) -> tuple[Any, int]:
        """Return ``(state, generation)``.

        Producers can compare the generation with a cached value to detect view
        or parameter changes.
        """
        return self._state

    def pace(self) -> None:
        """Block until at most ``max_in_flight - 1`` renders are outstanding.
        Producers should call this function before starting a new frame."""
        events = self._pace_events
        if events is not None and self._pace_index >= self._max_in_flight:
            events[self._pace_index % self._max_in_flight].wait()
        self._produce_start = perf_counter()

    def submit(self, frame: Any) -> bool:
        """Hand the latest finished frame to the consumer. ``frame`` is a
        finished frame object compatible with the selected sink. The function
        returns ``True`` when the frame was accepted for publication and
        ``False`` when no slot is available and the frame was dropped."""
        # Record a completion event tracking this render for pacing.
        events = self._pace_events
        if events is not None:
            events[self._pace_index % self._max_in_flight].record()
            self._pace_index += 1

        # Reserve a free slot, or drop the frame when none is available
        idx = self._free.pop() if self._free else -1
        if idx != -1:
            self._sink.write(idx, frame)
            self._writing.append(idx)

        # Producer stats: the busy time spans from the end of pace() (or of
        # the previous produce step) to this point, excluding waits.
        now = perf_counter()
        self.producer_rate.tick(now - self._produce_start)
        self._produce_start = now
        return idx != -1

    @contextlib.contextmanager
    def present(self, render_pass: ng.RenderPass) -> Iterator[ng.Texture | None]:
        """Yield the texture to be drawn in ``render_pass``.

        Use this from the NanoGUI draw callback:

            with stream.present(render_pass) as texture:
                if texture is not None:
                    quad.set_texture(texture)
                    quad.draw()

        The context manager yields ``None`` until the first frame is ready. Later,
        it always returns the last displayed texture until a newer one is ready.
        """
        sink = self._sink
        self.consumer_rate.tick()

        # Collect slots whose pending reads have retired
        freed = []
        pending = self._pending
        while pending and pending[0][0].completed():
            fence, idx = pending.popleft()
            self._fence_pool.append(fence)
            freed.append(idx)

        # Poll the in-flight writes, which complete in FIFO order. The newest
        # supersedes both earlier completions and an undisplayed ready slot.
        writing = self._writing
        while writing and sink.write_complete(writing[0]):
            if self._ready != -1:
                freed.append(self._ready)
            self._ready = writing.popleft()

        # Promote the ready slot (if any) to displayed and run one-time
        # preparation before its first draw. The displaced slot cannot be
        # reused yet and requires fencing.
        prepare, retired = self._ready, -1
        if prepare != -1:
            retired = self._displayed
            self._displayed = prepare
            self._ready = -1
            sink.prepare_present(prepare)
            self._stale = None
        self._free.extend(freed)

        # Before the first frame arrives, show the pre-resize texture if one
        # exists (kept in _stale) and otherwise yield None
        if self._displayed == -1:
            with render_pass:
                yield self._stale
            return

        tex = sink.texture(self._displayed)
        with render_pass:
            yield tex
            if retired != -1:
                # Guard the displaced slot with a fence. It rejoins the free
                # pool once the consumer's reads have retired (see above).
                fence = self._fence_pool.pop() if self._fence_pool else ng.Fence()
                render_pass.insert_fence(fence)
                self._pending.append((fence, retired))

    def resize(self, size: ng.Vector2i) -> None:
        """Resize the stream and all sink-owned frame storage.

        Call this on the NanoGUI/main thread, usually after the window or
        render pass has been resized. The producer must call
        ``wait_if_reconfiguring()`` regularly; ``resize()`` waits until the
        producer parks before destroying or replacing slot storage. It follows
        that this function must not be called once the producer thread has
        stopped, since the handshake would block forever.
        """
        size = ng.Vector2i(size)
        if size == self.size:
            return
        self._reconfiguring = True
        self._barrier.wait()

        # Drain the consumer's outstanding reads releasing the old slot storage.
        while self._pending:
            fence, _ = self._pending.popleft()
            fence.wait()
            self._fence_pool.append(fence)

        # Hold on to the frame currently on screen
        if self._displayed != -1:
            self._stale = self._sink.texture(self._displayed)

        self.size = size
        self._sink.resize(size, self._n)

        self._ready = -1
        self._displayed = -1
        self._free = list(range(self._n))

        # Restart rate trackers after the rebuild
        self.producer_rate.reset()
        self.consumer_rate.reset()
        self._reconfiguring = False
        self._barrier.wait()

    def wait_if_reconfiguring(self) -> bool:
        """This function parks the producer when the consumer wishes to change the
        target resolution. It returns ``False`` during normal operation."""
        if not self._reconfiguring:
            self._produce_start = perf_counter()
            return False

        # The consumer is blocked in resize() until the rendezvous below, so
        # present() cannot concurrently pop from _writing.
        self._writing.clear()

        # Drain device work
        if self._pace_events is not None:
            for event in self._pace_events[:self._pace_index]:
                event.wait()
        self._sink.sync_producer()

        # First rendezvous: parked. resize() then rebuilds the slot storage
        # and joins the barrier once more to release the producer.
        self._barrier.wait()
        self._barrier.wait()
        self._produce_start = perf_counter()
        return True


class FrameSink:
    """
    The FrameSink represents an abstract pool of textures and related
    synchronization operations. It is only used indirectly via the
    ``FrameStream`` class.
    """

    def resize(self, size: ng.Vector2i, n: int) -> None:
        """[Consumer] Release prior storage (if any) and allocate ``n`` slots
        of the given size."""
        raise NotImplementedError

    def write(self, idx: int, frame: Any) -> None:
        """[Producer] Begin writing the frame object ``frame`` to slot ``idx``.
        The write may be asynchronous; the consumer polls ``write_complete()``
        before using the slot."""
        raise NotImplementedError

    def write_complete(self, idx: int) -> bool:
        """[Consumer] Check if the write to slot ``idx`` has completed. The
        default implementation suits sinks whose writes finish immediately."""
        return True

    def sync_producer(self) -> None:
        """[Producer]: Drain outstanding work."""
        pass

    def event_factory(self) -> Callable[[], Any] | None:
        """[Consumer]: Return a zero-argument factory that creates GPU event
        objects with ``record()`` and ``wait()`` methods that ``FrameStream``
        requires for pacing. Returning ``None`` here disables pacing."""
        return None

    def prepare_present(self, idx: int) -> None:
        """[Consumer]: One-time preparation before slot ``idx`` is displayed."""
        pass

    def texture(self, idx: int) -> ng.Texture:
        """[Consumer] Return the NanoGUI texture to draw for slot ``idx``."""
        raise NotImplementedError


class ZeroCopySink(FrameSink):
    """Sink for producers that can write directly into NanoGUI textures.

    The implementation here is designed to interoperate with Dr.Jit
    (https://github.com/mitsuba-renderer/drjit) but could likely be generalized
    to other GPU frameworks.

    Each slot is an RGBA Float16 texture, exposed to the producer via
    ``texture_type.from_native_handle()``. The resulting wrappers must provide
    ``set_value_with_event(value, event)``; ``event_type`` instances must
    provide ``record()``, ``wait()`` and ``query()``, serving both as
    write-completion events and for render pacing. ``submit()`` must be handed
    exactly the value that ``set_value_with_event()`` accepts (e.g. a flat
    RGBA Float16 buffer).

    When producer and NanoGUI use different APIs (e.g. Dr.Jit CUDA writing
    into OpenGL textures), the wrappers additionally provide ``map()`` and
    ``unmap()``, and the textures stay mapped between ``resize()`` calls.
    Mapping around every write would be a cross-API synchronization point far
    more expensive than the copy itself. The RAW/WAR hazards of persistent
    mapping are covered by the write events and the consumer's fences.

    Pass ``sync_producer`` when the producer has asynchronous device work that
    must be drained before storage is rebuilt.
    """

    def __init__(self, texture_type: Any,
                 event_type: Callable[[], Any], *,
                 sync_producer: Callable[[], None] | None = None) -> None:
        self.Texture = texture_type
        self.Event = event_type
        self._sync_producer = sync_producer
        self.ptex: list[Any] = []

    def resize(self, size: ng.Vector2i, n: int) -> None:
        # Undo cross-API mappings before dropping the previous storage
        for t in self.ptex:
            if hasattr(t, "unmap"):
                t.unmap()

        flags = (ng.Texture.TextureFlags.ShaderRead |
                 ng.Texture.TextureFlags.ShaderWrite)
        self.textures = [ng.Texture(ng.Texture.PixelFormat.RGBA,
                                    ng.Texture.ComponentFormat.Float16,
                                    size, samples=1, flags=flags)
                         for _ in range(n)]
        self.ptex = [self.Texture.from_native_handle(t.native_handle())
                     for t in self.textures]
        for t in self.ptex:
            if hasattr(t, "map"):
                t.map()
        self.write_events = [self.Event() for _ in range(n)]

    def write(self, idx: int, frame: Any) -> None:
        self.ptex[idx].set_value_with_event(frame, self.write_events[idx])

    def write_complete(self, idx: int) -> bool:
        return self.write_events[idx].query()

    def sync_producer(self) -> None:
        if self._sync_producer is not None:
            self._sync_producer()

    def event_factory(self) -> Callable[[], Any]:
        return self.Event

    def texture(self, idx: int) -> ng.Texture:
        return self.textures[idx]


class UploadSink(FrameSink):
    """Sink for producers that render into CPU memory (e.g. NumPy)

    Each slot holds a reference to the submitted host array. Writes complete
    immediately without tracking by events. The consumer owns a single RGBA
    Float16 texture and starts an async upload before drawing. ``submit()``
    expects a C-contiguous CPU array of shape ``(height, width, 4)``, which the
    producer must not modify after ``submit()`` returns.
    """

    def resize(self, size: ng.Vector2i, n: int) -> None:
        self.tex = ng.Texture(ng.Texture.PixelFormat.RGBA,
                              ng.Texture.ComponentFormat.Float16,
                              flags=ng.Texture.TextureFlags.ShaderRead,
                              samples=1, size=size)
        self.host: list[Any] = [None] * n

    def write(self, idx: int, frame: Any) -> None:
        self.host[idx] = frame

    def prepare_present(self, idx: int) -> None:
        self.tex.upload_async(self.host[idx])

    def texture(self, idx: int) -> ng.Texture:
        return self.tex
