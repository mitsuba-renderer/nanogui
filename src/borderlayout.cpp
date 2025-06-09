/*
    src/borderlayout.cpp -- Layout inspired by Java's BorderLayout

    NanoGUI was developed by Wenzel Jakob <wenzel.jakob@epfl.ch>.
    The widget drawing code is based on the NanoVG demo application
    by Mikko Mononen.

    All rights reserved. Use of this source code is governed by a
    BSD-style license that can be found in the LICENSE.txt file.
*/
#include <nanogui/borderlayout.h>
#include <nanogui/widget.h>
#include <nanogui/window.h>

NAMESPACE_BEGIN(nanogui)

static Vector2i get_size(NVGcontext *ctx, const Widget *w)
{
    Vector2i ps = w->preferred_size(ctx), fs = w->fixed_size();
    return Vector2i(
        fs[0] ? fs[0] : ps[0],
        fs[1] ? fs[1] : ps[1]
    );
}

BorderLayout::BorderLayout(int margin, int spacing)
    : m_margin(margin), m_spacing(spacing)
{
}

void BorderLayout::set_side(const Widget *widget, Side side)
{
    m_side[widget] = side;
}

BorderLayout::Side BorderLayout::side(const Widget *widget) const
{
    auto it = m_side.find(widget);
    if (it == m_side.end())
        return Center;
    return it->second;
}

Vector2i BorderLayout::preferred_size(NVGcontext *ctx, const Widget *widget) const
{
    Vector2i ret{ 0, 0 };

    // First collect the center items
    for (Widget *w : widget->children()) {
        if (!w->visible())
            continue;
        Vector2i csize = get_size(ctx, w);
        if (side(w) == Center) {
            ret.x() = std::max(ret.x(), csize.x());
            ret.y() = std::max(ret.y(), csize.y());
            break;
        }
    }

    // Then process the border items, but in reverse order!
    for (int i = widget->child_count() - 1; i >= 0; i--) {
        const Widget *w = widget->child_at(i);
        if (!w->visible())
            continue;
        Vector2i csize = get_size(ctx, w);
        switch (side(w)) {
        case North:
        case South:
            ret.x() = std::max(ret.x(), csize.x());
            ret.y() += csize.y() + m_spacing;
            break;
        case West:
        case East:
            ret.x() += csize.x() + m_spacing;
            ret.y() = std::max(ret.y(), csize.y());
            break;
        case Center:
        default:
            /* NOP */
            break;
        }
    }

    // Add the margin
    ret.x() += 2 * m_margin;
    ret.y() += 2 * m_margin;

    // Add the window title height
    const Window *window = dynamic_cast<const Window *>(widget);
    if (window && !window->title().empty()) {
        ret.y() += widget->theme()->m_window_header_height;
    }

    return ret;
}

void BorderLayout::perform_layout(NVGcontext *ctx, Widget *widget) const
{
    Vector2i pos(0, 0), size = widget->size();

    // Add the window title height
    const Window *window = dynamic_cast<const Window *>(widget);
    if (window && !window->title().empty()) {
        pos.y() += widget->theme()->m_window_header_height;
        size.y() -= widget->theme()->m_window_header_height;
    }

    // Substract the margin
    if (m_margin) {
        pos += Vector2i{ m_margin, m_margin };
        size -= Vector2i{ 2 * m_margin, 2 * m_margin };
    }

    // Loop through the widgets and handle the non-center widgets only
    for (Widget *w : widget->children()) {
        if (!w->visible())
            continue;
        Side s = side(w);
        if (s == Center)
            continue;
        Vector2i csize = get_size(ctx, w);
        switch (s) {
        case North:
            w->set_position(pos);
            w->set_size({ size.x(), csize.y() });
            pos.y() += csize.y() + m_spacing;
            size.y() -= csize.y() + m_spacing;
            break;
        case South:
            w->set_position({ pos.x(), pos.y() + size.y() - csize.y() });
            w->set_size({ size.x(), csize.y() });
            size.y() -= csize.y() + m_spacing;
            break;
        case West:
            w->set_position(pos);
            w->set_size({ csize.x(), size.y() });
            pos.x() += csize.x() + m_spacing;
            size.x() -= csize.x() + m_spacing;
            break;
        case East:
            w->set_position({ pos.x() + size.x() - csize.x(), pos.y() });
            w->set_size({ csize.x(), size.y() });
            size.x() -= csize.x() + m_spacing;
            break;
        default:
            /* NOP */
            break;
        }
    }

    // Loop through the center widgets and assign them the remaining space
    for (Widget *w : widget->children()) {
        if (!w->visible())
            continue;
        if (side(w) != Center)
            continue;
        w->set_position(pos);
        w->set_size(size);
    }

    // Let the children perform layout as well
    for (Widget *w : widget->children()) {
        if (!w->visible())
            continue;
        w->perform_layout(ctx);
    }
}

NAMESPACE_END(nanogui)
