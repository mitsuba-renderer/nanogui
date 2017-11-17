/*
    nanogui/tabheader.cpp -- Widget used to control tabs.

    The tab header widget was contributed by Stefan Ivanov.

    NanoGUI was developed by Wenzel Jakob <wenzel.jakob@epfl.ch>.
    The widget drawing code is based on the NanoVG demo application
    by Mikko Mononen.

    All rights reserved. Use of this source code is governed by a
    BSD-style license that can be found in the LICENSE.txt file.
*/

#include <nanogui/tabheader.h>
#include <nanogui/theme.h>
#include <nanogui/opengl.h>
#include <numeric>

NAMESPACE_BEGIN(nanogui)

TabHeader::TabButton::TabButton(TabHeader &header, const std::string &label)
    : m_header(&header), m_label(label) { }

Vector2i TabHeader::TabButton::preferred_size(NVGcontext *ctx) const {
    // No need to call nvg font related functions since this is done by the tab header implementation
    float bounds[4];
    int label_width = nvgTextBounds(ctx, 0, 0, m_label.c_str(), nullptr, bounds);
    int button_width = label_width + 2 * m_header->theme()->m_tab_button_horizontal_padding;
    int button_height = bounds[3] - bounds[1] + 2 * m_header->theme()->m_tab_button_vertical_padding;
    return Vector2i(button_width, button_height);
}

void TabHeader::TabButton::calculate_visible_string(NVGcontext *ctx) {
    // The size must have been set in by the enclosing tab header.
    NVGtextRow displayed_text;
    nvgTextBreakLines(ctx, m_label.c_str(), nullptr, m_size.x(), &displayed_text, 1);

    // Check to see if the text need to be truncated.
    if (displayed_text.next[0]) {
        auto truncated_width = nvgTextBounds(ctx, 0.0f, 0.0f,
                                            displayed_text.start, displayed_text.end, nullptr);
        auto dots_width = nvgTextBounds(ctx, 0.0f, 0.0f, dots, nullptr, nullptr);
        while ((truncated_width + dots_width + m_header->theme()->m_tab_button_horizontal_padding) > m_size.x()
                && displayed_text.end != displayed_text.start) {
            --displayed_text.end;
            truncated_width = nvgTextBounds(ctx, 0.0f, 0.0f,
                                           displayed_text.start, displayed_text.end, nullptr);
        }

        // Remember the truncated width to know where to display the dots.
        m_visible_width = truncated_width;
        m_visible_text.last = displayed_text.end;
    } else {
        m_visible_text.last = nullptr;
        m_visible_width = 0;
    }
    m_visible_text.first = displayed_text.start;
}

void TabHeader::TabButton::draw_at_position(NVGcontext *ctx, const Vector2i& position, bool active) {
    int x_pos = position.x();
    int y_pos = position.y();
    int width = m_size.x();
    int height = m_size.y();
    auto theme = m_header->theme();

    nvgSave(ctx);
    nvgIntersectScissor(ctx, x_pos, y_pos, width+1, height);
    if (!active) {
        // Background gradients
        NVGcolor grad_top = theme->m_button_gradient_top_pushed;
        NVGcolor grad_bot = theme->m_button_gradient_bot_pushed;

        // Draw the background.
        nvgBeginPath(ctx);
        nvgRoundedRect(ctx, x_pos + 1, y_pos + 1, width - 1, height + 1,
                       theme->m_button_corner_radius);
        NVGpaint background_color = nvgLinearGradient(ctx, x_pos, y_pos, x_pos, y_pos + height,
                                                     grad_top, grad_bot);
        nvgFillPaint(ctx, background_color);
        nvgFill(ctx);
    }

    if (active) {
        nvgBeginPath(ctx);
        nvgStrokeWidth(ctx, 1.0f);
        nvgRoundedRect(ctx, x_pos + 0.5f, y_pos + 1.5f, width,
                       height + 1, theme->m_button_corner_radius);
        nvgStrokeColor(ctx, theme->m_border_light);
        nvgStroke(ctx);

        nvgBeginPath(ctx);
        nvgRoundedRect(ctx, x_pos + 0.5f, y_pos + 0.5f, width,
                       height + 1, theme->m_button_corner_radius);
        nvgStrokeColor(ctx, theme->m_border_dark);
        nvgStroke(ctx);
    } else {
        nvgBeginPath(ctx);
        nvgRoundedRect(ctx, x_pos + 0.5f, y_pos + 1.5f, width,
                       height, theme->m_button_corner_radius);
        nvgStrokeColor(ctx, theme->m_border_dark);
        nvgStroke(ctx);
    }
    nvgResetScissor(ctx);
    nvgRestore(ctx);

    // Draw the text with some padding
    int text_x = x_pos + m_header->theme()->m_tab_button_horizontal_padding;
    int text_y = y_pos + m_header->theme()->m_tab_button_vertical_padding;
    NVGcolor text_color = m_header->theme()->m_text_color;
    nvgBeginPath(ctx);
    nvgFillColor(ctx, text_color);
    nvgText(ctx, text_x, text_y, m_visible_text.first, m_visible_text.last);
    if (m_visible_text.last != nullptr)
        nvgText(ctx, text_x + m_visible_width, text_y, dots, nullptr);
}

void TabHeader::TabButton::draw_active_border_at(NVGcontext *ctx, const Vector2i &position,
                                              float offset, const Color &color) {
    int x_pos = position.x();
    int y_pos = position.y();
    int width = m_size.x();
    int height = m_size.y();
    nvgBeginPath(ctx);
    nvgLineJoin(ctx, NVG_ROUND);
    nvgMoveTo(ctx, x_pos + offset, y_pos + height + offset);
    nvgLineTo(ctx, x_pos + offset, y_pos + offset);
    nvgLineTo(ctx, x_pos + width - offset, y_pos + offset);
    nvgLineTo(ctx, x_pos + width - offset, y_pos + height + offset);
    nvgStrokeColor(ctx, color);
    nvgStrokeWidth(ctx, m_header->theme()->m_tab_border_width);
    nvgStroke(ctx);
}

void TabHeader::TabButton::draw_inactive_border_at(NVGcontext *ctx, const Vector2i &position,
                                                float offset, const Color& color) {
    int x_pos = position.x();
    int y_pos = position.y();
    int width = m_size.x();
    int height = m_size.y();
    nvgBeginPath(ctx);
    nvgRoundedRect(ctx, x_pos + offset, y_pos + offset, width - offset, height - offset,
                   m_header->theme()->m_button_corner_radius);
    nvgStrokeColor(ctx, color);
    nvgStroke(ctx);
}


TabHeader::TabHeader(Widget* parent, const std::string& font)
    : Widget(parent), m_font(font) { }

void TabHeader::set_active_tab(int tab_index) {
    assert(tab_index < tab_count());
    m_active_tab = tab_index;
    if (m_callback)
        m_callback(tab_index);
}

int TabHeader::active_tab() const {
    return m_active_tab;
}

bool TabHeader::is_tab_visible(int index) const {
    return index >= m_visible_start && index < m_visible_end;
}

void TabHeader::add_tab(const std::string & label) {
    add_tab(tab_count(), label);
}

void TabHeader::add_tab(int index, const std::string &label) {
    assert(index <= tab_count());
    m_tab_buttons.insert(std::next(m_tab_buttons.begin(), index), TabButton(*this, label));
    set_active_tab(index);
}

int TabHeader::remove_tab(const std::string &label) {
    auto element = std::find_if(m_tab_buttons.begin(), m_tab_buttons.end(),
                                [&](const TabButton& tb) { return label == tb.label(); });
    int index = (int) std::distance(m_tab_buttons.begin(), element);
    if (element == m_tab_buttons.end())
        return -1;
    m_tab_buttons.erase(element);
    if (index == m_active_tab && index != 0)
        set_active_tab(index - 1);
    return index;
}

void TabHeader::remove_tab(int index) {
    assert(index < tab_count());
    m_tab_buttons.erase(std::next(m_tab_buttons.begin(), index));
    if (index == m_active_tab && index != 0)
        set_active_tab(index - 1);
}

const std::string& TabHeader::tab_label_at(int index) const {
    assert(index < tab_count());
    return m_tab_buttons[index].label();
}

int TabHeader::tab_index(const std::string &label) {
    auto it = std::find_if(m_tab_buttons.begin(), m_tab_buttons.end(),
                           [&](const TabButton& tb) { return label == tb.label(); });
    if (it == m_tab_buttons.end())
        return -1;
    return (int) (it - m_tab_buttons.begin());
}

void TabHeader::ensure_tab_visible(int index) {
    auto visible_area = visible_button_area();
    auto visible_width = visible_area.second.x() - visible_area.first.x();
    int allowed_visible_width = m_size.x() - 2 * theme()->m_tab_control_width;
    assert(allowed_visible_width >= visible_width);
    assert(index >= 0 && index < (int) m_tab_buttons.size());

    auto first = visible_begin();
    auto last = visible_end();
    auto goal = tab_iterator(index);

    // Reach the goal tab with the visible range.
    if (goal < first) {
        do {
            --first;
            visible_width += first->size().x();
        } while (goal < first);
        while (allowed_visible_width < visible_width) {
            --last;
            visible_width -= last->size().x();
        }
    }
    else if (goal >= last) {
        do {
            visible_width += last->size().x();
            ++last;
        } while (goal >= last);
        while (allowed_visible_width < visible_width) {
            visible_width -= first->size().x();
            ++first;
        }
    }

    // Check if it is possible to expand the visible range on either side.
    while (first != m_tab_buttons.begin()
           && std::next(first, -1)->size().x() < allowed_visible_width - visible_width) {
        --first;
        visible_width += first->size().x();
    }
    while (last != m_tab_buttons.end()
           && last->size().x() < allowed_visible_width - visible_width) {
        visible_width += last->size().x();
        ++last;
    }

    m_visible_start = (int) std::distance(m_tab_buttons.begin(), first);
    m_visible_end = (int) std::distance(m_tab_buttons.begin(), last);
}

std::pair<Vector2i, Vector2i> TabHeader::visible_button_area() const {
    if (m_visible_start == m_visible_end)
        return { Vector2i(0), Vector2i(0) };
    auto top_left = m_pos + Vector2i(theme()->m_tab_control_width, 0);
    auto width = std::accumulate(visible_begin(), visible_end(), theme()->m_tab_control_width,
                                 [](int acc, const TabButton& tb) {
        return acc + tb.size().x();
    });
    auto bottom_right = m_pos + Vector2i(width, m_size.y());
    return { top_left, bottom_right };
}

std::pair<Vector2i, Vector2i> TabHeader::active_button_area() const {
    if (m_visible_start == m_visible_end || m_active_tab < m_visible_start || m_active_tab >= m_visible_end)
        return { Vector2i(0), Vector2i(0) };
    auto width = std::accumulate(visible_begin(), active_iterator(), theme()->m_tab_control_width,
                                 [](int acc, const TabButton& tb) {
        return acc + tb.size().x();
    });
    auto top_left = m_pos + Vector2i(width, 0);
    auto bottom_right = m_pos + Vector2i(width + active_iterator()->size().x(), m_size.y());
    return { top_left, bottom_right };
}

void TabHeader::perform_layout(NVGcontext* ctx) {
    Widget::perform_layout(ctx);

    Vector2i current_position = Vector2i(0);
    // Place the tab buttons relative to the beginning of the tab header.
    for (auto& tab : m_tab_buttons) {
        auto tab_preferred = tab.preferred_size(ctx);
        if (tab_preferred.x() < theme()->m_tab_min_button_width)
            tab_preferred.x() = theme()->m_tab_min_button_width;
        else if (tab_preferred.x() > theme()->m_tab_max_button_width)
            tab_preferred.x() = theme()->m_tab_max_button_width;
        tab.set_size(tab_preferred);
        tab.calculate_visible_string(ctx);
        current_position.x() += tab_preferred.x();
    }
    calculate_visible_end();
    if (m_visible_start != 0 || m_visible_end != tab_count())
        m_overflowing = true;
}

Vector2i TabHeader::preferred_size(NVGcontext* ctx) const {
    // Set up the nvg context for measuring the text inside the tab buttons.
    nvgFontFace(ctx, m_font.c_str());
    nvgFontSize(ctx, font_size());
    nvgTextAlign(ctx, NVG_ALIGN_LEFT | NVG_ALIGN_TOP);
    Vector2i size = Vector2i(2*theme()->m_tab_control_width, 0);
    for (auto& tab : m_tab_buttons) {
        auto tab_preferred = tab.preferred_size(ctx);
        if (tab_preferred.x() < theme()->m_tab_min_button_width)
            tab_preferred.x() = theme()->m_tab_min_button_width;
        else if (tab_preferred.x() > theme()->m_tab_max_button_width)
            tab_preferred.x() = theme()->m_tab_max_button_width;
        size.x() += tab_preferred.x();
        size.y() = std::max(size.y(), tab_preferred.y());
    }
    return size;
}

bool TabHeader::mouse_button_event(const Vector2i &p, int button, bool down, int modifiers) {
    Widget::mouse_button_event(p, button, down, modifiers);
    if (button == GLFW_MOUSE_BUTTON_1 && down) {
        switch (locate_click(p)) {
        case ClickLocation::LeftControls:
            on_arrow_left();
            return true;
        case ClickLocation::RightControls:
            on_arrow_right();
            return true;
        case ClickLocation::TabButtons:
            auto first = visible_begin();
            auto last = visible_end();
            int current_position = theme()->m_tab_control_width;
            int end_position = p.x();
            auto first_invisible = std::find_if(first, last,
                                               [&current_position, end_position](const TabButton& tb) {
                current_position += tb.size().x();
                return current_position > end_position;
            });

            // Did not click on any of the tab buttons
            if (first_invisible == last)
                return true;

            // Update the active tab and invoke the callback.
            set_active_tab((int) std::distance(m_tab_buttons.begin(), first_invisible));
            return true;
        }
    }
    return false;
}

void TabHeader::draw(NVGcontext* ctx) {
    // Draw controls.
    Widget::draw(ctx);
    if (m_overflowing)
        draw_controls(ctx);

    // Set up common text drawing settings.
    nvgFontFace(ctx, m_font.c_str());
    nvgFontSize(ctx, font_size());
    nvgTextAlign(ctx, NVG_ALIGN_LEFT | NVG_ALIGN_TOP);

    auto current = visible_begin();
    auto last = visible_end();
    auto active = std::next(m_tab_buttons.begin(), m_active_tab);
    Vector2i current_position = m_pos + Vector2i(theme()->m_tab_control_width, 0);

    // Flag to draw the active tab last. Looks a little bit better.
    bool draw_active = false;
    Vector2i active_position = Vector2i(0);

    // Draw inactive visible buttons.
    while (current != last) {
        if (current == active) {
            draw_active = true;
            active_position = current_position;
        } else {
            current->draw_at_position(ctx, current_position, false);
        }
        current_position.x() += current->size().x();
        ++current;
    }

    // Draw active visible button.
    if (draw_active)
        active->draw_at_position(ctx, active_position, true);
}

void TabHeader::calculate_visible_end() {
    auto first = visible_begin();
    auto last = m_tab_buttons.end();
    int current_position = theme()->m_tab_control_width;
    int last_position = m_size.x() - theme()->m_tab_control_width;
    auto first_invisible = std::find_if(first, last,
                                       [&current_position, last_position](const TabButton& tb) {
        current_position += tb.size().x();
        return current_position > last_position;
    });
    m_visible_end = (int) std::distance(m_tab_buttons.begin(), first_invisible);
}

void TabHeader::draw_controls(NVGcontext* ctx) {
    // Left button.
    bool active = m_visible_start != 0;

    // Draw the arrow.
    nvgBeginPath(ctx);
    auto icon_left = utf8(m_theme->m_tab_header_left_icon);
    int font_size = m_font_size == -1 ? m_theme->m_button_font_size : m_font_size;
    float ih = font_size;
    ih *= icon_scale();
    nvgFontSize(ctx, ih);
    nvgFontFace(ctx, "icons");
    NVGcolor arrow_color;
    if (active)
        arrow_color = m_theme->m_text_color;
    else
        arrow_color = m_theme->m_button_gradient_bot_pushed;
    nvgFillColor(ctx, arrow_color);
    nvgTextAlign(ctx, NVG_ALIGN_LEFT | NVG_ALIGN_MIDDLE);
    float y_scale_left = 0.5f;
    float x_scale_left = 0.2f;
    Vector2f left_icon_pos = Vector2f(m_pos) + Vector2f(x_scale_left*theme()->m_tab_control_width, y_scale_left*m_size.y());
    nvgText(ctx, left_icon_pos.x(), left_icon_pos.y() + 1, icon_left.data(), nullptr);

    // Right button.
    active = m_visible_end != tab_count();
    // Draw the arrow.
    nvgBeginPath(ctx);
    auto icon_right = utf8(m_theme->m_tab_header_right_icon);
    font_size = m_font_size == -1 ? m_theme->m_button_font_size : m_font_size;
    ih = font_size;
    ih *= icon_scale();
    nvgFontSize(ctx, ih);
    nvgFontFace(ctx, "icons");
    float right_width = nvgTextBounds(ctx, 0, 0, icon_right.data(), nullptr, nullptr);
    if (active)
        arrow_color = m_theme->m_text_color;
    else
        arrow_color = m_theme->m_button_gradient_bot_pushed;
    nvgFillColor(ctx, arrow_color);
    nvgTextAlign(ctx, NVG_ALIGN_LEFT | NVG_ALIGN_MIDDLE);
    float y_scale_right = .5f;
    float x_scale_right = 1.f - x_scale_left - right_width / theme()->m_tab_control_width;
    Vector2f right_icon_pos = Vector2f(m_pos) + Vector2f((float) m_size.x(), (float) m_size.y()*y_scale_right) -
                            Vector2f(x_scale_right*theme()->m_tab_control_width + right_width, 0);

    nvgText(ctx, right_icon_pos.x(), right_icon_pos.y() + 1, icon_right.data(), nullptr);
}

TabHeader::ClickLocation TabHeader::locate_click(const Vector2i& p) {
    auto left_distance = p - m_pos;
    bool hit_left = all(left_distance >= 0) && all(left_distance < Vector2i(theme()->m_tab_control_width, m_size.y()));
    if (hit_left)
        return ClickLocation::LeftControls;
    auto right_distance = p - (m_pos + Vector2i(m_size.x() - theme()->m_tab_control_width, 0));
    bool hit_right = all(right_distance >= 0) && all(right_distance < Vector2i(theme()->m_tab_control_width, m_size.y()));
    if (hit_right)
        return ClickLocation::RightControls;
    return ClickLocation::TabButtons;
}

void TabHeader::on_arrow_left() {
    if (m_visible_start == 0)
        return;
    --m_visible_start;
    calculate_visible_end();
}

void TabHeader::on_arrow_right() {
    if (m_visible_end == tab_count())
        return;
    ++m_visible_start;
    calculate_visible_end();
}

NAMESPACE_END(nanogui)
