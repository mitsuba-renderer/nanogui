# Simplified OpenGL/Metal rendering test using the Quad class

import sys
sys.path.append('python')
import nanogui as ng
from nanogui import glfw
import numpy as np
from PIL import Image
import os
import math


class MyScreen(ng.Screen):
    def __init__(self):
        ng.Screen.__init__(self,
            size=[512, 512],
            caption="Quad Renderer Test"
        )

        # Load texture
        base_dir = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
        image_fname = os.path.join(base_dir, "resources/icons/icon1.png")
        image = np.array(Image.open(image_fname))

        self.texture = ng.Texture(
            pixel_format=ng.Texture.PixelFormat.RGBA,
            component_format=ng.Texture.ComponentFormat.UInt8,
            size=image.shape[:2]
        )
        self.texture.upload(image)
        self.render_pass = ng.RenderPass(color_targets=[self])
        self.quad = ng.Quad(self.render_pass)
        self.quad.set_texture(self.texture)

    def draw_contents(self):
        with self.render_pass:
            # Create transformation matrices
            view = ng.Matrix4f.look_at(
                origin=[0, -2, -10],
                target=[0, 0, 0],
                up=[0, 1, 0]
            )

            model = ng.Matrix4f.rotate(
                [0, 1, 0],
                math.sin(glfw.getTime())
            )

            fbsize = self.framebuffer_size()
            proj = ng.Matrix4f.perspective(
                fov=25 * np.pi / 180,
                near=0.1,
                far=20,
                aspect=fbsize[0] / float(fbsize[1])
            )

            # Set MVP and render
            mvp = proj @ view @ model
            self.quad.set_mvp(mvp.T) # -> col-major
            self.quad.draw()

    def keyboard_event(self, key, scancode, action, modifiers):
        if super(MyScreen, self).keyboard_event(key, scancode,
                                                action, modifiers):
            return True
        if key == glfw.KEY_ESCAPE and action == glfw.PRESS:
            self.set_visible(False)
            return True
        return False

    def resize_event(self, size):
        self.render_pass.resize(self.framebuffer_size())
        super(MyScreen, self).resize_event(size)
        return True


def run():
    ng.init()
    s = MyScreen()
    s.set_visible(True)
    ng.mainloop(1 / 60.0 * 1000)
    ng.shutdown()

if __name__ == '__main__':
    run()
