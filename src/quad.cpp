/*
    NanoGUI was developed by Wenzel Jakob <wenzel.jakob@epfl.ch>.
    The widget drawing code is based on the NanoVG demo application
    by Mikko Mononen.

    All rights reserved. Use of this source code is governed by a
    BSD-style license that can be found in the LICENSE.txt file.
*/

#include <nanogui/quad.h>
#include <nanogui/renderpass.h>
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
        uniform bool linear;
        uniform bool hdr;
        uniform float exposure;
        uniform bool depth_from_alpha;
        uniform vec4 depth_proj;

        vec3 linearToGamma22(vec3 linear) {
            return sign(linear) * pow(abs(linear), vec3(1.0/2.2));
        }

        void main() {
            vec4 color = texture(texture_sampler, uv_frag);
            color.rgb *= exposure;

            if (!hdr)
                color.rgb = clamp(color.rgb, 0.0, 1.0);

            if (linear)
                color.rgb = linearToGamma22(color.rgb);

            if (depth_from_alpha) {
                // Map the eye-space depth in color.a to a window-space
                // value using the set_depth_projection() coefficients.
                // Non-finite values mark the background (far plane).
                float z = -color.a;
                float depth = 1.0;
                if (!isinf(color.a) && !isnan(color.a))
                    depth = clamp((depth_proj.x * z + depth_proj.y) /
                                  (depth_proj.z * z + depth_proj.w), 0.0, 1.0);
                gl_FragDepth = depth;
                color.a = 1.0;
            } else {
                gl_FragDepth = gl_FragCoord.z;
            }

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
        uniform bool linear;
        uniform bool hdr;
        uniform float exposure;
        uniform bool depth_from_alpha;
        uniform vec4 depth_proj;

        vec3 linearToGamma22(vec3 linear) {
            return sign(linear) * pow(abs(linear), vec3(1.0/2.2));
        }

        void main() {
            vec4 color = texture(texture_sampler, uv_frag);
            color.rgb *= exposure;

            if (!hdr)
                color.rgb = clamp(color.rgb, 0.0, 1.0);

            if (linear)
                color.rgb = linearToGamma22(color.rgb);

            if (depth_from_alpha) {
                // Map the eye-space depth in color.a to a window-space
                // value using the set_depth_projection() coefficients.
                // Non-finite values mark the background (far plane).
                float z = -color.a;
                float depth = 1.0;
                if (!isinf(color.a) && !isnan(color.a))
                    depth = clamp((depth_proj.x * z + depth_proj.y) /
                                  (depth_proj.z * z + depth_proj.w), 0.0, 1.0);
                gl_FragDepth = depth;
                color.a = 1.0;
            } else {
                gl_FragDepth = gl_FragCoord.z;
            }

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

        struct FragmentOut {
            float4 color [[color(0)]];
            float depth [[depth(any)]];
        };

        fragment FragmentOut fragment_main(VertexOut vert [[stage_in]],
                     texture2d<float, access::sample> texture_sampler,
                     sampler texture_sampler_sampler,
                     constant bool &linear,
                     constant bool &hdr,
                     constant float &exposure,
                     constant bool &depth_from_alpha,
                     constant float4 &depth_proj) {
            float4 color = texture_sampler.sample(texture_sampler_sampler, vert.uv);
            color.rgb *= exposure;

            if (!hdr)
                color.rgb = clamp(color.rgb, 0.f, 1.f);

            if (linear)
                color.rgb = linearToGamma22(color.rgb);

            FragmentOut out;
            if (depth_from_alpha) {
                // Map the eye-space depth in color.a to a window-space
                // value using the set_depth_projection() coefficients.
                // Non-finite values mark the background (far plane).
                float z = -color.a;
                float depth = 1.f;
                if (!isinf(color.a) && !isnan(color.a))
                    depth = clamp((depth_proj.x * z + depth_proj.y) /
                                  (depth_proj.z * z + depth_proj.w), 0.f, 1.f);
                out.depth = depth;
                color.a = 1.f;
            } else {
                out.depth = vert.position.z;
            }
            out.color = color;
            return out;
        }
    )";
#endif

#if defined(NANOGUI_USE_METAL)
/* Variant for render passes without a depth attachment, where Metal rejects
   pipelines whose fragment shader declares a depth output */
static const char *quad_fragment_shader_nodepth =
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
                     constant bool &linear,
                     constant bool &hdr,
                     constant float &exposure) {
            float4 color = texture_sampler.sample(texture_sampler_sampler, vert.uv);
            color.rgb *= exposure;

            if (!hdr)
                color.rgb = clamp(color.rgb, 0.f, 1.f);

            if (linear)
                color.rgb = linearToGamma22(color.rgb);

            return color;
        }
    )";
#endif

// The GL variants keep their depth write in either case, where it is legal
// and simply ignored when the framebuffer lacks a depth buffer
static const char *select_fragment_shader(RenderPass *render_pass) {
#if defined(NANOGUI_USE_METAL)
    if (!render_pass->targets()[0])
        return quad_fragment_shader_nodepth;
#else
    (void) render_pass;
#endif
    return quad_fragment_shader;
}

TexturedQuad::TexturedQuad(RenderPass *render_pass, BlendMode blend_mode)
    : Shader(render_pass, "quad_shader",
             quad_vertex_shader,
             select_fragment_shader(render_pass),
             blend_mode),
      m_has_depth(render_pass->targets()[0] != nullptr) {

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
    set_uniform("linear", false);
    set_uniform("hdr", true);
    set_uniform("exposure", 1.0f);
    if (m_has_depth) {
        set_uniform("depth_from_alpha", false);
        set_depth_projection(Matrix4f::perspective(1.f, 0.1f, 100.f));
    }
}

void TexturedQuad::set_texture(Texture *texture) {
    Shader::set_texture("texture_sampler", texture);
}

void TexturedQuad::set_mvp(const Matrix4f &mvp) {
    set_uniform("mvp", mvp);
}

void TexturedQuad::set_linear(bool linear) {
    m_linear = linear;
    set_uniform("linear", linear);
}

void TexturedQuad::set_hdr(bool hdr) {
    m_hdr = hdr;
    set_uniform("hdr", hdr);
}

void TexturedQuad::set_depth_from_alpha(bool enabled) {
    if (!m_has_depth) {
        if (enabled)
            throw std::runtime_error(
                "TexturedQuad::set_depth_from_alpha(): the render pass has "
                "no depth attachment");
        return;
    }
    m_depth_from_alpha = enabled;
    set_uniform("depth_from_alpha", enabled);
}

void TexturedQuad::set_depth_projection(const Matrix4f &projection, float scale) {
    if (!m_has_depth)
        return;
    // Rows 2 and 3 of the projection applied to (0, 0, z, 1) give clip-space
    // z and w. The matrix is stored column-major, and z arrives in units of
    // 'scale' from the alpha channel.
    float zz = projection.m[2][2] * scale, zw = projection.m[3][2],
          wz = projection.m[2][3] * scale, ww = projection.m[3][3];

    // The shader writes a window-space depth in [0, 1] on either backend.
    // Folding the remap from the API's clip depth range into the
    // coefficients keeps the fragment shader free of backend conventions.
    auto [z0, z1] = Matrix4f::clip_depth_range();
    float s = 1.f / (z1 - z0), t = -z0 * s;
    zz = s * zz + t * wz;
    zw = s * zw + t * ww;

    set_uniform("depth_proj", Vector4f(zz, zw, wz, ww));
}

void TexturedQuad::set_exposure(float exposure) {
    m_exposure = exposure;
    set_uniform("exposure", exposure);
}

void TexturedQuad::draw() {
    begin();
    draw_array(PrimitiveType::Triangle, 0, 6, true);
    end();
}

NAMESPACE_END(nanogui)
