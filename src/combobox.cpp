/*
    src/combobox.cpp -- simple combo box widget based on a popup button

    NanoGUI was developed by Wenzel Jakob <wenzel.jakob@epfl.ch>.
    The widget drawing code is based on the NanoVG demo application
    by Mikko Mononen.

    All rights reserved. Use of this source code is governed by a
    BSD-style license that can be found in the LICENSE.txt file.
*/

#include <nanogui/combobox.h>
#include <nanogui/layout.h>
#include <cassert>

NAMESPACE_BEGIN(nanogui)

ComboBox::ComboBox(Widget *parent) : PopupButton(parent), m_selected_index(0) {
}

ComboBox::ComboBox(Widget *parent, const std::vector<std::string> &items)
    : PopupButton(parent), m_selected_index(0) {
    set_items(items);
}

ComboBox::ComboBox(Widget *parent, const std::vector<std::string> &items, const std::vector<std::string> &items_short)
    : PopupButton(parent), m_selected_index(0) {
    set_items(items, items_short);
}

void ComboBox::set_selected_index(int idx) {
    if (m_items_short.empty())
        return;
    const std::vector<Widget *> &children = popup()->children();
    ((Button *) children[m_selected_index])->set_pushed(false);
    ((Button *) children[idx])->set_pushed(true);
    m_selected_index = idx;
    set_caption(m_items_short[idx]);
}

void ComboBox::set_items(const std::vector<std::string> &items, const std::vector<std::string> &items_short) {
    assert(items.size() == items_short.size());
    m_items = items;
    m_items_short = items_short;
    if (m_selected_index < 0 || m_selected_index >= (int) items.size())
        m_selected_index = 0;
    while (m_popup->child_count() != 0)
        m_popup->remove_child(m_popup->child_count()-1);
    m_popup->set_layout(new GroupLayout(10));
    int index = 0;
    for (const auto &str: items) {
        Button *button = new Button(m_popup, str);
        button->set_flags(Button::RadioButton);
        button->set_callback([&, index] {
            m_selected_index = index;
            set_caption(m_items_short[index]);
            set_pushed(false);
            popup()->set_visible(false);
            if (m_callback)
                m_callback(index);
        });
        index++;
    }
    set_selected_index(m_selected_index);
}

bool ComboBox::scroll_event(const Vector2i &p, const Vector2f &rel) {
    if (rel.y() < 0) {
        set_selected_index(std::min(m_selected_index+1, (int)(items().size()-1)));
        if (m_callback)
            m_callback(m_selected_index);
        return true;
    } else if (rel.y() > 0) {
        set_selected_index(std::max(m_selected_index-1, 0));
        if (m_callback)
            m_callback(m_selected_index);
        return true;
    }
    return Widget::scroll_event(p, rel);
}

NAMESPACE_END(nanogui)
