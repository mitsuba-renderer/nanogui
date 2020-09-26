/*
    src/TreeView.cpp -- simple combo box widget based on a popup button

    NanoGUI was developed by Wenzel Jakob <wenzel.jakob@epfl.ch>.
    The widget drawing code is based on the NanoVG demo application
    by Mikko Mononen.

    All rights reserved. Use of this source code is governed by a
    BSD-style license that can be found in the LICENSE.txt file.
*/

#include <nanogui/TreeView.h>
#include <nanogui/layout.h>
#include <nanogui/scrollpanel.h>
#include <nanogui/button.h>
#include <cassert>

NAMESPACE_BEGIN(nanogui)

TreeView::TreeView(Widget* parent)
    : Widget(parent),  m_selected_index(0) {
    m_items_container = new Widget(this);
    m_items_container->set_layout(new BoxLayout(Orientation::Vertical, Alignment::Middle));
}

TreeView::TreeView(Widget* parent, const std::vector<TreeNode>& items)
    : Widget(parent), m_selected_index(0) {
    m_items_container = new Widget(this);
    m_items_container->set_layout(new BoxLayout(Orientation::Vertical, Alignment::Middle));

    set_items(items);
}

void TreeView::set_selected_index(int idx) {

    const std::vector<Widget*>& children = m_items_container->children();
    ((Button*)children[m_selected_index])->set_pushed(false);
    ((Button*)children[idx])->set_pushed(true);
    m_selected_index = idx;
}

void TreeView::set_items(const std::vector<TreeNode>& items) {

    m_items = items;

    if (m_selected_index < 0 || m_selected_index >= (int)items.size())
        m_selected_index = 0;
    while (m_items_container->child_count() != 0)
        m_items_container->remove_child_at(m_items_container->child_count() - 1);

    if (m_scroll == nullptr && items.size() > 8) {
        m_scroll = new ScrollPanel(this);
        m_scroll->set_fixed_height(300);
        m_items_container = new Widget(m_scroll);
    }

    m_items_container->set_layout(new GroupLayout(10));

    int index = 0;
    for (const auto& str : items) {
        Button* button = new Button(m_items_container, str.Name);
        button->set_flags(Button::RadioButton);
        button->set_callback([&, index] {
            m_selected_index = index;
            if (m_callback)
                m_callback(index);
            });
        index++;
    }
    set_selected_index(m_selected_index);
}

bool TreeView::scroll_event(const Vector2i& p, const Vector2f& rel) {
    if (rel.y() < 0) {
        set_selected_index(std::min(m_selected_index + 1, (int)(items().size() - 1)));
        if (m_callback)
            m_callback(m_selected_index);
        return true;
    }
    else if (rel.y() > 0) {
        set_selected_index(std::max(m_selected_index - 1, 0));
        if (m_callback)
            m_callback(m_selected_index);
        return true;
    }
    return Widget::scroll_event(p, rel);
}

NAMESPACE_END(nanogui)
