# OpenGL/Metal rendering test: render a textured spinning cube

import sys
sys.path.append('python')
import nanogui as ng
from nanogui import glfw
import numpy as np
from PIL import Image
import os


class MyScreen(ng.Screen):
    def __init__(self):
        ng.Screen.__init__(self,
            size=[512, 512],
            caption="Unnamed"
        )

        if ng.api == 'opengl':
            vertex_program = '''
                #version 330
                in vec3 position;
                in vec2 uv;
                out vec2 uv_frag;
                uniform mat4 mvp;

                void main() {
                    gl_Position = mvp * vec4(position, 1.0);
                    uv_frag = uv;
                }
            '''

            fragment_program = '''
                #version 330
                in vec2 uv_frag;
                out vec4 fragColor;
                uniform sampler2D albedo_texture;

                void main() {
                    fragColor = texture(albedo_texture, uv_frag);
                }
            '''
        elif ng.api == 'metal':
            vertex_program = '''
                using namespace metal;

                struct VertexOut {
                    float4 position [[position]];
                    float2 uv;
                };

                vertex VertexOut vertex_main(const device packed_float3 *position,
                                             const device float2 *uv,
                                             constant float4x4 &mvp,
                                             uint id [[vertex_id]]) {
                    VertexOut vert;
                    vert.position = mvp * float4(position[id], 1.f);
                    vert.uv = uv[id];
                    return vert;
                }
            '''

            fragment_program = '''
                using namespace metal;

                struct VertexOut {
                    float4 position [[position]];
                    float2 uv;
                };

                fragment float4 fragment_main(VertexOut vert [[stage_in]],
                             texture2d<float, access::sample> albedo_texture,
                             sampler albedo_sampler) {
                    return albedo_texture.sample(albedo_sampler, vert.uv);
                }
            '''

        base_dir = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
        image_fname = os.path.join(base_dir, "resources/icons/icon1.png")
        image = np.array(Image.open(image_fname))

        self.albedo_texture = ng.Texture(
            pixel_format=ng.Texture.PixelFormat.RGBA,
            component_format=ng.Texture.ComponentFormat.UInt8,
            size=image.shape[:2]
        )
        self.albedo_texture.upload(image)

        self.render_pass = ng.RenderPass(
            color_targets=[self]
        )

        self.shader = ng.Shader(
            self.render_pass,
            "test_shader",
            vertex_program,
            fragment_program
        )

        p = np.array([
            [-1, -1, 0], [1, -1, 0],
            [1, 1, 0], [-1, 1, 0]],
            dtype=np.float32
        )

        uv = np.array([
            [1, 1], [0, 1],
            [0, 0], [1, 0]],
            dtype=np.float32
        )

        indices = np.array([
            0, 2, 1, 3, 2, 0],
            dtype=np.uint32
        )

        self.shader.set_buffer("position", p)
        self.shader.set_buffer("uv", uv)
        self.shader.set_buffer("indices", indices)
        self.shader.set_texture("albedo_texture", self.albedo_texture)

    def draw_contents(self):
        with self.render_pass:
            view = ng.Matrix4f.look_at(
                origin=[0, -2, -10],
                target=[0, 0, 0],
                up=[0, 1, 0]
            )
            import math

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

            mvp = proj @ view @ model
            self.shader.set_buffer("mvp", mvp.T)
            with self.shader:
                self.shader.draw_array(ng.Shader.PrimitiveType.Triangle,
                                       0, 6, indexed=True)

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
    ng.run()
    ng.shutdown()

if __name__ == '__main__':
    run()
