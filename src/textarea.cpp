/*
    src/textarea.cpp -- Multi-line read-only text widget

    NanoGUI was developed by Wenzel Jakob <wenzel.jakob@epfl.ch>.
    The widget drawing code is based on the NanoVG demo application
    by Mikko Mononen.

    All rights reserved. Use of this source code is governed by a
    BSD-style license that can be found in the LICENSE.txt file.
*/

#include <nanogui/textarea.h>
#include <nanogui/opengl.h>
#include <nanogui/theme.h>
#include <nanogui/screen.h>
#include <nanogui/vscrollpanel.h>

NAMESPACE_BEGIN(nanogui)

TextArea::TextArea(Widget *parent) : Widget(parent),
  m_foreground_color(Color(0, 0)), m_background_color(Color(0, 0)),
  m_font("sans"), m_offset(0), m_max_size(0), m_padding(0) {
}

void TextArea::append(const std::string &text) {
    NVGcontext *ctx = screen()->nvg_context();

    nvgFontSize(ctx, font_size());
    nvgFontFace(ctx, m_font.c_str());

    const char *str = text.c_str();
    do {
        const char *begin = str;

        while (*str != 0 && *str != '\n')
            str++;

        std::string line(begin, str);
        m_blocks.push_back(Block { m_offset, line, m_foreground_color });

        m_offset.x() += nvgTextBounds(ctx, 0, 0, line.c_str(), nullptr, nullptr);
        m_max_size = max(m_max_size, m_offset);
        if (*str == '\n') {
            m_offset = Vector2i(0, m_offset.y() + font_size());
            m_max_size = max(m_max_size, m_offset);
        }
    } while (*str++ != 0);

    VScrollPanel *vscroll = dynamic_cast<VScrollPanel *>(m_parent);
    if (vscroll)
        vscroll->perform_layout(ctx);
}

void TextArea::clear() {
    m_blocks.clear();
    m_offset = m_max_size = 0;
}

Vector2i TextArea::preferred_size(NVGcontext *) const {
    return m_max_size + m_padding * 2;
}

void TextArea::draw(NVGcontext *ctx) {
    VScrollPanel *vscroll = dynamic_cast<VScrollPanel *>(m_parent);

    std::vector<Block>::iterator start_it = m_blocks.begin(),
                                 end_it = m_blocks.end();
    if (vscroll) {
        int window_offset = -position().y(),
            window_size = vscroll->size().y();

        start_it = std::lower_bound(
            m_blocks.begin(),
            m_blocks.end(),
            window_offset,
            [&](const Block &block, int value) { return block.offset.y() + font_size() < value; }
        );

        end_it = std::upper_bound(
            m_blocks.begin(),
            m_blocks.end(),
            window_offset + window_size,
            [](int value, const Block &block) { return value < block.offset.y(); }
        );
    }

    if (m_background_color.w() != 0.f) {
        nvgFillColor(ctx, m_background_color);
        nvgBeginPath(ctx);
        nvgRect(ctx, m_pos.x(), m_pos.y(), m_size.x(), m_size.y());
        nvgFill(ctx);
    }

    nvgFontFace(ctx, m_font.c_str());
    nvgFontSize(ctx, font_size());
    nvgTextAlign(ctx, NVG_ALIGN_LEFT | NVG_ALIGN_TOP);

    for (auto it = start_it; it != end_it; ++it) {
        const Block &block = *it;
        Color color = block.color;
        if (color == Color(0, 0))
            color = m_theme->m_text_color;
        nvgFillColor(ctx, color);
        nvgText(ctx, m_pos.x() + block.offset.x() + m_padding,
                     m_pos.y() + block.offset.y() + m_padding,
                block.text.c_str(), nullptr);
    }
}

NAMESPACE_END(nanogui)
