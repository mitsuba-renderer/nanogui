/*
    nanogui/tabwidget.h -- Widget for organizing multiple
    sub-widgets into tabs

    NanoGUI was developed by Wenzel Jakob <wenzel.jakob@epfl.ch>.
    The widget drawing code is based on the NanoVG demo application
    by Mikko Mononen.

    All rights reserved. Use of this source code is governed by a
    BSD-style license that can be found in the LICENSE.txt file.
*/
/** \file */

#pragma once

#include <nanogui/widget.h>
#include <functional>
#include <unordered_map>

NAMESPACE_BEGIN(nanogui)

/**
 * \class TabWidgetBase tabwidget.h nanogui/tabwidget.h
 *
 * \brief Basic implementation of a tab widget with (optionally) draggable and
 * closeable tabs and popup menus. No rendering of the content is done---for an
 * example implementation, refer to \ref TabWidget.
 */
class NANOGUI_EXPORT TabWidgetBase : public Widget {
public:
    /// Construct a new tab widget
    TabWidgetBase(Widget *parent, const std::string &font = "sans-bold");

    /// Return the total number of tabs
    int tab_count() const { return (int) m_tab_captions.size(); };

    /// Return the ID of the tab at a given index
    int tab_id(int index) const { return m_tab_ids[index]; };

    /// Return the index of the tab with a given ID (or throw an exception)
    int tab_index(int id) const;

    /// Inserts a new tab at the specified position and returns its ID.
    int insert_tab(int index, const std::string &caption);

    /// Appends a new tab and returns its ID.
    int append_tab(const std::string &caption);

    /// Removes a tab with the specified ID
    virtual void remove_tab(int id);

    /// Return the ID of the currently active tab
    int selected_id() const { return m_tab_ids.empty() ? -1 : tab_id(m_active_tab); }
    /// Set the ID of the currently active tab
    void set_selected_id(int id) { m_active_tab = tab_index(id); update_visibility(); }

    /// Return the index of the currently active tab
    int selected_index() const { return m_active_tab; }
    /// Set the index of the currently active tab
    void set_selected_index(int index) { m_active_tab = index; update_visibility(); }

    /// Return the caption of the tab with the given ID
    const std::string& tab_caption(int id) const { return m_tab_captions[tab_index(id)]; };
    /// Change the caption of the tab with the given ID
    void set_tab_caption(int id, const std::string &caption) { m_tab_captions[tab_index(id)] = caption; };

    /// Return whether tabs provide a close button
    bool tabs_closeable() const { return m_tabs_closeable; }
    void set_tabs_closeable(bool value) { m_tabs_closeable = value; }

    /// Return whether tabs can be dragged to different positions
    bool tabs_draggable() const { return m_tabs_draggable; }
    void set_tabs_draggable(bool value) { m_tabs_draggable = value; }

    /// Return the padding between the tab widget boundary and child widgets
    int padding() const { return m_padding; }
    void set_padding(int value) { m_padding = value; }

    /// Set the widget's background color (a global property)
    void set_background_color(const Color &background_color) {
        m_background_color = background_color;
    }

    /// Return the widget's background color (a global property)
    const Color &background_color() const {
        return m_background_color;
    }

    /// Callback that is used to notify a listener about tab changes (will be called with the tab ID)
    std::function<void(int)> callback() const { return m_callback; }
    /// Set a callback that is used to notify a listener about tab changes (will be called with the tab ID)
    void set_callback(const std::function<void(int)> &callback) { m_callback = callback; }

    /// Callback that is used to notify a listener about tab close events (will be called with the tab ID)
    std::function<void(int)> close_callback() const { return m_close_callback; }
    /// Set a callback that is used to notify a listener about tab close events (will be called with the tab ID)
    void set_close_callback(const std::function<void(int)> &close_callback) { m_close_callback = close_callback; }

    /// Callback that is used to notify a listener about popup events (will be called with the tab ID)
    std::function<Popup *(int, Screen*)> popup_callback() const { return m_popup_callback; }
    /// Set a callback that is used to notify a listener about popup events (will be called with the tab ID)
    void set_popup_callback(const std::function<Popup *(int, Screen*)> &popup_callback) { m_popup_callback = popup_callback; }

    // Widget implementation
    virtual void perform_layout(NVGcontext* ctx) override;
    virtual Vector2i preferred_size(NVGcontext* ctx) const override;
    virtual void draw(NVGcontext* ctx) override;
    virtual bool mouse_button_event(const Vector2i &p, int button, bool down,
                                    int modifiers) override;
    virtual bool mouse_enter_event(const Vector2i &p, bool enter) override;
    virtual bool mouse_motion_event(const Vector2i &p, const Vector2i &rel, int button,
                                    int modifiers) override;

protected:
    std::pair<int, bool> tab_at_position(const Vector2i &p,
                                         bool test_vertical = true) const;
    virtual void update_visibility();

protected:
    std::string m_font;
    std::vector<std::string> m_tab_captions;
    std::vector<int> m_tab_ids;
    std::vector<int> m_tab_offsets;
    int m_close_width = 0;
    int m_active_tab = 0;
    int m_tab_drag_index = -1;
    int m_tab_drag_min   = -1, m_tab_drag_max = -1;
    int m_tab_drag_start = -1, m_tab_drag_end = -1;
    int m_close_index = -1, m_close_index_pushed = -1;
    bool m_tabs_draggable = false;
    bool m_tabs_closeable = false;
    Popup *m_popup = nullptr;
    int m_tab_counter = 0;
    int m_padding = 3;
    std::function<void(int)> m_callback;
    std::function<void(int)> m_close_callback;
    std::function<Popup*(int, Screen*)> m_popup_callback;
    Color m_background_color;
};

/**
 * \class TabWidget tabwidget.h nanogui/tabwidget.h
 *
 * \brief A wrapper around the widgets TabHeader and StackedWidget which hooks
 *        the two classes together.
 *
 * \rst
 *
 * .. warning::
 *
 *    Unlike other widgets, children may **not** be added *directly* to a
 *    TabWidget.  For example, the following code will raise an exception:
 *
 *    .. code-block:: cpp
 *
 *       // `this` might be say a nanogui::Screen instance
 *       Window *window = new Window(this, "Window Title");
 *       TabWidget *tab_widget = window->add<TabWidget>();
 *       // this label would be a direct child of tabWidget,
 *       // which is forbidden, so an exception will be raised
 *       new Label(tab_widget, "Some Label");
 *
 *    Instead, you are expected to be creating tabs and adding widgets to those.
 *
 *    .. code-block:: cpp
 *
 *       // `this` might e.g. be a nanogui::Screen instance
 *       Window *window = new Window(this, "Window Title");
 *       TabWidget *tab_widget = window->add<TabWidget>();
 *       // Create a tab first
 *       Widget *tab = new Widget(tab_widget);
 *       int tab_id = tab_widget->append_tab("Tab Name", tab);
 *       // Add children to the created tabs
 *       tab->set_layout(new GroupLayout());
 *       new Label(tab, "Some Label");
 *
 *    A slightly more involved example of creating a TabWidget can also be found
 *    in :ref:`nanogui_example_1` (search for ``tab_widget`` in the file).
 *
 * \endrst
 */
class NANOGUI_EXPORT TabWidget : public TabWidgetBase {
public:
    /// Construct a new tab widget
    TabWidget(Widget *parent, const std::string &font = "sans-bold");

    /// Inserts a new tab at the specified position and returns its ID.
    int insert_tab(int index, const std::string &caption, Widget *widget);

    /// Appends a new tab and returns its ID.
    int append_tab(const std::string &caption, Widget *widget);

    /// Removes a tab with the specified ID
    virtual void remove_tab(int id) override;

    /// Remove child widgets when the associated tab is closed/removed?
    bool remove_children() const { return m_remove_children; }

    /// Remove child widgets when the associated tab is closed/removed?
    void set_remove_children(bool value) { m_remove_children = value; }

    virtual void perform_layout(NVGcontext* ctx) override;
    virtual Vector2i preferred_size(NVGcontext* ctx) const override;
protected:
    virtual void update_visibility() override;
protected:
    std::unordered_map<int, Widget *> m_widgets;
    bool m_remove_children = true;
};

NAMESPACE_END(nanogui)
