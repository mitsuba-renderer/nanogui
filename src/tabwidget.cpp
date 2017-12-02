/*
    nanogui/tabwidget.cpp -- A wrapper around the widgets TabHeader and StackedWidget
    which hooks the two classes together.

    The tab widget was contributed by Stefan Ivanov.

    NanoGUI was developed by Wenzel Jakob <wenzel.jakob@epfl.ch>.
    The widget drawing code is based on the NanoVG demo application
    by Mikko Mononen.

    All rights reserved. Use of this source code is governed by a
    BSD-style license that can be found in the LICENSE.txt file.
*/

#include <nanogui/tabwidget.h>
#include <nanogui/tabheader.h>
#include <nanogui/stackedwidget.h>
#include <nanogui/theme.h>
#include <nanogui/opengl.h>
#include <nanogui/window.h>
#include <nanogui/screen.h>
#include <algorithm>

NAMESPACE_BEGIN(nanogui)

TabWidget::TabWidget(Widget* parent)
    : Widget(parent), m_header(new TabHeader(this)), m_content(new StackedWidget(this)) {
    m_header->set_callback([this](int i) {
        m_content->set_selected_index(i);
        if (m_callback)
            m_callback(i);
    });
}

void TabWidget::set_active_tab(int tab_index) {
    m_header->set_active_tab(tab_index);
    m_content->set_selected_index(tab_index);
}

int TabWidget::active_tab() const {
    assert(m_header->active_tab() == m_content->selected_index());
    return m_content->selected_index();
}

int TabWidget::tab_count() const {
    assert(m_content->child_count() == m_header->tab_count());
    return m_header->tab_count();
}

Widget* TabWidget::create_tab(int index, const std::string &label) {
    Widget* tab = new Widget(nullptr);
    add_tab(index, label, tab);
    return tab;
}

Widget* TabWidget::create_tab(const std::string &label) {
    return create_tab(tab_count(), label);
}

void TabWidget::add_tab(const std::string &name, Widget *tab) {
    add_tab(tab_count(), name, tab);
}

void TabWidget::add_tab(int index, const std::string &label, Widget *tab) {
    assert(index <= tab_count());
    // It is important to add the content first since the callback
    // of the header will automatically fire when a new tab is added.
    m_content->add_child(index, tab);
    m_header->add_tab(index, label);
    assert(m_header->tab_count() == m_content->child_count());
}

int TabWidget::tab_label_index(const std::string &label) {
    return m_header->tab_index(label);
}

int TabWidget::tab_index(Widget* tab) {
    return m_content->child_index(tab);
}

void TabWidget::ensure_tab_visible(int index) {
    if (!m_header->is_tab_visible(index))
        m_header->ensure_tab_visible(index);
}

const Widget *TabWidget::tab(const std::string &tab_name) const {
    int index = m_header->tab_index(tab_name);
    if (index == -1 || index == m_content->child_count())
        return nullptr;
    return m_content->children()[index];
}

Widget *TabWidget::tab(const std::string &tab_name) {
    int index = m_header->tab_index(tab_name);
    if (index == -1 || index == m_content->child_count())
        return nullptr;
    return m_content->children()[index];
}

const Widget *TabWidget::tab(int index) const {
    if (index < 0 || index >= m_content->child_count())
        return nullptr;
    return m_content->children()[index];
}

Widget *TabWidget::tab(int index) {
    if (index < 0 || index >= m_content->child_count())
        return nullptr;
    return m_content->children()[index];
}

bool TabWidget::remove_tab(const std::string &tab_name) {
    int index = m_header->remove_tab(tab_name);
    if (index == -1)
        return false;
    m_content->remove_child(index);
    return true;
}

void TabWidget::remove_tab(int index) {
    assert(m_content->child_count() < index);
    m_header->remove_tab(index);
    m_content->remove_child(index);
    if (active_tab() == index)
        set_active_tab(index == (index - 1) ? index - 1 : 0);
}

const std::string &TabWidget::tab_label_at(int index) const {
    return m_header->tab_label_at(index);
}

void TabWidget::perform_layout(NVGcontext* ctx) {
    int header_height = m_header->preferred_size(ctx).y();
    int margin = m_theme->m_tab_inner_margin;
    m_header->set_position({ 0, 0 });
    m_header->set_size({ m_size.x(), header_height });
    m_header->perform_layout(ctx);
    m_content->set_position({ margin, header_height + margin });
    m_content->set_size({ m_size.x() - 2 * margin, m_size.y() - 2*margin - header_height });
    m_content->perform_layout(ctx);
}

Vector2i TabWidget::preferred_size(NVGcontext* ctx) const {
    auto content_size = m_content->preferred_size(ctx);
    auto header_size = m_header->preferred_size(ctx);
    int margin = m_theme->m_tab_inner_margin;
    auto border_size = Vector2i(2 * margin, 2 * margin);
    Vector2i tab_preferred_size = content_size + border_size + Vector2i(0, header_size.y());
    return tab_preferred_size;
}

void TabWidget::draw(NVGcontext* ctx) {
    int tab_height = m_header->preferred_size(ctx).y();
    auto active_area = m_header->active_button_area();


    for (int i = 0; i < 3; ++i) {
        nvgSave(ctx);
        if (i == 0)
            nvgIntersectScissor(ctx, m_pos.x(), m_pos.y(), active_area.first.x() + 1, m_size.y());
        else if (i == 1)
            nvgIntersectScissor(ctx, m_pos.x() + active_area.second.x(), m_pos.y(), m_size.x() - active_area.second.x(), m_size.y());
        else
            nvgIntersectScissor(ctx, m_pos.x(), m_pos.y() + tab_height + 2, m_size.x(), m_size.y());

        nvgBeginPath(ctx);
        nvgStrokeWidth(ctx, 1.0f);
        nvgRoundedRect(ctx, m_pos.x() + 0.5f, m_pos.y() + tab_height + 1.5f, m_size.x() - 1,
                       m_size.y() - tab_height - 2, m_theme->m_button_corner_radius);
        nvgStrokeColor(ctx, m_theme->m_border_light);
        nvgStroke(ctx);

        nvgBeginPath(ctx);
        nvgRoundedRect(ctx, m_pos.x() + 0.5f, m_pos.y() + tab_height + 0.5f, m_size.x() - 1,
                       m_size.y() - tab_height - 2, m_theme->m_button_corner_radius);
        nvgStrokeColor(ctx, m_theme->m_border_dark);
        nvgStroke(ctx);
        nvgRestore(ctx);
    }

    Widget::draw(ctx);
}

NAMESPACE_END(nanogui)
