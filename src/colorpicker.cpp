/*
    src/colorpicker.cpp -- push button with a popup to tweak a color value

    This widget was contributed by Christian Schueller.

    NanoGUI was developed by Wenzel Jakob <wenzel.jakob@epfl.ch>.
    The widget drawing code is based on the NanoVG demo application
    by Mikko Mononen.

    All rights reserved. Use of this source code is governed by a
    BSD-style license that can be found in the LICENSE.txt file.
*/

#include <nanogui/colorpicker.h>
#include <nanogui/layout.h>
#include <nanogui/colorwheel.h>

NAMESPACE_BEGIN(nanogui)

ColorPicker::ColorPicker(Widget *parent, const Color& color) : PopupButton(parent, "") {
    set_background_color(color);
    Popup *popup = this->popup();
    popup->set_layout(new GroupLayout());

    m_color_wheel = new ColorWheel(popup);
    m_pick_button = new Button(popup, "Pick");
    m_pick_button->set_fixed_size(Vector2i(100, 25));

    PopupButton::set_change_callback([&](bool) {
        set_color(background_color());
        m_callback(background_color());
    });

    m_color_wheel->set_callback([&](const Color &value) {
        m_pick_button->set_background_color(value);
        m_pick_button->set_text_color(value.contrasting_color());
        m_callback(value);
    });

    m_pick_button->set_callback([&]() {
        Color value = m_color_wheel->color();
        set_pushed(false);
        set_color(value);
        m_callback(value);
    });
}

Color ColorPicker::color() const {
    return background_color();
}

void ColorPicker::set_color(const Color& color) {
    /* Ignore set_color() calls when the user is currently editing */
    if (!m_pushed) {
        Color fg = color.contrasting_color();
        set_background_color(color);
        set_text_color(fg);
        m_color_wheel->set_color(color);
        m_pick_button->set_background_color(color);
        m_pick_button->set_text_color(fg);
    }
}

NAMESPACE_END(nanogui)
