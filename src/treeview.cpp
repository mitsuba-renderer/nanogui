/*
    src/TreeView.cpp -- simple combo box widget based on a popup button

    NanoGUI was developed by Wenzel Jakob <wenzel.jakob@epfl.ch>.
    The widget drawing code is based on the NanoVG demo application
    by Mikko Mononen.

    All rights reserved. Use of this source code is governed by a
    BSD-style license that can be found in the LICENSE.txt file.
*/

#include <nanogui/TreeView.h>
#include <nanogui/label.h>
#include <nanogui/layout.h>
#include <nanogui/scrollpanel.h>
#include <nanogui/button.h>
#include <nanogui/screen.h>
#include <string>
#include <cassert>

NAMESPACE_BEGIN(nanogui)

void TreeView::update_tree_items(std::string KeyString, int Level, bool ParentExpanded, int ChildIdx)
{
    m_data_tree->Objects[KeyString]->Level = Level;
    if (ParentExpanded)
        create_tree_object(m_data_tree->Objects[KeyString]->KeyString,
            m_data_tree->Objects[KeyString]->Parent == nullptr ?
            m_items_container->child_count() :
            m_items_container->child_index(m_data_tree->Objects[KeyString]->Parent->NodeWidget) + 1 + ChildIdx);
    else if (m_data_tree->Objects[KeyString]->NodeWidget != nullptr)
    {
        screen()->m_focus_path.erase(std::remove(screen()->m_focus_path.begin(), screen()->m_focus_path.end(), m_data_tree->Objects[KeyString]->NodeWidget), screen()->m_focus_path.end());
        m_items_container->remove_child(m_data_tree->Objects[KeyString]->NodeWidget);
        m_data_tree->Objects[KeyString]->NodeWidget = nullptr;
    }

    int ChildrenCnt = 0;
    for (auto CurrItem : m_data_tree->Objects[KeyString]->Children)
        update_tree_items(CurrItem.second->KeyString, Level + 1, ParentExpanded && m_data_tree->Objects[KeyString]->Expanded, ChildrenCnt++);
}

void TreeView::create_tree_object(std::string object_name, int Index)
{
    Widget* CurrWidget = new Widget(nullptr);
    CurrWidget->set_layout(new BoxLayout(Orientation::Horizontal, Alignment::Minimum));
    Label* CurrLabel = new Label(CurrWidget, "");
    CurrLabel->set_fixed_width(20 * m_data_tree->Objects[object_name]->Level);
    Button* NewButton = new Button(CurrWidget, "", (m_data_tree->Objects[object_name]->Children.size() == 0 ? NULL : (m_data_tree->Objects[object_name]->Expanded ? FA_CARET_DOWN : FA_CARET_RIGHT)));
    NewButton->set_transparent(true);
    NewButton->set_icon_extra_scale(2);
    NewButton->set_font_size(15);
    NewButton->set_fixed_width(15);
    NewButton->set_callback([=] {arrow_callback(object_name); });
    NewButton->DebugName = "arrow_" + object_name;
    if (m_data_tree->Objects[object_name]->Children.size() == 0)NewButton->set_enabled(false);
    NewButton = new Button(CurrWidget, m_data_tree->Objects[object_name]->Name, m_data_tree->Objects[object_name]->Icon);
    NewButton->set_transparent(true);
    NewButton->set_icon_extra_scale(2);
    NewButton->set_font_size(15);
    NewButton->set_callback(m_data_tree->Objects[object_name]->CallBack);
    NewButton->DebugName = object_name;
    m_items_container->add_child(Index, CurrWidget);

    m_data_tree->Objects[object_name]->NodeWidget = CurrWidget;
}

TreeView::TreeView(Widget* parent)
    : Widget(parent), m_selected_index(0) {
    DebugName = m_parent->DebugName + ",TreeV";
    set_layout(new BoxLayout(Orientation::Vertical, Alignment::Minimum));
    m_scrollpanel = new ScrollPanel(this);
    m_scrollpanel->set_scroll_type(ScrollPanel::ScrollTypes::Both);
    m_items_container = new Widget(m_scrollpanel);
    m_items_container->set_layout(new BoxLayout(Orientation::Vertical, Alignment::Minimum));
}

TreeView::TreeView(Widget* parent, NanoTree* items)
    : Widget(parent), m_selected_index(0) {
    DebugName = m_parent->DebugName + ",TreeV";
    set_layout(new BoxLayout(Orientation::Vertical, Alignment::Minimum));
    m_scrollpanel = new ScrollPanel(this);
    m_scrollpanel->set_scroll_type(ScrollPanel::ScrollTypes::Both);
    m_items_container = new Widget(m_scrollpanel);
    m_items_container->set_layout(new BoxLayout(Orientation::Vertical, Alignment::Minimum, 15));

    set_items(items);
}
void TreeView::arrow_callback(std::string keystring)
{
    m_data_tree->Objects[keystring]->Expanded = !m_data_tree->Objects[keystring]->Expanded;

    ((Button*)(m_data_tree->Objects[keystring]->NodeWidget->children()[1]))->set_icon((m_data_tree->Objects[keystring]->Expanded ? FA_CARET_DOWN : FA_CARET_RIGHT));
    
    if (m_expand_callback && m_data_tree->Objects[keystring]->Expanded)m_expand_callback(keystring);

    int ChildrenCnt = 0;
    for (auto CurrItem : m_data_tree->Objects[keystring]->Children)
        update_tree_items(CurrItem.second->KeyString, m_data_tree->Objects[keystring]->Level + 1, m_data_tree->Objects[keystring]->Expanded, ChildrenCnt++);

    perform_layout(screen()->nvg_context());
}

void TreeView::set_fixed_size(const Vector2i& fixed_size)
{
    m_scrollpanel->set_fixed_size(fixed_size);
    Widget::set_fixed_size(fixed_size);
}

void TreeView::set_items(NanoTree* items) {
    if (m_data_tree != items)free(m_data_tree);
    m_data_tree = items;
    Screen* My_Screen = screen();
    while (m_items_container->children().size() > 0)
    {
        screen()->m_focus_path.erase(std::remove(screen()->m_focus_path.begin(), screen()->m_focus_path.end(), m_items_container->child_at(0)), screen()->m_focus_path.end());
        m_items_container->remove_child_at(0);
    }
    if (m_data_tree->Objects.size() == 0)return;

    update_tree_items(m_data_tree->Root->KeyString, 0, true);
}
void TreeView::perform_layout(NVGcontext* ctx)
{
    Widget::perform_layout(ctx);
}
bool TreeView::scroll_event(const Vector2i& p, const Vector2f& rel) {
    return Widget::scroll_event(p, rel);
}

NAMESPACE_END(nanogui)
