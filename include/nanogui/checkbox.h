/*
    NanoGUI was developed by Wenzel Jakob <wenzel.jakob@epfl.ch>.
    The widget drawing code is based on the NanoVG demo application
    by Mikko Mononen.

    All rights reserved. Use of this source code is governed by a
    BSD-style license that can be found in the LICENSE.txt file.
*/
/**
 * \file nanogui/checkbox.h
 *
 * \brief Two-state check box Widget.
 */

#pragma once

#include <nanogui/widget.h>

NAMESPACE_BEGIN(nanogui)

/**
 * \class CheckBox checkbox.h nanogui/checkbox.h
 *
 * \brief Two-state check box widget.
 *
 * \remark
 *     This class overrides \ref nanogui::Widget::mIconExtraScale to be ``1.2f``,
 *     which affects all subclasses of this Widget.  Subclasses must explicitly
 *     set a different value if needed (e.g., in their constructor).
 */
class NANOGUI_EXPORT CheckBox : public Widget {
public:
    /**
     * Adds a CheckBox to the specified ``parent``.
     *
     * \param parent
     *     The Widget to add this CheckBox to.
     *
     * \param caption
     *     The caption text of the CheckBox (default ``"Untitled"``).
     *
     * \param callback
     *     If provided, the callback to execute when the CheckBox is checked or
     *     unchecked.  Default parameter function does nothing.  See
     *     \ref nanogui::CheckBox::mPushed for the difference between "pushed"
     *     and "checked".
     */
    CheckBox(Widget *parent, const std::string &caption = "Untitled",
             const std::function<void(bool)> &callback = std::function<void(bool)>());

    /// The caption of this CheckBox.
   const std::string &caption() const { return m_caption; }

    /// Sets the caption of this CheckBox.
    void set_caption(const std::string &caption) { m_caption = caption; }

    /// Whether or not this CheckBox is currently checked.
    const bool &checked() const { return m_checked; }

    /// Sets whether or not this CheckBox is currently checked.
    void set_checked(const bool &checked) { m_checked = checked; }

    /// Whether or not this CheckBox is currently pushed.  See \ref nanogui::CheckBox::m_pushed.
    const bool &pushed() const { return m_pushed; }
    void set_pushed(const bool &pushed) { m_pushed = pushed; }

    /// Returns the current callback of this CheckBox.
    std::function<void(bool)> callback() const { return m_callback; }

    /// Sets the callback to be executed when this CheckBox is checked / unchecked.
    void set_callback(const std::function<void(bool)> &callback) { m_callback = callback; }

    /// Mouse button event processing for this check box
    virtual bool mouse_button_event(const Vector2i &p, int button, bool down, int modifiers) override;

    /// The preferred size of this CheckBox.
    virtual Vector2i preferred_size(NVGcontext *ctx) const override;

    /// Draws this CheckBox.
    virtual void draw(NVGcontext *ctx) override;
protected:
    /// The caption text of this CheckBox.
    std::string m_caption;
    /**
     * Internal tracking variable to distinguish between mouse click and release.
     * \ref nanogui::CheckBox::m_callback is only called upon release.  See
     * \ref nanogui::CheckBox::mouse_button_event for specific conditions.
     */
    bool m_pushed;

    /// Whether or not this CheckBox is currently checked or unchecked.
    bool m_checked;

    /// The function to execute when \ref nanogui::CheckBox::m_checked is changed.
    std::function<void(bool)> m_callback;
};

NAMESPACE_END(nanogui)
