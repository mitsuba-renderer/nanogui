/*
    The text box widget was contributed by Christian Schueller.

    NanoGUI was developed by Wenzel Jakob <wenzel.jakob@epfl.ch>.
    The widget drawing code is based on the NanoVG demo application
    by Mikko Mononen.

    All rights reserved. Use of this source code is governed by a
    BSD-style license that can be found in the LICENSE.txt file.
*/
/**
 * \file nanogui/theme.h
 *
 * \brief Storage class for basic theme-related properties.
 */

#pragma once

#include <nanogui/vector.h>
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
    /// The standard font face (default: ``"sans"`` from ``resources/roboto_regular.ttf``).
    int m_font_sans_regular;
    /// The bold font face (default: ``"sans-bold"`` from ``resources/roboto_regular.ttf``).
    int m_font_sans_bold;
    /// The icon font face (default: ``"icons"`` from ``resources/entypo.ttf``).
    int m_font_icons;
    /// The monospace font face (default: ``"mono"`` from ``resources/inconsolata_regular.ttf``).
    int m_font_mono_regular;
    /**
     * The amount of scaling that is applied to each icon to fit the size of
     * NanoGUI widgets.  The default value is ``0.77f``, setting to e.g. higher
     * than ``1.0f`` is generally discouraged.
     */
    float m_icon_scale;

    /* Spacing-related parameters */
    /// The font size for all widgets other than buttons and textboxes (default: `` 16``).
    int m_standard_font_size;
    /// The font size for buttons (default: ``20``).
    int m_button_font_size;
    /// The font size for text boxes (default: ``20``).
    int m_text_box_font_size;
    /// Rounding radius for Window widget corners (default: ``2``).
    int m_window_corner_radius;
    /// Default size of Window widget titles (default: ``30``).
    int m_window_header_height;
    /// Size of drop shadow rendered behind the Window widgets (default: ``10``).
    int m_window_drop_shadow_size;
    /// Rounding radius for Button (and derived types) widgets (default: ``2``).
    int m_button_corner_radius;
    /// The border width for Tab_header widgets (default: ``0.75f``).
    float m_tab_border_width;
    /// The inner margin on a Tab_header widget (default: ``5``).
    int m_tab_inner_margin;
    /// The minimum size for buttons on a Tab_header widget (default: ``20``).
    int m_tab_min_button_width;
    /// The maximum size for buttons on a Tab_header widget (default: ``160``).
    int m_tab_max_button_width;
    /// Used to help specify what lies "in bound" for a Tab_header widget (default: ``20``).
    int m_tab_control_width;
    /// The amount of horizontal padding for a Tab_header widget (default: ``10``).
    int m_tab_button_horizontal_padding;
    /// The amount of vertical padding for a Tab_header widget (default: ``2``).
    int m_tab_button_vertical_padding;

    /* Generic colors */
    /**
     * The color of the drop shadow drawn behind widgets
     * (default: intensity=``0``, alpha=``128``; see \ref nanogui::Color::Color(int,int)).
     */
    Color m_drop_shadow;
    /**
     * The transparency color
     * (default: intensity=``0``, alpha=``0``; see \ref nanogui::Color::Color(int,int)).
     */
    Color m_transparent;
    /**
     * The dark border color
     * (default: intensity=``29``, alpha=``255``; see \ref nanogui::Color::Color(int,int)).
     */
    Color m_border_dark;
    /**
     * The light border color
     * (default: intensity=``92``, alpha=``255``; see \ref nanogui::Color::Color(int,int)).
     */
    Color m_border_light;
    /**
     * The medium border color
     * (default: intensity=``35``, alpha=``255``; see \ref nanogui::Color::Color(int,int)).
     */
    Color m_border_medium;
    /**
     * The text color
     * (default: intensity=``255``, alpha=``160``; see \ref nanogui::Color::Color(int,int)).
     */
    Color m_text_color;
    /**
     * The disable dtext color
     * (default: intensity=``255``, alpha=``80``; see \ref nanogui::Color::Color(int,int)).
     */
    Color m_disabled_text_color;
    /**
     * The text shadow color
     * (default: intensity=``0``, alpha=``160``; see \ref nanogui::Color::Color(int,int)).
     */
    Color m_text_color_shadow;
    /// The icon color (default: \ref nanogui::Theme::m_text_color).
    Color m_icon_color;

    /* Button colors */
    /**
     * The top gradient color for buttons in focus
     * (default: intensity=``64``, alpha=``255``; see \ref nanogui::Color::Color(int,int)).
     */
    Color m_button_gradient_top_focused;
    /**
     * The bottom gradient color for buttons in focus
     * (default: intensity=``48``, alpha=``255``; see \ref nanogui::Color::Color(int,int)).
     */
    Color m_button_gradient_bot_focused;
    /**
     * The top gradient color for buttons not in focus
     * (default: intensity=``74``, alpha=``255``; see \ref nanogui::Color::Color(int,int)).
     */
    Color m_button_gradient_top_unfocused;
    /**
     * The bottom gradient color for buttons not in focus
     * (default: intensity=``58``, alpha=``255``; see \ref nanogui::Color::Color(int,int)).
     */
    Color m_button_gradient_bot_unfocused;
    /**
     * The top gradient color for buttons currently pushed
     * (default: intensity=``41``, alpha=``255``; see \ref nanogui::Color::Color(int,int)).
     */
    Color m_button_gradient_top_pushed;
    /**
     * The bottom gradient color for buttons currently pushed
     * (default: intensity=``29``, alpha=``255``; see \ref nanogui::Color::Color(int,int)).
     */
    Color m_button_gradient_bot_pushed;

    /* Window colors */
    /**
     * The fill color for a Window that is not in focus
     * (default: intensity=``43``, alpha=``230``; see \ref nanogui::Color::Color(int,int)).
     */
    Color m_window_fill_unfocused;
    /**
     * The fill color for a Window that is in focus
     * (default: intensity=``45``, alpha=``230``; see \ref nanogui::Color::Color(int,int)).
     */
    Color m_window_fill_focused;
    /**
     * The title color for a Window that is not in focus
     * (default: intensity=``220``, alpha=``160``; see \ref nanogui::Color::Color(int,int)).
     */
    Color m_window_title_unfocused;
    /**
     * The title color for a Window that is in focus
     * (default: intensity=``255``, alpha=``190``; see \ref nanogui::Color::Color(int,int)).
     */
    Color m_window_title_focused;

    /**
     * The top gradient color for Window headings
     * (default: \ref nanogui::Theme::m_button_gradient_top_unfocused).
     */
    Color m_window_header_gradient_top;
    /**
     * The bottom gradient color for Window headings
     * (default: \ref nanogui::Theme::m_button_gradient_bot_unfocused).
     */
    Color m_window_header_gradient_bot;
    /// The Window header top separation color (default: \ref nanogui::Theme::m_border_light).
    Color m_window_header_sep_top;
    /// The Window header bottom separation color (default: \ref nanogui::Theme::m_border_dark).
    Color m_window_header_sep_bot;

    /**
     * The popup window color
     * (default: intensity=``50``, alpha=``255``; see \ref nanogui::Color::Color(int,int))).
     */
    Color m_window_popup;
    /**
     * The transparent popup window color
     * (default: intensity=``50``, alpha=``0``; see \ref nanogui::Color::Color(int,int))).
     */
    Color m_window_popup_transparent;

    /// Icon to use for check box widgets (default: ``FA_CHECK``).
    int m_check_box_icon;
    /// Icon to use for informational message dialog widgets (default: ``FA_INFO_CIRCLE``).
    int m_message_information_icon;
    /// Icon to use for interrogative message dialog widgets (default: ``FA_QUESTION_CIRCLE``).
    int m_message_question_icon;
    /// Icon to use for warning message dialog widgets (default: ``FA_EXCLAMATION_TRINAGLE``).
    int m_message_warning_icon;
    /// Icon to use on message dialog alt button (default: ``FA_CIRCLE_WITH_CROSS``).
    int m_message_alt_button_icon;
    /// Icon to use on message_dialog primary button (default: ``FA_CHECK``).
    int m_message_primary_button_icon;
    /// Icon to use for Popup_button widgets opening to the right (default: ``FA_CHEVRON_RIGHT``).
    int m_popup_chevron_right_icon;
    /// Icon to use for Popup_button widgets opening to the left (default: ``FA_CHEVRON_LEFT``).
    int m_popup_chevron_left_icon;
    /// Icon to use when a text box has an up toggle (e.g. IntBox) (default: ``FA_CHEVRON_UP``).
    int m_text_box_up_icon;
    /// Icon to use when a text box has a down toggle (e.g. IntBox) (default: ``FA_CHEVRON_DOWN``).
    int m_text_box_down_icon;

protected:
    /// Default destructor does nothing; allows for inheritance.
    virtual ~Theme() { };
};

NAMESPACE_END(nanogui)
