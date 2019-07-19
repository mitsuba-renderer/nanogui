/*
    src/widget.cpp -- Base class of all widgets

    NanoGUI was developed by Wenzel Jakob <wenzel.jakob@epfl.ch>.
    The widget drawing code is based on the NanoVG demo application
    by Mikko Mononen.

    All rights reserved. Use of this source code is governed by a
    BSD-style license that can be found in the LICENSE.txt file.
*/

#include <nanogui/widget.h>
#include <nanogui/layout.h>
#include <nanogui/theme.h>
#include <nanogui/window.h>
#include <nanogui/opengl.h>
#include <nanogui/screen.h>

/* Uncomment the following definition to draw red bounding
   boxes around widgets (useful for debugging drawing code) */

// #define NANOGUI_SHOW_WIDGET_BOUNDS 1

NAMESPACE_BEGIN(nanogui)

Widget::Widget(Widget *parent)
    : m_parent(nullptr), m_theme(nullptr), m_layout(nullptr),
      m_pos(0), m_size(0), m_fixed_size(0), m_visible(true), m_enabled(true),
      m_focused(false), m_mouse_focus(false), m_tooltip(""), m_font_size(-1.f),
      m_icon_extra_scale(1.f), m_cursor(Cursor::Arrow) {
    if (parent)
        parent->add_child(this);
}

Widget::~Widget() {
    if (std::uncaught_exceptions() > 0) {
        /* If a widget constructor throws an exception, it is immediately
           dealloated but may still be referenced by a parent. Be conservative
           and don't decrease the reference count of children while dispatching
           exceptions. */
        return;
    }
    for (auto child : m_children) {
        if (child)
            child->dec_ref();
    }
}

void Widget::set_theme(Theme *theme) {
    if (m_theme.get() == theme)
        return;
    m_theme = theme;
    for (auto child : m_children)
        child->set_theme(theme);
}

int Widget::font_size() const {
    return (m_font_size < 0 && m_theme) ? m_theme->m_standard_font_size : m_font_size;
}

Vector2i Widget::preferred_size(NVGcontext *ctx) const {
    if (m_layout)
        return m_layout->preferred_size(ctx, this);
    else
        return m_size;
}

void Widget::perform_layout(NVGcontext *ctx) {
    if (m_layout) {
        m_layout->perform_layout(ctx, this);
    } else {
        for (auto c : m_children) {
            Vector2i pref = c->preferred_size(ctx), fix = c->fixed_size();
            c->set_size(Vector2i(
                fix[0] ? fix[0] : pref[0],
                fix[1] ? fix[1] : pref[1]
            ));
            c->perform_layout(ctx);
        }
    }
}

Widget *Widget::find_widget(const Vector2i &p) {
    for (auto it = m_children.rbegin(); it != m_children.rend(); ++it) {
        Widget *child = *it;
        if (child->visible() && child->contains(p - m_pos))
            return child->find_widget(p - m_pos);
    }
    return contains(p) ? this : nullptr;
}

const Widget *Widget::find_widget(const Vector2i &p) const {
    for (auto it = m_children.rbegin(); it != m_children.rend(); ++it) {
        Widget *child = *it;
        if (child->visible() && child->contains(p - m_pos))
            return child->find_widget(p - m_pos);
    }
    return contains(p) ? this : nullptr;
}

bool Widget::mouse_button_event(const Vector2i &p, int button, bool down, int modifiers) {
    for (auto it = m_children.rbegin(); it != m_children.rend(); ++it) {
        Widget *child = *it;
        if (child->visible() && child->contains(p - m_pos) &&
            child->mouse_button_event(p - m_pos, button, down, modifiers))
            return true;
    }
    if (button == GLFW_MOUSE_BUTTON_1 && down && !m_focused)
        request_focus();
    return false;
}

bool Widget::mouse_motion_event(const Vector2i &p, const Vector2i &rel, int button, int modifiers) {
    bool handled = false;

    for (auto it = m_children.rbegin(); it != m_children.rend(); ++it) {
        Widget *child = *it;
        if (!child->visible())
            continue;

        bool contained      = child->contains(p - m_pos),
             prev_contained = child->contains(p - m_pos - rel);

        if (contained != prev_contained)
            handled |= child->mouse_enter_event(p, contained);

        if (contained || prev_contained)
            handled |= child->mouse_motion_event(p - m_pos, rel, button, modifiers);
    }

    return handled;
}

bool Widget::scroll_event(const Vector2i &p, const Vector2f &rel) {
    for (auto it = m_children.rbegin(); it != m_children.rend(); ++it) {
        Widget *child = *it;
        if (!child->visible())
            continue;
        if (child->contains(p - m_pos) && child->scroll_event(p - m_pos, rel))
            return true;
    }
    return false;
}

bool Widget::mouse_drag_event(const Vector2i &, const Vector2i &, int, int) {
    return false;
}

bool Widget::mouse_enter_event(const Vector2i &, bool enter) {
    m_mouse_focus = enter;
    return false;
}

bool Widget::focus_event(bool focused) {
    m_focused = focused;
    return false;
}

bool Widget::keyboard_event(int, int, int, int) {
    return false;
}

bool Widget::keyboard_character_event(unsigned int) {
    return false;
}

void Widget::add_child(int index, Widget * widget) {
    assert(index <= child_count());
    m_children.insert(m_children.begin() + index, widget);
    widget->inc_ref();
    widget->set_parent(this);
    widget->set_theme(m_theme);
}

void Widget::add_child(Widget * widget) {
    add_child(child_count(), widget);
}

void Widget::remove_child(const Widget *widget) {
    size_t child_count = m_children.size();
    m_children.erase(std::remove(m_children.begin(), m_children.end(), widget),
                     m_children.end());
    if (m_children.size() == child_count)
        throw std::runtime_error("Widget::remove_child(): widget not found!");
    widget->dec_ref();
}

void Widget::remove_child_at(int index) {
    if (index < 0 || index >= (int) m_children.size())
        throw std::runtime_error("Widget::remove_child_at(): out of bounds!");
    Widget *widget = m_children[index];
    m_children.erase(m_children.begin() + index);
    widget->dec_ref();
}

int Widget::child_index(Widget *widget) const {
    auto it = std::find(m_children.begin(), m_children.end(), widget);
    if (it == m_children.end())
        return -1;
    return (int) (it - m_children.begin());
}

Window *Widget::window() {
    Widget *widget = this;
    while (true) {
        if (!widget)
            return nullptr;
        Window *window = dynamic_cast<Window *>(widget);
        if (window)
            return window;
        widget = widget->parent();
    }
}

Screen *Widget::screen() {
    Widget *widget = this;
    while (true) {
        if (!widget)
            return nullptr;
        Screen *screen = dynamic_cast<Screen *>(widget);
        if (screen)
            return screen;
        widget = widget->parent();
    }
}

const Screen *Widget::screen() const { return const_cast<Widget*>(this)->screen(); }
const Window *Widget::window() const { return const_cast<Widget*>(this)->window(); }

void Widget::request_focus() {
    Widget *widget = this;
    while (widget->parent())
        widget = widget->parent();
    ((Screen *) widget)->update_focus(this);
}

void Widget::draw(NVGcontext *ctx) {
    #if defined(NANOGUI_SHOW_WIDGET_BOUNDS)
        nvgStrokeWidth(ctx, 1.0f);
        nvgBeginPath(ctx);
        nvgRect(ctx, m_pos.x() - 0.5f, m_pos.y() - 0.5f,
                m_size.x() + 1, m_size.y() + 1);
        nvgStrokeColor(ctx, nvgRGBA(255, 0, 0, 255));
        nvgStroke(ctx);
    #endif

    if (m_children.empty())
        return;

    nvgTranslate(ctx, m_pos.x(), m_pos.y());
    for (auto child : m_children) {
        if (!child->visible())
            continue;
        #if !defined(NANOGUI_SHOW_WIDGET_BOUNDS)
            nvgSave(ctx);
            nvgIntersectScissor(ctx, child->m_pos.x(), child->m_pos.y(),
                                child->m_size.x(), child->m_size.y());
        #endif

        child->draw(ctx);

        #if !defined(NANOGUI_SHOW_WIDGET_BOUNDS)
            nvgRestore(ctx);
        #endif
    }
    nvgTranslate(ctx, -m_pos.x(), -m_pos.y());
}

NAMESPACE_END(nanogui)
