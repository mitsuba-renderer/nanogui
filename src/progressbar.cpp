/*
    src/progressbar.cpp -- Standard widget for visualizing progress

    NanoGUI was developed by Wenzel Jakob <wenzel.jakob@epfl.ch>.
    The widget drawing code is based on the NanoVG demo application
    by Mikko Mononen.

    All rights reserved. Use of this source code is governed by a
    BSD-style license that can be found in the LICENSE.txt file.
*/

#include <nanogui/progressbar.h>
#include <nanogui/opengl.h>

NAMESPACE_BEGIN(nanogui)

ProgressBar::ProgressBar(Widget *parent)
    : Widget(parent), m_value(0.0f) {}

Vector2i ProgressBar::preferred_size(NVGcontext *) const {
    return Vector2i(70, 12);
}

void ProgressBar::draw(NVGcontext* ctx) {
    Widget::draw(ctx);

    NVGpaint paint = nvgBoxGradient(
        ctx, m_pos.x() + 1, m_pos.y() + 1,
        m_size.x()-2, m_size.y(), 3, 4, Color(0, 32), Color(0, 92));
    nvgBeginPath(ctx);
    nvgRoundedRect(ctx, m_pos.x(), m_pos.y(), m_size.x(), m_size.y(), 3);
    nvgFillPaint(ctx, paint);
    nvgFill(ctx);

    float value = std::min(std::max(0.0f, m_value), 1.0f);
    int bar_pos = (int) std::round((m_size.x() - 2) * value);

    paint = nvgBoxGradient(
        ctx, m_pos.x(), m_pos.y(),
        bar_pos+1.5f, m_size.y()-1, 3, 4,
        Color(220, 100), Color(128, 100));

    nvgBeginPath(ctx);
    nvgRoundedRect(
        ctx, m_pos.x()+1, m_pos.y()+1,
        bar_pos, m_size.y()-2, 3);
    nvgFillPaint(ctx, paint);
    nvgFill(ctx);
}

NAMESPACE_END(nanogui)
