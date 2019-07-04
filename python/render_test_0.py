# OpenGL/Metal rendering test: render a red triangle to a texture
# and save it to a PNG file

import sys
sys.path.append('python')
import nanogui
from nanogui import Shader, Texture, RenderPass, Screen
import numpy as np
from PIL import Image

nanogui.init()

if nanogui.api == 'opengl':
    s = Screen([16, 16], "Unnamed")

    vertex_program = '''
        #version 330
        in vec3 position;

        void main() {
            gl_Position = vec4(position, 1.0);
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

texture = Texture(
    pixel_format=Texture.PixelFormat.RGBA,
    component_format=Texture.ComponentFormat.UInt8,
    size=[512, 512],
    flags=Texture.TextureFlags.ShaderRead | Texture.TextureFlags.RenderTarget
)

render_pass = RenderPass([texture])
render_pass.set_viewport(
    [10, 10], [512 - 10, 512 - 20]
)

shader = Shader(
    render_pass,
    "test_shader",
    vertex_program,
    fragment_program
)

p = np.array([[ 0.0,  0.5, 0],
              [-0.5, -0.5, 0],
              [ 0.5, -0.5, 0]], dtype=np.float32)

shader.set_buffer("position", p)
shader.set_buffer("color", np.array([1, 0, 0, 1], dtype=np.float32))
shader.set_buffer("indices", np.array([0, 1, 2], dtype=np.uint32))

with render_pass:
    with shader:
        if False:
            shader.draw_array(Shader.PrimitiveType.Triangle, 0, 3)
        else:
            shader.draw_array(Shader.PrimitiveType.Triangle, 0, 3, indexed=True)

result = texture.download()

Image.fromarray(result).save('test_0_%s.png' % nanogui.api)
