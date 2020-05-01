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

    return Vector2i(
        std::max(result.x(), (int) (bounds[2]-bounds[0] + 20)),
        std::max(result.y(), (int) (bounds[3]-bounds[1]))
    );
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

void Window::drawWindowFrame(NVGcontext* ctx) {
    int cr = m_theme->m_window_corner_radius;
    nvgSave(ctx);
    nvgBeginPath(ctx);
    nvgRoundedRect(ctx, m_pos.x(), m_pos.y(), m_size.x(), m_size.y(), cr);

    nvgFillColor(ctx, m_mouse_focus ? m_theme->m_window_fill_focused
        : m_theme->m_window_fill_unfocused);
    nvgFill(ctx);
}

void Window::drawWindowShadow(NVGcontext* ctx) {
    int ds = m_theme->m_window_drop_shadow_size;
    int cr = m_theme->m_window_corner_radius;
    NVGpaint shadow_paint = nvgBoxGradient(
        ctx, m_pos.x(), m_pos.y(), m_size.x(), m_size.y(), cr * 2, ds * 2,
        m_theme->m_drop_shadow, m_theme->m_transparent);

    nvgSave(ctx);
    nvgResetScissor(ctx);
    nvgBeginPath(ctx);
    nvgRect(ctx, m_pos.x() - ds, m_pos.y() - ds, m_size.x() + 2 * ds, m_size.y() + 2 * ds);
    nvgRoundedRect(ctx, m_pos.x(), m_pos.y(), m_size.x(), m_size.y(), cr);
    nvgPathWinding(ctx, NVG_HOLE);
    nvgFillPaint(ctx, shadow_paint);
    nvgFill(ctx);
    nvgRestore(ctx);
}

void Window::drawTitleBar(NVGcontext* ctx) {
    int cr = m_theme->m_window_corner_radius;
    int hh = m_theme->m_window_header_height;
    NVGpaint header_paint = nvgLinearGradient(
        ctx, m_pos.x(), m_pos.y(), m_pos.x(),
        m_pos.y() + hh,
        m_theme->m_window_header_gradient_top,
        m_theme->m_window_header_gradient_bot);

    nvgBeginPath(ctx);
    nvgRoundedRect(ctx, m_pos.x(), m_pos.y(), m_size.x(), hh, cr);

    nvgFillPaint(ctx, header_paint);
    nvgFill(ctx);
    ;
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

void Window::draw(NVGcontext *ctx) {
    /* Draw window */
    drawWindowFrame(ctx);
    /* Draw a drop shadow */
    drawWindowShadow(ctx);
    if (!m_title.empty()) {
        /* Draw header */
        drawTitleBar(ctx);
    }
    Widget::draw(ctx);

    nvgRestore(ctx);
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

ResizableWindow::ResizableWindow(Widget* parent, const std::string& title, bool enforceMinHeight)
    : Window(parent, title),
    m_resizing(false),
    m_draggingScrollbar(false),
    m_layoutChanged(false),
    m_downPos(Vector2i(0, 0)),
    m_preferred(Vector2i(0, 0)),
    m_enforceMinHeight(enforceMinHeight),
    m_scroll(0)
{ }

bool ResizableWindow::mouse_button_event(const Vector2i& p, int button, bool down, int modifiers)
{
    if (down) {
        if (m_pos.x() + m_size.x() - p.x() < 10) {
            m_downPos = p;
            if (m_pos.y() + m_size.y() - p.y() < 10) {
                m_resizing = true;
                return true;
            } else {
                m_draggingScrollbar = true;
                return true;
            }
        }
    }
    m_resizing = false;
    m_draggingScrollbar = false;
    return Window::mouse_button_event(p, button, down, modifiers);
}
bool ResizableWindow::mouse_motion_event(const Vector2i& p, const Vector2i& rel, int button, int modifiers)
{
    if (m_resizing) {
        Vector2i delta = p - m_downPos;
        Vector2i size = m_size + delta;
        if (size.x() < m_preferred.x())
            size = Vector2i(m_preferred.x(), size.y());
        if (m_enforceMinHeight) {
            if (size.y() < m_preferred.y())
                size = Vector2i(size.x(), m_preferred.y());
        }
        set_size(size);
        m_downPos = p;
        m_layoutChanged = true;
        return true;
    } else if (m_draggingScrollbar) {
        int hh = m_theme->m_window_header_height;
        float scrollbarAreaHeight = m_size.y() - hh - 10;
        float scrollHeight = scrollbarAreaHeight * m_size.y() / m_preferred.y();
        float scrollTop = m_scroll * (scrollbarAreaHeight - scrollHeight);
        scrollTop += p.y() - m_downPos.y();
        m_scroll = scrollTop / (scrollbarAreaHeight - scrollHeight);
        if (m_scroll < 0)
            m_scroll = 0;
        else if (m_scroll > 1)
            m_scroll = 1;
        m_layoutChanged = true;
        m_downPos = p;
        return true;
    }
    return Window::mouse_motion_event(p, rel, button, modifiers);
}

bool ResizableWindow::scroll_event(const Vector2i& p, const Vector2f& rel) {
    if (m_size.y() < m_preferred.y()) {
        float maxScroll = (m_preferred.y() - m_size.y());
        float currentScroll = maxScroll * m_scroll;
        currentScroll -= rel.y() * 16;
        m_scroll = currentScroll / maxScroll;
        if (m_scroll < 0)
            m_scroll = 0;
        else if (m_scroll > 1)
            m_scroll = 1;
        m_layoutChanged = true;
        return true;
    } else {
        return Window::scroll_event(p, rel);
    }
}

void ResizableWindow::perform_layout(NVGcontext* ctx) {
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
    if (m_size.y() < m_preferred.y()) {
        float maxScroll = (m_preferred.y() - m_size.y());
        float currentScroll = maxScroll * m_scroll;
        for (auto c : m_children) {
            c->set_position(Vector2i(c->position().x(), c->position().y() - currentScroll));
        }
    }
}

void ResizableWindow::draw(NVGcontext* ctx) {
    m_preferred = this->preferred_size(ctx);
    if (m_layoutChanged) {
        perform_layout(ctx);
        m_layoutChanged = false;
    }

    /* Draw window */
    drawWindowFrame(ctx);
    /* Draw a drop shadow */
    drawWindowShadow(ctx);
    if (!m_title.empty()) {
        /* Draw header */
        drawTitleBar(ctx);
    }
    nvgIntersectScissor(ctx, m_pos.x(), m_pos.y() + m_theme->m_window_header_height, m_size.x(), m_size.y());
    Widget::draw(ctx);

    if (m_size.y() < m_preferred.y()) {
        int cr = m_theme->m_window_corner_radius;
        int hh = m_theme->m_window_header_height;
        float scrollbarAreaHeight = m_size.y() - hh - 10;
        float scrollHeight = scrollbarAreaHeight * m_size.y() / m_preferred.y();
        nvgBeginPath(ctx);
        nvgRoundedRect(ctx, m_pos.x() + m_size.x() - 8, m_pos.y() + hh + m_scroll * (scrollbarAreaHeight - scrollHeight), 6, scrollHeight, cr);

        nvgFillColor(ctx, m_theme->m_window_header_gradient_top);
        nvgFill(ctx);
    }
    nvgBeginPath(ctx);
    nvgMoveTo(ctx, m_pos.x() + m_size.x() - 10, m_pos.y() + m_size.y());
    nvgLineTo(ctx, m_pos.x() + m_size.x(), m_pos.y() + m_size.y() - 10);
    nvgLineTo(ctx, m_pos.x() + m_size.x(), m_pos.y() + m_size.y());
    nvgFillColor(ctx, m_theme->m_window_header_gradient_top);
    nvgFill(ctx);

    nvgRestore(ctx);
}

NAMESPACE_END(nanogui)
