/*
    nanogui/glcanvas.h -- Canvas widget for rendering OpenGL content

    NanoGUI was developed by Wenzel Jakob <wenzel.jakob@epfl.ch>.
    The widget drawing code is based on the NanoVG demo application
    by Mikko Mononen.

    All rights reserved. Use of this source code is governed by a
    BSD-style license that can be found in the LICENSE.txt file.
*/

#pragma once

#include <iostream>

#include <nanogui/widget.h>
#include <nanogui/opengl.h>
#include <nanogui/glutil.h>

NAMESPACE_BEGIN(nanogui)

/**
 * \class GLCanvas glcanvas.h nanogui/glcanvas.h
 * \brief Canvas widget for rendering OpenGL content
 *
 * Canvas widget that can be used to display arbitrary OpenGL content. This is
 * useful to display and manipulate 3D objects as part of an interactive
 * application. The implementation uses scissoring to ensure that rendered
 * objects don't spill into neighboring widgets.
 *
 * Usage: override `draw_gl` in subclasses to provide custom drawing code.
 */
class NANOGUI_EXPORT GLCanvas : public Widget {
public:
    GLCanvas(Widget *parent);

    /// Return the background color
    const Color &background_color() const { return m_background_color; }
    /// Set the background color
    void set_background_color(const Color &background_color) { m_background_color = background_color; }

    /// Set whether to draw the widget border or not
    void set_draw_border(const bool b_draw_border) { m_draw_border = b_draw_border; }
    /// Return whether the widget border gets drawn or not
    const bool &draw_border() const { return m_draw_border; }

    /// Draw the canvas
    virtual void draw(NVGcontext *ctx) override;

    /// Draw the GL scene. Override this method to draw the actual GL content.
    virtual void draw_gl() { }
protected:
    /// Internal helper function for drawing the widget border
    void draw_widget_border(NVGcontext* ctx) const;

protected:
    Color m_background_color;
    bool m_draw_border;
};

NAMESPACE_END(nanogui)
