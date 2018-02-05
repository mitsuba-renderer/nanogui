/*
    nanogui/tabwidget.h -- A wrapper around the widgets TabHeader and StackedWidget
    which hooks the two classes together.

    The tab widget was contributed by Stefan Ivanov.

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

NAMESPACE_BEGIN(nanogui)

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
 *       // `this` might be say a nanogui::Screen instance
 *       Window *window = new Window(this, "Window Title");
 *       TabWidget *tab_widget = window->add<TabWidget>();
 *       // Create a tab first
 *       auto *layer = tab_widget->createTab("Tab Name");
 *       // Add children to the created tabs
 *       layer->setLayout(new GroupLayout());
 *       new Label(layer, "Some Label");
 *
 *    A slightly more involved example of creating a TabWidget can also be found
 *    in :ref:`nanogui_example_1` (search for ``tabWidget`` in the file).
 *
 * \endrst
 */
class NANOGUI_EXPORT TabWidget : public Widget {
public:
    TabWidget(Widget *parent);

    /**
     * \brief Forcibly prevent mis-use of the class by throwing an exception.
     *        Children are not to be added directly to the TabWidget, see
     *        the class level documentation (\ref TabWidget) for an example.
     *
     * \throws std::runtime_error
     *     An exception is always thrown, as children are not allowed to be
     *     added directly to this Widget.
     */
    virtual void add_child(int index, Widget *widget) override;

    void set_active_tab(int tab_index);
    int active_tab() const;
    int tab_count() const;

    /**
     * Sets the callable objects which is invoked when a tab is changed.
     * The argument provided to the callback is the index of the new active tab.
     */
    void set_callback(const std::function<void(int)> &callback) { m_callback = callback; };
    const std::function<void(int)> &callback() const { return m_callback; }

    /// Creates a new tab with the specified name and returns a pointer to the layer.
    Widget* create_tab(const std::string &label);
    Widget* create_tab(int index, const std::string &label);

    /// Inserts a tab at the end of the tabs collection and associates it with the provided widget.
    void add_tab(const std::string &label, Widget *tab);

    /// Inserts a tab into the tabs collection at the specified index and associates it with the provided widget.
    void add_tab(int index, const std::string &label, Widget *tab);

    /**
     * Removes the tab with the specified label and returns the index of the label.
     * Returns whether the removal was successful.
     */
    bool remove_tab(const std::string &label);

    /// Removes the tab with the specified index.
    void remove_tab(int index);

    /// Retrieves the label of the tab at a specific index.
    const std::string &tab_label_at(int index) const;

    /**
     * Retrieves the index of a specific tab using its tab label.
     * Returns -1 if there is no such tab.
     */
    int tab_label_index(const std::string &label);

    /**
     * Retrieves the index of a specific tab using a widget pointer.
     * Returns -1 if there is no such tab.
     */
    int tab_index(Widget* tab);

    /**
     * This function can be invoked to ensure that the tab with the provided
     * index the is visible, i.e to track the given tab. Forwards to the tab
     * header widget. This function should be used whenever the client wishes
     * to make the tab header follow a newly added tab, as the content of the
     * new tab is made visible but the tab header does not track it by default.
     */
    void ensure_tab_visible(int index);

    /**
     * \brief Returns a ``const`` pointer to the Widget associated with the
     *        specified label.
     *
     * \param label
     *     The label used to create the tab.
     *
     * \return
     *     The Widget associated with this label, or ``nullptr`` if not found.
     */
    const Widget *tab(const std::string &label) const;

    /**
     * \brief Returns a pointer to the Widget associated with the specified label.
     *
     * \param label
     *     The label used to create the tab.
     *
     * \return
     *     The Widget associated with this label, or ``nullptr`` if not found.
     */
    Widget *tab(const std::string &label);

    /**
     * \brief Returns a ``const`` pointer to the Widget associated with the
     *        specified index.
     *
     * \param index
     *     The current index of the desired Widget.
     *
     * \return
     *     The Widget at the specified index, or ``nullptr`` if ``index`` is not
     *     a valid index.
     */
    const Widget *tab(int index) const;

    /**
     * \brief Returns a pointer to the Widget associated with the specified index.
     *
     * \param index
     *     The current index of the desired Widget.
     *
     * \return
     *     The Widget at the specified index, or ``nullptr`` if ``index`` is not
     *     a valid index.
     */
    Widget *tab(int index);

    virtual void perform_layout(NVGcontext* ctx) override;
    virtual Vector2i preferred_size(NVGcontext* ctx) const override;
    virtual void draw(NVGcontext* ctx) override;

private:
    TabHeader* m_header;
    StackedWidget* m_content;
    std::function<void(int)> m_callback;
};

NAMESPACE_END(nanogui)
