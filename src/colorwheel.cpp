/*
    src/colorwheel.cpp -- fancy analog widget to select a color value

    This widget was contributed by Dmitriy Morozov.

    NanoGUI was developed by Wenzel Jakob <wenzel.jakob@epfl.ch>.
    The widget drawing code is based on the NanoVG demo application
    by Mikko Mononen.

    All rights reserved. Use of this source code is governed by a
    BSD-style license that can be found in the LICENSE.txt file.
*/

#include <nanogui/colorwheel.h>
#include <nanogui/theme.h>
#include <nanogui/opengl.h>
#include <enoki/matrix.h>

NAMESPACE_BEGIN(nanogui)

ColorWheel::ColorWheel(Widget *parent, const Color& rgb)
    : Widget(parent), m_drag_region(None) {
    set_color(rgb);
}

Vector2i ColorWheel::preferred_size(NVGcontext *) const {
    return { 100, 100. };
}

void ColorWheel::draw(NVGcontext *ctx) {
    Widget::draw(ctx);

    if (!m_visible)
        return;

    float x = m_pos.x(),  y = m_pos.y(),
          w = m_size.x(), h = m_size.y();

    NVGcontext* vg = ctx;

    float hue = m_hue;
    NVGpaint paint;

    nvgSave(vg);

    float cx = x + w*0.5f;
    float cy = y + h*0.5f;
    float r1 = (w < h ? w : h) * 0.5f - 5.0f;
    float r0 = r1 * .75f;

    float aeps = 0.5f / r1;   // half a pixel arc length in radians (2pi cancels out).

    for (int i = 0; i < 6; i++) {
        float a0 = (float)i / 6.0f * NVG_PI * 2.0f - aeps;
        float a1 = (float)(i+1.0f) / 6.0f * NVG_PI * 2.0f + aeps;
        nvgBeginPath(vg);
        nvgArc(vg, cx,cy, r0, a0, a1, NVG_CW);
        nvgArc(vg, cx,cy, r1, a1, a0, NVG_CCW);
        nvgClosePath(vg);
        float ax = cx + cosf(a0) * (r0+r1)*0.5f;
        float ay = cy + sinf(a0) * (r0+r1)*0.5f;
        float bx = cx + cosf(a1) * (r0+r1)*0.5f;
        float by = cy + sinf(a1) * (r0+r1)*0.5f;
        paint = nvgLinearGradient(vg, ax, ay, bx, by,
                                  nvgHSLA(a0 / (NVG_PI * 2), 1.0f, 0.55f, 255),
                                  nvgHSLA(a1 / (NVG_PI * 2), 1.0f, 0.55f, 255));
        nvgFillPaint(vg, paint);
        nvgFill(vg);
    }

    nvgBeginPath(vg);
    nvgCircle(vg, cx,cy, r0-0.5f);
    nvgCircle(vg, cx,cy, r1+0.5f);
    nvgStrokeColor(vg, nvgRGBA(0,0,0,64));
    nvgStrokeWidth(vg, 1.0f);
    nvgStroke(vg);

    // Selector
    nvgSave(vg);
    nvgTranslate(vg, cx,cy);
    nvgRotate(vg, hue*NVG_PI*2);

    // Marker on
    float u = std::max(r1/50, 1.5f);
          u = std::min(u, 4.f);
    nvgStrokeWidth(vg, u);
    nvgBeginPath(vg);
    nvgRect(vg, r0-1,-2*u,r1-r0+2,4*u);
    nvgStrokeColor(vg, nvgRGBA(255,255,255,192));
    nvgStroke(vg);

    paint = nvgBoxGradient(vg, r0-3,-5,r1-r0+6,10, 2,4, nvgRGBA(0,0,0,128), nvgRGBA(0,0,0,0));
    nvgBeginPath(vg);
    nvgRect(vg, r0-2-10,-4-10,r1-r0+4+20,8+20);
    nvgRect(vg, r0-2,-4,r1-r0+4,8);
    nvgPathWinding(vg, NVG_HOLE);
    nvgFillPaint(vg, paint);
    nvgFill(vg);

    // Center triangle
    float r = r0 - 6;
    float ax = -0.5f * r;
    float ay =  0.5f * std::sqrt(3.f) * r;
    float bx = -0.5f * r;
    float by = -0.5f * std::sqrt(3.f) * r;
    nvgBeginPath(vg);
    nvgMoveTo(vg, r,0);
    nvgLineTo(vg, ax, ay);
    nvgLineTo(vg, bx, by);
    nvgClosePath(vg);
    paint = nvgLinearGradient(vg, r, 0, ax, ay, nvgHSLA(hue, 1.0f, 0.5f, 255),
                              nvgRGBA(255, 255, 255, 255));
    nvgFillPaint(vg, paint);
    nvgFill(vg);
    paint = nvgLinearGradient(vg, (r + ax) * 0.5f, (0 + ay) * 0.5f, bx, by,
                              nvgRGBA(0, 0, 0, 0), nvgRGBA(0, 0, 0, 255));
    nvgFillPaint(vg, paint);
    nvgFill(vg);
    nvgStrokeColor(vg, nvgRGBA(0, 0, 0, 64));
    nvgStroke(vg);

    // Select circle on triangle
    float sx = r*(1 - m_white - m_black) + ax*m_white + bx*m_black;
    float sy =                           ay*m_white + by*m_black;

    nvgStrokeWidth(vg, u);
    nvgBeginPath(vg);
    nvgCircle(vg, sx,sy,2*u);
    nvgStrokeColor(vg, nvgRGBA(255,255,255,192));
    nvgStroke(vg);

    nvgRestore(vg);

    nvgRestore(vg);
}

bool ColorWheel::mouse_button_event(const Vector2i &p, int button, bool down,
                                  int modifiers) {
    Widget::mouse_button_event(p, button, down, modifiers);
    if (!m_enabled || button != GLFW_MOUSE_BUTTON_1)
        return false;

    if (down) {
        m_drag_region = adjust_position(p);
        return m_drag_region != None;
    } else {
        m_drag_region = None;
        return true;
    }
}

bool ColorWheel::mouse_drag_event(const Vector2i &p, const Vector2i &,
                                int, int) {
    return adjust_position(p, m_drag_region) != None;
}

ColorWheel::Region ColorWheel::adjust_position(const Vector2i &p, Region considered_regions) {
    float x = p.x() - m_pos.x(),
          y = p.y() - m_pos.y(),
          w = m_size.x(),
          h = m_size.y();

    float cx = w * 0.5f;
    float cy = h * 0.5f;
    float r1 = (w < h ? w : h) * 0.5f - 5.0f;
    float r0 = r1 * .75f;

    x -= cx;
    y -= cy;

    float mr = std::sqrt(x*x + y*y);

    if ((considered_regions & OuterCircle) &&
        ((mr >= r0 && mr <= r1) || (considered_regions == OuterCircle))) {
        if (!(considered_regions & OuterCircle))
            return None;
        m_hue = std::atan(y / x);
        if (x < 0)
            m_hue += NVG_PI;
        m_hue /= 2*NVG_PI;

        if (m_callback)
            m_callback(color());

        return OuterCircle;
    }

    auto sc = enoki::sincos(-m_hue * 2 * NVG_PI);
    Vector2f xy(sc.second * x - sc.first * y,
                sc.first * x + sc.second * y);

    float r = r0 - 6;
    float l0 = (r-xy.x() + std::sqrt(3) * xy.y()) / (3*r);
    float l1 = (r-xy.x() - std::sqrt(3) * xy.y()) / (3*r);
    float l2 = 1 - l0 - l1;
    bool triangle_test = l0 >= 0 && l0 <= 1.f && l1 >= 0.f && l1 <= 1.f &&
                         l2 >= 0.f && l2 <= 1.f;

    if ((considered_regions & InnerTriangle) &&
        (triangle_test || considered_regions == InnerTriangle)) {
        if (!(considered_regions & InnerTriangle))
            return None;
        l0 = std::min(std::max(0.f, l0), 1.f);
        l1 = std::min(std::max(0.f, l1), 1.f);
        l2 = std::min(std::max(0.f, l2), 1.f);
        float sum = l0 + l1 + l2;
        l0 /= sum;
        l1 /= sum;
        m_white = l0;
        m_black = l1;
        if (m_callback)
            m_callback(color());
        return InnerTriangle;
    }

    return None;
}

Color ColorWheel::hue2rgb(float h) const {
    float s = 1., v = 1.;

    if (h < 0) h += 1;

    int i = int(h * 6);
    float f = h * 6 - i;
    float p = v * (1 - s);
    float q = v * (1 - f * s);
    float t = v * (1 - (1 - f) * s);

    float r = 0, g = 0, b = 0;
    switch (i % 6) {
        case 0: r = v, g = t, b = p; break;
        case 1: r = q, g = v, b = p; break;
        case 2: r = p, g = v, b = t; break;
        case 3: r = p, g = q, b = v; break;
        case 4: r = t, g = p, b = v; break;
        case 5: r = v, g = p, b = q; break;
    }

    return { r, g, b, 1.f };
}

Color ColorWheel::color() const {
    Color rgb    = hue2rgb(m_hue);
    Color black  { 0.f, 0.f, 0.f, 1.f };
    Color white  { 1.f, 1.f, 1.f, 1.f };
    return rgb * (1 - m_white - m_black) + black * m_black + white * m_white;
}

void ColorWheel::set_color(const Color &rgb) {
    float r = rgb[0], g = rgb[1], b = rgb[2];

    float M = std::max({ r, g, b });
    float m = std::min({ r, g, b });

    if (M == m) {
        float l = 0.5f * (M + m);
        m_hue = 0.f;
        m_black = 1.f - l;
        m_white = l;
    } else {
        float d = M - m, h;

        if (M == r)
            h = (g - b) / d + (g < b ? 6 : 0);
        else if (M == g)
            h = (b - r) / d + 2;
        else
            h = (r - g) / d + 4;
        h /= 6;

        Color ch = hue2rgb(m_hue);
        float M2 = std::max({ ch[0], ch[1], ch[2] });
        float m2 = std::min({ ch[0], ch[1], ch[2] });

        m_white = (M*m2 - m*M2) / (m2 - M2);
        m_black = (M + m2 + m*M2 - m - M*m2 - M2) / (m2 - M2);
        m_hue = h;
    }
}

NAMESPACE_END(nanogui)

