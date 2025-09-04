/*
    nanogui/colorpass.h -- HDR/extended gamut color management render pass

    The color management pass was contributed by Thomas Müller (@tom94).

    NanoGUI was developed by Wenzel Jakob <wenzel.jakob@epfl.ch>.
    The widget drawing code is based on the NanoVG demo application
    by Mikko Mononen.

    All rights reserved. Use of this source code is governed by a
    BSD-style license that can be found in the LICENSE.txt file.
*/

#pragma once

#if defined(NANOGUI_USE_OPENGL) || defined(NANOGUI_USE_GLES)

#include <nanogui/renderpass.h>
#include <nanogui/shader.h>
#include <nanogui/texture.h>
#include <nanogui/vector.h>

NAMESPACE_BEGIN(nanogui)

/**
 * \class ColorPass colorpass.h nanogui/colorpass.h
 *
 * \brief Color management pass for rendering HDR and/or extended color gamuts
 * on Windows or Linux/Wayland.
 *
 * NanoGUI generally expresses color information using the nonlinear sRGB gamma
 * encoding and color gamut. When working with HDR data or color gamuts that are
 * larger than sRGB, nanogui adopts the macOS convention of "extended sRGB",
 * which uses the the standard nonlinear encoding, except that components can
 * now exceed the value 1.0 or be negative.
 *
 * On Linux/Wayland and Windows, the compositor expects a different color
 * representation that furthermore depends on the precise setup. This class
 * determines at runtime what transformation is needed and then converts from
 * extended sRGB to this format. This is implemented by first rendering to a
 * texture and then blitting a full-screen quad with a fragment shader that
 * performs the color conversion.
 */
class NANOGUI_EXPORT ColorPass : public RenderPass {
public:
    /// Create a render target for later processing
    ColorPass(Texture *color_texture,
              Texture *depth_texture,
              Texture *stencil_texture,
              uint32_t bits_per_channel,
              bool float_buffer);

    /// Destructor
    virtual ~ColorPass();

    /// Update color management settings
    void configure(GLFWwindow *window,
                   float display_sdr_white_level_override = .0f);

    /// Draw the color management quad
    void draw_quad();

protected:
    Texture *color_texture() { return static_cast<Texture *>(m_targets[2]); }

protected:
    ref<Shader> m_color_shader;
    ref<Texture> m_dither_matrix;
    bool m_float_buffer;
};

NAMESPACE_END(nanogui)

#endif
