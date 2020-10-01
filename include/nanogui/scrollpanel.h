/*
    nanogui/ScrollPanel.h -- Adds a vertical scrollbar around a widget
    that is too big to fit into a certain area

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
 * \class ScrollPanel ScrollPanel.h nanogui/ScrollPanel.h
 *
 * \brief Adds a vertical scrollbar around a widget that is too big to fit into
 *        a certain area.
 */
    class NANOGUI_EXPORT ScrollPanel : public Widget {
    public:
        enum class ScrollTypes
        {
            Horizontal,
            Vertical,
            Both,
            None
        };
        ScrollPanel(Widget* parent);
        ScrollPanel(Widget* parent, ScrollTypes scroll_type);
        // functions For Readability
        bool VScrollable() { return (m_scroll_type == ScrollTypes::Vertical || m_scroll_type == ScrollTypes::Both); }
        bool HScrollable() { return (m_scroll_type == ScrollTypes::Horizontal || m_scroll_type == ScrollTypes::Both); }

        /**
         * Return the current scroll amount as a value between 0 and 1. 0 means
         * scrolled to the top and 1 to the bottom.
         */
        Vector2f scroll() const { return m_scroll; }

        /**
         * Set the scroll amount to a value between 0 and 1. 0 means scrolled to
         * the top and 1 to the bottom.
         */
        void set_scroll(float scroll) { m_scroll = scroll; }

        void set_scroll_type(ScrollTypes scroll_type) { m_scroll_type = scroll_type; }
        ScrollTypes scroll_type() { return m_scroll_type; }

        virtual void perform_layout(NVGcontext* ctx) override;
        virtual Vector2i preferred_size(NVGcontext* ctx) const override;
        virtual bool mouse_button_event(const Vector2i& p, int button, bool down,
            int modifiers) override;
        virtual bool mouse_drag_event(const Vector2i& p, const Vector2i& rel,
            int button, int modifiers) override;
        virtual bool scroll_event(const Vector2i& p, const Vector2f& rel) override;
        virtual void draw(NVGcontext* ctx) override;

    protected:

        Vector2i m_child_preferred_size;
        Vector2i m_child_pos;
        Vector2f m_scroll;
        bool m_scrolling_x, m_scrolling_y;
        bool m_update_layout;
        ScrollTypes m_scroll_type;
};

NAMESPACE_END(nanogui)
