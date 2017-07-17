/*
    nanogui/stackedwidget.cpp -- Widget used to stack widgets on top
    of each other. Only the active widget is visible.

    The stacked widget was contributed by Stefan Ivanov.

    NanoGUI was developed by Wenzel Jakob <wenzel.jakob@epfl.ch>.
    The widget drawing code is based on the NanoVG demo application
    by Mikko Mononen.

    All rights reserved. Use of this source code is governed by a
    BSD-style license that can be found in the LICENSE.txt file.
*/

#include <nanogui/stackedwidget.h>

NAMESPACE_BEGIN(nanogui)

StackedWidget::StackedWidget(Widget *parent)
    : Widget(parent) { }

void StackedWidget::set_selected_index(int index) {
    assert(index < child_count());
    if (m_selected_index >= 0)
        m_children[m_selected_index]->set_visible(false);
    m_selected_index = index;
    m_children[m_selected_index]->set_visible(true);
}

int StackedWidget::selected_index() const {
    return m_selected_index;
}

void StackedWidget::perform_layout(NVGcontext *ctx) {
    for (auto child : m_children) {
        child->set_position(Vector2i(0));
        child->set_size(m_size);
        child->perform_layout(ctx);
    }
}

Vector2i StackedWidget::preferred_size(NVGcontext *ctx) const {
    Vector2i size = Vector2i(0);
    for (auto child : m_children)
        size = max(size, child->preferred_size(ctx));
    return size;
}

void StackedWidget::add_child(int index, Widget *widget) {
    if (m_selected_index >= 0)
        m_children[m_selected_index]->set_visible(false);
    Widget::add_child(index, widget);
    widget->set_visible(true);
    set_selected_index(index);
}

NAMESPACE_END(nanogui)
