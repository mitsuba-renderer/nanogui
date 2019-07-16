#include <metal_stdlib>

using namespace metal;

struct VertexOut {
    float4 position_image [[position]];
    float2 position_background;
    float2 uv;
};

fragment float4 fragment_main(VertexOut vert [[stage_in]],
                              texture2d<float, access::sample> image,
                              constant float4 &background_color,
                              sampler image_sampler) {
    float2 frac = vert.position_background - floor(vert.position_background);
    float checkerboard = ((frac.x > .5f) == (frac.y > .5f)) ? .4f : .5f;

    float4 background = (1.f - background_color.a) * float4(float3(checkerboard), 1.f) +
                                background_color.a * float4(background_color.rgb, 1.f);

    float4 value = image.sample(image_sampler, vert.uv);
    return (1.f - value.a) * background + value.a * float4(value.rgb, 1.f);
}
