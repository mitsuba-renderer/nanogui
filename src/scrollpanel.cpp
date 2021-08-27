/*
    src/ScrollPanel.cpp -- Adds a vertical scrollbar around a widget
    that is too big to fit into a certain area

    NanoGUI was developed by Wenzel Jakob <wenzel.jakob@epfl.ch>.
    The widget drawing code is based on the NanoVG demo application
    by Mikko Mononen.

    All rights reserved. Use of this source code is governed by a
    BSD-style license that can be found in the LICENSE.txt file.
*/

#include <nanogui/scrollpanel.h>
#include <nanogui/theme.h>
#include <nanogui/opengl.h>

NAMESPACE_BEGIN(nanogui)

ScrollPanel::ScrollPanel(Widget* parent)
    : Widget(parent), m_child_preferred_size(Vector2i(0, 0)),
    m_scroll(0.f, 0.f), m_update_layout(false), m_scroll_type(ScrollTypes::Vertical) { }

ScrollPanel::ScrollPanel(Widget* parent, ScrollTypes scroll_type)
    : Widget(parent), m_child_preferred_size(Vector2i(0, 0)),
    m_scroll(0.f, 0.f), m_update_layout(false), m_scroll_type(scroll_type) { }

void ScrollPanel::perform_layout(NVGcontext* ctx) {
    Widget::perform_layout(ctx);

    if (m_children.empty())
        return;
    if (m_children.size() > 1)
        throw std::runtime_error("ScrollPanel should have one child.");

    Widget* child = m_children[0];
    m_child_preferred_size = child->preferred_size(ctx);
    m_child_pos = child->position();

    Vector2i new_child_pos = Vector2i(0);
    Vector2i new_child_size = Vector2i(m_size);

    if (m_child_preferred_size.y() > m_size.y() && VScrollable() && VScrollable()) {
        new_child_pos.y() = -m_scroll.y() * (m_child_preferred_size.y() - m_size.y());
        new_child_size.y() = m_child_preferred_size.y();
    }
    else
        m_scroll.y() = 0.f;

    if (m_child_preferred_size.x() > m_size.x() && HScrollable()) {
        new_child_pos.x() = -m_scroll.x() * (m_child_preferred_size.x() - m_size.x());
        new_child_size.x() = m_child_preferred_size.x();
    }
    else
        m_scroll.x() = 0.f;

    child->set_position(new_child_pos);
    child->set_size(new_child_size);

    child->perform_layout(ctx);
}

Vector2i ScrollPanel::preferred_size(NVGcontext* ctx) const {
    if (m_children.empty())
        return Vector2i(0);
    return m_children[0]->preferred_size(ctx) + Vector2i(12, 0);
}

bool ScrollPanel::mouse_drag_event(const Vector2i& p, const Vector2i& rel, int button, int modifiers) {

    if ((m_scrolling_y || m_scrolling_x) && !m_children.empty() && (m_child_preferred_size.y() > m_size.y() || m_child_preferred_size.x() > m_size.x())) {
        if (m_scrolling_y && m_child_preferred_size.y() > m_size.y() && VScrollable())
        {
            float scrollh = height() * std::min(1.f, height() / (float)m_child_preferred_size.y());
            m_scroll.y() = std::max(0.f, std::min(1.f, m_scroll.y() + rel.y() / (m_size.y() - 8.f - scrollh)));
        }
        if (m_scrolling_x && m_child_preferred_size.x() > m_size.x() && HScrollable())
        {
            float scrollw = width() * std::min(1.f, width() / (float)m_child_preferred_size.x());
            m_scroll.x() = std::max(0.f, std::min(1.f, m_scroll.x() + rel.x() / (m_size.x() - 8.f - scrollw)));
        }
        m_update_layout = true;
        return true;
    }
    return Widget::mouse_drag_event(p, rel, button, modifiers);
}

bool ScrollPanel::mouse_button_event(const Vector2i& p, int button, bool down, int modifiers) {

    bool OverVertical = m_child_preferred_size.y() > m_size.y() &&
        p.x() > m_pos.x() + m_size.x() - 13 &&
        p.x() < m_pos.x() + m_size.x() - 4;
    bool OverHorizontal = m_child_preferred_size.x() > m_size.x() &&
        p.y() > m_pos.y() + m_size.y() - 13 &&
        p.y() < m_pos.y() + m_size.y() - 4;
    if (down && button == GLFW_MOUSE_BUTTON_1 && !m_children.empty() && VScrollable() && OverVertical) {

        m_scrolling_y = true;

        int scrollh = (int)(height() *
            std::min(1.f, height() / (float)m_child_preferred_size.y()));
        int start = (int)(m_pos.y() + 4 + 1 + (m_size.y() - 8 - scrollh) * m_scroll.y());

        float delta = 0.f;

        if (p.y() < start)
            delta = -m_size.y() / (float)m_child_preferred_size.y();
        else if (p.y() > start + scrollh)
            delta = m_size.y() / (float)m_child_preferred_size.y();

        m_scroll.y() = std::max(0.f, std::min(1.f, m_scroll.y() + delta * 0.98f));

        m_children[0]->set_position(
            Vector2i(0, -m_scroll.y() * (m_child_preferred_size.y() - m_size.y())));
        m_update_layout = true;
        return true;
    }
    else m_scrolling_y = false;
    if (down && button == GLFW_MOUSE_BUTTON_1 && !m_children.empty() && HScrollable() && OverHorizontal) {

        m_scrolling_x = true;
        int scrollw = (int)(width() *
            std::min(1.f, width() / (float)m_child_preferred_size.x()));
        int start = (int)(m_pos.x() + 4 + 1 + (m_size.x() - 8 - scrollw) * m_scroll.x());

        float delta = 0.f;

        if (p.x() < start)
            delta = -m_size.x() / (float)m_child_preferred_size.x();
        else if (p.x() > start + scrollw)
            delta = m_size.x() / (float)m_child_preferred_size.x();

        m_scroll.x() = std::max(0.f, std::min(1.f, m_scroll.x() + delta * 0.98f));

        m_children[0]->set_position(
            Vector2i(0, -m_scroll.x() * (m_child_preferred_size.x() - m_size.x())));
        m_update_layout = true;
        return true;
    }
    else m_scrolling_x = false;
    if (OverVertical || OverHorizontal)//if mouse is on scrollbar, dont check the other widgets

        return true;
    if (Widget::mouse_button_event(p, button, down, modifiers))
        return true;

    return false;
}

bool ScrollPanel::scroll_event(const Vector2i& p, const Vector2f& rel) {
    if (!m_children.empty() && m_child_preferred_size.y() > m_size.y() && VScrollable()) {
        auto child = m_children[0];
        float scroll_amount = rel.y() * m_size.y() * .25f;

        m_scroll.y() = std::max(0.f, std::min(1.f, m_scroll.y() - scroll_amount / m_child_preferred_size.y()));

        Vector2i old_pos = child->position();
        child->set_position(Vector2i(old_pos.x(), -m_scroll.y() * (m_child_preferred_size.y() - m_size.y())));
        Vector2i new_pos = child->position();
        m_update_layout = true;
        child->mouse_motion_event(p - m_pos, old_pos - new_pos, 0, 0);

        return true;
    }
    if (!m_children.empty() && m_child_preferred_size.x() > m_size.x() && HScrollable()) {
        auto child = m_children[0];
        float scroll_amount = rel.y() * m_size.x() * .25f;

        m_scroll.x() = std::max(0.f, std::min(1.f, m_scroll.x() - scroll_amount / m_child_preferred_size.x()));

        Vector2i old_pos = child->position();
        child->set_position(Vector2i(-m_scroll.x() * (m_child_preferred_size.x() - m_size.x()), old_pos.y()));
        Vector2i new_pos = child->position();
        m_update_layout = true;
        child->mouse_motion_event(p - m_pos, old_pos - new_pos, 0, 0);
        return true;
    }
    return Widget::scroll_event(p, rel);
}

void ScrollPanel::draw(NVGcontext* ctx) {
    if (m_children.empty())
        return;
    Widget* child = m_children[0];

    if (m_update_layout) {
        m_update_layout = false;
        child->perform_layout(ctx);
    }
    int yoffset = 0;
    int xoffset = 0;

    if (m_child_preferred_size.y() > m_size.y() && VScrollable())
        yoffset = -m_scroll.y() * (m_child_preferred_size.y() - m_size.y());
    if (m_child_preferred_size.x() > m_size.x() && HScrollable())
        xoffset = -m_scroll.x() * (m_child_preferred_size.x() - m_size.x());

    child->set_position(Vector2i(xoffset, yoffset));
    m_child_preferred_size = child->preferred_size(ctx);
    float scrollw = width() * std::min(1.f, width() / (float)m_child_preferred_size.x());
    float scrollh = height() * std::min(1.f, height() / (float)m_child_preferred_size.y());

    nvgSave(ctx);
    nvgTranslate(ctx, m_pos.x(), m_pos.y());
    nvgIntersectScissor(ctx, 0, 0, m_size.x() - 10, m_size.y() - 10);
    if (child->visible())
        child->draw(ctx);
    nvgRestore(ctx);

    if (m_child_preferred_size.y() > m_size.y() && VScrollable())
    {

        NVGpaint paint = nvgBoxGradient(
            ctx, m_pos.x() + m_size.x() - 12 + 1, m_pos.y() + 4 + 1, 8,
            m_size.y() - 8, 3, 4, Color(0, 32), Color(0, 92));
        nvgBeginPath(ctx);
        nvgRoundedRect(ctx, m_pos.x() + m_size.x() - 12, m_pos.y() + 4, 8,
            m_size.y() - 8, 3);
        nvgFillPaint(ctx, paint);
        nvgFill(ctx);

        paint = nvgBoxGradient(
            ctx, m_pos.x() + m_size.x() - 12 - 1,
            m_pos.y() + 4 + (m_size.y() - 8 - scrollh) * m_scroll.y() - 1, 8, scrollh,
            3, 4, Color(220, 100), Color(128, 100));

        nvgBeginPath(ctx);
        nvgRoundedRect(ctx, m_pos.x() + m_size.x() - 12 + 1,
            m_pos.y() + 4 + 1 + (m_size.y() - 8 - scrollh) * m_scroll.y(), 8 - 2,
            scrollh - 2, 2);
        nvgFillPaint(ctx, paint);
        nvgFill(ctx);
    }
    if (m_child_preferred_size.x() > m_size.x() && HScrollable())
    {
        NVGpaint paint = nvgBoxGradient(
            ctx, m_pos.x() + m_size.x() + 4 + 1, m_pos.y() - 12 + 1, m_size.x() - 8, 8, 3, 4, Color(0, 32), Color(0, 92));
        nvgBeginPath(ctx);
        nvgRoundedRect(ctx, m_pos.x() + 4, m_pos.y() + m_size.y() - 12, m_size.x() - 8, 8, 3);
        nvgFillPaint(ctx, paint);
        nvgFill(ctx);

        paint = nvgBoxGradient(
            ctx, m_pos.x() + 4 + (m_size.x() - 8 - scrollw) * m_scroll.x() - 1, m_pos.y() + m_size.y() - 12 - 1, scrollw, 8,
            3, 4, Color(220, 100), Color(128, 100));

        nvgBeginPath(ctx);
        nvgRoundedRect(ctx,
            m_pos.x() + 4 + 1 + (m_size.x() - 8 - scrollw) * m_scroll.x(), m_pos.y() + m_size.y() - 12 + 1, scrollw - 2, 8 - 2, 2);
        nvgFillPaint(ctx, paint);
        nvgFill(ctx);
    }

}

NAMESPACE_END(nanogui)
