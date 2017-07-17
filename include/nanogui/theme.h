/*
    nanogui/theme.h -- Storage class for basic theme-related properties

    The text box widget was contributed by Christian Schueller.

    NanoGUI was developed by Wenzel Jakob <wenzel.jakob@epfl.ch>.
    The widget drawing code is based on the NanoVG demo application
    by Mikko Mononen.

    All rights reserved. Use of this source code is governed by a
    BSD-style license that can be found in the LICENSE.txt file.
*/
/** \file */

#pragma once

#include <nanogui/common.h>
#include <nanogui/object.h>

NAMESPACE_BEGIN(nanogui)

/**
 * \class Theme theme.h nanogui/theme.h
 *
 * \brief Storage class for basic theme-related properties.
 */
class NANOGUI_EXPORT Theme : public Object {
public:
    Theme(NVGcontext *ctx);

    /* Fonts */
    int m_font_normal;
    int m_font_bold;
    int m_font_icons;

    /* Spacing-related parameters */
    int m_standard_font_size;
    int m_button_font_size;
    int m_text_box_font_size;
    int m_window_corner_radius;
    int m_window_header_height;
    int m_window_drop_shadow_size;
    int m_button_corner_radius;
    float m_tab_border_width;
    int m_tab_inner_margin;
    int m_tab_min_button_width;
    int m_tab_max_button_width;
    int m_tab_control_width;
    int m_tab_button_horizontal_padding;
    int m_tab_button_vertical_padding;

    /* Generic colors */
    Color m_drop_shadow;
    Color m_transparent;
    Color m_border_dark;
    Color m_border_light;
    Color m_border_medium;
    Color m_text_color;
    Color m_disabled_text_color;
    Color m_text_color_shadow;
    Color m_icon_color;

    /* Button colors */
    Color m_button_gradient_top_focused;
    Color m_button_gradient_bot_focused;
    Color m_button_gradient_top_unfocused;
    Color m_button_gradient_bot_unfocused;
    Color m_button_gradient_top_pushed;
    Color m_button_gradient_bot_pushed;

    /* Window colors */
    Color m_window_fill_unfocused;
    Color m_window_fill_focused;
    Color m_window_title_unfocused;
    Color m_window_title_focused;

    Color m_window_header_gradient_top;
    Color m_window_header_gradient_bot;
    Color m_window_header_sep_top;
    Color m_window_header_sep_bot;

    Color m_window_popup;
    Color m_window_popup_transparent;
protected:
    virtual ~Theme() { };
};

NAMESPACE_END(nanogui)
