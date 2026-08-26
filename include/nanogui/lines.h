/*
    NanoGUI was developed by Wenzel Jakob <wenzel.jakob@epfl.ch>.
    The widget drawing code is based on the NanoVG demo application
    by Mikko Mononen.

    All rights reserved. Use of this source code is governed by a
    BSD-style license that can be found in the LICENSE.txt file.
*/

/**
 * \file nanogui/lines.h
 *
 * \brief Defines a renderer for 3D line segments with a screen-space width
 */

#pragma once

#include <nanogui/shader.h>
#include <nanogui/vector.h>

NAMESPACE_BEGIN(nanogui)

/**
 * \brief Renderer for 3D line segments with a constant width in pixels
 *
 * This class draws 3D line segments on OpenGL, GLES, and Metal. It takes a list
 * of segments (or an axis-aligned box via \ref set_box()) and draws them with
 * a specified screen-space line width and color. It is suitable for overlays
 * such as bounding boxes, gizmos, and wireframes.
 */
class NANOGUI_EXPORT LineRenderer : public Shader {
public:
    /**
     * \brief Initialize the line renderer
     *
     * \param render_pass
     *     RenderPass object encoding targets to which the lines will be rendered
     *
     * \param blend_mode
     *     Alpha blending mode for rendering
     */
    LineRenderer(RenderPass *render_pass, BlendMode blend_mode = BlendMode::None);

    /**
     * \brief Upload a set of line segments
     *
     * \param endpoints
     *     Pointer to <tt>2 * count</tt> vertices holding the start and end
     *     point of every segment in turn
     *
     * \param count
     *     Number of segments
     */
    void set_segments(const Vector3f *endpoints, size_t count);

    /// Upload the 12 edges of an axis-aligned box
    void set_box(const Vector3f &min, const Vector3f &max);

    /// Set the model-view-projection matrix
    void set_mvp(const Matrix4f &mvp);

    /// Set the line width in framebuffer pixels (default: 1)
    void set_width(float width);

    /// Return the line width in framebuffer pixels
    float width() const { return m_width; }

    /// Set the line color (default: white)
    void set_color(const Color &color);

    /**
     * \brief Render the segments
     *
     * This method handles begin(), draw_array(), and end() internally.
     */
    void draw();

private:
    size_t m_segment_count = 0;
    float m_width = 1.f;
};

NAMESPACE_END(nanogui)
