/*
    nanogui/button.h -- [Normal/Toggle/Radio/Popup] Button widget

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
 * \class Button button.h nanogui/button.h
 *
 * \brief [Normal/Toggle/Radio/Popup] Button widget.
 */
class NANOGUI_EXPORT Button : public Widget {
public:
    /// Flags to specify the button behavior (can be combined with binary OR)
    enum Flags {
        NormalButton = (1 << 0), // 1
        RadioButton  = (1 << 1), // 2
        ToggleButton = (1 << 2), // 4
        PopupButton  = (1 << 3)  // 8
    };

    /// The available icon positions.
    enum class IconPosition {
        Left,
        LeftCentered,
        RightCentered,
        Right
    };

    Button(Widget *parent, const std::string &caption = "Untitled", int icon = 0);

    const std::string &caption() const { return m_caption; }
    void set_caption(const std::string &caption) { m_caption = caption; }

    const Color &background_color() const { return m_background_color; }
    void set_background_color(const Color &background_color) { m_background_color = background_color; }

    const Color &text_color() const { return m_text_color; }
    void set_text_color(const Color &text_color) { m_text_color = text_color; }

    int icon() const { return m_icon; }
    void set_icon(int icon) { m_icon = icon; }

    int flags() const { return m_flags; }
    void set_flags(int button_flags) { m_flags = button_flags; }

    IconPosition icon_position() const { return m_icon_position; }
    void set_icon_position(IconPosition icon_position) { m_icon_position = icon_position; }

    bool pushed() const { return m_pushed; }
    void set_pushed(bool pushed) { m_pushed = pushed; }

    /// Set the push callback (for any type of button)
    std::function<void()> callback() const { return m_callback; }
    void set_callback(const std::function<void()> &callback) { m_callback = callback; }

    /// Set the change callback (for toggle buttons)
    std::function<void(bool)> change_callback() const { return m_change_callback; }
    void set_change_callback(const std::function<void(bool)> &callback) { m_change_callback = callback; }

    /// Set the button group (for radio buttons)
    void set_button_group(const std::vector<Button *> &button_group) { m_button_group = button_group; }
    const std::vector<Button *> &button_group() const { return m_button_group; }

    virtual Vector2i preferred_size(NVGcontext *ctx) const override;
    virtual bool mouse_enter_event(const Vector2i &p, bool enter) override;
    virtual bool mouse_button_event(const Vector2i &p, int button, bool down, int modifiers) override;
    virtual void draw(NVGcontext *ctx) override;
protected:
    std::string m_caption;
    int m_icon;
    IconPosition m_icon_position;
    bool m_pushed;
    int m_flags;
    Color m_background_color;
    Color m_text_color;
    std::function<void()> m_callback;
    std::function<void(bool)> m_change_callback;
    std::vector<Button *> m_button_group;
};

NAMESPACE_END(nanogui)
