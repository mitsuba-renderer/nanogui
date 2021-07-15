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
        Window(Widget* parent, const std::string& title = "Untitled", bool resizable = false);

        /// Return the window title
        const std::string& title() const { return m_title; }
        /// Set the window title
        void set_title(const std::string& title) { m_title = title; }

        /// Is this a model dialog?
        bool modal() const { return m_modal; }
        /// Set whether or not this is a modal dialog
        void set_modal(bool modal) { m_modal = modal; }
        /// Drow size for this window?
        bool draw_shadow() const { return m_draw_shadow; }
        /// Set whether or not to draw shadow for this window
        void set_draw_shadow(bool draw_shadow) { m_draw_shadow = draw_shadow; }
        /// Is this a resizable window?
        bool resizable() const { return m_resizable; }
        /// Set whether or not this window is resizable
        void set_resizable(bool resizable) { m_resizable = resizable; }
        /// Can this window be moved?
        bool can_move() const { return m_can_move; }
        /// Set whether the window can move
        void set_can_move(bool can_move) { m_can_move = can_move; }
        /// Can this window snap to other windows?
        bool can_snap() const { return m_can_snap; }
        /// Set whether the window can snap
        void set_can_snap(bool can_snap) { m_can_snap = can_snap; }

        /// Return the panel used to house window buttons
        Widget* button_panel();

        /// Dispose the window
        void dispose();

        /// Center the window in the current \ref Screen
        void center();

        /// Draw the window
        virtual void draw(NVGcontext* ctx) override;
        /// Handle mouse enter/leave events
        virtual bool mouse_enter_event(const Vector2i& p, bool enter) override;
        /// Handle window drag events
        virtual bool mouse_drag_event(const Vector2i& p, const Vector2i& rel, int button, int modifiers) override;
        /// Handle a mouse motion event (default implementation: propagate to children)
        virtual bool mouse_motion_event(const Vector2i& p, const Vector2i& rel, int button, int modifiers) override;
        /// Handle mouse events recursively and bring the current window to the top
        virtual bool mouse_button_event(const Vector2i& p, int button, bool down, int modifiers) override;
        /// Accept scroll events and propagate them to the widget under the mouse cursor
        virtual bool scroll_event(const Vector2i& p, const Vector2f& rel) override;
        /// Compute the preferred size of the widget
        virtual Vector2i preferred_size(NVGcontext* ctx) const override;
        /// Invoke the associated layout generator to properly place child widgets, if any
        virtual void perform_layout(NVGcontext* ctx) override;
    protected:
        /// Internal helper function to maintain nested window position values; overridden in \ref Popup
        virtual void refresh_relative_placement();
        virtual bool check_horizontal_resize(const Vector2i& mousePos);
        virtual bool check_vertical_resize(const Vector2i& mousePos);
    protected:
        std::string m_title;
        Widget* m_button_panel;
        bool m_modal;
        bool m_drag;
        bool m_resize;
        Vector2i m_resize_dir;
        Vector2i m_min_size;
        Vector2i m_first_size;
        bool m_draw_shadow;
        bool m_resizable;
        bool m_can_move;
        bool m_can_snap;
        int m_snap_offset;
        Vector2i m_snap_tot_rel;
        Vector2i m_snap_init;
};

NAMESPACE_END(nanogui)
