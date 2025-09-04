/*
    src/colorpass.cpp -- HDR/extended gamut color management render pass

    The color management pass was contributed by Thomas Müller (@tom94).

    NanoGUI was developed by Wenzel Jakob <wenzel.jakob@epfl.ch>.
    The widget drawing code is based on the NanoVG demo application
    by Mikko Mononen.

    All rights reserved. Use of this source code is governed by a
    BSD-style license that can be found in the LICENSE.txt file.
*/

#if defined(NANOGUI_USE_OPENGL) || defined(NANOGUI_USE_GLES)

#include <nanogui/colorpass.h>
#include <nanogui/chroma.h>
#include <nanogui/opengl.h>
#include <GLFW/glfw3.h>
#include <array>
#include <cstdio>

#include "opengl_check.h"

NAMESPACE_BEGIN(nanogui)

// Dither matrix size and type definitions
static const size_t DITHER_MATRIX_SIZE = 8;
using DitherMatrix = std::array<float, DITHER_MATRIX_SIZE * DITHER_MATRIX_SIZE>;

// Generate dither matrix for color quantization
static DitherMatrix dither_matrix(float scale) {
    // 8x8 Bayer dithering matrix scaled to [-0.5f, 0.5f] / 255
    DitherMatrix mat = {
        {0, 32, 8, 40, 2, 34, 10, 42,
         48, 16, 56, 24, 50, 18, 58, 26,
         12, 44, 4, 36, 14, 46, 6, 38,
         60, 28, 52, 20, 62, 30, 54, 22,
         3, 35, 11, 43, 1, 33, 9, 41,
         51, 19, 59, 27, 49, 17, 57, 25,
         15, 47, 7, 39, 13, 45, 5, 37,
         63, 31, 55, 23, 61, 29, 53, 21}
    };

    for (size_t i = 0; i < DITHER_MATRIX_SIZE * DITHER_MATRIX_SIZE; ++i)
        mat[i] = (mat[i] / DITHER_MATRIX_SIZE / DITHER_MATRIX_SIZE - 0.5f) * scale;

    return mat;
}

#if defined(NANOGUI_USE_OPENGL)
#  define GLSL_PRELUDE "#version 110\n"
#elif defined(NANOGUI_USE_GLES)
#  define GLSL_PRELUDE \
    "#version 100\n" \
    "precision highp float;\n" \
    "precision highp sampler2D;\n"
#endif

static const char *vertex_shader = GLSL_PRELUDE R"glsl(
    uniform vec2 dither_scale;

    attribute vec2 position;
    varying vec2 imageUv;
    varying vec2 ditherUv;

    void main() {
        vec2 pos = position * 0.5 + 0.5; // Convert from [-1, 1] to [0, 1]
        imageUv = pos;
        ditherUv = pos * dither_scale;

        gl_Position = vec4(position, 1.0, 1.0);
    }
)glsl";

static const char *fragment_shader = GLSL_PRELUDE R"glsl(
    varying vec2 imageUv;
    varying vec2 ditherUv;

    uniform sampler2D framebuffer_texture;
    uniform sampler2D dither_matrix;

    uniform float display_sdr_white_level;
    uniform float min_luminance;
    uniform float max_luminance;

    uniform int out_transfer_function;
    uniform mat3 display_color_matrix;
    uniform bool clip_to_unit_interval;

    #define CM_TRANSFER_FUNCTION_BT1886     1
    #define CM_TRANSFER_FUNCTION_GAMMA22    2
    #define CM_TRANSFER_FUNCTION_GAMMA28    3
    #define CM_TRANSFER_FUNCTION_ST240      4
    #define CM_TRANSFER_FUNCTION_EXT_LINEAR 5
    #define CM_TRANSFER_FUNCTION_LOG_100    6
    #define CM_TRANSFER_FUNCTION_LOG_316    7
    #define CM_TRANSFER_FUNCTION_XVYCC      8
    #define CM_TRANSFER_FUNCTION_SRGB       9
    #define CM_TRANSFER_FUNCTION_EXT_SRGB   10
    #define CM_TRANSFER_FUNCTION_ST2084_PQ  11
    #define CM_TRANSFER_FUNCTION_ST428      12
    #define CM_TRANSFER_FUNCTION_HLG        13

    #define SRGB_POW 2.4
    #define SRGB_CUT 0.0031308
    #define SRGB_SCALE 12.92
    #define SRGB_ALPHA 1.055

    #define BT1886_POW (1.0 / 0.45)
    #define BT1886_CUT 0.018053968510807
    #define BT1886_SCALE 4.5
    #define BT1886_ALPHA (1.0 + 5.5 * BT1886_CUT)

    // See http://car.france3.mars.free.fr/HD/INA-%2026%20jan%2006/SMPTE%20normes%20et%20confs/s240m.pdf
    #define ST240_POW (1.0 / 0.45)
    #define ST240_CUT 0.0228
    #define ST240_SCALE 4.0
    #define ST240_ALPHA 1.1115

    #define ST428_POW 2.6
    #define ST428_SCALE (52.37 / 48.0)

    #define PQ_M1 0.1593017578125
    #define PQ_M2 78.84375
    #define PQ_INV_M1 (1.0 / PQ_M1)
    #define PQ_INV_M2 (1.0 / PQ_M2)
    #define PQ_C1 0.8359375
    #define PQ_C2 18.8515625
    #define PQ_C3 18.6875

    #define HLG_D_CUT (1.0 / 12.0)
    #define HLG_E_CUT 0.5
    #define HLG_A 0.17883277
    #define HLG_B 0.28466892
    #define HLG_C 0.55991073

    #define M_E 2.718281828459045

    vec3 mixb(vec3 a, vec3 b, bvec3 mask) {
        return mix(a, b, vec3(mask));
    }

    // The primary source for these transfer functions is https://www.itu.int/dms_pubrec/itu-r/rec/bt/R-REC-BT.1361-0-199802-W!!PDF-E.pdf
    // Outputs are assumed to have 1 == SDR White which is different for each transfer function.
    vec3 tfInvPQ(vec3 color) {
        vec3 E = pow(max(color.rgb, vec3(0.0)), vec3(PQ_INV_M2));
        return pow(
            (max(E - PQ_C1, vec3(0.0))) / max(PQ_C2 - PQ_C3 * E, vec3(1e-5)),
            vec3(PQ_INV_M1)
        );
    }

    vec3 tfInvHLG(vec3 color) {
        bvec3 isLow = lessThanEqual(color.rgb, vec3(HLG_E_CUT));
        vec3 lo = color.rgb * color.rgb / 3.0;
        vec3 hi = (exp((color.rgb - HLG_C) / HLG_A) + HLG_B) / 12.0;
        return mixb(hi, lo, isLow);
    }

    // Many transfer functions (including sRGB) follow the same pattern: a linear
    // segment for small values and a power function for larger values. The
    // following function implements this pattern from which sRGB, BT.1886, and
    // others can be derived by plugging in the right constants.
    vec3 tfInvLinPow(vec3 color, float gamma, float thres, float scale, float alpha) {
        bvec3 isLow = lessThanEqual(color.rgb, vec3(thres * scale));
        vec3 lo = color.rgb / scale;
        vec3 hi = pow((color.rgb + alpha - 1.0) / alpha, vec3(gamma));
        return mixb(hi, lo, isLow);
    }

    vec3 tfInvSRGB(vec3 color) {
        return tfInvLinPow(color, SRGB_POW, SRGB_CUT, SRGB_SCALE, SRGB_ALPHA);
    }

    vec3 tfInvExtSRGB(vec3 color) {
        // EXT sRGB is the sRGB transfer function mirrored around 0.
        return sign(color) * tfInvSRGB(abs(color));
    }

    vec3 tfInvBT1886(vec3 color) {
        return tfInvLinPow(color, BT1886_POW, BT1886_CUT, BT1886_SCALE, BT1886_ALPHA);
    }

    vec3 tfInvXVYCC(vec3 color) {
        // The inverse transfer function for XVYCC is the BT1886 transfer function mirrored around 0,
        // same as what EXT sRGB is to sRGB.
        return sign(color) * tfInvBT1886(abs(color));
    }

    vec3 tfInvST240(vec3 color) {
        return tfInvLinPow(color, ST240_POW, ST240_CUT, ST240_SCALE, ST240_ALPHA);
    }

    // Forward transfer functions corresponding to the inverse functions above.
    // Inputs are assumed to have 1 == 80 nits with a scale factor pre-applied to adjust for SDR white!
    vec3 tfPQ(vec3 color) {
        vec3 E = pow(max(color.rgb, vec3(0.0)), vec3(PQ_M1));
        return pow(
            (vec3(PQ_C1) + PQ_C2 * E) / max(vec3(1.0) + PQ_C3 * E, vec3(1e-5)),
            vec3(PQ_M2)
        );
    }

    vec3 tfHLG(vec3 color) {
        bvec3 isLow = lessThanEqual(color.rgb, vec3(HLG_D_CUT));
        vec3 lo = sqrt(max(color.rgb, vec3(0.0)) * 3.0);
        vec3 hi = HLG_A * log(max(12.0 * color.rgb - HLG_B, vec3(0.0001))) + HLG_C;
        return mixb(hi, lo, isLow);
    }

    vec3 tfLinPow(vec3 color, float gamma, float thres, float scale, float alpha) {
        bvec3 isLow = lessThanEqual(color.rgb, vec3(thres));
        vec3 lo = color.rgb * scale;
        vec3 hi = pow(color.rgb, vec3(1.0 / gamma)) * alpha - (alpha - 1.0);
        return mixb(hi, lo, isLow);
    }

    vec3 tfSRGB(vec3 color) {
        return tfLinPow(color, SRGB_POW, SRGB_CUT, SRGB_SCALE, SRGB_ALPHA);
    }

    vec3 tfExtSRGB(vec3 color) {
        // EXT sRGB is the sRGB transfer function mirrored around 0.
        return sign(color) * tfSRGB(abs(color));
    }

    vec3 tfBT1886(vec3 color) {
        return tfLinPow(color, BT1886_POW, BT1886_CUT, BT1886_SCALE, BT1886_ALPHA);
    }

    vec3 tfXVYCC(vec3 color) {
        // The transfer function for XVYCC is the BT1886 transfer function mirrored around 0,
        // same as what EXT sRGB is to sRGB.
        return sign(color) * tfBT1886(abs(color));
    }

    vec3 tfST240(vec3 color) {
        return tfLinPow(color, ST240_POW, ST240_CUT, ST240_SCALE, ST240_ALPHA);
    }

    vec3 toLinearRGB(vec3 color, int tf) {
        if (tf == CM_TRANSFER_FUNCTION_EXT_LINEAR) {
            return color;
        } else if (tf == CM_TRANSFER_FUNCTION_ST2084_PQ) {
            return tfInvPQ(color);
        } else if (tf == CM_TRANSFER_FUNCTION_GAMMA22) {
            return pow(max(color, vec3(0.0)), vec3(2.2));
        } else if (tf == CM_TRANSFER_FUNCTION_GAMMA28) {
            return pow(max(color, vec3(0.0)), vec3(2.8));
        } else if (tf == CM_TRANSFER_FUNCTION_HLG) {
            return tfInvHLG(color);
        } else if (tf == CM_TRANSFER_FUNCTION_EXT_SRGB) {
            return tfInvExtSRGB(color);
        } else if (tf == CM_TRANSFER_FUNCTION_BT1886) {
            return tfInvBT1886(color);
        } else if (tf == CM_TRANSFER_FUNCTION_ST240) {
            return tfInvST240(color);
        } else if (tf == CM_TRANSFER_FUNCTION_LOG_100) {
            return mixb(exp((color - 1.0) * 2.0 * log(10.0)), vec3(0.0), lessThanEqual(color, vec3(0.0)));
        } else if (tf == CM_TRANSFER_FUNCTION_LOG_316) {
            return mixb(exp((color - 1.0) * 2.5 * log(10.0)), vec3(0.0), lessThanEqual(color, vec3(0.0)));
        } else if (tf == CM_TRANSFER_FUNCTION_XVYCC) {
            return tfInvXVYCC(color);
        } else if (tf == CM_TRANSFER_FUNCTION_ST428) {
            return pow(max(color, vec3(0.0)), vec3(ST428_POW)) * ST428_SCALE;
        } else if (tf == CM_TRANSFER_FUNCTION_SRGB) {
            return tfInvSRGB(color);
        } else {
            return tfInvSRGB(color);
        }
    }

    vec3 fromLinearRGB(vec3 color, int tf) {
        if (tf == CM_TRANSFER_FUNCTION_EXT_LINEAR) {
            return color;
        } else if (tf == CM_TRANSFER_FUNCTION_ST2084_PQ) {
            return tfPQ(color);
        } else if (tf == CM_TRANSFER_FUNCTION_GAMMA22) {
            return pow(max(color, vec3(0.0)), vec3(1.0 / 2.2));
        } else if (tf == CM_TRANSFER_FUNCTION_GAMMA28) {
            return pow(max(color, vec3(0.0)), vec3(1.0 / 2.8));
        } else if (tf == CM_TRANSFER_FUNCTION_HLG) {
            return tfHLG(color);
        } else if (tf == CM_TRANSFER_FUNCTION_EXT_SRGB) {
            return tfExtSRGB(color);
        } else if (tf == CM_TRANSFER_FUNCTION_BT1886) {
            return tfBT1886(color);
        } else if (tf == CM_TRANSFER_FUNCTION_ST240) {
            return tfST240(color);
        } else if (tf == CM_TRANSFER_FUNCTION_LOG_100) {
            return mixb(1.0 + log(color) / log(10.0) / 2.0, vec3(0.0), lessThanEqual(color, vec3(0.01)));
        } else if (tf == CM_TRANSFER_FUNCTION_LOG_316) {
            return mixb(1.0 + log(color) / log(10.0) / 2.5, vec3(0.0), lessThanEqual(color, vec3(sqrt(10.0) / 1000.0)));
        } else if (tf == CM_TRANSFER_FUNCTION_XVYCC) {
            return tfXVYCC(color);
        } else if (tf == CM_TRANSFER_FUNCTION_ST428) {
            return pow(max(color, vec3(0.0)) / ST428_SCALE, vec3(1.0 / ST428_POW));
        } else if (tf == CM_TRANSFER_FUNCTION_SRGB) {
            return tfSRGB(color);
        } else {
            return tfSRGB(color);
        }
    }

    float transferWhiteLevel(int tf) {
        if (tf == CM_TRANSFER_FUNCTION_ST2084_PQ) {
            return 10000.0;
        } else if (tf == CM_TRANSFER_FUNCTION_HLG) {
            return 1000.0;
        } else if (tf == CM_TRANSFER_FUNCTION_BT1886) {
            return 100.0;
        } else if (tf == CM_TRANSFER_FUNCTION_XVYCC) {
            return 100.0;
        } else {
            return 80.0;
        }
    }

    vec3 dither(vec3 color) {
        return color + texture2D(dither_matrix, fract(ditherUv)).r;
    }

    void main() {
        vec4 color = texture2D(framebuffer_texture, imageUv);

        // nanogui uses colors in extended sRGB with a scale that assumes SDR white corresponds to a value of 1. Hence, to convert to
        // absolute nits in the display's color space, we need to undo the extended sRGB transfer function, multiply by the SDR white
        // level of the display, apply the display's color matrix, and finally apply the display's transfer function.
        vec3 nits = display_color_matrix * (display_sdr_white_level * toLinearRGB(color.rgb, CM_TRANSFER_FUNCTION_EXT_SRGB));

        // Some displays perform strange tonemapping when provided with values outside of their luminance range. Make sure we don't
        // let this happen -- we strongly prefer hard clipping because we want the displayable colors to be preserved.
        if (max_luminance > 0.0) {
            nits = clamp(nits, vec3(min_luminance), vec3(max_luminance));
        }

        // On Linux, some drivers only let us have an 8-bit framebuffer. When dealing with HDR content in such a situation,
        // dithering is essential to avoid banding artifacts.
        color.rgb = dither(fromLinearRGB(nits / transferWhiteLevel(out_transfer_function), out_transfer_function));

        if (clip_to_unit_interval) {
            color = clamp(color, vec4(0.0), vec4(1.0));
        }

        gl_FragColor = color;
    }
)glsl";

ColorPass::ColorPass(Texture *color_texture,
                     Texture *depth_texture,
                     Texture *stencil_texture,
                     uint32_t bits_per_channel)
    : RenderPass({color_texture}, depth_texture, stencil_texture, nullptr, true) {

    // Disable depth testing if we have a depth buffer (only used for stencil)
    if (depth_texture)
        set_depth_test(RenderPass::DepthTest::Always, true);

    // Color texture for
    m_float_buffer = color_texture->component_format() == Texture::ComponentFormat::Float16 ||
                     color_texture->component_format() == Texture::ComponentFormat::Float32;

    m_dither_matrix = new Texture{
        Texture::PixelFormat::R,
        Texture::ComponentFormat::Float32,
        Vector2i(DITHER_MATRIX_SIZE),
        Texture::InterpolationMode::Nearest,
        Texture::InterpolationMode::Nearest,
        Texture::WrapMode::Repeat,
    };

    const float dither_scale =
        m_float_buffer ? 0.f : (1.f / (1u << bits_per_channel));

    m_dither_matrix->upload((uint8_t*) dither_matrix(dither_scale).data());

    // Shader to render a full-screen quad while processing colors
    m_color_shader = new Shader(nullptr, "color_management", vertex_shader, fragment_shader);

    const uint32_t indices[3 * 2] = {0, 1, 2, 2, 3, 0};
    const float positions[2 * 4] = {-1.f, -1.f, 1.f, -1.f, 1.f, 1.f, -1.f, 1.f};

    m_color_shader->set_buffer("indices", VariableType::UInt32, {3 * 2}, indices);
    m_color_shader->set_buffer("position", VariableType::Float32, {4, 2}, positions);
    m_color_shader->set_texture("dither_matrix", m_dither_matrix);
}

ColorPass::~ColorPass() { }

void ColorPass::configure(GLFWwindow *window,
                          float display_sdr_white_level_override) {
    Texture *tex = color_texture();
    m_color_shader->set_texture("framebuffer_texture", tex);

    float display_sdr_white_level = display_sdr_white_level_override > 0 ?
        display_sdr_white_level_override : glfwGetWindowSdrWhiteLevel(window);

    m_color_shader->set_uniform("display_sdr_white_level", display_sdr_white_level);
    m_color_shader->set_uniform("out_transfer_function", (int)glfwGetWindowTransfer(window));

    const auto display_chroma = chroma_from_wp_primaries(glfwGetWindowPrimaries(window));
    Matrix3f display_color_matrix = inverse(chroma_to_rec709_matrix(display_chroma));

    m_color_shader->set_uniform("display_color_matrix", display_color_matrix);

    m_color_shader->set_uniform("min_luminance", glfwGetWindowMinLuminance(window));
    m_color_shader->set_uniform("max_luminance", glfwGetWindowMaxLuminance(window));

    m_color_shader->set_uniform("clip_to_unit_interval", !m_float_buffer);

    m_color_shader->set_uniform("dither_scale", Vector2f(tex->size()) *
                                                    (1.f / DITHER_MATRIX_SIZE));

    m_color_shader->set_texture("dither_matrix", m_dither_matrix);
}

void ColorPass::draw_quad() {
    m_color_shader->begin();
    m_color_shader->draw_array(Shader::PrimitiveType::Triangle, 0, 6, true);
    m_color_shader->end();
}

NAMESPACE_END(nanogui)
#endif
