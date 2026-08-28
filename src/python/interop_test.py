"""Tiny NumPy / NanoGUI demo using UploadSink and CameraController."""

import time

import numpy as np

import nanogui as ng
from nanogui.interop import FrameStream, UploadSink


SIZE = ng.Vector2i(640, 480)
BG = (0.2, 0.2, 0.2, 1.0)
FOV = 0.7  # vertical field of view in radians

V = np.array([
    [-1, -1, -1], [1, -1, -1], [1, 1, -1], [-1, 1, -1],
    [-1, -1,  1], [1, -1,  1], [1, 1,  1], [-1, 1,  1],
], dtype=np.float32)

E = ((0, 1), (1, 2), (2, 3), (3, 0),
     (4, 5), (5, 6), (6, 7), (7, 4),
     (0, 4), (1, 5), (2, 6), (3, 7))


def line(img, a, b):
    n = max(abs(b[0] - a[0]), abs(b[1] - a[1])) + 1
    x = np.linspace(a[0], b[0], n).astype(np.int32)
    y = np.linspace(a[1], b[1], n).astype(np.int32)
    ok = (0 <= x) & (x < img.shape[1]) & (0 <= y) & (y < img.shape[0])
    img[y[ok], x[ok], :3] = (0.9, 0.95, 1.0)


def cube(size, cam):
    """Draw the wireframe cube seen from the camera state ``cam``"""
    w, h = int(size[0]), int(size[1])
    img = np.empty((h, w, 4), dtype=np.float32)
    img[:] = BG

    view = np.array(cam.view_matrix())
    p = V @ view[:3, :3].T + view[:3, 3]
    d = -p[:, 2]  # the camera looks along -z
    focal = 0.5 * h / np.tan(0.5 * FOV)
    limit = 4.0 * max(w, h)
    with np.errstate(divide="ignore", invalid="ignore"):
        xy = np.c_[w / 2 + focal * p[:, 0] / d,
                   h / 2 - focal * p[:, 1] / d]
    xy = np.clip(np.nan_to_num(xy), -limit, limit).astype(np.int32)

    for a, b in E:
        # Skip edges with an endpoint behind the camera
        if d[a] > 1e-2 and d[b] > 1e-2:
            line(img, xy[a], xy[b])
    return img


class Viewer(ng.Screen):
    def __init__(self):
        super().__init__(size=SIZE, caption="NumPy / NanoGUI interop")
        self.set_background(ng.Color(*BG))
        self.render_pass = ng.RenderPass(color_targets=[self])
        self.render_pass.set_clear_color(0, ng.Color(*BG))
        self.quad = ng.TexturedQuad(self.render_pass)

        self.rendered = 0
        self.dropped = 0
        self.last_update = time.perf_counter()

        window = ng.Window(self, "Interop")
        window.set_position(ng.Vector2i(15, 15))
        window.set_fixed_width(300)
        window.set_layout(ng.GroupLayout())
        self.render_label = ng.Label(window, "Render: -")
        self.display_label = ng.Label(window, "Display: -")
        self.render_label.set_fixed_width(270)
        self.display_label.set_fixed_width(270)

        self.controller = ng.CameraController(
            ng.CameraState(ng.Vector3f(3, 2, 4), ng.Vector3f(0),
                           ng.Vector3f(0, 1, 0)),
            ng.Vector3f(0, 1, 0))
        self.controller.scene_scale = 3.5  # cube diagonal

        self.stream = FrameStream(UploadSink(), SIZE,
                                  state=self.controller.state())
        self.controller.set_callback(self.stream.set_state)
        self.stream.start(self.producer)
        self.perform_layout()

    def producer(self):
        while self.stream.active:
            if self.stream.wait_if_reconfiguring():
                continue
            cam, _ = self.stream.state()
            frame = cube(self.stream.size, cam)
            self.rendered += 1
            if not self.stream.submit(frame, cam):
                self.dropped += 1

    def draw_contents(self):
        self.controller.update()

        now = time.perf_counter()
        if now - self.last_update > 0.25:
            pr, cr = self.stream.producer_rate, self.stream.consumer_rate
            self.render_label.set_caption(
                f"Render: {pr.interval() * 1e3:.1f} ms ({pr.rate():.1f} FPS) "
                f"[Host: {pr.busy() * 1e3:.1f} ms]")
            self.display_label.set_caption(
                f"Display: {cr.interval() * 1e3:.1f} ms ({cr.rate():.1f} FPS)")
            self.last_update = now

        with self.stream.present(self.render_pass) as (tex, _):
            if tex is not None:
                self.quad.set_texture(tex)
                self.quad.draw()
        self.redraw()

    def resize_event(self, size):
        self.render_pass.resize(self.framebuffer_size())
        super().resize_event(size)
        self.stream.resize(size)
        return True

    def mouse_button_event(self, p, button, down, modifiers):
        if super().mouse_button_event(p, button, down, modifiers):
            return True
        return self.controller.mouse_button_event(p, button, down, modifiers)

    def mouse_motion_event_f(self, p, rel, button, modifiers):
        if super().mouse_motion_event_f(p, rel, button, modifiers):
            return True
        return self.controller.mouse_motion_event(p, rel, button, modifiers)

    def scroll_event(self, p, rel, flags):
        return (super().scroll_event(p, rel, flags) or
                self.controller.scroll_event(p, rel, flags))

    def keyboard_event(self, key, scancode, action, modifiers):
        if super().keyboard_event(key, scancode, action, modifiers):
            return True
        return self.controller.keyboard_event(key, scancode, action, modifiers)

    def focus_event(self, focused):
        self.controller.focus_event(focused)
        return super().focus_event(focused)


def run():
    ng.init()
    viewer = Viewer()
    viewer.set_visible(True)
    try:
        ng.run()
    finally:
        viewer.stream.close()
    if viewer.rendered:
        print(f"rendered {viewer.rendered} frames, dropped {viewer.dropped} "
              f"({100.0 * viewer.dropped / viewer.rendered:.1f}%)")
    ng.shutdown()


if __name__ == "__main__":
    run()
