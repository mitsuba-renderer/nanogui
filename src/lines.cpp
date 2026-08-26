/*
    NanoGUI was developed by Wenzel Jakob <wenzel.jakob@epfl.ch>.
    The widget drawing code is based on the NanoVG demo application
    by Mikko Mononen.

    All rights reserved. Use of this source code is governed by a
    BSD-style license that can be found in the LICENSE.txt file.
*/

#include <nanogui/lines.h>
#include <nanogui/renderpass.h>
#include <vector>

NAMESPACE_BEGIN(nanogui)

/* Each segment becomes a quad whose four vertices carry both endpoints
   (position, other) and a side (+1/-1). The vertex shader projects both
   endpoints and offsets the vertex sideways by half the line width in
   screen space. */

static const char *line_vertex_shader =
#if defined(NANOGUI_USE_OPENGL) || defined(NANOGUI_USE_GLES)
#  if defined(NANOGUI_USE_OPENGL)
    "#version 330\n"
#  else
    "#version 300 es\n"
    "precision highp float;\n"
#  endif
    R"(
        in vec3 position;
        in vec3 other;
        in float side;
        uniform mat4 mvp;
        uniform vec2 viewport;
        uniform float half_width;

        void main() {
            vec4 p = mvp * vec4(position, 1.0);
            vec4 q = mvp * vec4(other, 1.0);
            // The perspective division below needs both endpoints in front of
            // the camera. Clip the segment against a plane just in front of
            // it, and discard it when it lies entirely behind.
            const float eps = 1e-4;
            if (p.w < eps && q.w < eps) {
                gl_Position = vec4(0.0, 0.0, 2.0, 1.0);
                return;
            } else if (q.w < eps) {
                q = mix(p, q, (p.w - eps) / (p.w - q.w));
            } else if (p.w < eps) {
                p = mix(q, p, (q.w - eps) / (q.w - p.w));
            }
            vec2 delta = (q.xy / q.w - p.xy / p.w) * viewport;
            // A zero-length segment has no direction to offset along
            vec2 dir = dot(delta, delta) > 0.0 ? normalize(delta) : vec2(1.0, 0.0);
            vec2 normal = vec2(-dir.y, dir.x) * side * half_width / viewport;
            gl_Position = vec4(p.xy + normal * 2.0 * p.w, p.zw);
        }
    )";
#elif defined(NANOGUI_USE_METAL)
    R"(
        using namespace metal;

        struct VertexOut {
            float4 position [[position]];
        };

        vertex VertexOut vertex_main(const device packed_float3 *position,
                                     const device packed_float3 *other,
                                     const device float *side,
                                     constant float4x4 &mvp,
                                     constant float2 &viewport,
                                     constant float &half_width,
                                     uint id [[vertex_id]]) {
            float4 p = mvp * float4(position[id], 1.f);
            float4 q = mvp * float4(other[id], 1.f);
            VertexOut vert;
            // The perspective division below needs both endpoints in front of
            // the camera. Clip the segment against a plane just in front of
            // it, and discard it when it lies entirely behind.
            const float eps = 1e-4f;
            if (p.w < eps && q.w < eps) {
                vert.position = float4(0.f, 0.f, 2.f, 1.f);
                return vert;
            } else if (q.w < eps) {
                q = mix(p, q, (p.w - eps) / (p.w - q.w));
            } else if (p.w < eps) {
                p = mix(q, p, (q.w - eps) / (q.w - p.w));
            }
            float2 delta = (q.xy / q.w - p.xy / p.w) * viewport;
            // A zero-length segment has no direction to offset along
            float2 dir = dot(delta, delta) > 0.f ? normalize(delta) : float2(1.f, 0.f);
            float2 normal = float2(-dir.y, dir.x) * side[id] * half_width / viewport;
            vert.position = float4(p.xy + normal * 2.f * p.w, p.zw);
            return vert;
        }
    )";
#endif

static const char *line_fragment_shader =
#if defined(NANOGUI_USE_OPENGL) || defined(NANOGUI_USE_GLES)
#  if defined(NANOGUI_USE_OPENGL)
    "#version 330\n"
#  else
    "#version 300 es\n"
    "precision highp float;\n"
#  endif
    R"(
        out vec4 frag_color;
        uniform vec4 color;

        void main() {
            frag_color = color;
        }
    )";
#elif defined(NANOGUI_USE_METAL)
    R"(
        using namespace metal;

        struct VertexOut {
            float4 position [[position]];
        };

        fragment float4 fragment_main(VertexOut vert [[stage_in]],
                                      constant float4 &color) {
            return color;
        }
    )";
#endif

LineRenderer::LineRenderer(RenderPass *render_pass, BlendMode blend_mode)
    : Shader(render_pass, "line_shader", line_vertex_shader,
             line_fragment_shader, blend_mode) {
    set_mvp(Matrix4f(1.f));
    set_width(1.f);
    set_color(Color(1.f, 1.f));
    set_uniform("viewport", Vector2f(1.f));
}

void LineRenderer::set_segments(const Vector3f *endpoints, size_t count) {
    // Vertex k of a segment sits at endpoint END[k] and is offset to SIDE[k]
    const int END[4] = { 0, 0, 1, 1 };
    const float SIDE[4] = { 1.f, -1.f, -1.f, 1.f };
    const uint32_t TRI[6] = { 0, 2, 3, 0, 3, 1 };

    std::vector<Vector3f> position(4 * count), other(4 * count);
    std::vector<float> side(4 * count);
    std::vector<uint32_t> indices(6 * count);

    for (size_t i = 0; i < count; ++i) {
        for (size_t k = 0; k < 4; ++k) {
            position[4 * i + k] = endpoints[2 * i + END[k]];
            other[4 * i + k] = endpoints[2 * i + 1 - END[k]];
            side[4 * i + k] = SIDE[k];
        }
        for (size_t k = 0; k < 6; ++k)
            indices[6 * i + k] = (uint32_t) (4 * i + TRI[k]);
    }

    m_segment_count = count;
    set_buffer("position", VariableType::Float32, { 4 * count, 3 }, position.data());
    set_buffer("other", VariableType::Float32, { 4 * count, 3 }, other.data());
    set_buffer("side", VariableType::Float32, { 4 * count, 1 }, side.data());
    set_buffer("indices", VariableType::UInt32, { 6 * count }, indices.data());
}

void LineRenderer::set_box(const Vector3f &min, const Vector3f &max) {
    // Corners are enumerated as (min/max x) + 2 * (min/max y) + 4 * (min/max z)
    const int EDGES[12][2] = { { 0, 1 }, { 1, 3 }, { 3, 2 }, { 2, 0 },
                               { 4, 5 }, { 5, 7 }, { 7, 6 }, { 6, 4 },
                               { 0, 4 }, { 1, 5 }, { 2, 6 }, { 3, 7 } };
    Vector3f corners[8];
    for (int i = 0; i < 8; ++i)
        corners[i] = Vector3f(i & 1 ? max.x() : min.x(),
                              i & 2 ? max.y() : min.y(),
                              i & 4 ? max.z() : min.z());
    Vector3f endpoints[24];
    for (int i = 0; i < 12; ++i) {
        endpoints[2 * i] = corners[EDGES[i][0]];
        endpoints[2 * i + 1] = corners[EDGES[i][1]];
    }
    set_segments(endpoints, 12);
}

void LineRenderer::set_mvp(const Matrix4f &mvp) {
    set_uniform("mvp", mvp);
}

void LineRenderer::set_width(float width) {
    m_width = width;
    set_uniform("half_width", 0.5f * width);
}

void LineRenderer::set_color(const Color &color) {
    set_uniform("color", Vector4f(color));
}

void LineRenderer::draw() {
    if (m_segment_count == 0)
        return;
    RenderPass *rp = render_pass();
    Vector2i size = rp->viewport().second;
    set_uniform("viewport", Vector2f((float) size.x(), (float) size.y()));

    RenderPass::CullMode cull_mode = rp->cull_mode();
    rp->set_cull_mode(RenderPass::CullMode::Disabled);
    begin();
    draw_array(PrimitiveType::Triangle, 0, 6 * m_segment_count, true);
    end();
    rp->set_cull_mode(cull_mode);
}

NAMESPACE_END(nanogui)
