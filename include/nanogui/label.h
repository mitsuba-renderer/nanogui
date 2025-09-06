/*
    nanogui/label.h -- Text label with an arbitrary font, color, and size

    NanoGUI was developed by Wenzel Jakob <wenzel.jakob@epfl.ch>.
    The widget drawing code is based on the NanoVG demo application
    by Mikko Mononen.

    All rights reserved. Use of this source code is governed by a
    BSD-style license that can be found in the LICENSE.txt file.
*/
/** \file */

#pragma once

#include <nanogui/widget.h>

NAMESPACE_BEGIN(nanogui)

/**
 * \class Label label.h nanogui/label.h
 *
 * \brief Text label widget.
 *
 * The font and color can be customized. When \ref Widget::set_fixed_width()
 * is used, the text is wrapped when it surpasses the specified width.
 */
class NANOGUI_EXPORT Label : public Widget {
public:
    Label(Widget *parent, std::string_view caption,
          std::string_view font = "sans", int font_size = -1);

    /// Get the label's text caption
    std::string_view caption() const { return m_caption; }
    /// Set the label's text caption
    void set_caption(std::string_view caption) {
        if (m_caption != caption) {
            m_caption = caption;
            preferred_size_changed();
        }
    }

    /// Set the currently active font (2 are available by default: 'sans' and 'sans-bold')
    void set_font(std::string_view font) {
        if (m_font != font) {
            m_font = font;
            preferred_size_changed();
        }
    }
    /// Get the currently active font
    std::string_view font() const { return m_font; }

    /// Get the label color
    Color color() const { return m_color; }
    /// Set the label color
    void set_color(const Color& color) { m_color = color; }

    /// Set the \ref Theme used to draw this widget
    virtual void set_theme(Theme *theme) override;

protected:
    /// Compute the size needed to fully display the label
    virtual Vector2i preferred_size_impl(NVGcontext *ctx) const override;

public:

    /// Draw the label
    virtual void draw(NVGcontext *ctx) override;
protected:
    std::string m_caption;
    std::string m_font;
    Color m_color;
};

NAMESPACE_END(nanogui)
