/*
    src/button.cpp -- [Normal/Toggle/Radio/Popup] Button widget

    NanoGUI was developed by Wenzel Jakob <wenzel.jakob@epfl.ch>.
    The widget drawing code is based on the NanoVG demo application
    by Mikko Mononen.

    All rights reserved. Use of this source code is governed by a
    BSD-style license that can be found in the LICENSE.txt file.
*/

#include <nanogui/button.h>
#include <nanogui/popupbutton.h>
#include <nanogui/theme.h>
#include <nanogui/opengl.h>

NAMESPACE_BEGIN(nanogui)

Button::Button(Widget *parent, const std::string &caption, int icon)
    : Widget(parent), m_caption(caption), m_icon(icon),
      m_icon_position(IconPosition::LeftCentered), m_pushed(false),
      m_flags(NormalButton), m_background_color(Color(0, 0)),
      m_text_color(Color(0, 0)) { }

Vector2i Button::preferred_size(NVGcontext *ctx) const {
    int font_size = m_font_size == -1 ? m_theme->m_button_font_size : m_font_size;
    nvgFontSize(ctx, font_size);
    nvgFontFace(ctx, "sans-bold");
    float tw = nvgTextBounds(ctx, 0,0, m_caption.c_str(), nullptr, nullptr);
    float iw = 0.0f, ih = font_size;

    if (m_icon) {
        if (nvg_is_font_icon(m_icon)) {
            ih *= icon_scale();
            nvgFontFace(ctx, "icons");
            nvgFontSize(ctx, ih);
            iw = nvgTextBounds(ctx, 0, 0, utf8(m_icon).data(), nullptr, nullptr)
                + m_size.y() * 0.15f;
        } else {
            int w, h;
            ih *= 0.9f;
            nvgImageSize(ctx, m_icon, &w, &h);
            iw = w * ih / h;
        }
    }
    return Vector2i((int)(tw + iw) + 20, font_size + 10);
}

bool Button::mouse_enter_event(const Vector2i &p, bool enter) {
    Widget::mouse_enter_event(p, enter);
    return true;
}

bool Button::mouse_button_event(const Vector2i &p, int button, bool down, int modifiers) {
    Widget::mouse_button_event(p, button, down, modifiers);
    /* Temporarily increase the reference count of the button in case the
       button causes the parent window to be destructed */
    ref<Button> self = this;

    if (m_enabled == 1 &&
        ((button == GLFW_MOUSE_BUTTON_1 && !(m_flags & MenuButton)) ||
         (button == GLFW_MOUSE_BUTTON_2 &&  (m_flags & MenuButton)))) {
        bool pushed_backup = m_pushed;
        if (down) {
            if (m_flags & RadioButton) {
                if (m_button_group.empty()) {
                    for (auto widget : parent()->children()) {
                        Button *b = dynamic_cast<Button *>(widget);
                        if (b != this && b && (b->flags() & RadioButton) && b->m_pushed) {
                            b->m_pushed = false;
                            if (b->m_change_callback)
                                b->m_change_callback(false);
                        }
                    }
                } else {
                    for (auto b : m_button_group) {
                        if (b != this && (b->flags() & RadioButton) && b->m_pushed) {
                            b->m_pushed = false;
                            if (b->m_change_callback)
                                b->m_change_callback(false);
                        }
                    }
                }
            }
            if (m_flags & PopupButton) {
                for (auto widget : parent()->children()) {
                    Button *b = dynamic_cast<Button *>(widget);
                    if (b != this && b && (b->flags() & PopupButton) && b->m_pushed) {
                        b->m_pushed = false;
                        if (b->m_change_callback)
                            b->m_change_callback(false);
                    }
                }
                dynamic_cast<nanogui::PopupButton*>(this)->popup()->request_focus();
            }
            if (m_flags & ToggleButton)
                m_pushed = !m_pushed;
            else
                m_pushed = true;
        } else if (m_pushed || (m_flags & MenuButton)) {
            if (contains(p) && m_callback)
                m_callback();
            if (m_flags & NormalButton)
                m_pushed = false;
        }
        if (pushed_backup != m_pushed && m_change_callback)
            m_change_callback(m_pushed);

        return true;
    }
    return false;
}

void Button::draw(NVGcontext *ctx) {
    Widget::draw(ctx);

    NVGcolor grad_top = m_theme->m_button_gradient_top_unfocused;
    NVGcolor grad_bot = m_theme->m_button_gradient_bot_unfocused;

    if (m_pushed || (m_mouse_focus && (m_flags & MenuButton))) {
        grad_top = m_theme->m_button_gradient_top_pushed;
        grad_bot = m_theme->m_button_gradient_bot_pushed;
    } else if (m_mouse_focus && m_enabled) {
        grad_top = m_theme->m_button_gradient_top_focused;
        grad_bot = m_theme->m_button_gradient_bot_focused;
    }

    nvgBeginPath(ctx);

    nvgRoundedRect(ctx, m_pos.x() + 1, m_pos.y() + 1.0f, m_size.x() - 2,
                   m_size.y() - 2, m_theme->m_button_corner_radius - 1);

    if (m_background_color.w() != 0) {
        nvgFillColor(ctx, Color(m_background_color[0], m_background_color[1],
                                m_background_color[2], 1.f));
        nvgFill(ctx);
        if (m_pushed) {
            grad_top.a = grad_bot.a = 0.8f;
        } else {
            double v = 1 - m_background_color.w();
            grad_top.a = grad_bot.a = m_enabled ? v : v * .5f + .5f;
        }
    }

    NVGpaint bg = nvgLinearGradient(ctx, m_pos.x(), m_pos.y(), m_pos.x(),
                                    m_pos.y() + m_size.y(), grad_top, grad_bot);

    nvgFillPaint(ctx, bg);
    nvgFill(ctx);

    nvgBeginPath(ctx);
    nvgStrokeWidth(ctx, 1.0f);
    nvgRoundedRect(ctx, m_pos.x() + 0.5f, m_pos.y() + (m_pushed ? 0.5f : 1.5f), m_size.x() - 1,
                   m_size.y() - 1 - (m_pushed ? 0.0f : 1.0f), m_theme->m_button_corner_radius);
    nvgStrokeColor(ctx, m_theme->m_border_light);
    nvgStroke(ctx);

    nvgBeginPath(ctx);
    nvgRoundedRect(ctx, m_pos.x() + 0.5f, m_pos.y() + 0.5f, m_size.x() - 1,
                   m_size.y() - 2, m_theme->m_button_corner_radius);
    nvgStrokeColor(ctx, m_theme->m_border_dark);
    nvgStroke(ctx);

    int font_size = m_font_size == -1 ? m_theme->m_button_font_size : m_font_size;
    nvgFontSize(ctx, font_size);
    nvgFontFace(ctx, "sans-bold");
    float tw = nvgTextBounds(ctx, 0,0, m_caption.c_str(), nullptr, nullptr);

    Vector2f center = Vector2f(m_pos) + Vector2f(m_size) * 0.5f;
    Vector2f text_pos(center.x() - tw * 0.5f, center.y() - 1);
    NVGcolor text_color =
        m_text_color.w() == 0 ? m_theme->m_text_color : m_text_color;
    if (!m_enabled)
        text_color = m_theme->m_disabled_text_color;

    if (m_icon) {
        auto icon = utf8(m_icon);

        float iw, ih = font_size;
        if (nvg_is_font_icon(m_icon)) {
            ih *= icon_scale();
            nvgFontSize(ctx, ih);
            nvgFontFace(ctx, "icons");
            iw = nvgTextBounds(ctx, 0, 0, icon.data(), nullptr, nullptr);
        } else {
            int w, h;
            ih *= 0.9f;
            nvgImageSize(ctx, m_icon, &w, &h);
            iw = w * ih / h;
        }
        if (m_caption != "")
            iw += m_size.y() * 0.15f;
        nvgFillColor(ctx, text_color);
        nvgTextAlign(ctx, NVG_ALIGN_LEFT | NVG_ALIGN_MIDDLE);
        Vector2f icon_pos = center;
        icon_pos.y() -= 1;

        if (m_icon_position == IconPosition::LeftCentered) {
            icon_pos.x() -= (tw + iw) * 0.5f;
            text_pos.x() += iw * 0.5f;
        } else if (m_icon_position == IconPosition::RightCentered) {
            text_pos.x() -= iw * 0.5f;
            icon_pos.x() += tw * 0.5f;
        } else if (m_icon_position == IconPosition::Left) {
            icon_pos.x() = m_pos.x() + 8;
        } else if (m_icon_position == IconPosition::Right) {
            icon_pos.x() = m_pos.x() + m_size.x() - iw - 8;
        }

        if (nvg_is_font_icon(m_icon)) {
            nvgText(ctx, icon_pos.x(), icon_pos.y()+1, icon.data(), nullptr);
        } else {
            NVGpaint img_paint = nvgImagePattern(ctx,
                    icon_pos.x(), icon_pos.y() - ih/2, iw, ih, 0, m_icon, m_enabled ? 0.5f : 0.25f);

            nvgFillPaint(ctx, img_paint);
            nvgFill(ctx);
        }
    }

    nvgFontSize(ctx, font_size);
    nvgFontFace(ctx, "sans-bold");
    nvgTextAlign(ctx, NVG_ALIGN_LEFT | NVG_ALIGN_MIDDLE);
    nvgFillColor(ctx, m_theme->m_text_color_shadow);
    nvgText(ctx, text_pos.x(), text_pos.y(), m_caption.c_str(), nullptr);
    nvgFillColor(ctx, text_color);
    nvgText(ctx, text_pos.x(), text_pos.y() + 1, m_caption.c_str(), nullptr);
}

NAMESPACE_END(nanogui)
