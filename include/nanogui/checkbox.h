/*
    nanogui/checkbox.h -- Two-state check box widget

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
 * \class CheckBox checkbox.h nanogui/checkbox.h
 *
 * \brief Two-state check box widget.
 */
class NANOGUI_EXPORT CheckBox : public Widget {
public:
    CheckBox(Widget *parent, const std::string &caption = "Untitled",
             const std::function<void(bool)> &callback = std::function<void(bool)>());

    const std::string &caption() const { return m_caption; }
    void set_caption(const std::string &caption) { m_caption = caption; }

    const bool &checked() const { return m_checked; }
    void set_checked(const bool &checked) { m_checked = checked; }

    const bool &pushed() const { return m_pushed; }
    void set_pushed(const bool &pushed) { m_pushed = pushed; }

    std::function<void(bool)> callback() const { return m_callback; }
    void set_callback(const std::function<void(bool)> &callback) { m_callback = callback; }

    virtual bool mouse_button_event(const Vector2i &p, int button, bool down, int modifiers) override;
    virtual Vector2i preferred_size(NVGcontext *ctx) const override;
    virtual void draw(NVGcontext *ctx) override;
protected:
    std::string m_caption;
    bool m_pushed, m_checked;
    std::function<void(bool)> m_callback;
};

NAMESPACE_END(nanogui)
