#include <nanogui/knob.h>
#include <nanogui/theme.h>
#include <nanogui/opengl.h>

NAMESPACE_BEGIN(nanogui)

Knob::Knob(Widget *parent, int rad)
    : Widget(parent), m_value(0.0f), m_notches{12}, m_range(0.f, 1.f),
      m_highlighted_range(0.f, 0.f)
{
    m_highlight_color = Color(255, 80, 80, 70);
    Widget::set_size({rad,rad});
}

const float Knob::m_pi = std::acos(-1.0f);

Vector2i Knob::preferred_size(NVGcontext *) const {
    return m_size;
}

bool Knob::mouse_drag_event(const Vector2i &p, const Vector2i & /* rel */,
                              int /* button */, int /* modifiers */) {
    if (!m_enabled)
        return false;

    set_value_by_mouse_position(p);
    if (m_callback)
        m_callback(m_value);

    return true;
}

bool Knob::mouse_button_event(const Vector2i &p, int /* button */, bool down, int /* modifiers */) {
    if (!m_enabled)
        return false;

    set_value_by_mouse_position(p);
    if (m_final_callback && !down)
        m_final_callback(m_value);

    return true;
}

void Knob::draw(NVGcontext* ctx) {
    Vector2f center = Vector2f(m_pos) + Vector2f(m_size) * 0.5f;
    float radius = static_cast<float>( width()/2 - 1 );

    NVGpaint knob_gradient = nvgRadialGradient(ctx,
        center.x(), center.y(), radius * 0.65f, radius,
        m_theme->m_border_light, m_theme->m_border_medium);

    nvgBeginPath(ctx);
    nvgCircle(ctx, center.x(), center.y(), radius);
    nvgFillPaint(ctx, knob_gradient);
    nvgStroke(ctx);
    nvgFill(ctx);

    nvgBeginPath(ctx);
    nvgMoveTo(ctx,center.x() + 0.7f * radius, center.y());
    nvgLineTo(ctx,center.x() + radius, center.y());
    nvgFillColor(ctx, Color(200, 200, 200, 200) );
    nvgFill(ctx);

    float angle_step = (2.0f * m_pi) / static_cast<float>(m_notches);
    for(int i=1; i<m_notches; ++i){
        nvgBeginPath(ctx);
        float angle = static_cast<float>(i) * angle_step;
        float begin_radius = 0.9f * radius;
        auto begin_notch = center + Vector2f{begin_radius * std::cos(angle), begin_radius * std::sin(angle)};
        auto end_notch = center + Vector2f{radius * std::cos(angle), radius * std::sin(angle)};
        nvgMoveTo(ctx,begin_notch.x(), begin_notch.y());
        nvgLineTo(ctx,end_notch.x(), end_notch.y());
        nvgFillColor(ctx, Color(200, 200, 200, 200) );
        nvgFill(ctx);
    }

    auto clove_radius = 0.7f * radius;
    nvgBeginPath(ctx);
    nvgArc(ctx, center.x(), center.y(), clove_radius, 0.3f * m_pi, 0, NVGwinding::NVG_CCW);
    nvgArc(ctx, center.x() + 0.34f * radius, center.y(), 0.5f * clove_radius, 0, 0.5f * m_pi, NVGwinding::NVG_CW);
    nvgFillColor(ctx, Color(200, 200, 200, 200) );
    nvgStroke(ctx);
    nvgFill(ctx);

    if( m_highlighted_range.second > m_highlighted_range.first &&
            m_highlighted_range.first >= m_range.first &&
            m_highlighted_range.second <= m_range.second ){
        float angle_begin = 2.0f * m_pi * (m_highlighted_range.first - m_range.first) / (m_range.second - m_range.first);
        float angle_end = 2.0f * m_pi * (m_highlighted_range.second - m_range.first) / (m_range.second - m_range.first);

        nvgBeginPath(ctx);
        nvgArc(ctx, center.x(), center.y(), radius, angle_end, angle_begin, NVGwinding::NVG_CCW);
        nvgArc(ctx, center.x(), center.y(), 0.85f * radius, angle_begin, angle_end, NVGwinding::NVG_CW);
        nvgFillColor(ctx, m_highlight_color);
        nvgFill(ctx);
    }

    float hand_angle = 2.0f * m_pi * (value() - range().first) / (range().second - range().first);
    float hand_dist = 0.8f * radius;
    Vector2f hand_center = center + Vector2f{hand_dist * std::cos(hand_angle), hand_dist * std::sin(hand_angle)};
    float hand_radius = radius / 8;

    nvgBeginPath(ctx);
    nvgCircle(ctx, hand_center.x(), hand_center.y(), hand_radius);
    nvgFillColor(ctx, Color(200, 200, 200, 255));
    nvgStroke(ctx);
    nvgFill(ctx);
}

void Knob::set_value_by_mouse_position(const Vector2i &p)
{
    Vector2f center = Vector2f(m_pos) + Vector2f(m_size) * 0.5f;
    float angle = m_pi/2 - std::atan( ( static_cast<float>(p.x() ) - center.x()) / ( static_cast<float>(p.y() ) - center.y()) );
    float value01 = 0.5f * angle / m_pi + ( static_cast<float>(p.y()) - center.y() < 0 ? 0.5f : 0.0f);
    m_value = m_range.first + value01 * (m_range.second - m_range.first);
}

NAMESPACE_END(nanogui)
