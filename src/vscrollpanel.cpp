/*
    src/vscrollpanel.cpp -- Adds a vertical scrollbar around a widget
    that is too big to fit into a certain area

    NanoGUI was developed by Wenzel Jakob <wenzel.jakob@epfl.ch>.
    The widget drawing code is based on the NanoVG demo application
    by Mikko Mononen.

    All rights reserved. Use of this source code is governed by a
    BSD-style license that can be found in the LICENSE.txt file.
*/

#include <nanogui/vscrollpanel.h>
#include <nanogui/theme.h>
#include <nanogui/opengl.h>

NAMESPACE_BEGIN(nanogui)

VScrollPanel::VScrollPanel(Widget *parent)
    : Widget(parent), m_child_preferred_height(0),
      m_scroll(0.f), m_update_layout(false) { }

void VScrollPanel::perform_layout(NVGcontext *ctx) {
    Widget::perform_layout(ctx);

    if (m_children.empty())
        return;
    if (m_children.size() > 1)
        throw std::runtime_error("VScrollPanel should have one child.");

    Widget *child = m_children[0];
    m_child_preferred_height = child->preferred_size(ctx).y();

    if (m_child_preferred_height > m_size.y()) {
        child->set_position(Vector2i(0, -m_scroll * (m_child_preferred_height - m_size.y())));
        child->set_size(Vector2i(m_size.x() - 12, m_child_preferred_height));
    } else {
        child->set_position(Vector2i(0));
        child->set_size(m_size);
        m_scroll = 0;
    }
    child->perform_layout(ctx);
}

Vector2i VScrollPanel::preferred_size(NVGcontext *ctx) const {
    if (m_children.empty())
        return Vector2i(0);
    return m_children[0]->preferred_size(ctx) + Vector2i(12, 0);
}

bool VScrollPanel::mouse_drag_event(const Vector2i &p, const Vector2i &rel,
                                    int button, int modifiers) {
    if (!m_children.empty() && m_child_preferred_height > m_size.y()) {
        float scrollh = height() *
            std::min(1.f, height() / (float) m_child_preferred_height);

        m_scroll = std::max(0.f, std::min(1.f,
                     m_scroll + rel.y() / (m_size.y() - 8.f - scrollh)));
        m_update_layout = true;
        return true;
    } else {
        return Widget::mouse_drag_event(p, rel, button, modifiers);
    }
}

bool VScrollPanel::mouse_button_event(const Vector2i &p, int button, bool down,
                                      int modifiers) {
    if (Widget::mouse_button_event(p, button, down, modifiers))
        return true;

    if (down && button == GLFW_MOUSE_BUTTON_1 && !m_children.empty() &&
        m_child_preferred_height > m_size.y() &&
        p.x() > m_pos.x() + m_size.x() - 13 &&
        p.x() < m_pos.x() + m_size.x() - 4) {

        int scrollh = (int) (height() *
            std::min(1.f, height() / (float) m_child_preferred_height));
        int start = (int) (m_pos.y() + 4 + 1 + (m_size.y() - 8 - scrollh) * m_scroll);

        float delta = 0.f;

        if (p.y() < start)
            delta = -m_size.y() / (float) m_child_preferred_height;
        else if (p.y() > start + scrollh)
            delta = m_size.y() / (float) m_child_preferred_height;

        m_scroll = std::max(0.f, std::min(1.f, m_scroll + delta*0.98f));

        m_children[0]->set_position(
            Vector2i(0, -m_scroll * (m_child_preferred_height - m_size.y())));
        m_update_layout = true;
        return true;
    }
    return false;
}

bool VScrollPanel::scroll_event(const Vector2i &p, const Vector2f &rel) {
    if (!m_children.empty() && m_child_preferred_height > m_size.y()) {
        auto child = m_children[0];
        float scroll_amount = rel.y() * m_size.y() * .25f;

        m_scroll = std::max(0.f, std::min(1.f,
                m_scroll - scroll_amount / m_child_preferred_height));

        Vector2i old_pos = child->position();
        child->set_position(Vector2i(0, -m_scroll*(m_child_preferred_height - m_size.y())));
        Vector2i new_pos = child->position();
        m_update_layout = true;
        child->mouse_motion_event(p-m_pos, old_pos - new_pos, 0, 0);

        return true;
    } else {
        return Widget::scroll_event(p, rel);
    }
}

void VScrollPanel::draw(NVGcontext *ctx) {
    if (m_children.empty())
        return;
    Widget *child = m_children[0];
    int yoffset = 0;
    if (m_child_preferred_height > m_size.y())
        yoffset = -m_scroll*(m_child_preferred_height - m_size.y());
    child->set_position(Vector2i(0, yoffset));
    m_child_preferred_height = child->preferred_size(ctx).y();
    float scrollh = height() *
        std::min(1.f, height() / (float) m_child_preferred_height);

    if (m_update_layout) {
        m_update_layout = false;
        child->perform_layout(ctx);
    }

    nvgSave(ctx);
    nvgTranslate(ctx, m_pos.x(), m_pos.y());
    nvgIntersectScissor(ctx, 0, 0, m_size.x(), m_size.y());
    if (child->visible())
        child->draw(ctx);
    nvgRestore(ctx);

    if (m_child_preferred_height <= m_size.y())
        return;

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
        m_pos.y() + 4 + (m_size.y() - 8 - scrollh) * m_scroll - 1, 8, scrollh,
        3, 4, Color(220, 100), Color(128, 100));

    nvgBeginPath(ctx);
    nvgRoundedRect(ctx, m_pos.x() + m_size.x() - 12 + 1,
                   m_pos.y() + 4 + 1 + (m_size.y() - 8 - scrollh) * m_scroll, 8 - 2,
                   scrollh - 2, 2);
    nvgFillPaint(ctx, paint);
    nvgFill(ctx);
}

NAMESPACE_END(nanogui)
