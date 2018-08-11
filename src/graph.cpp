/*
    src/graph.cpp -- Simple graph widget for showing a function plot

    NanoGUI was developed by Wenzel Jakob <wenzel.jakob@epfl.ch>.
    The widget drawing code is based on the NanoVG demo application
    by Mikko Mononen.

    All rights reserved. Use of this source code is governed by a
    BSD-style license that can be found in the LICENSE.txt file.
*/

#include <nanogui/graph.h>
#include <nanogui/theme.h>
#include <nanogui/opengl.h>

NAMESPACE_BEGIN(nanogui)

Graph::Graph(Widget *parent, const std::string &caption)
    : Widget(parent), m_caption(caption) {
    m_background_color = Color(20, 128);
    m_fill_color = Color(255, 192, 0, 128);
    m_stroke_color = Color(100, 255);
    m_text_color = Color(240, 192);
}

Vector2i Graph::preferred_size(NVGcontext *) const {
    return Vector2i(180, 45);
}

void Graph::draw(NVGcontext *ctx) {
    Widget::draw(ctx);

    nvgBeginPath(ctx);
    nvgRect(ctx, m_pos.x(), m_pos.y(), m_size.x(), m_size.y());
    nvgFillColor(ctx, m_background_color);
    nvgFill(ctx);

    if (m_values.size() < 2)
        return;

    nvgBeginPath(ctx);
    nvgMoveTo(ctx, m_pos.x(), m_pos.y()+m_size.y());
    for (size_t i = 0; i < (size_t) m_values.size(); i++) {
        float value = m_values[i];
        float vx = m_pos.x() + i * m_size.x() / (float) (m_values.size() - 1);
        float vy = m_pos.y() + (1-value) * m_size.y();
        nvgLineTo(ctx, vx, vy);
    }

    nvgLineTo(ctx, m_pos.x() + m_size.x(), m_pos.y() + m_size.y());
    nvgStrokeColor(ctx, m_stroke_color);
    nvgStroke(ctx);
    if (m_fill_color.w() > 0) {
        nvgFillColor(ctx, m_fill_color);
        nvgFill(ctx);
    }

    nvgFontFace(ctx, "sans");

    if (!m_caption.empty()) {
        nvgFontSize(ctx, 14.0f);
        nvgTextAlign(ctx, NVG_ALIGN_LEFT | NVG_ALIGN_TOP);
        nvgFillColor(ctx, m_text_color);
        nvgText(ctx, m_pos.x() + 3, m_pos.y() + 1, m_caption.c_str(), NULL);
    }

    if (!m_header.empty()) {
        nvgFontSize(ctx, 18.0f);
        nvgTextAlign(ctx, NVG_ALIGN_RIGHT | NVG_ALIGN_TOP);
        nvgFillColor(ctx, m_text_color);
        nvgText(ctx, m_pos.x() + m_size.x() - 3, m_pos.y() + 1, m_header.c_str(), NULL);
    }

    if (!m_footer.empty()) {
        nvgFontSize(ctx, 15.0f);
        nvgTextAlign(ctx, NVG_ALIGN_RIGHT | NVG_ALIGN_BOTTOM);
        nvgFillColor(ctx, m_text_color);
        nvgText(ctx, m_pos.x() + m_size.x() - 3, m_pos.y() + m_size.y() - 1, m_footer.c_str(), NULL);
    }

    nvgBeginPath(ctx);
    nvgRect(ctx, m_pos.x(), m_pos.y(), m_size.x(), m_size.y());
    nvgStrokeColor(ctx, Color(100, 255));
    nvgStroke(ctx);
}

NAMESPACE_END(nanogui)
