# OpenGL/Metal rendering test: render a green triangle on screen

import sys
sys.path.append('python')
import nanogui
from nanogui import Shader, Texture, RenderPass, Screen
import numpy as np
from PIL import Image


class MyScreen(Screen):
    def __init__(self):
        Screen.__init__(self, [512, 512], "Unnamed")

        if nanogui.api == 'opengl':
            vertex_program = '''
                #version 330
                in vec3 position;

                void main() {
                    gl_Position = vec4(position, 1);
                }
            '''

            fragment_program = '''
                #version 330
                uniform vec4 color;
                out vec4 fragColor;

                void main() {
                    fragColor = color;
                }
            '''
        elif nanogui.api == 'metal':
            vertex_program = '''
                using namespace metal;

                struct VertexOut {
                    float4 position [[position]];
                };

                vertex VertexOut vertex_main(const device packed_float3 *position,
                                             uint id [[vertex_id]]) {
                    VertexOut vert;
                    vert.position = float4(position[id], 1.f);
                    return vert;
                }
            '''

            fragment_program = '''
                using namespace metal;

                struct VertexOut {
                    float4 position [[position]];
                };

                fragment float4 fragment_main(VertexOut vert [[stage_in]],
                                              const constant float4 &color) {
                    return color;
                }
            '''

        self.render_pass = RenderPass([self])
        self.render_pass.set_viewport(
            [10, 10], self.framebuffer_size() - [10, 20]
        )

        self.shader = Shader(
            self.render_pass,
            "test_shader",
            vertex_program,
            fragment_program
        )

        p = np.array([[ 0.0,  0.5, 0],
                      [-0.5, -0.5, 0],
                      [ 0.5, -0.5, 0]], dtype=np.float32)

        self.shader.set_buffer("position", p)
        self.shader.set_buffer("color", np.array([0, 1, 0, 1], dtype=np.float32))
        self.shader.set_buffer("indices", np.array([0, 1, 2], dtype=np.uint32))

    def draw_contents(self):
        print('draw_contents()')
        with self.render_pass:
            with self.shader:
                if False:
                    self.shader.draw_array(Shader.PrimitiveType.Triangle, 0, 3)
                else:
                    self.shader.draw_array(Shader.PrimitiveType.Triangle, 0, 3, indexed=True)

nanogui.init()
s = MyScreen()
s.set_visible(True)
nanogui.mainloop()
nanogui.shutdown()
