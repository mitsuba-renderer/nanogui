/*
    nanogui/window.h -- Top-level window widget

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
 * \class Window window.h nanogui/window.h
 *
 * \brief Top-level window widget.
 */
class NANOGUI_EXPORT Window : public Widget {
    friend class Popup;
public:
    Window(Widget *parent, const std::string &title = "Untitled");

    /// Return the window title
    const std::string &title() const { return m_title; }
    /// Set the window title
    void set_title(const std::string &title) { m_title = title; }

    /// Is this a model dialog?
    bool modal() const { return m_modal; }
    /// Set whether or not this is a modal dialog
    void set_modal(bool modal) { m_modal = modal; }

    /// Return the panel used to house window buttons
    Widget *button_panel();

    /// Dispose the window
    void dispose();

    /// Center the window in the current \ref Screen
    void center();

    /// Draw the window
    virtual void draw(NVGcontext *ctx) override;
    /// Handle mouse enter/leave events
    virtual bool mouse_enter_event(const Vector2i &p, bool enter) override;
    /// Handle window drag events
    virtual bool mouse_drag_event(const Vector2i &p, const Vector2i &rel, int button, int modifiers) override;
    /// Handle mouse events recursively and bring the current window to the top
    virtual bool mouse_button_event(const Vector2i &p, int button, bool down, int modifiers) override;
    /// Accept scroll events and propagate them to the widget under the mouse cursor
    virtual bool scroll_event(const Vector2i &p, const Vector2f &rel) override;
    /// Compute the preferred size of the widget
    virtual Vector2i preferred_size(NVGcontext *ctx) const override;
    /// Invoke the associated layout generator to properly place child widgets, if any
    virtual void perform_layout(NVGcontext *ctx) override;
protected:
    /// Internal helper function to maintain nested window position values; overridden in \ref Popup
    virtual void refresh_relative_placement();
protected:
    std::string m_title;
    Widget *m_button_panel;
    bool m_modal;
    bool m_drag;
};

NAMESPACE_END(nanogui)
