/*
    nanogui/colorwheel.h -- fancy analog widget to select a color value

    This widget was contributed by Dmitriy Morozov.

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
 * \class ColorWheel colorwheel.h nanogui/colorwheel.h
 *
 * \brief Fancy analog widget to select a color value.
 */
class NANOGUI_EXPORT ColorWheel : public Widget {
public:
    ColorWheel(Widget *parent, const Color& color = Color(1.0f, 0.0f, 0.0f, 1.0f));

    /// Set the change callback
    std::function<void(const Color &)> callback() const                  { return m_callback;     }
    void set_callback(const std::function<void(const Color &)> &callback) { m_callback = callback; }

    /// Get the current color
    Color color() const;
    /// Set the current color
    void set_color(const Color& color);

    virtual Vector2i preferred_size(NVGcontext *ctx) const override;
    virtual void draw(NVGcontext *ctx) override;
    virtual bool mouse_button_event(const Vector2i &p, int button, bool down, int modifiers) override;
    virtual bool mouse_drag_event(const Vector2i &p, const Vector2i &rel, int button, int modifiers) override;
private:
    enum Region {
        None = 0,
        InnerTriangle = 1,
        OuterCircle = 2,
        Both = 3
    };

    Color hue2rgb(float h) const;
    Region adjust_position(const Vector2i &p, Region considered_regions = Both);

protected:
    float m_hue;
    float m_white;
    float m_black;
    Region m_drag_region;
    std::function<void(const Color &)> m_callback;
};

NAMESPACE_END(nanogui)
