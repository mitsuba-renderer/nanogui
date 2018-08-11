/*
    nanogui/graph.h -- Simple graph widget for showing a function plot

    NanoGUI was developed by Wenzel Jakob <wenzel.jakob@epfl.ch>.
    The widget drawing code is based on the NanoVG demo application
    by Mikko Mononen.

    All rights reserved. Use of this source code is governed by a
    BSD-style license that can be found in the LICENSE.txt file.
*/
/** \file */

#pragma once

#include <nanogui/widget.h>

NAMESPACE_BEGIN(nanogui)

/**
 * \class Graph graph.h nanogui/graph.h
 *
 * \brief Simple graph widget for showing a function plot.
 */
class NANOGUI_EXPORT Graph : public Widget {
public:
    Graph(Widget *parent, const std::string &caption = "Untitled");

    const std::string &caption() const { return m_caption; }
    void set_caption(const std::string &caption) { m_caption = caption; }

    const std::string &header() const { return m_header; }
    void set_header(const std::string &header) { m_header = header; }

    const std::string &footer() const { return m_footer; }
    void set_footer(const std::string &footer) { m_footer = footer; }

    const Color &background_color() const { return m_background_color; }
    void set_background_color(const Color &background_color) { m_background_color = background_color; }

    const Color &stroke_color() const { return m_stroke_color; }
    void set_stroke_color(const Color &stroke_color) { m_stroke_color = stroke_color; }

    const Color &fill_color() const { return m_fill_color; }
    void set_fill_color(const Color &fill_color) { m_fill_color = fill_color; }

    const Color &text_color() const { return m_text_color; }
    void set_text_color(const Color &text_color) { m_text_color = text_color; }

    const std::vector<float> &values() const { return m_values; }
    std::vector<float> &values() { return m_values; }
    void set_values(const std::vector<float> &values) { m_values = values; }

    virtual Vector2i preferred_size(NVGcontext *ctx) const override;
    virtual void draw(NVGcontext *ctx) override;
protected:
    std::string m_caption, m_header, m_footer;
    Color m_background_color, m_fill_color, m_stroke_color, m_text_color;
    std::vector<float> m_values;
};

NAMESPACE_END(nanogui)
