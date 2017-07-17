/*
    nanogui/combobox.h -- simple combo box widget based on a popup button

    NanoGUI was developed by Wenzel Jakob <wenzel.jakob@epfl.ch>.
    The widget drawing code is based on the NanoVG demo application
    by Mikko Mononen.

    All rights reserved. Use of this source code is governed by a
    BSD-style license that can be found in the LICENSE.txt file.
*/
/** \file */

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

    std::function<void(int)> callback() const { return m_callback; }
    void set_callback(const std::function<void(int)> &callback) { m_callback = callback; }

    int selected_index() const { return m_selected_index; }
    void set_selected_index(int idx);

    void set_items(const std::vector<std::string> &items, const std::vector<std::string> &items_short);
    void set_items(const std::vector<std::string> &items) { set_items(items, items); }
    const std::vector<std::string> &items() const { return m_items; }
    const std::vector<std::string> &items_short() const { return m_items_short; }

    virtual bool scroll_event(const Vector2i &p, const Vector2f &rel) override;
protected:
    std::vector<std::string> m_items, m_items_short;
    std::function<void(int)> m_callback;
    int m_selected_index;
};

NAMESPACE_END(nanogui)
