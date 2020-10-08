/*
    NanoGUI was developed by Wenzel Jakob <wenzel.jakob@epfl.ch>.
    The widget drawing code is based on the NanoVG demo application
    by Mikko Mononen.

    All rights reserved. Use of this source code is governed by a
    BSD-style license that can be found in the LICENSE.txt file.
*/
/**
 * \file nanogui/treeview.h
 *
 * \brief Simple tree view widget.
 */

#pragma once

#include <nanogui/widget.h>
#include <nanogui/icons.h>
#include <map>

NAMESPACE_BEGIN(nanogui)
/**
* \class  NanoTree
*
* \brief Structure used to construct the tree.
*/
class NanoTree
{

public:
    enum class NanoTreeErrors
    {
        NoError = 0,
        DuplicateName,
        ParentIsChild,
        NoSuchParent,
        NoSuchChild,
        CannotRemoveRoot
    };
    struct NanoTreeNode
    {
        std::string Name = "";
        int Icon = 0;
        std::function<void()> CallBack;

        // don't edit by hand
        int Level = 0;
        bool Expanded = false;
        Widget* NodeWidget = nullptr;
        std::string KeyString = "";
        NanoTreeNode* Parent = nullptr;
        std::map<std::string, NanoTreeNode*> Children;
    };
    // these are public for simplicity, but is better to set with the methods provided.
    std::map<std::string, NanoTreeNode*> Objects;
    NanoTreeNode* Root;
    // creates a node and puts it in the root. if a root already exists, the the old root becomes a child of the new root.
    NanoTreeErrors NanoTree::set_root(std::string NewRoot)
    {
        if (Objects.find(NewRoot) != Objects.end())return NanoTree::NanoTreeErrors::DuplicateName;

        NanoTreeNode* NewNode = new NanoTreeNode;
        NewNode->KeyString = NewRoot;
        Objects[NewRoot] = NewNode;
        if (Root != NULL)NewNode->Children.insert(std::pair<std::string, NanoTreeNode*>(Root->KeyString, Root));
        Root = NewNode;

        return NanoTree::NanoTreeErrors::NoError;
    }
    // Addd a node. All needed tests are done. Error code is returned
    NanoTreeErrors NanoTree::add_node(std::string Parent, std::string Child)
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
    // remove a node. All needed tests are done. Error code is returned
    NanoTreeErrors NanoTree::remove_node(std::string ToRemove)
    {
        if (Objects.find(ToRemove) == Objects.end())return NanoTree::NanoTreeErrors::NoSuchChild;
        if (Objects[ToRemove] == Root && Objects[ToRemove]->Children.size() != 0)return NanoTree::NanoTreeErrors::CannotRemoveRoot;//cannot remove root if is not the last node.

        if (Objects[ToRemove]->Parent != NULL)
        {
            Objects[Objects[ToRemove]->Parent->KeyString]->Children.erase(ToRemove);
            for (auto child : Objects[ToRemove]->Children)// put all the children of the node to be erased to the parent
                Objects[Objects[ToRemove]->Parent->KeyString]->Children.insert(std::pair<std::string, NanoTreeNode*>(child.second->KeyString, child.second));
        }
        Objects.erase(ToRemove);

        return NanoTree::NanoTreeErrors::NoError;
    }
    // change the parent of a given node. All needed tests are done. Error code is returned
    NanoTreeErrors NanoTree::change_parent(std::string Parent, std::string Child)
    {
        NanoTree::NanoTreeErrors check_error = MakeBasicChecks(Parent, Child, false);
        if (check_error != NanoTree::NanoTreeErrors::NoError)
            return check_error;

        Objects[Child]->Parent->Children.erase(Child);
        Objects[Child]->Parent = Objects[Parent];
        Objects[Parent]->Children.insert(std::pair<std::string, NanoTreeNode*>(Child, Objects[Child]));

        return NanoTree::NanoTreeErrors::NoError;
    }

private:
    // make checks on teh objects of the tree
    NanoTreeErrors NanoTree::MakeBasicChecks(std::string Parent, std::string Child, bool ChildIsNew)
    {
        if (Parent == Child)return NanoTree::NanoTreeErrors::ParentIsChild;
        if (Objects.find(Parent) == Objects.end())return NanoTree::NanoTreeErrors::NoSuchParent;
        if (!ChildIsNew) { if (Objects.find(Child) == Objects.end())return NanoTree::NanoTreeErrors::NoSuchChild; }// child is not new. it should be found
        else { if (Objects.find(Child) != Objects.end())return NanoTree::NanoTreeErrors::DuplicateName; }

        return NanoTree::NanoTreeErrors::NoError;
    }

};
/**
 * \class TreeView treeview.h nanogui/treeview.h
 *
 * \brief Simple treeview widget .
 */
class NANOGUI_EXPORT TreeView : public Widget {
    void update_tree_items(std::string KeyString, int Level, bool ParentExpanded, int ChildIdx = 0);
public:

    /// Create an empty treeview
    TreeView(Widget* parent);

    /// Create a new treeview with the given items
    TreeView(Widget* parent, NanoTree* items);

    /// Override of the Widget set_size
    virtual void set_fixed_size(const Vector2i& fixed_size) override;

    /// The callback to execute for this Treeview.
    std::function<void(int)> callback() const { return m_callback; }

    /// Sets the callback to execute for this Treeview.
    void set_callback(const std::function<void(int)>& callback) { m_callback = callback; }

    /// Sets the items for this Treeview. Execute this every time you ake a change to the NanoTree tructure
    void set_items(NanoTree* items);
    /// The items associated with this Treeview.
    NanoTree* items() { return m_data_tree; }
    /// Invoke the associated layout generator to properly place child widgets, if any
    virtual void perform_layout(NVGcontext* ctx) override;
    /// Handles mouse scrolling events for this Treeview.
    virtual bool scroll_event(const Vector2i& p, const Vector2f& rel) override;
protected:

    void create_tree_object(std::string obbject_name, int Index);

    void expand_callback(std::string keystring);
    /// scroll panell containing everything else
    ScrollPanel* m_scrollpanel = nullptr;
    /// Container containing the buttons
    Widget* m_items_container = nullptr;

    /// The items associated with this ComboBox.
    NanoTree* m_data_tree = nullptr;

    /// The callback for this ComboBox.
    std::function<void(int)> m_callback;

    /// The current index this ComboBox has selected.
    int m_selected_index;
};

NAMESPACE_END(nanogui)
