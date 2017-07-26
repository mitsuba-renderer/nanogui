/*
    src/window.cpp -- Top-level window widget

    NanoGUI was developed by Wenzel Jakob <wenzel.jakob@epfl.ch>.
    The widget drawing code is based on the NanoVG demo application
    by Mikko Mononen.

    All rights reserved. Use of this source code is governed by a
    BSD-style license that can be found in the LICENSE.txt file.
*/

#include <nanogui/window.h>
#include <nanogui/theme.h>
#include <nanogui/opengl.h>
#include <nanogui/screen.h>
#include <nanogui/layout.h>

NAMESPACE_BEGIN(nanogui)

Window::Window(Widget *parent, const std::string &title)
    : Widget(parent), m_title(title), m_button_panel(nullptr), m_modal(false),
      m_drag(false) { }

Vector2i Window::preferred_size(NVGcontext *ctx) const {
    if (m_button_panel)
        m_button_panel->set_visible(false);
    Vector2i result = Widget::preferred_size(ctx);
    if (m_button_panel)
        m_button_panel->set_visible(true);

    nvgFontSize(ctx, 18.0f);
    nvgFontFace(ctx, "sans-bold");
    float bounds[4];
    nvgTextBounds(ctx, 0, 0, m_title.c_str(), nullptr, bounds);

    return max(result, Vector2i(
        bounds[2]-bounds[0] + 20,
        bounds[3]-bounds[1]
    ));
}

Widget *Window::button_panel() {
    if (!m_button_panel) {
        m_button_panel = new Widget(this);
        m_button_panel->set_layout(new BoxLayout(Orientation::Horizontal, Alignment::Middle, 0, 4));
    }
    return m_button_panel;
}

void Window::perform_layout(NVGcontext *ctx) {
    if (!m_button_panel) {
        Widget::perform_layout(ctx);
    } else {
        m_button_panel->set_visible(false);
        Widget::perform_layout(ctx);
        for (auto w : m_button_panel->children()) {
            w->set_fixed_size(Vector2i(22, 22));
            w->set_font_size(15);
        }
        m_button_panel->set_visible(true);
        m_button_panel->set_size(Vector2i(width(), 22));
        m_button_panel->set_position(Vector2i(
            width() - (m_button_panel->preferred_size(ctx).x() + 5), 3));
        m_button_panel->perform_layout(ctx);
    }
}

void Window::draw(NVGcontext *ctx) {
    int ds = m_theme->m_window_drop_shadow_size, cr = m_theme->m_window_corner_radius;
    int hh = m_theme->m_window_header_height;

    /* Draw window */
    nvgSave(ctx);
    nvgBeginPath(ctx);
    nvgRoundedRect(ctx, m_pos.x(), m_pos.y(), m_size.x(), m_size.y(), cr);

    nvgFillColor(ctx, m_mouse_focus ? m_theme->m_window_fill_focused
                                    : m_theme->m_window_fill_unfocused);
    nvgFill(ctx);


    /* Draw a drop shadow */
    NVGpaint shadow_paint = nvgBoxGradient(
        ctx, m_pos.x(), m_pos.y(), m_size.x(), m_size.y(), cr*2, ds*2,
        m_theme->m_drop_shadow, m_theme->m_transparent);

    nvgSave(ctx);
    nvgResetScissor(ctx);
    nvgBeginPath(ctx);
    nvgRect(ctx, m_pos.x()-ds,m_pos.y()-ds, m_size.x()+2*ds, m_size.y()+2*ds);
    nvgRoundedRect(ctx, m_pos.x(), m_pos.y(), m_size.x(), m_size.y(), cr);
    nvgPathWinding(ctx, NVG_HOLE);
    nvgFillPaint(ctx, shadow_paint);
    nvgFill(ctx);
    nvgRestore(ctx);

    if (!m_title.empty()) {
        /* Draw header */
        NVGpaint header_paint = nvgLinearGradient(
            ctx, m_pos.x(), m_pos.y(), m_pos.x(),
            m_pos.y() + hh,
            m_theme->m_window_header_gradient_top,
            m_theme->m_window_header_gradient_bot);

        nvgBeginPath(ctx);
        nvgRoundedRect(ctx, m_pos.x(), m_pos.y(), m_size.x(), hh, cr);

        nvgFillPaint(ctx, header_paint);
        nvgFill(ctx);

        nvgBeginPath(ctx);
        nvgRoundedRect(ctx, m_pos.x(), m_pos.y(), m_size.x(), hh, cr);
        nvgStrokeColor(ctx, m_theme->m_window_header_sep_top);

        nvgSave(ctx);
        nvgIntersectScissor(ctx, m_pos.x(), m_pos.y(), m_size.x(), 0.5f);
        nvgStroke(ctx);
        nvgRestore(ctx);

        nvgBeginPath(ctx);
        nvgMoveTo(ctx, m_pos.x() + 0.5f, m_pos.y() + hh - 1.5f);
        nvgLineTo(ctx, m_pos.x() + m_size.x() - 0.5f, m_pos.y() + hh - 1.5);
        nvgStrokeColor(ctx, m_theme->m_window_header_sep_bot);
        nvgStroke(ctx);

        nvgFontSize(ctx, 18.0f);
        nvgFontFace(ctx, "sans-bold");
        nvgTextAlign(ctx, NVG_ALIGN_CENTER | NVG_ALIGN_MIDDLE);

        nvgFontBlur(ctx, 2);
        nvgFillColor(ctx, m_theme->m_drop_shadow);
        nvgText(ctx, m_pos.x() + m_size.x() / 2,
                m_pos.y() + hh / 2, m_title.c_str(), nullptr);

        nvgFontBlur(ctx, 0);
        nvgFillColor(ctx, m_focused ? m_theme->m_window_title_focused
                                    : m_theme->m_window_title_unfocused);
        nvgText(ctx, m_pos.x() + m_size.x() / 2, m_pos.y() + hh / 2 - 1,
                m_title.c_str(), nullptr);
    }

    nvgRestore(ctx);
    Widget::draw(ctx);
}

void Window::dispose() {
    Widget *widget = this;
    while (widget->parent())
        widget = widget->parent();
    ((Screen *) widget)->dispose_window(this);
}

void Window::center() {
    Widget *widget = this;
    while (widget->parent())
        widget = widget->parent();
    ((Screen *) widget)->center_window(this);
}

bool Window::mouse_enter_event(const Vector2i &p, bool enter) {
    Widget::mouse_enter_event(p, enter);
    return true;
}

bool Window::mouse_drag_event(const Vector2i &, const Vector2i &rel,
                            int button, int /* modifiers */) {
    if (m_drag && (button & (1 << GLFW_MOUSE_BUTTON_1)) != 0) {
        m_pos += rel;
        m_pos = max(m_pos, Vector2i(0));
        m_pos = min(m_pos, parent()->size() - m_size);
        return true;
    }
    return false;
}

bool Window::mouse_button_event(const Vector2i &p, int button, bool down, int modifiers) {
    if (Widget::mouse_button_event(p, button, down, modifiers))
        return true;
    if (button == GLFW_MOUSE_BUTTON_1) {
        m_drag = down && (p.y() - m_pos.y()) < m_theme->m_window_header_height;
        return true;
    }
    return false;
}

bool Window::scroll_event(const Vector2i &p, const Vector2f &rel) {
    Widget::scroll_event(p, rel);
    return true;
}

void Window::refresh_relative_placement() {
    /* Overridden in \ref Popup */
}

NAMESPACE_END(nanogui)
