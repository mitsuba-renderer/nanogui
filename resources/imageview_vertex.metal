#include <metal_stdlib>

using namespace metal;

struct VertexOut {
    float4 position_image [[position]];
    float2 position_background;
    float2 uv;
};

vertex VertexOut vertex_main(const device float2 *position,
                             constant float4x4 &matrix_image,
                             constant float4x4 &matrix_background,
                             uint id [[vertex_id]]) {
    float4 p = float4(position[id], 0.f, 1.f);
    VertexOut vert;
    vert.position_image = matrix_image * p;
    vert.position_background = (matrix_background * p).xy;
    vert.uv = p.xy;
    return vert;
}
