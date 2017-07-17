/*
    nanogui/colorpicker.h -- push button with a popup to tweak a color value

    This widget was contributed by Christian Schueller.

    NanoGUI was developed by Wenzel Jakob <wenzel.jakob@epfl.ch>.
    The widget drawing code is based on the NanoVG demo application
    by Mikko Mononen.

    All rights reserved. Use of this source code is governed by a
    BSD-style license that can be found in the LICENSE.txt file.
*/
/** \file */

#pragma once

#include <nanogui/popupbutton.h>

NAMESPACE_BEGIN(nanogui)

/**
 * \class ColorPicker colorpicker.h nanogui/colorpicker.h
 *
 * \brief Push button with a popup to tweak a color value.
 */
class NANOGUI_EXPORT ColorPicker : public PopupButton {
public:
    ColorPicker(Widget *parent, const Color& color = Color(1.0f, 0.0f, 0.0f, 1.0f));

    /// Set the change callback
    std::function<void(const Color &)> callback() const                  { return m_callback; }
    void set_callback(const std::function<void(const Color &)> &callback) { m_callback = callback; }

    /// Get the current color
    Color color() const;
    /// Set the current color
    void set_color(const Color& color);
protected:
    std::function<void(const Color &)> m_callback;
    ColorWheel *m_color_wheel;
    Button *m_pick_button;
};

NAMESPACE_END(nanogui)
