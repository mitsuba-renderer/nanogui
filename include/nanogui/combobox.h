/*
    NanoGUI was developed by Wenzel Jakob <wenzel.jakob@epfl.ch>.
    The widget drawing code is based on the NanoVG demo application
    by Mikko Mononen.

    All rights reserved. Use of this source code is governed by a
    BSD-style license that can be found in the LICENSE.txt file.
*/
/**
 * \file nanogui/combobox.h
 *
 * \brief Simple combo box widget based on a popup button.
 */

#pragma once

#include <nanogui/popupbutton.h>

NAMESPACE_BEGIN(nanogui)

/**
 * \class ComboBox combobox.h nanogui/combobox.h
 *
 * \brief Simple combo box widget based on a popup button.
 */
class NANOGUI_EXPORT ComboBox : public PopupButton {
public:
    /// Create an empty combo box
    ComboBox(Widget *parent);

    /// Create a new combo box with the given items
    ComboBox(Widget *parent, const std::vector<std::string> &items);

    /**
     * \brief Create a new combo box with the given items, providing both short and
     * long descriptive labels for each item
     */
    ComboBox(Widget *parent, const std::vector<std::string> &items,
             const std::vector<std::string> &items_short);

    /// The current index this ComboBox has selected.
    int selected_index() const { return m_selected_index; }

    /// Sets the current index this ComboBox has selected.
    void set_selected_index(int idx);

    /// The callback to execute for this ComboBox.
    std::function<void(int)> callback() const { return m_callback; }

    /// Sets the callback to execute for this ComboBox.
    void set_callback(const std::function<void(int)> &callback) { m_callback = callback; }

    /// Sets the items for this ComboBox, providing both short and long descriptive lables for each item.
    void set_items(const std::vector<std::string> &items, const std::vector<std::string> &items_short);
    /// Sets the items for this ComboBox.
    void set_items(const std::vector<std::string> &items) { set_items(items, items); }
    /// The items associated with this ComboBox.
    const std::vector<std::string> &items() const { return m_items; }
    /// The short descriptions associated with this ComboBox.
    const std::vector<std::string> &items_short() const { return m_items_short; }

    /// Handles mouse scrolling events for this ComboBox.
    virtual bool scroll_event(const Vector2i &p, const Vector2f &rel) override;
protected:
    /// Scroll panel used to store the combo box contents
    VScrollPanel *m_scroll = nullptr;

    /// Container containing the buttons
    Widget *m_container = nullptr;

    /// The items associated with this ComboBox.
    std::vector<std::string> m_items;

    /// The short descriptions of items associated with this ComboBox.
    std::vector<std::string> m_items_short;

    /// The callback for this ComboBox.
    std::function<void(int)> m_callback;

    /// The current index this ComboBox has selected.
    int m_selected_index;
};

NAMESPACE_END(nanogui)
