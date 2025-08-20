/*
    NanoGUI was developed by Wenzel Jakob <wenzel.jakob@epfl.ch>.
    The widget drawing code is based on the NanoVG demo application
    by Mikko Mononen.

    All rights reserved. Use of this source code is governed by a
    BSD-style license that can be found in the LICENSE.txt file.
*/

/**
 * \file nanogui/quad.h
 *
 * \brief Defines a simple quad renderer for displaying textures
 */

#pragma once

#include <nanogui/shader.h>
#include <nanogui/texture.h>
#include <nanogui/vector.h>

NAMESPACE_BEGIN(nanogui)

/**
 * \brief Textured quad
 *
 * This convenience class implements a shader that renders a textured quad
 * on the supported platforms (OpenGL, EGL, Metal)
 */
class NANOGUI_EXPORT TexturedQuad : public Shader {
public:
    /**
     * \brief Initialize the quad renderer
     *
     * \param render_pass
     *     RenderPass object encoding targets to which the quad will be rendered
     *
     * \param blend_mode
     *     Alpha blending mode for rendering
     */
    TexturedQuad(RenderPass *render_pass, BlendMode blend_mode = BlendMode::None);

    /**
     * \brief Set the texture to be rendered on the quad
     *
     * \param texture
     *     The texture to display
     */
    void set_texture(Texture *texture);

    /**
     * \brief Set the model-view-projection matrix
     *
     * \param mvp
     *     The transformation matrix
     */
    void set_mvp(const Matrix4f &mvp);

    /**
     * \brief Render the quad
     *
     * This method handles begin(), draw_array(), and end() internally
     */
    void draw();

    /**
     * \brief Set whether the texture is in linear space
     *
     * When true, the shader will convert from linear to sRGB space.
     * When false, the texture is assumed to already be in sRGB space.
     * Default is false.
     *
     * \param linear
     *     True if texture is in linear space, false if in sRGB space
     */
    void set_texture_linear(bool linear);

    /**
     * \brief Get whether the texture is treated as linear space
     *
     * \return True if texture is treated as linear space
     */
    bool texture_linear() const { return m_texture_linear; }

    /**
     * \brief Set the exposure multiplier for the texture
     *
     * This value is multiplied onto the texture color before
     * linear-to-sRGB conversion. Default is 1.0.
     *
     * \param exposure
     *     Exposure multiplier (typically 0.0 to 10.0)
     */
    void set_texture_exposure(float exposure);

    /**
     * \brief Get the current exposure multiplier
     *
     * \return Current exposure value
     */
    float texture_exposure() const { return m_texture_exposure; }

private:
    bool m_texture_linear = false;
    float m_texture_exposure = 1.0f;
};

NAMESPACE_END(nanogui)
