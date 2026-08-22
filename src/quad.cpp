/*
    NanoGUI was developed by Wenzel Jakob <wenzel.jakob@epfl.ch>.
    The widget drawing code is based on the NanoVG demo application
    by Mikko Mononen.

    All rights reserved. Use of this source code is governed by a
    BSD-style license that can be found in the LICENSE.txt file.
*/

#include <nanogui/quad.h>
#include <nanogui/opengl.h>

NAMESPACE_BEGIN(nanogui)

static const char *quad_vertex_shader =
#if defined(NANOGUI_USE_OPENGL)
    R"(
        #version 330
        in vec3 position;
        in vec2 uv;
        out vec2 uv_frag;
        uniform mat4 mvp;

        void main() {
            gl_Position = mvp * vec4(position, 1.0);
            uv_frag = uv;
        }
    )";
#elif defined(NANOGUI_USE_GLES)
    R"(
        #version 300 es
        precision highp float;
        in vec3 position;
        in vec2 uv;
        out vec2 uv_frag;
        uniform mat4 mvp;

        void main() {
            gl_Position = mvp * vec4(position, 1.0);
            uv_frag = uv;
        }
    )";
#elif defined(NANOGUI_USE_METAL)
    R"(
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
    )";
#endif

static const char *quad_fragment_shader =
#if defined(NANOGUI_USE_OPENGL)
    R"(
        #version 330
        in vec2 uv_frag;
        out vec4 fragColor;
        uniform sampler2D texture_sampler;
        uniform bool texture_linear;
        uniform float texture_exposure;

        vec3 linearToGamma22(vec3 linear) {
            return sign(linear) * pow(abs(linear), vec3(1.0/2.2));
        }

        void main() {
            vec4 color = texture(texture_sampler, uv_frag);
            color.rgb *= texture_exposure;

            if (texture_linear)
                color.rgb = linearToGamma22(color.rgb);

            fragColor = color;
        }
    )";
#elif defined(NANOGUI_USE_GLES)
    R"(
        #version 300 es
        precision highp float;
        in vec2 uv_frag;
        out vec4 fragColor;
        uniform sampler2D texture_sampler;
        uniform bool texture_linear;
        uniform float texture_exposure;

        vec3 linearToGamma22(vec3 linear) {
            return sign(linear) * pow(abs(linear), vec3(1.0/2.2));
        }

        void main() {
            vec4 color = texture(texture_sampler, uv_frag);
            color.rgb *= texture_exposure;

            if (texture_linear)
                color.rgb = linearToGamma22(color.rgb);

            fragColor = color;
        }
    )";
#elif defined(NANOGUI_USE_METAL)
    R"(
        using namespace metal;

        struct VertexOut {
            float4 position [[position]];
            float2 uv;
        };

        float3 linearToGamma22(float3 linear) {
            return sign(linear) * pow(abs(linear), float3(1.0/2.2));
        }

        fragment float4 fragment_main(VertexOut vert [[stage_in]],
                     texture2d<float, access::sample> texture_sampler,
                     sampler texture_sampler_sampler,
                     constant bool &texture_linear,
                     constant float &texture_exposure) {
            float4 color = texture_sampler.sample(texture_sampler_sampler, vert.uv);
            color.rgb *= texture_exposure;

            if (texture_linear)
                color.rgb = linearToGamma22(color.rgb);

            return color;
        }
    )";
#endif

TexturedQuad::TexturedQuad(RenderPass *render_pass, BlendMode blend_mode)
    : Shader(render_pass, "quad_shader",
             quad_vertex_shader,
             quad_fragment_shader,
             blend_mode) {

    float positions[] = {
        -1.0f, -1.0f, 0.0f,
         1.0f, -1.0f, 0.0f,
         1.0f,  1.0f, 0.0f,
        -1.0f,  1.0f, 0.0f
    };

    float uvs[] = {
        1.0f, 1.0f,
        0.0f, 1.0f,
        0.0f, 0.0f,
        1.0f, 0.0f
    };

    uint32_t indices[] = {
        0, 2, 1,
        3, 2, 0
    };

    set_buffer("position", VariableType::Float32, { 4, 3 }, positions);
    set_buffer("uv", VariableType::Float32, { 4, 2 }, uvs);
    set_buffer("indices", VariableType::UInt32, { 6 }, indices);
    set_uniform("mvp",
                Matrix4f(-1, 0, 0, 0,
                          0, 1, 0, 0,
                          0, 0, 1, 0,
                          0, 0, 0, 1));

    // Initialize texture uniforms with defaults
    set_uniform("texture_linear", false);
    set_uniform("texture_exposure", 1.0f);
}

void TexturedQuad::set_texture(Texture *texture) {
    Shader::set_texture("texture_sampler", texture);
}

void TexturedQuad::set_mvp(const Matrix4f &mvp) {
    set_uniform("mvp", mvp);
}

void TexturedQuad::set_texture_linear(bool linear) {
    m_texture_linear = linear;
    set_uniform("texture_linear", linear);
}

void TexturedQuad::set_texture_exposure(float exposure) {
    m_texture_exposure = exposure;
    set_uniform("texture_exposure", exposure);
}

void TexturedQuad::draw() {
    begin();
    draw_array(PrimitiveType::Triangle, 0, 6, true);
    end();
}

NAMESPACE_END(nanogui)
