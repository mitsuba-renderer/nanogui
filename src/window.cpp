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
#include <nanogui/popup.h>
#include <nanogui/scrollpanel.h>
#include <nanogui/messagedialog.h>

NAMESPACE_BEGIN(nanogui)

Window::Window(Widget* parent, const std::string& title, bool resizable)
    : Widget(parent), m_title(title), m_button_panel(nullptr), m_modal(false), m_drag(false),
    m_resize_dir(Vector2i(0, 0)), m_first_size(0), m_resizable(resizable), m_can_move(true), m_snap_offset(20), m_can_snap(true), m_draw_shadow(true) { }

Vector2i Window::preferred_size(NVGcontext* ctx) const {
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
        std::max(result.x(), (int)(bounds[2] - bounds[0] + 20)),
        std::max(result.y(), (int)(bounds[3] - bounds[1]))
    );
}

Widget* Window::button_panel() {

    if (!m_button_panel) {
        m_button_panel = new Widget(this);
        m_button_panel->set_layout(new BoxLayout(Orientation::Horizontal, Alignment::Middle, 0, 4));
    }
    return m_button_panel;
}

void Window::perform_layout(NVGcontext* ctx) {
    if (!m_button_panel) {

        if (m_children.size() == 1)
        {
            ScrollPanel* CanICastScrollPanel = dynamic_cast<ScrollPanel*>(m_children[0]);
            if (CanICastScrollPanel != NULL)
                CanICastScrollPanel->set_fixed_size(m_size - 10 - Vector2i(0, !m_title.empty() ? m_theme->m_window_header_height : 0));
        }
        Widget::perform_layout(ctx);
    }
    else {
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
    //// Calclate the minimum size that the window can resize to.
    if (m_first_size == Vector2i(0, 0))
        m_first_size = m_size;

    m_min_size = Vector2i(40, 40);
    bool VScrollable = false;
    bool HScrollable = false;

    if (m_children.size() == 1)
    {
        ScrollPanel* CanICastScrollPanel = dynamic_cast<ScrollPanel*>(m_children[0]);
        if (CanICastScrollPanel != NULL)
        {
            VScrollable = CanICastScrollPanel->VScrollable();
            HScrollable = CanICastScrollPanel->HScrollable();
        }
    }
    if (!VScrollable)
        m_min_size.y() = m_first_size.y();
    if (!HScrollable)
        m_min_size.x() = m_first_size.x();

}

void Window::draw(NVGcontext* ctx) {
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
    if (m_draw_shadow)
    {
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
    if (m_resizable)
    {
        nvgSave(ctx);
        nvgBeginPath(ctx);
        nvgMoveTo(ctx, m_pos.x() + m_size.x() - 10, m_pos.y() + m_size.y());
        nvgLineTo(ctx, m_pos.x() + m_size.x(), m_pos.y() + m_size.y() - 10);
        nvgLineTo(ctx, m_pos.x() + m_size.x(), m_pos.y() + m_size.y());
        nvgFillColor(ctx, m_theme->m_window_header_gradient_top);
        nvgFill(ctx);
        nvgRestore(ctx);
    }

    nvgRestore(ctx);
    Widget::draw(ctx);
}

void Window::dispose() {
    Widget* widget = this;
    while (widget->parent())
        widget = widget->parent();
    ((Screen*)widget)->dispose_window(this);
}

void Window::center() {
    Widget* widget = this;
    while (widget->parent())
        widget = widget->parent();
    ((Screen*)widget)->center_window(this);
}

bool Window::mouse_enter_event(const Vector2i& p, bool enter) {
    Widget::mouse_enter_event(p, enter);
    return true;
}

bool Window::mouse_drag_event(const Vector2i& p, const Vector2i& rel, int button, int  modifiers) {
    if (m_can_move && m_drag && (button & (1 << GLFW_MOUSE_BUTTON_1)) != 0) {
        if (m_can_snap)
        {
            int MinLR = INT_MAX;
            int MinRL = INT_MAX;
            int MinTB = INT_MAX;
            int MinBT = INT_MAX;

            m_snap_tot_rel += rel;
            Vector2i temp_position = m_snap_init + m_snap_tot_rel;
            int Top = temp_position.y();
            int Bottom = temp_position.y() + size().y();
            int Left = temp_position.x();
            int Right = temp_position.x() + size().x();
            for (Widget* ChildWindow : screen()->children())
            {
                // Make sure the child ius a true window and not a hidden popup or a message box (derived classes)
                Window* CanICastWindow = dynamic_cast<Window*>(ChildWindow);
                Popup* CanICastPopup = dynamic_cast<Popup*>(ChildWindow);
                MessageDialog* CanICastDialog = dynamic_cast<MessageDialog*>(ChildWindow);
                bool IsWindow = (CanICastWindow != NULL && CanICastPopup == NULL && CanICastDialog == NULL);
                if (ChildWindow == this || !IsWindow)continue;// continue if the window is itself or the widget is not a window
                int child_Top = ChildWindow->position().y();
                int child_Bottom = ChildWindow->position().y() + ChildWindow->size().y();
                int child_Left = ChildWindow->position().x();
                int child_Right = ChildWindow->position().x() + ChildWindow->size().x();
                bool CheckX =
                    ((Bottom >= child_Top) && (Bottom <= child_Bottom))
                    || ((Top >= child_Top) && (Top <= child_Bottom))
                    || ((Top <= child_Top) && (Bottom >= child_Bottom));
                bool CheckY =
                    ((Right >= child_Left) && (Right <= child_Right))
                    || ((Left >= child_Left) && (Left <= child_Right))
                    || ((Left <= child_Left) && (Right >= child_Right));
                if (CheckX)
                {
                    int LR = abs(Left - child_Right);
                    int RL = abs(Right - child_Left);
                    if (LR < MinLR && LR < m_snap_offset) { MinLR = LR;  temp_position.x() = child_Right; }
                    if (RL < MinRL && RL < m_snap_offset) { MinRL = RL; temp_position.x() = child_Left - size().x(); }
                }
                if (CheckY)
                {
                    int TB = abs(Top - child_Bottom);
                    int BT = abs(Bottom - child_Top);
                    if (TB < MinTB && TB < m_snap_offset) { MinTB = TB; temp_position.y() = child_Bottom; }
                    if (BT < MinBT && BT < m_snap_offset) { MinBT = BT; temp_position.y() = child_Top - size().y(); }
                }
            }
            m_pos = temp_position;
        }
        else
            m_pos += rel;
        m_pos = max(m_pos, Vector2i(0));
        m_pos = min(m_pos, parent()->size() - m_size);
        return true;
    }
    else if (m_resizable && m_resize && (button & (1 << GLFW_MOUSE_BUTTON_1)) != 0) {
        const Vector2i& lowerRightCorner = m_pos + m_size;
        const Vector2i& upperLeftCorner = m_pos;
        NVGcontext* ctx = static_cast<Screen*>(parent())->nvg_context();
        bool resized = false;


        if (m_resize_dir.x() == 1) {
            if ((rel.x() > 0 && p.x() >= lowerRightCorner.x()) || (rel.x() < 0)) {
                m_size.x() += rel.x();
                m_snap_tot_rel.x() += rel.x();
                resized = true;
            }
        }

        if (m_resize_dir.y() == 1) {
            if ((rel.y() > 0 && p.y() >= lowerRightCorner.y()) || (rel.y() < 0)) {
                m_size.y() += rel.y();
                m_snap_tot_rel.y() += rel.y();
                resized = true;
            }
        }

        if (m_can_snap)
        {
            int MinRL = INT_MAX;
            int MinBT = INT_MAX;

            if (m_snap_init.x() + m_snap_tot_rel.x() <= m_min_size.x())
                m_snap_tot_rel.x() = m_min_size.x() - m_snap_init.x();
            if (m_snap_init.y() + m_snap_tot_rel.y() <= m_min_size.y())
                m_snap_tot_rel.y() = m_min_size.y() - m_snap_init.y();
            Vector2i temp_size = m_snap_init + m_snap_tot_rel;
            int Top = position().y();
            int Bottom = position().y() + temp_size.y();
            int Left = position().x();
            int Right = position().x() + temp_size.x();
            for (Widget* ChildWindow : screen()->children())
            {
                // Make sure the child is a true window and not a hidden popup or a message box (derived classes)
                Window* CanICastWindow = dynamic_cast<Window*>(ChildWindow);
                Popup* CanICastPopup = dynamic_cast<Popup*>(ChildWindow);
                MessageDialog* CanICastDialog = dynamic_cast<MessageDialog*>(ChildWindow);
                bool IsWindow = (CanICastWindow != NULL && CanICastPopup == NULL && CanICastDialog == NULL);
                if (ChildWindow == this || !IsWindow)continue;// continue if the window is itself or the widget is not a window
                int child_Top = ChildWindow->position().y();
                int child_Bottom = ChildWindow->position().y() + ChildWindow->size().y();
                int child_Left = ChildWindow->position().x();
                int child_Right = ChildWindow->position().x() + ChildWindow->size().x();
                bool CheckX =
                    ((Bottom >= child_Top) && (Bottom <= child_Bottom))
                    || ((Top >= child_Top) && (Top <= child_Bottom))
                    || ((Top <= child_Top) && (Bottom >= child_Bottom));
                bool CheckY =
                    ((Right >= child_Left) && (Right <= child_Right))
                    || ((Left >= child_Left) && (Left <= child_Right))
                    || ((Left <= child_Left) && (Right >= child_Right));
                if (CheckX)
                {
                    int RL = abs(Right - child_Left);
                    if (RL < MinRL && RL < m_snap_offset) {
                        MinRL = RL;
                        m_size.x() = child_Left - Left;
                    }
                }
                if (CheckY)
                {
                    int BT = abs(Bottom - child_Top);
                    if (BT < MinBT && BT < m_snap_offset) {
                        MinBT = BT;
                        m_size.y() = child_Top - Top;
                    }
                }

            }
            {// Do a final check for the screen boarders
                int RL = abs(Right - screen()->size().x());
                if (RL < MinRL && RL < m_snap_offset) {
                    MinRL = RL;
                    m_size.x() = screen()->size().x() - Left;
                }
                int BT = abs(Bottom - screen()->size().y());
                if (BT < MinBT && BT < m_snap_offset) {
                    MinBT = BT;
                    m_size.y() = screen()->size().y() - Top;
                }

            }
            if (MinRL == INT_MAX)
                m_size.x() = temp_size.x();
            if (MinBT == INT_MAX)
                m_size.y() = temp_size.y();
        }

        m_size = max(m_size, m_min_size);

        if (resized)
            perform_layout(ctx);
        return true;
    }

    if (Widget::mouse_drag_event(p, rel, button, modifiers))
        return true;

    return false;
}

bool Window::mouse_motion_event(const Vector2i& p, const Vector2i& rel, int button, int modifiers) {

    if (m_resizable && m_fixed_size.x() == 0 && check_horizontal_resize(p) && check_vertical_resize(p))
        m_cursor = Cursor::HVResize;
    else if (m_resizable && m_fixed_size.x() == 0 && check_horizontal_resize(p))
        m_cursor = Cursor::HResize;
    else if (m_resizable && m_fixed_size.y() == 0 && check_vertical_resize(p))
        m_cursor = Cursor::VResize;
    else
    {
        m_cursor = Cursor::Arrow;

        return (Widget::mouse_motion_event(p, rel, button, modifiers));
    }
    return true;

}

bool Window::mouse_button_event(const Vector2i& p, int button, bool down, int modifiers) {


    if (button == GLFW_MOUSE_BUTTON_1) {
        m_drag = down && !m_title.empty() && (p.y() - m_pos.y()) < m_theme->m_window_header_height;
        if (m_drag)
        {
            m_snap_init = position();
            m_snap_tot_rel = Vector2f(0, 0);
            return true;
        }
        else if (m_resizable && down) {
            m_resize_dir.x() = (m_fixed_size.x() == 0) ? check_horizontal_resize(p) : 0;
            m_resize_dir.y() = (m_fixed_size.y() == 0) ? check_vertical_resize(p) : 0;
            m_resize = m_resize_dir.x() != 0 || m_resize_dir.y() != 0;
            m_snap_init = size();
            m_snap_tot_rel = Vector2f(0, 0);
            if (m_resize)
                return true;
        }
    }
    if (Widget::mouse_button_event(p, button, down, modifiers))
        return true;

    return false;
}

bool Window::scroll_event(const Vector2i& p, const Vector2f& rel) {
    Widget::scroll_event(p, rel);
    return true;
}

void Window::refresh_relative_placement() {
    /* Overridden in \ref Popup */
}
bool Window::check_horizontal_resize(const Vector2i& mousePos) {
    int offset = m_theme->m_resize_area_offset;
    Vector2i lowerRightCorner = absolute_position() + size();
    int headerLowerLeftCornerY = absolute_position().y() + m_theme->m_window_header_height;

    if (mousePos.y() > headerLowerLeftCornerY &&
        mousePos.x() >= lowerRightCorner.x() - offset && mousePos.x() <= lowerRightCorner.x()) {
        return true;
    }

    return false;
}

bool Window::check_vertical_resize(const Vector2i& mousePos) {
    int offset = m_theme->m_resize_area_offset;
    Vector2i lowerRightCorner = absolute_position() + size();

    // Do not check for resize area on top of the window. It is to prevent conflict drag and resize event.
    if (mousePos.y() >= lowerRightCorner.y() - offset && mousePos.y() <= lowerRightCorner.y()) {
        return true;
    }

    return false;
}
NAMESPACE_END(nanogui)
