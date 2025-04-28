/*
    src/popup.cpp -- Simple popup widget which is attached to another given
    window (can be nested)

    NanoGUI was developed by Wenzel Jakob <wenzel.jakob@epfl.ch>.
    The widget drawing code is based on the NanoVG demo application
    by Mikko Mononen.

    All rights reserved. Use of this source code is governed by a
    BSD-style license that can be found in the LICENSE.txt file.
*/

#include <nanogui/popup.h>
#include <nanogui/screen.h>
#include <nanogui/theme.h>
#include <nanogui/opengl.h>

NAMESPACE_BEGIN(nanogui)

Popup::Popup(Widget *parent, Window *parent_window)
    : Window(parent, ""), m_parent_window(parent_window), m_anchor_pos(Vector2i(0)),
      m_anchor_offset(30), m_anchor_size(15), m_side(Side::Right) { }

void Popup::perform_layout(NVGcontext *ctx) {
    if (m_layout || m_children.size() != 1) {
        Widget::perform_layout(ctx);
    } else {
        m_children[0]->set_position(Vector2i(0));
        m_children[0]->set_size(m_size);
        m_children[0]->perform_layout(ctx);
    }
    if (m_side == Side::Left || m_side == Side::LeftInside)
        m_anchor_pos[0] -= size()[0];
}

void Popup::refresh_relative_placement() {
    if (!m_parent_window)
        return;
    m_parent_window->refresh_relative_placement();
    m_visible &= m_parent_window->visible_recursive();
    m_pos = m_parent_window->position() + m_anchor_pos - Vector2i(0, m_anchor_offset);

    // No matter what, make sure the popup is within the screen area
    if (screen()) {
        Vector2i screen_size = screen()->size();
        for (int axis = 0; axis < 2; axis++) {
            if (m_size[axis] >= screen_size[axis]) {
                // Popup too big, just center it
                m_pos[axis] = (screen_size[axis] - m_size[axis]) / 2;
            } else {
                // Clamp to screen space
                m_pos[axis] = std::max(0, std::min(screen_size[axis] - m_size[axis], m_pos[axis]));
            }
        }
    }
}

void Popup::draw(NVGcontext* ctx) {
    refresh_relative_placement();

    if (!m_visible)
        return;

    int ds = m_theme->m_window_drop_shadow_size,
        cr = m_theme->m_window_corner_radius;

    nvgSave(ctx);
    nvgResetScissor(ctx);

    /* Draw a drop shadow */
    NVGpaint shadow_paint = nvgBoxGradient(
        ctx, m_pos.x(), m_pos.y(), m_size.x(), m_size.y(), cr*2, ds*2,
        m_theme->m_drop_shadow, m_theme->m_transparent);

    nvgBeginPath(ctx);
    nvgRect(ctx, m_pos.x()-ds,m_pos.y()-ds, m_size.x()+2*ds, m_size.y()+2*ds);
    nvgRoundedRect(ctx, m_pos.x(), m_pos.y(), m_size.x(), m_size.y(), cr);
    nvgPathWinding(ctx, NVG_HOLE);
    nvgFillPaint(ctx, shadow_paint);
    nvgFill(ctx);

    /* Draw window */
    nvgBeginPath(ctx);
    nvgRoundedRect(ctx, m_pos.x(), m_pos.y(), m_size.x(), m_size.y(), cr);

    Vector2i base = m_pos + Vector2i(0, m_anchor_offset);
    int sign = -1;
    if (m_side == Side::Left || m_side == Side::LeftInside) {
        base.x() += m_size.x();
        sign = 1;
    }

    nvgMoveTo(ctx, base.x() + m_anchor_size*sign, base.y());
    nvgLineTo(ctx, base.x() - 1*sign, base.y() - m_anchor_size);
    nvgLineTo(ctx, base.x() - 1*sign, base.y() + m_anchor_size);

    nvgFillColor(ctx, m_theme->m_window_popup);
    nvgFill(ctx);
    nvgRestore(ctx);

    Widget::draw(ctx);
}

void Popup::update_anchor(const Widget * ref)
{
    const Window *parent_window = ref->window();

    int anchor_size = this->anchor_size();

    if (parent_window) {
        int pos_x = ref->absolute_position().x() - parent_window->position().x();
        int pos_y = ref->absolute_position().y() - parent_window->position().y() + ref->height() / 2;
        if (side() == Popup::Right)
            set_anchor_pos(Vector2i(parent_window->width() + anchor_size, pos_y));
        else if (side() == Popup::RightInside)
            set_anchor_pos(Vector2i(pos_x + ref->width() + anchor_size, pos_y));
        else if (side() == Popup::Left)
            set_anchor_pos(Vector2i(-anchor_size, pos_y));
        else
            set_anchor_pos(Vector2i(pos_x - anchor_size, pos_y));
    } else {
        set_position(ref->absolute_position() + Vector2i(ref->width() + anchor_size + 1,  ref->height() / 2 - anchor_size));
    }
}

void Popup::update_anchor(const Vector2i &p)
{
    set_anchor_pos(p);
    refresh_relative_placement();
}

NAMESPACE_END(nanogui)
