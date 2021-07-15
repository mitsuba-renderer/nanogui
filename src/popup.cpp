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
#include <nanogui/popupbutton.h>
#include <nanogui/theme.h>
#include <nanogui/opengl.h>

NAMESPACE_BEGIN(nanogui)

Popup::Popup(Widget* parent, Window* parent_window, PopupButton* parent_button)
    : Window(parent, ""), m_parent_window(parent_window), m_parent_button(parent_button), m_anchor_pos(Vector2i(0)),
    m_anchor_offset(30), m_anchor_size(10), m_side(Side::Right) { }

void Popup::perform_layout(NVGcontext* ctx) {
    if (m_layout || m_children.size() != 1) {
        Widget::perform_layout(ctx);
    }
    else {
        m_children[0]->set_position(Vector2i(0));
        m_children[0]->set_size(m_size);
        m_children[0]->perform_layout(ctx);
    }
}

void Popup::refresh_relative_placement() {
    if (!m_parent_window)
        return;
    m_parent_window->refresh_relative_placement();
    m_visible &= m_parent_window->visible_recursive();

    Vector2i TempPos;
    Vector2i AnchorPos;
    // calculate anchor position here.
    if (side() == Popup::Right)
    {
        TempPos = m_parent_button->absolute_position() + Vector2i(m_parent_button->size().x() + m_anchor_size, (m_parent_button->size().y() - m_size.y()) / 2);
        AnchorPos = m_parent_button->absolute_position() + Vector2i(m_parent_button->size().x(), m_parent_button->size().y() / 2);
        if (TempPos.y() < 0)//if the popup does not fit the top of the  screen, then move it lower
            TempPos = Vector2i(m_parent_button->absolute_position().x() + m_parent_button->size().x() + m_anchor_size, 0);
    }
    else if (side() == Popup::Left)
    {
        TempPos = m_parent_button->absolute_position() + Vector2i(-m_size.x() - m_anchor_size, (m_parent_button->size().y() - m_size.y()) / 2);
        AnchorPos = m_parent_button->absolute_position() + Vector2i(-m_anchor_size, m_parent_button->size().y() / 2);
        if (TempPos.x() < 0)//if the popup does not fit in the left of screen, then move it below the button
        {
            TempPos = Vector2i(0, m_parent_button->absolute_position().y() + m_anchor_size + m_parent_button->size().y());
            AnchorPos = m_parent_button->absolute_position() + Vector2i(0, m_parent_button->size().y() + m_anchor_size);
        }
        else if (TempPos.y() < 0)//if the popup does not fit the top of the  screen, then move it lower
            TempPos = Vector2i(m_parent_button->absolute_position().x() - m_size.x() - m_anchor_size, 0);
    }
    else// bottom
    {
        TempPos = m_parent_button->absolute_position() + Vector2i((m_parent_button->size().x() - m_size.x()) / 2, m_parent_button->size().y() + m_anchor_size);
        AnchorPos = m_parent_button->absolute_position() + Vector2i(m_parent_button->size().x() / 2, m_parent_button->size().y());
        if (TempPos.x() < 0)//if the popup does not fit the top of the  screen, then move it lower
            TempPos = Vector2i(0, m_parent_button->absolute_position().y() + m_parent_button->size().y() + m_anchor_size);
    }

    set_anchor_pos(AnchorPos);
    m_pos = TempPos;

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
        ctx, m_pos.x(), m_pos.y(), m_size.x(), m_size.y(), cr * 2, ds * 2,
        m_theme->m_drop_shadow, m_theme->m_transparent);

    nvgBeginPath(ctx);
    nvgRect(ctx, m_pos.x() - ds, m_pos.y() - ds, m_size.x() + 2 * ds, m_size.y() + 2 * ds);
    nvgRoundedRect(ctx, m_pos.x(), m_pos.y(), m_size.x(), m_size.y(), cr);
    nvgPathWinding(ctx, NVG_HOLE);
    nvgFillPaint(ctx, shadow_paint);
    nvgFill(ctx);

    /* Draw window */
    nvgBeginPath(ctx);
    nvgRoundedRect(ctx, m_pos.x(), m_pos.y(), m_size.x(), m_size.y(), cr);
    if (m_side == Side::Right) {
        nvgMoveTo(ctx, m_anchor_pos.x() + m_anchor_size, m_anchor_pos.y());
        nvgLineTo(ctx, m_anchor_pos.x() - 1, m_anchor_pos.y() - m_anchor_size);
        nvgLineTo(ctx, m_anchor_pos.x() - 1, m_anchor_pos.y() + m_anchor_size);
    }
    if (m_side == Side::Left) {// draw a diamond in case we move the anchor to the bottom
        nvgMoveTo(ctx, m_anchor_pos.x() + m_anchor_size, m_anchor_pos.y());
        nvgLineTo(ctx, m_anchor_pos.x(), m_anchor_pos.y() - m_anchor_size);
        nvgLineTo(ctx, m_anchor_pos.x() - m_anchor_size, m_anchor_pos.y());
        nvgLineTo(ctx, m_anchor_pos.x(), m_anchor_pos.y() + m_anchor_size);
    }
    else// bottom
    {
        nvgMoveTo(ctx, m_anchor_pos.x(), m_anchor_pos.y() + m_anchor_size);
        nvgLineTo(ctx, m_anchor_pos.x() - m_anchor_size, m_anchor_pos.y() - 1);
        nvgLineTo(ctx, m_anchor_pos.x() + m_anchor_size, m_anchor_pos.y() - 1);
    }
    nvgFillColor(ctx, m_theme->m_window_popup);
    nvgFill(ctx);
    nvgRestore(ctx);

    Widget::draw(ctx);
}

NAMESPACE_END(nanogui)
