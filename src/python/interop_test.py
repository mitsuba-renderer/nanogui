"""Tiny NumPy / NanoGUI demo using UploadSink."""

import time

import numpy as np

import nanogui as ng
from nanogui.interop import FrameStream, UploadSink


SIZE = ng.Vector2i(640, 480)
BG = (0.2, 0.2, 0.2, 1.0)

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


def cube(size, t):
    w, h = int(size[0]), int(size[1])
    img = np.empty((h, w, 4), dtype=np.float32)
    img[:] = BG

    c, s = np.cos(t), np.sin(t)
    q, r = np.cos(0.7 * t), np.sin(0.7 * t)
    rot = np.array([[c, s * r, s * q],
                    [0, q, -r],
                    [-s, c * r, c * q]], dtype=np.float32)
    p = V @ rot.T
    z = p[:, 2] + 4
    scale = 0.8 * min(w, h)
    xy = np.c_[w / 2 + scale * p[:, 0] / z,
               h / 2 - scale * p[:, 1] / z].astype(np.int32)

    for a, b in E:
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

        self.stream = FrameStream(UploadSink(), SIZE)
        self.stream.start(self.producer)
        self.perform_layout()

    def producer(self):
        while self.stream.active:
            if self.stream.wait_if_reconfiguring():
                continue
            frame = cube(self.stream.size, time.perf_counter())
            self.rendered += 1
            if not self.stream.submit(frame):
                self.dropped += 1

    def draw_contents(self):
        now = time.perf_counter()
        if now - self.last_update > 0.25:
            pr, cr = self.stream.producer_rate, self.stream.consumer_rate
            self.render_label.set_caption(
                f"Render: {pr.interval() * 1e3:.1f} ms ({pr.rate():.1f} FPS) "
                f"[Host: {pr.busy() * 1e3:.1f} ms]")
            self.display_label.set_caption(
                f"Display: {cr.interval() * 1e3:.1f} ms ({cr.rate():.1f} FPS)")
            self.last_update = now

        with self.stream.present(self.render_pass) as tex:
            if tex is not None:
                self.quad.set_texture(tex)
                self.quad.draw()
        self.redraw()

    def resize_event(self, size):
        self.render_pass.resize(self.framebuffer_size())
        super().resize_event(size)
        self.stream.resize(size)
        return True


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
