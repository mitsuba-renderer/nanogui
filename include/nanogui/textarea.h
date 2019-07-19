/*
    nanogui/textarea.h -- Multi-line read-only text widget

    NanoGUI was developed by Wenzel Jakob <wenzel.jakob@epfl.ch>.
    The widget drawing code is based on the NanoVG demo application
    by Mikko Mononen.

    All rights reserved. Use of this source code is governed by a
    BSD-style license that can be found in the LICENSE.txt file.
*/
/** \file */

#pragma once

#include <nanogui/widget.h>
#include <cstdio>
#include <sstream>

NAMESPACE_BEGIN(nanogui)

/**
 * \class TextBox textarea.h nanogui/textarea.h
 *
 * \brief Multi-line read-only text widget, ideal for displaying
 * log messages etc.
 */
class NANOGUI_EXPORT TextArea : public Widget {
public:
    TextArea(Widget *parent);

    /// Set the used font
    void set_font(const std::string &font) { m_font = font; }

    /// Return the used font
    const std::string &font() const { return m_font; }

    /// Set the foreground color (applies to all subsequently added text)
    void set_foreground_color(const Color &color) {
        m_foreground_color = color;
    }

    /// Return the foreground color (applies to all subsequently added text)
    const Color &foreground_color() const {
        return m_foreground_color;
    }

    /// Set the widget's background color (a global property)
    void set_background_color(const Color &background_color) {
        m_background_color = background_color;
    }

    /// Return the widget's background color (a global property)
    const Color &background_color() const {
        return m_background_color;
    }

    /// Set the amount of padding to add around the text
    void set_padding(int padding) { m_padding = padding; }

    /// Return the amount of padding that is added around the text
    int padding() const { return m_padding; }

    /// Append text at the end of the widget
    void append(const std::string &text);

    /// Append a line of text at the bottom
    void append_line(const std::string &text) {
        append(text);
        append("\n");
    }

    /// Clear all current contents
    void clear();

    /* Widget implementation */
    virtual void draw(NVGcontext *ctx) override;
    virtual Vector2i preferred_size(NVGcontext *ctx) const override;

private:
    struct Block {
        Vector2i offset;
        std::string text;
        Color color;
    };

    std::vector<Block> m_blocks;
    Color m_foreground_color;
    Color m_background_color;
    std::string m_font;
    Vector2i m_offset, m_max_size;
    int m_padding;
};

NAMESPACE_END(nanogui)
