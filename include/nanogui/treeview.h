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
#include <map>

NAMESPACE_BEGIN(nanogui)
/**
* \class  TreeNode
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
        std::string Name;
        Button* NodeButton;
        int NodeType;
        int NodeID;

        // don't edit by hand
        std::string KeyString;
        NanoTreeNode* Parent;
        std::map<std::string, NanoTreeNode*> Children;
    };
    // these are public for simplicity, but is better to set with the methods provided.
    std::map<std::string, NanoTreeNode*> Objects;
    NanoTreeNode* Root;
    // creates a node and puts it in the root. if a root already exists, the the old root becomes a child of the new root.
    NanoTreeErrors set_root(std::string NewRoot);
    // Addd a node. All needed tests are done. Error code is returned
    NanoTreeErrors add_node(std::string Parent, std::string Child);
    // remove a node. All needed tests are done. Error code is returned
    NanoTreeErrors remove_node(std::string ToRemove);
    // change the parent of a given node. All needed tests are done. Error code is returned
    NanoTreeErrors change_parent(std::string  Parent, std::string  Child);
protected:
    // make checks on teh objects of the tree
    NanoTreeErrors MakeBasicChecks(std::string Parent, std::string Child, bool ChildIsNew);

};
/**
 * \class TreeView treeview.h nanogui/treeview.h
 *
 * \brief Simple treeview widget .
 */
class NANOGUI_EXPORT TreeView : public Widget {
public:
    /**
 * \struct  TreeNode
 *
 * \brief Structuer used to construct the tree.
 */
    enum class TreeNodeTypes
    {
        Folder,
        File,
        None
    };
    struct TreeNode
    {
        std::string Name;
        TreeNode* Parent;
        std::map<std::string, TreeNode> Children;
        Button* NodeButton;
        TreeNodeTypes NodeType;
    };

    /// Create an empty treeview
    TreeView(Widget* parent);

    /// Create a new treeview with the given items
    TreeView(Widget* parent, const TreeNode items);

    /// The current index this TreeView has selected.
    int selected_index() const { return m_selected_index; }

    /// Sets the current index this TreeViwew has selected.
    void set_selected_index(int idx);

    /// The callback to execute for this Treeview.
    std::function<void(int)> callback() const { return m_callback; }

    /// Sets the callback to execute for this Treeview.
    void set_callback(const std::function<void(int)>& callback) { m_callback = callback; }

    /// Sets the items for this Treeview.
    void set_items(const TreeNode items);
    /// The items associated with this Treeview.
    const TreeNode items() const { return m_items; }

    /// Handles mouse scrolling events for this Treeview.
    virtual bool scroll_event(const Vector2i& p, const Vector2f& rel) override;
protected:
    /// Scroll panel used to store the combo box contents
    ScrollPanel* m_scroll = nullptr;

    /// Container containing the buttons
    Widget* m_items_container = nullptr;

    /// The items associated with this ComboBox.
    TreeNode m_items;

    /// The callback for this ComboBox.
    std::function<void(int)> m_callback;

    /// The current index this ComboBox has selected.
    int m_selected_index;
};

NAMESPACE_END(nanogui)
