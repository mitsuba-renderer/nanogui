/*
    src/theme.cpp -- Storage class for basic theme-related properties

    The text box widget was contributed by Christian Schueller.

    NanoGUI was developed by Wenzel Jakob <wenzel.jakob@epfl.ch>.
    The widget drawing code is based on the NanoVG demo application
    by Mikko Mononen.

    All rights reserved. Use of this source code is governed by a
    BSD-style license that can be found in the LICENSE.txt file.
*/

#include <nanogui/theme.h>
#include <nanogui/opengl.h>
#include <nanogui/icons.h>
#include <nanogui_resources.h>

NAMESPACE_BEGIN(nanogui)

Theme::Theme(NVGcontext *ctx) {
    m_standard_font_size                 = 16;
    m_button_font_size                   = 20;
    m_text_box_font_size                 = 20;
    m_icon_scale                         = 0.60f;

    m_window_corner_radius               = 2;
    m_window_header_height               = 30;
    m_window_drop_shadow_size            = 10;
    m_button_corner_radius               = 2;
    m_tab_border_width                   = 0.75f;
    m_tab_inner_margin                   = 5;
    m_tab_min_button_width               = 20;
    m_tab_max_button_width               = 160;
    m_tab_control_width                  = 20;
    m_tab_button_horizontal_padding      = 10;
    m_tab_button_vertical_padding        = 2;

    m_drop_shadow                        = Color(0, 128);
    m_transparent                        = Color(0, 0);
    m_border_dark                        = Color(29, 255);
    m_border_light                       = Color(92, 255);
    m_border_medium                      = Color(35, 255);
    m_text_color                         = Color(255, 160);
    m_disabled_text_color                = Color(255, 80);
    m_text_color_shadow                  = Color(0, 160);
    m_icon_color                         = m_text_color;

    m_button_gradient_top_focused        = Color(64, 255);
    m_button_gradient_bot_focused        = Color(48, 255);
    m_button_gradient_top_unfocused      = Color(74, 255);
    m_button_gradient_bot_unfocused      = Color(58, 255);
    m_button_gradient_top_pushed         = Color(41, 255);
    m_button_gradient_bot_pushed         = Color(29, 255);

    /* Window-related */
    m_window_fill_unfocused              = Color(43, 230);
    m_window_fill_focused                = Color(45, 230);
    m_window_title_unfocused             = Color(220, 160);
    m_window_title_focused               = Color(255, 190);

    m_window_header_gradient_top         = m_button_gradient_top_unfocused;
    m_window_header_gradient_bot         = m_button_gradient_bot_unfocused;
    m_window_header_sep_top              = m_border_light;
    m_window_header_sep_bot              = m_border_dark;

    m_window_popup                       = Color(50, 255);
    m_window_popup_transparent           = Color(50, 0);

    m_check_box_icon                    = FA_CHECK;
    m_message_information_icon          = FA_INFO_CIRCLE;
    m_message_question_icon             = FA_QUESTION_CIRCLE;
    m_message_warning_icon              = FA_EXCLAMATION_TRIANGLE;
    m_message_alt_button_icon           = FA_TIMES_CIRCLE;
    m_message_primary_button_icon       = FA_CHECK;
    m_popup_chevron_right_icon          = FA_CHEVRON_RIGHT;
    m_popup_chevron_left_icon           = FA_CHEVRON_LEFT;
    m_text_box_up_icon                  = FA_CHEVRON_UP;
    m_text_box_down_icon                = FA_CHEVRON_DOWN;

    m_font_sans_regular = nvgCreateFontMem(ctx, "sans", (uint8_t *) roboto_regular_ttf,
                                           roboto_regular_ttf_size, 0);
    m_font_sans_bold = nvgCreateFontMem(ctx, "sans-bold", (uint8_t *) roboto_bold_ttf,
                                        roboto_bold_ttf_size, 0);
    m_font_icons = nvgCreateFontMem(ctx, "icons", (uint8_t *) fontawesome_solid_ttf,
                                    fontawesome_solid_ttf_size, 0);
    m_font_mono_regular = nvgCreateFontMem(ctx, "mono", (uint8_t *) inconsolata_regular_ttf,
                                           inconsolata_regular_ttf_size, 0);

    if (m_font_sans_regular == -1 || m_font_sans_bold == -1 ||
        m_font_icons == -1 || m_font_mono_regular == -1)
        throw std::runtime_error("Could not load fonts!");
}

NAMESPACE_END(nanogui)
