# OpenGL/Metal rendering test: render a spinning cube with a perspective
# camera; the window is resizable

import sys
sys.path.append('python')
import nanogui
from nanogui import Shader, Texture, RenderPass, Screen, Matrix4f
from nanogui import glfw
import numpy as np


class MyScreen(Screen):
    def __init__(self):
        Screen.__init__(self,
            size=[512, 512],
            caption="Unnamed",
            depth_buffer=True
        )

        if nanogui.api == 'opengl':
            vertex_program = '''
                #version 330
                in vec3 position;
                in vec4 color;
                out vec4 color_frag;
                uniform mat4 mvp;

                void main() {
                    gl_Position = mvp * vec4(position, 1.0);
                    color_frag = color;
                }
            '''

            fragment_program = '''
                #version 330
                in vec4 color_frag;
                out vec4 fragColor;

                void main() {
                    fragColor = color_frag;
                }
            '''
        elif nanogui.api == 'metal':
            vertex_program = '''
                using namespace metal;

                struct VertexOut {
                    float4 position [[position]];
                    float4 color;
                };

                vertex VertexOut vertex_main(const device packed_float3 *position,
                                             const device float4 *color,
                                             constant float4x4 &mvp,
                                             uint id [[vertex_id]]) {
                    VertexOut vert;
                    vert.position = mvp * float4(position[id], 1.f);
                    vert.color = color[id];
                    return vert;
                }
            '''

            fragment_program = '''
                using namespace metal;

                struct VertexOut {
                    float4 position [[position]];
                    float4 color;
                };

                fragment float4 fragment_main(VertexOut vert [[stage_in]]) {
                    return vert.color;
                }
            '''

        self.render_pass = RenderPass(
            color_targets=[self],
            depth_target=self
        )

        self.shader = Shader(
            self.render_pass,
            "test_shader",
            vertex_program,
            fragment_program
        )

        p = np.array([
            [-1, 1, 1], [-1, -1, 1],
            [1, -1, 1], [1, 1, 1],
            [-1, 1, -1], [-1, -1, -1],
            [1, -1, -1], [1, 1, -1]],
            dtype=np.float32
        )

        color = np.array([
            [0, 1, 1, 1], [0, 0, 1, 1],
            [1, 0, 1, 1], [1, 1, 1, 1],
            [0, 1, 0, 1], [0, 0, 0, 1],
            [1, 0, 0, 1], [1, 1, 0, 1]],
            dtype=np.float32
        )

        indices = np.array([
            3, 2, 6, 6, 7, 3,
            4, 5, 1, 1, 0, 4,
            4, 0, 3, 3, 7, 4,
            1, 5, 6, 6, 2, 1,
            0, 1, 2, 2, 3, 0,
            7, 6, 5, 5, 4, 7],
            dtype=np.uint32
        )

        self.shader.set_buffer("position", p)
        self.shader.set_buffer("color", color)
        self.shader.set_buffer("indices", indices)

    def draw_contents(self):
        with self.render_pass:
            view = Matrix4f.look_at(
                origin=[0, -2, -10],
                target=[0, 0, 0],
                up=[0, 1, 0]
            )

            model = Matrix4f.rotate(
                [0, 1, 0],
                glfw.getTime()
            )

            fbsize = self.framebuffer_size()
            proj = Matrix4f.perspective(
                fov=25 * np.pi / 180,
                near=0.1,
                far=20,
                aspect=fbsize[0] / float(fbsize[1])
            )

            mvp = proj @ view @ model
            self.shader.set_buffer("mvp", np.float32(mvp).T)
            with self.shader:
                self.shader.draw_array(Shader.PrimitiveType.Triangle,
                                       0, 36, indexed=True)

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


nanogui.init()
s = MyScreen()
s.set_visible(True)
nanogui.mainloop(1 / 60.0 * 1000)
nanogui.shutdown()
