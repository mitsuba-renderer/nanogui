/*
    nanogui/slider.cpp -- Fractional slider widget with mouse control

    NanoGUI was developed by Wenzel Jakob <wenzel.jakob@epfl.ch>.
    The widget drawing code is based on the NanoVG demo application
    by Mikko Mononen.

    All rights reserved. Use of this source code is governed by a
    BSD-style license that can be found in the LICENSE.txt file.
*/

#include <nanogui/slider.h>
#include <nanogui/theme.h>
#include <nanogui/opengl.h>

NAMESPACE_BEGIN(nanogui)

Slider::Slider(Widget *parent)
    : Widget(parent), m_value(0.0f), m_range(0.f, 1.f),
      m_highlighted_range(0.f, 0.f) {
    m_highlight_color = Color(255, 80, 80, 70);
}

Vector2i Slider::preferred_size(NVGcontext *) const {
    return Vector2i(70, 16);
}

bool Slider::mouse_drag_event(const Vector2i &p, const Vector2i & /* rel */,
                              int /* button */, int /* modifiers */) {
    if (!m_enabled)
        return false;

    const float kr = (int) (m_size.y() * 0.4f), kshadow = 3;
    const float start_x = kr + kshadow + m_pos.x() - 1;
    const float width_x = m_size.x() - 2 * (kr + kshadow);

    float value = (p.x() - start_x) / width_x, old_value = m_value;
    value = value * (m_range.second - m_range.first) + m_range.first;
    m_value = std::min(std::max(value, m_range.first), m_range.second);
    if (m_callback && m_value != old_value)
        m_callback(m_value);
    return true;
}

bool Slider::mouse_button_event(const Vector2i &p, int /* button */, bool down, int /* modifiers */) {
    if (!m_enabled)
        return false;

    const float kr = (int) (m_size.y() * 0.4f), kshadow = 3;
    const float start_x = kr + kshadow + m_pos.x() - 1;
    const float width_x = m_size.x() - 2 * (kr + kshadow);

    float value = (p.x() - start_x) / width_x, old_value = m_value;
    value = value * (m_range.second - m_range.first) + m_range.first;
    m_value = std::min(std::max(value, m_range.first), m_range.second);
    if (m_callback && m_value != old_value)
        m_callback(m_value);
    if (m_final_callback && !down)
        m_final_callback(m_value);
    return true;
}

void Slider::draw(NVGcontext* ctx) {
    Vector2f center = Vector2f(m_pos) + Vector2f(m_size) * 0.5f;
    float kr = (int) (m_size.y() * 0.4f), kshadow = 3;

    float start_x = kr + kshadow + m_pos.x();
    float width_x = m_size.x() - 2*(kr+kshadow);

    Vector2f knob_pos(start_x + (m_value - m_range.first) /
            (m_range.second - m_range.first) * width_x,
            center.y() + 0.5f);

    NVGpaint bg = nvgBoxGradient(
        ctx, start_x, center.y() - 3 + 1, width_x, 6, 3, 3,
        Color(0, m_enabled ? 32 : 10), Color(0, m_enabled ? 128 : 210));

    nvgBeginPath(ctx);
    nvgRoundedRect(ctx, start_x, center.y() - 3 + 1, width_x, 6, 2);
    nvgFillPaint(ctx, bg);
    nvgFill(ctx);

    if (m_highlighted_range.second != m_highlighted_range.first) {
        nvgBeginPath(ctx);
        nvgRoundedRect(ctx, start_x + m_highlighted_range.first * m_size.x(),
                       center.y() - kshadow + 1,
                       width_x *
                           (m_highlighted_range.second - m_highlighted_range.first),
                       kshadow * 2, 2);
        nvgFillColor(ctx, m_highlight_color);
        nvgFill(ctx);
    }

    NVGpaint knob_shadow =
        nvgRadialGradient(ctx, knob_pos.x(), knob_pos.y(), kr - kshadow,
                          kr + kshadow, Color(0, 64), m_theme->m_transparent);

    nvgBeginPath(ctx);
    nvgRect(ctx, knob_pos.x() - kr - 5, knob_pos.y() - kr - 5, kr * 2 + 10,
            kr * 2 + 10 + kshadow);
    nvgCircle(ctx, knob_pos.x(), knob_pos.y(), kr);
    nvgPathWinding(ctx, NVG_HOLE);
    nvgFillPaint(ctx, knob_shadow);
    nvgFill(ctx);

    NVGpaint knob = nvgLinearGradient(ctx,
        m_pos.x(), center.y() - kr, m_pos.x(), center.y() + kr,
        m_theme->m_border_light, m_theme->m_border_medium);
    NVGpaint knob_reverse = nvgLinearGradient(ctx,
        m_pos.x(), center.y() - kr, m_pos.x(), center.y() + kr,
        m_theme->m_border_medium,
        m_theme->m_border_light);

    nvgBeginPath(ctx);
    nvgCircle(ctx, knob_pos.x(), knob_pos.y(), kr);
    nvgStrokeColor(ctx, m_theme->m_border_dark);
    nvgFillPaint(ctx, knob);
    nvgStroke(ctx);
    nvgFill(ctx);
    nvgBeginPath(ctx);
    nvgCircle(ctx, knob_pos.x(), knob_pos.y(), kr/2);
    nvgFillColor(ctx, Color(150, m_enabled ? 255 : 100));
    nvgStrokePaint(ctx, knob_reverse);
    nvgStroke(ctx);
    nvgFill(ctx);
}

NAMESPACE_END(nanogui)
