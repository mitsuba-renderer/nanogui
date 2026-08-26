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
     * When true, the shader applies the gamma 2.2 encoding expected by
     * NanoGUI's framebuffer. When false, the texture is assumed to
     * be encoded already. Default is false.
     *
     * \param linear
     *     True if the texture holds linear values, false if it is encoded
     */
    void set_linear(bool linear);

    /**
     * \brief Get whether the texture is treated as linear space
     *
     * \return True if texture is treated as linear space
     */
    bool linear() const { return m_linear; }

    /**
     * \brief Write depth taken from the texture's alpha channel
     *
     * When set, the class interprets the texture's alpha channel as depth
     * and writes it to the depth buffer. A stored value is the distance of
     * the surface seen by that texel from the camera plane, measured along
     * the view axis (the negated eye-space z coordinate). Values that are
     * not finite, such as an infinity marking the background, land on the
     * far plane.
     *
     * \ref set_depth_projection() configures the conversion from these
     * values to the depth buffer.
     *
     * Geometry drawn into the same render pass with depth testing is then
     * occluded by the image, which is useful for 3D overlays. The render
     * pass must have a depth attachment.
     *
     * The default is false.
     *
     * \param enabled
     *     True to derive the depth buffer from the alpha channel
     */
    void set_depth_from_alpha(bool enabled);

    /// Get whether the depth buffer is written from the alpha channel
    bool depth_from_alpha() const { return m_depth_from_alpha; }

    /**
     * \brief Set the projection used by \ref set_depth_from_alpha()
     *
     * \param projection
     *     Camera-to-clip transformation, e.g. from \ref Matrix4f::perspective()
     *     or \ref Matrix4f::ortho().
     *
     * \param scale
     *     Unit of the depth values stored in the alpha channel. Default is 1.
     */
    void set_depth_projection(const Matrix4f &projection, float scale = 1.f);

    /**
     * \brief Set the exposure multiplier for the texture
     *
     * This value is multiplied onto the texture color before the gamma 2.2
     * conversion. Default is 1.0.
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
    bool m_linear = false;
    float m_texture_exposure = 1.0f;
    bool m_depth_from_alpha = false;
    bool m_has_depth;
};

NAMESPACE_END(nanogui)
