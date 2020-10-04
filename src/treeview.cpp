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

NanoTree::NanoTreeErrors NanoTree::MakeBasicChecks(std::string Parent, std::string Child, bool ChildIsNew)
{
    if (Parent == Child)return NanoTree::NanoTreeErrors::ParentIsChild;
    if (Objects.find(Parent) == Objects.end())return NanoTree::NanoTreeErrors::NoSuchParent;
    if (!ChildIsNew) { if (Objects.find(Child) == Objects.end())return NanoTree::NanoTreeErrors::NoSuchChild; }// child is not new. it should be found
    else { if (Objects.find(Child) != Objects.end())return NanoTree::NanoTreeErrors::DuplicateName; }

    return NanoTree::NanoTreeErrors::NoError;
}
NanoTree::NanoTreeErrors NanoTree::set_root(std::string NewRoot)
{
    if (Objects.find(NewRoot) != Objects.end())return NanoTree::NanoTreeErrors::DuplicateName;

    NanoTreeNode* NewNode = new NanoTreeNode;
    NewNode->KeyString = NewRoot;
    Objects[NewRoot] = NewNode;
    if (Root != NULL)NewNode->Children.insert(std::pair<std::string, NanoTreeNode*>(Root->KeyString, Root));
    Root = NewNode;

    return NanoTree::NanoTreeErrors::NoError;
}
NanoTree::NanoTreeErrors NanoTree::add_node(std::string Parent, std::string Child)
{
    NanoTree::NanoTreeErrors check_error = MakeBasicChecks(Parent, Child, true);
    if (check_error != NanoTree::NanoTreeErrors::NoError)
        return check_error;

    NanoTreeNode* NewNode = new NanoTreeNode;
    NewNode->KeyString = Child;
    Objects[Child] = NewNode;
    NewNode->Parent = Objects[Parent];
    Objects[Parent]->Children.insert(std::pair<std::string, NanoTreeNode*>(Child, NewNode));

    return NanoTree::NanoTreeErrors::NoError;
}
NanoTree::NanoTreeErrors NanoTree::remove_node(std::string ToRemove)
{
    if (Objects.find(ToRemove) == Objects.end())return NanoTree::NanoTreeErrors::NoSuchChild;
    if (Objects[ToRemove] == Root && Objects[ToRemove]->Children.size()!=0)return NanoTree::NanoTreeErrors::CannotRemoveRoot;//cannot remove root if is not the last node.

    if (Objects[ToRemove]->Parent != NULL)
    {
        Objects[Objects[ToRemove]->Parent->KeyString]->Children.erase(ToRemove);
        for (auto child : Objects[ToRemove]->Children)// put all the children of the node to be erased to the parent
            Objects[Objects[ToRemove]->Parent->KeyString]->Children.insert(std::pair<std::string, NanoTreeNode*>(child.second->KeyString, child.second));
    }
    Objects.erase(ToRemove);

    return NanoTree::NanoTreeErrors::NoError;
}
NanoTree::NanoTreeErrors NanoTree::change_parent(std::string Parent, std::string Child)
{
    NanoTree::NanoTreeErrors check_error = MakeBasicChecks(Parent, Child, false);
    if (check_error != NanoTree::NanoTreeErrors::NoError)
        return check_error;

    Objects[Child]->Parent->Children.erase(Child);
    Objects[Child]->Parent = Objects[Parent];
    Objects[Parent]->Children.insert(std::pair<std::string, NanoTreeNode*>(Child, Objects[Child]));

    return NanoTree::NanoTreeErrors::NoError;
}


TreeView::TreeView(Widget* parent)
    : Widget(parent), m_selected_index(0) {
    m_items_container = new Widget(this);
    m_items_container->set_layout(new BoxLayout(Orientation::Vertical, Alignment::Middle));
}

TreeView::TreeView(Widget* parent, const TreeNode items)
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

void TreeView::set_items(const TreeNode items) {

    m_items = items;

    m_selected_index = 0;
    while (m_items_container->child_count() != 0)
        m_items_container->remove_child_at(m_items_container->child_count() - 1);

    if (m_scroll == nullptr && items.Children.size() > 8) {
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
