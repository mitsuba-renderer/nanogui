# python/example4.py -- Python version of an example application that
# shows how to use the Canvas widget. For a C++ implementation, see
# '../src/example4.cpp'.
#
# NanoGUI was developed by Wenzel Jakob <wenzel@inf.ethz.ch>.
# The widget drawing code is based on the NanoVG demo application
# by Mikko Mononen.
#
# All rights reserved. Use of this source code is governed by a
# BSD-style license that can be found in the LICENSE.txt file.

import nanogui
import random
import math
import gc

from nanogui import Canvas, Shader, RenderPass, Screen, Window, \
    GroupLayout, Color, Widget, BoxLayout, Orientation, Alignment, \
    Button

from nanogui import glfw

class MyCanvas(Canvas):
    def __init__(self, parent):
        super(MyCanvas, self).__init__(parent)

        try:
            import numpy as np

            if nanogui.api == 'opengl':
                vertex_shader = """
                    #version 330
                    uniform mat4 mvp;
                    in vec3 position;
                    in vec3 color;
                    out vec4 frag_color;
                    void main() {
                        frag_color = vec4(color, 1.0);
                        gl_Position = mvp * vec4(position, 1.0);
                    }
                    """

                fragment_shader = """
                    #version 330
                    out vec4 color;
                    in vec4 frag_color;
                    void main() {
                        color = frag_color;
                    }
                    """
            elif nanogui.api == 'gles2' or nanogui.api == 'gles3':
                vertex_shader = """
                    precision highp float;
                    uniform mat4 mvp;
                    attribute vec3 position;
                    attribute vec3 color;
                    varying vec4 frag_color;
                    void main() {
                        frag_color = vec4(color, 1.0);
                        gl_Position = mvp * vec4(position, 1.0);
                    }
                    """

                fragment_shader = """
                    precision highp float;
                    varying vec4 frag_color;
                    void main() {
                        gl_FragColor = frag_color;
                    }
                    """
            elif nanogui.api == 'metal':
                vertex_shader = """
                    using namespace metal;
                    struct VertexOut {
                        float4 position [[position]];
                        float4 color;
                    };

                    vertex VertexOut vertex_main(const device packed_float3 *position,
                                                 const device packed_float3 *color,
                                                 constant float4x4 &mvp,
                                                 uint id [[vertex_id]]) {
                        VertexOut vert;
                        vert.position = mvp * float4(position[id], 1.f);
                        vert.color = float4(color[id], 1.f);
                        return vert;
                    }
                    """

                fragment_shader = """
                    using namespace metal;

                    struct VertexOut {
                        float4 position [[position]];
                        float4 color;
                    };

                    fragment float4 fragment_main(VertexOut vert [[stage_in]]) {
                        return vert.color;
                    }
                    """
            else:
                raise Exception("Unknown graphics API!")

            self.shader = Shader(
                self.render_pass(),
                # An identifying name
                "a_simple_shader",
                vertex_shader,
                fragment_shader
            )

            # Draw a cube
            indices = np.array(
                [3, 2, 6, 6, 7, 3,
                 4, 5, 1, 1, 0, 4,
                 4, 0, 3, 3, 7, 4,
                 1, 5, 6, 6, 2, 1,
                 0, 1, 2, 2, 3, 0,
                 7, 6, 5, 5, 4, 7],
                dtype=np.uint32)

            positions = np.array(
                [[-1, 1, 1], [-1, -1, 1],
                 [1, -1, 1], [1, 1, 1],
                 [-1, 1, -1], [-1, -1, -1],
                 [1, -1, -1], [1, 1, -1]],
                dtype=np.float32)

            colors = np.array(
                [[0, 1, 1], [0, 0, 1],
                 [1, 0, 1], [1, 1, 1],
                 [0, 1, 0], [0, 0, 0],
                 [1, 0, 0], [1, 1, 0]],
                dtype=np.float32)

            self.shader.set_buffer("indices", indices)
            self.shader.set_buffer("position", positions)
            self.shader.set_buffer("color", colors)
            self.rotation = 0
        except ImportError:
            self.shader = None
            pass

    def draw_contents(self):
        if self.shader is None:
            return
        import numpy as np

        view = nanogui.look_at(
            origin=[0, -2, -10],
            target=[0, 0, 0],
            up=[0, 1, 0]
        )

        model = nanogui.rotate(
            [0, 1, 0],
            glfw.getTime()
        )

        model2 = nanogui.rotate(
            [1, 0, 0],
            self.rotation
        )

        size = self.size()
        proj = nanogui.perspective(
            fov=25 * np.pi / 180,
            near=0.1,
            far=20,
            aspect=size[0] / float(size[1])
        )

        mvp = proj @ view @ model @ model2

        self.shader.set_buffer("mvp", np.float32(mvp.T))
        with self.shader:
            self.shader.draw_array(Shader.PrimitiveType.Triangle,
                                   0, 36, indexed=True)


class TestApp(Screen):
    def __init__(self):
        super(TestApp, self).__init__((800, 600), "NanoGUI Test", False)

        window = Window(self, "Canvas widget demo")
        window.set_position((15, 15))
        window.set_layout(GroupLayout())

        self.canvas = MyCanvas(window)
        self.canvas.set_background_color(Color(0.5, 0.5, 0.5, 1.0))
        self.canvas.set_size((400, 400))

        tools = Widget(window)
        tools.set_layout(BoxLayout(Orientation.Horizontal,
                                  Alignment.Middle, 0, 5))

        b0 = Button(tools, "Random Color")
        def cb0():
            self.canvas.set_background_color(Color(random.random(), random.random(), random.random(), 1.0))
        b0.set_callback(cb0)

        b1 = Button(tools, "Random Rotation")
        def cb1():
            self.canvas.rotation = random.random() * math.pi
        b1.set_callback(cb1)

        self.perform_layout()

    def keyboard_event(self, key, scancode, action, modifiers):
        if super(TestApp, self).keyboard_event(key, scancode,
                                              action, modifiers):
            return True
        if key == glfw.KEY_ESCAPE and action == glfw.PRESS:
            self.set_visible(False)
            return True
        return False

if __name__ == "__main__":
    nanogui.init()
    test = TestApp()
    test.draw_all()
    test.set_visible(True)
    nanogui.mainloop(refresh=1 / 60.0 * 1000)
    del test
    gc.collect()
    nanogui.shutdown()
