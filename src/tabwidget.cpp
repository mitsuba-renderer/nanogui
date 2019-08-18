/*
    nanogui/tabwidget.cpp -- Widget for organizing multiple
    sub-widgets into tabs

    NanoGUI was developed by Wenzel Jakob <wenzel.jakob@epfl.ch>.
    The widget drawing code is based on the NanoVG demo application
    by Mikko Mononen.

    All rights reserved. Use of this source code is governed by a
    BSD-style license that can be found in the LICENSE.txt file.
*/

#include <nanogui/tabwidget.h>
#include <nanogui/theme.h>
#include <nanogui/screen.h>
#include <nanogui/popup.h>
#include <nanogui/layout.h>
#include <nanogui/button.h>
#include <nanogui/opengl.h>
#include <nanogui/icons.h>

NAMESPACE_BEGIN(nanogui)

TabWidgetBase::TabWidgetBase(Widget *parent, const std::string &font)
    : Widget(parent), m_font(font), m_background_color(Color(0.f, 0.f)) {
    m_tab_offsets.push_back(0);
}

void TabWidgetBase::remove_tab(int id) {
    int index = tab_index(id);
    bool close_active = index == m_active_tab;
    m_tab_captions.erase(m_tab_captions.begin() + index);
    m_tab_ids.erase(m_tab_ids.begin() + index);
    if (index <= m_active_tab)
        m_active_tab = std::max(0, m_active_tab - 1);
    TabWidgetBase::perform_layout(screen()->nvg_context());
    if (m_close_callback)
        m_close_callback(id);
    if (close_active && m_callback) {
        m_callback(selected_id());
        update_visibility();
    }
}

int TabWidgetBase::insert_tab(int index, const std::string &caption) {
    int id = m_tab_counter++;
    m_tab_captions.insert(m_tab_captions.begin() + index, caption);
    m_tab_ids.insert(m_tab_ids.begin() + index, id);
    TabWidgetBase::perform_layout(screen()->nvg_context());
    if (index < m_active_tab)
        m_active_tab++;
    if (m_tab_ids.size() == 1 && m_callback) {
        m_active_tab = 0;
        m_callback(id);
        update_visibility();
    }
    return id;
}

int TabWidgetBase::append_tab(const std::string &caption) {
    return insert_tab((int) m_tab_captions.size(), caption);
}

int TabWidgetBase::tab_index(int id) const {
    for (size_t i = 0; i<m_tab_ids.size(); ++i) {
        if (m_tab_ids[i] == id)
            return (int) i;
    }
    throw std::runtime_error("TabWidgetBase::tab_index(): not found!");
}

void TabWidgetBase::update_visibility() { /* No-op */ }

void TabWidgetBase::perform_layout(NVGcontext* ctx) {
    m_tab_offsets.clear();
    nvgFontFace(ctx, m_font.c_str());
    nvgFontSize(ctx, font_size());
    nvgTextAlign(ctx, NVG_ALIGN_LEFT | NVG_ALIGN_TOP);

    m_tab_offsets.clear();
    int width = 0;
    float unused[4];
    for (const std::string &label : m_tab_captions) {
        int label_width = nvgTextBounds(ctx, 0, 0, label.c_str(), nullptr, unused);
        m_tab_offsets.push_back(width);
        width += label_width + 2 * m_theme->m_tab_button_horizontal_padding;
        if (m_tabs_closeable)
            width += m_close_width;
    }
    m_tab_offsets.push_back(width);

    nvgFontFace(ctx, "icons");
    m_close_width =
        nvgTextBounds(ctx, 0, 0, utf8(FA_TIMES_CIRCLE).data(), nullptr, unused);
}

Vector2i TabWidgetBase::preferred_size(NVGcontext* ctx) const {
    nvgFontFace(ctx, m_font.c_str());
    nvgFontSize(ctx, font_size());
    nvgTextAlign(ctx, NVG_ALIGN_LEFT | NVG_ALIGN_TOP);

    int width = 0;
    for (const std::string &label : m_tab_captions) {
        float unused[4];
        int label_width = nvgTextBounds(ctx, 0, 0, label.c_str(), nullptr, unused);
        width += label_width + 2 * m_theme->m_tab_button_horizontal_padding;
        if (m_tabs_closeable)
            width += m_close_width;
    }

    return Vector2i(width + 1,
                    font_size() + 2 * m_theme->m_tab_button_vertical_padding + 2*m_padding);
}

void TabWidgetBase::draw(NVGcontext* ctx) {
    if (m_tab_offsets.size() != m_tab_captions.size() + 1)
        throw std::runtime_error("Must run TabWidget::perform_layout() after adding/removing tabs!");

    int tab_height = font_size() + 2 * m_theme->m_tab_button_vertical_padding;

    if (m_background_color.w() != 0.f) {
        nvgFillColor(ctx, m_background_color);
        nvgBeginPath(ctx);
        nvgRoundedRect(ctx, m_pos.x() + .5f, m_pos.y() + .5f + tab_height, m_size.x(),
                       m_size.y() - tab_height - 2, m_theme->m_button_corner_radius);
        nvgFill(ctx);
    }

    Widget::draw(ctx);

    NVGpaint tab_background_color = nvgLinearGradient(
        ctx, m_pos.x(), m_pos.y() + 1, m_pos.x(), m_pos.y() + tab_height,
        m_theme->m_button_gradient_top_pushed, m_theme->m_button_gradient_bot_pushed);

    nvgSave(ctx);
    nvgIntersectScissor(ctx, m_pos.x(), m_pos.y(), m_size.x(), tab_height);
    nvgFontSize(ctx, font_size());
    nvgTextAlign(ctx, NVG_ALIGN_LEFT | NVG_ALIGN_TOP);
    for (size_t i = 0; i< m_tab_captions.size(); ++i) {
        int x_pos = m_pos.x() + m_tab_offsets[i],
            y_pos = m_pos.y(),
            width = m_tab_offsets[i + 1] - m_tab_offsets[i];

        if (i == (size_t) m_active_tab) {
            nvgBeginPath(ctx);
            nvgRoundedRect(ctx, x_pos + 0.5f, y_pos + 1.5f, width,
                           tab_height + 4, m_theme->m_button_corner_radius);
            nvgStrokeColor(ctx, m_theme->m_border_light);
            nvgStroke(ctx);

            nvgBeginPath(ctx);
            nvgRoundedRect(ctx, x_pos + 0.5f, y_pos + 0.5f, width,
                           tab_height + 4, m_theme->m_button_corner_radius);
            nvgStrokeColor(ctx, m_theme->m_border_dark);
            nvgStroke(ctx);
        } else {
            nvgBeginPath(ctx);
            nvgRoundedRect(ctx, x_pos + 0.5f, y_pos + 1.5f, width,
                           tab_height + 4, m_theme->m_button_corner_radius);

            nvgFillPaint(ctx, tab_background_color);
            nvgFill(ctx);

            nvgStrokeColor(ctx, m_theme->m_border_dark);
            nvgStroke(ctx);
        }
        x_pos += m_theme->m_tab_button_horizontal_padding;
        y_pos += m_theme->m_tab_button_vertical_padding + 1;
        nvgFillColor(ctx, m_theme->m_text_color);
        nvgFontFace(ctx, m_font.c_str());

        nvgText(ctx, x_pos, y_pos, m_tab_captions[i].c_str(), nullptr);

        if (m_tabs_closeable) {
            x_pos = m_pos.x() + m_tab_offsets[i + 1] -
                    m_theme->m_tab_button_horizontal_padding - m_close_width + 5;
            nvgFontFace(ctx, "icons");
            nvgFillColor(ctx, i == (size_t) m_close_index_pushed ? m_theme->m_text_color_shadow
                                                                 : m_theme->m_text_color);
            bool highlight = m_close_index == (int) i;
            auto icon = highlight ? FA_TIMES_CIRCLE : FA_TIMES;
            float fs       = font_size() * (highlight ? 1.f : .70f),
                  offset_x = highlight ? 0.f : (fs * .40f),
                  offset_y = highlight ? 0.f : (fs * .21f);
            nvgFontSize(ctx, fs);
            nvgText(ctx, x_pos + offset_x, y_pos + offset_y + .5f, utf8(icon).data(), nullptr);
            nvgFontSize(ctx, font_size());
        }
    }
    if (m_tab_drag_index != -1 && m_tab_drag_start != m_tab_drag_end) {
        int x_pos = m_pos.x() + m_tab_drag_min + m_tab_drag_end - m_tab_drag_start;
        nvgBeginPath(ctx);
        nvgRoundedRect(ctx, x_pos + 0.5f, m_pos.y() + 1.5f, m_tab_drag_max - m_tab_drag_min,
                       tab_height + 4, m_theme->m_button_corner_radius);
        nvgFillColor(ctx, Color(255, 255, 255, 30));
        nvgFill(ctx);
    }
    nvgRestore(ctx);

    int x0 = m_tab_offsets[m_active_tab],
        x1 = m_tab_offsets[m_tab_offsets.size() > 1 ? m_active_tab + 1 : 0];
    for (int i = 1; i >= 0; --i) {
        /* Top border */
        nvgBeginPath(ctx);
        nvgMoveTo(ctx, m_pos.x() + .5f, m_pos.y() + tab_height + i + .5f);
        nvgLineTo(ctx, m_pos.x() + x0 + 1.0f, m_pos.y() + tab_height + i + .5f);
        nvgMoveTo(ctx, m_pos.x() + x1, m_pos.y() + tab_height + i + .5f);
        nvgLineTo(ctx, m_pos.x() + m_size.x() + .5f, m_pos.y() + tab_height + i + .5f);
        nvgStrokeWidth(ctx, 1.0f);
        nvgStrokeColor(ctx, (i == 0) ? m_theme->m_border_dark : m_theme->m_border_light);
        nvgStroke(ctx);

        /* Bottom + side borders */
        nvgSave(ctx);
        nvgIntersectScissor(ctx, m_pos.x(), m_pos.y() + tab_height, m_size.x(), m_size.y());
        nvgBeginPath(ctx);
        nvgRoundedRect(ctx, m_pos.x() + .5f, m_pos.y() + i + .5f, m_size.x() - 1,
                       m_size.y() - 2, m_theme->m_button_corner_radius);
        nvgStroke(ctx);
        nvgRestore(ctx);
    }
}

std::pair<int, bool> TabWidgetBase::tab_at_position(const Vector2i &p, bool test_vertical) const {
    int tab_height = font_size() + 2 * m_theme->m_tab_button_vertical_padding;
    if (test_vertical && (p.y() <= m_pos.y() || p.y() > m_pos.y() + tab_height))
        return { -1, false };

    int x = p.x() - m_pos.x();
    for (size_t i = 0; i < m_tab_offsets.size() - 1; ++i) {
        if (x >= m_tab_offsets[i] && x < m_tab_offsets[i + 1]) {
            int r = m_tab_offsets[i + 1] - x;
            return {
                (int) i, m_tabs_closeable &&
                   r < m_theme->m_tab_button_horizontal_padding + m_close_width - 4 &&
                   r > m_theme->m_tab_button_horizontal_padding - 4 &&
                   p.y() - m_pos.y() > m_theme->m_tab_button_vertical_padding &&
                   p.y() - m_pos.y() <= tab_height - m_theme->m_tab_button_vertical_padding
            };
        }
    }
    return { -1, false };
}

bool TabWidgetBase::mouse_button_event(const Vector2i &p, int button, bool down,
                                       int modifiers) {
    int index; bool close;
    std::tie(index, close) = tab_at_position(p);
    bool handled = false;

    Screen *screen = this->screen();
    if (m_popup) {
        m_popup->mouse_button_event(
            p - m_pos + absolute_position() - m_popup->absolute_position() + m_popup->position(),
            button, down, modifiers
        );
        screen->update_focus(this);
        screen->remove_child(m_popup);
        m_popup = nullptr;
        handled = true;
    }

    bool drag_in_progress = m_tab_drag_index != -1 && m_tab_drag_start != m_tab_drag_end;
    if (m_popup_callback && button == GLFW_MOUSE_BUTTON_2 && down && index != -1 &&
        !drag_in_progress) {
        m_popup = m_popup_callback(tab_id(index), screen);
        m_popup->set_position(p + Vector2i(8, -6));
        m_popup->set_anchor_offset(8);
        m_popup->set_anchor_size(8);
        if (m_popup->layout() == nullptr)
            m_popup->set_layout(new GroupLayout(5, 3));
        for (Widget *w : m_popup->children()) {
            Button *b = dynamic_cast<Button *>(w);
            if (!b)
                continue;
            b->set_icon_position(Button::IconPosition::Right);
            b->set_flags(Button::MenuButton);
        }
        NVGcontext *ctx = screen->nvg_context();
        m_popup->set_size(m_popup->preferred_size(ctx) + Vector2i(40, 0));
        m_popup->perform_layout(ctx);
        handled = true;
    }

    if (button == GLFW_MOUSE_BUTTON_1 && m_popup == nullptr) {
        if (index >= 0) {
            if (close && m_tab_drag_index == -1) {
                if (down) {
                    m_close_index_pushed = index;
                } else if (m_close_index == m_close_index_pushed) {
                    remove_tab(tab_id(index));
                    mouse_motion_event(p, Vector2i(0), 0, 0);
                }
            } else {
                if (down) {
                    bool tab_changed = m_active_tab != index;
                    m_active_tab = index;
                    m_tab_drag_index = m_tabs_draggable ? index : -1;
                    m_tab_drag_start = m_tab_drag_end = p.x();
                    m_tab_drag_min = m_tab_offsets[index];
                    m_tab_drag_max = m_tab_offsets[index + 1];
                    m_close_index_pushed = -1;
                    if (tab_changed && m_callback) {
                        m_callback(selected_id());
                        update_visibility();
                    }
                } else if (m_tab_drag_index != -1) {
                    m_tab_drag_index = -1;
                    mouse_motion_event(p, Vector2i(0), 0, 0);
                }
            }
            handled = true;
        }

        if (!down) {
            handled = m_close_index_pushed != -1 || m_tab_drag_index != -1;
            m_close_index_pushed = -1;
            m_tab_drag_index = -1;
        }

        handled = true;
    }

    handled |= Widget::mouse_button_event(p, button, down, modifiers);

    return handled;
}

bool TabWidgetBase::mouse_enter_event(const Vector2i &/* p */, bool /* enter */) {
    if (m_tabs_closeable && m_close_index >= 0) {
        m_close_index = -1;
        m_close_index_pushed = -1;
        return true;
    }
    return false;
}

bool TabWidgetBase::mouse_motion_event(const Vector2i &p, const Vector2i &rel,
                                       int button, int modifiers) {
    auto [index, close] = tab_at_position(p, false);

    if (m_tab_drag_index != -1) {
        m_tab_drag_end = p.x();
        if (index != -1 && m_tab_drag_index != index) {
            int i0 = std::min(m_tab_drag_index, index),
                i1 = std::max(m_tab_drag_index, index);
            int mid = (m_tab_offsets[i0] + m_tab_offsets[i1 + 1]) / 2;
            if ((m_tab_drag_index < index && p.x() - m_pos.y() > mid) ||
                (m_tab_drag_index > index && p.x() - m_pos.y() < mid)) {
                std::swap(m_tab_captions[index], m_tab_captions[m_tab_drag_index]);
                std::swap(m_tab_ids[index], m_tab_ids[m_tab_drag_index]);
                TabWidgetBase::perform_layout(screen()->nvg_context());
                m_tab_drag_index = index;
                m_active_tab = index;
            }
        }
        return true;
    }

    if (!close)
        index = -1;

    if (index != m_close_index) {
        m_close_index = index;
        m_close_index_pushed = -1;
        return true;
    }

    return Widget::mouse_motion_event(p, rel, button, modifiers);
}

TabWidget::TabWidget(Widget *parent, const std::string &font)
    : TabWidgetBase(parent, font) { }

void TabWidget::perform_layout(NVGcontext* ctx) {
    TabWidgetBase::perform_layout(ctx);

    int tab_height = font_size() + 2 * m_theme->m_tab_button_vertical_padding;

    for (Widget *child : m_children) {
        child->set_position(Vector2i(m_padding, m_padding + tab_height + 1));
        child->set_size(m_size - Vector2i(2*m_padding, 2*m_padding + tab_height + 1));
        child->perform_layout(ctx);
    }
}

void TabWidget::update_visibility() {
    if (tab_count() == 0)
        return;
    for (Widget *child : m_children)
        child->set_visible(false);
    auto it = m_widgets.find(selected_id());
    if (it != m_widgets.end())
        it->second->set_visible(true);
}

Vector2i TabWidget::preferred_size(NVGcontext* ctx) const {
    Vector2i base_size = TabWidgetBase::preferred_size(ctx),
             content_size = Vector2i(0);
    for (Widget *child : m_children)
        content_size = max(content_size, child->preferred_size(ctx));

    return Vector2i(
        std::max(base_size.x(), content_size.x() + 2 * m_padding),
        base_size.y() + content_size.y() + 2 * m_padding
    );
}

int TabWidget::insert_tab(int index, const std::string &caption, Widget *widget) {
    int id = TabWidgetBase::insert_tab(index, caption);
    m_widgets[id] = widget;
    update_visibility();
    return id;
}

int TabWidget::append_tab(const std::string &caption, Widget *widget) {
    widget->set_visible(false);
    int id = TabWidgetBase::append_tab(caption);
    m_widgets[id] = widget;
    update_visibility();
    return id;
}

void TabWidget::remove_tab(int id) {
    TabWidgetBase::remove_tab(id);
    Widget *widget = m_widgets[id];
    m_widgets.erase(id);
    if (m_remove_children)
        remove_child(widget);
}

NAMESPACE_END(nanogui)
