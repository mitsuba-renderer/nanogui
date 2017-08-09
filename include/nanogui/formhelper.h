/*
    nanogui/formhelper.h -- helper class to construct forms for editing a set
    of variables of various types

    NanoGUI was developed by Wenzel Jakob <wenzel.jakob@epfl.ch>.
    The widget drawing code is based on the NanoVG demo application
    by Mikko Mononen.

    All rights reserved. Use of this source code is governed by a
    BSD-style license that can be found in the LICENSE.txt file.
*/
/** \file */

#pragma once

#include <nanogui/screen.h>
#include <nanogui/label.h>
#include <nanogui/checkbox.h>
#include <nanogui/textbox.h>
#include <nanogui/combobox.h>
#include <nanogui/colorpicker.h>
#include <nanogui/layout.h>
#include <cassert>

NAMESPACE_BEGIN(nanogui)

NAMESPACE_BEGIN(detail)
/**
 * \class FormWidget formhelper.h nanogui/formhelper.h
 *
 * \brief A template wrapper class for assisting in the creation of various form widgets.
 *
 * \rst
 * The partial template specializations are:
 *
 * - Inheritance from :ref:`class_nanogui__ComboBox` for ``enum`` types:
 *
 *   .. code-block:: cpp
 *
 *      template <typename T>
 *      class FormWidget<T, typename std::is_enum<T>::type> : public ComboBox
 *
 * - Inheritance from :ref:`class_nanogui__IntBox` for integral types:
 *
 *   .. code-block:: cpp
 *
 *      template <typename T>
 *      class FormWidget<T, typename std::is_integral<T>::type> : public IntBox<T>
 *
 * - Inheritance from :ref:`class_nanogui__FloatBox` for floating point types:
 *
 *   .. code-block:: cpp
 *
 *      template <typename T>
 *      class FormWidget<T, typename std::is_floating_point<T>::type> : public FloatBox<T>
 *
 * The full template specializations are:
 *
 * - Inheritance from :ref:`class_nanogui__CheckBox` for booleans:
 *
 *   .. code-block:: cpp
 *
 *      template <>
 *      class FormWidget<bool, std::true_type> : public CheckBox
 *
 * - Inheritance from :ref:`class_nanogui__TextBox` for strings:
 *
 *   .. code-block:: cpp
 *
 *      template <>
 *      class FormWidget<std::string, std::true_type> : public TextBox
 *
 * - Inheritance from :ref:`class_nanogui__ColorPicker` for `Color` types:
 *
 *   .. code-block:: cpp
 *
 *      template <>
 *      class FormWidget<Color, std::true_type> : public ColorPicker
 *
 * Please refer to the bottom of :ref:`program_listing_file_include_nanogui_formhelper.h`
 * for the implementation details.
 * \endrst
 */
template <typename T, typename sfinae = std::true_type> class FormWidget { };
NAMESPACE_END(detail)

/**
 * \class FormHelper formhelper.h nanogui/formhelper.h
 *
 * \brief Convenience class to create simple AntTweakBar-style layouts that
 *        expose variables of various types using NanoGUI widgets
 *
 * **Example**:
 *
 * \rst
 * .. code-block:: cpp
 *
 *    // [ ... initialize NanoGUI, construct screen ... ]
 *
 *    FormHelper* h = new FormHelper(screen);
 *
 *    // Add a new windows widget
 *    h->add_window(Vector2i(10,10),"Menu");
 *
 *    // Start a new group
 *    h->add_group("Group 1");
 *
 *    // Expose an integer variable by reference
 *    h->add_variable("integer variable", a_int);
 *
 *    // Expose a float variable via setter/getter functions
 *    h->add_variable(
 *      [&](float value) { a_float = value; },
 *      [&]() { return *a_float; },
 *      "float variable");
 *
 *    // add a new button
 *    h->add_button("Button", [&]() { std::cout << "Button pressed" << std::endl; });
 *
 * \endrst
 */
class FormHelper {
public:
    /// Create a helper class to construct NanoGUI widgets on the given screen
    FormHelper(Screen *screen) : m_screen(screen) { }

    /// Add a new top-level window
    Window *add_window(const Vector2i &pos,
                         const std::string &title = "Untitled") {
        assert(m_screen);
        m_window = new Window(m_screen, title);
        m_layout = new AdvancedGridLayout({10, 0, 10, 0}, {});
        m_layout->set_margin(10);
        m_layout->set_col_stretch(2, 1);
        m_window->set_position(pos);
        m_window->set_layout(m_layout);
        m_window->set_visible(true);
        return m_window;
    }

    /// Add a new group that may contain several sub-widgets
    Label *add_group(const std::string &caption) {
        Label* label = new Label(m_window, caption, m_group_font_name, m_group_font_size);
        if (m_layout->row_count() > 0)
            m_layout->append_row(m_pre_group_spacing); /* Spacing */
        m_layout->append_row(0);
        m_layout->set_anchor(label, AdvancedGridLayout::Anchor(0, m_layout->row_count()-1, 4, 1));
        m_layout->append_row(m_post_group_spacing);
        return label;
    }

    /// Add a new data widget controlled using custom getter/setter functions
    template <typename Type> detail::FormWidget<Type> *
    add_variable(const std::string &label, const std::function<void(const Type &)> &setter,
                const std::function<Type()> &getter, bool editable = true) {
        Label *label_w = new Label(m_window, label, m_label_font_name, m_label_font_size);
        auto widget = new detail::FormWidget<Type>(m_window);
        auto refresh = [widget, getter] {
            Type value = getter(), current = widget->value();
            if (value != current)
                widget->set_value(value);
        };
        refresh();
        widget->set_callback(setter);
        widget->set_editable(editable);
        widget->set_font_size(m_widget_font_size);
        Vector2i fs = widget->fixed_size();
        widget->set_fixed_size(Vector2i(fs.x() != 0 ? fs.x() : m_fixed_size.x(),
                                      fs.y() != 0 ? fs.y() : m_fixed_size.y()));
        m_refresh_callbacks.push_back(refresh);
        if (m_layout->row_count() > 0)
            m_layout->append_row(m_variable_spacing);
        m_layout->append_row(0);
        m_layout->set_anchor(label_w, AdvancedGridLayout::Anchor(1, m_layout->row_count()-1));
        m_layout->set_anchor(widget, AdvancedGridLayout::Anchor(3, m_layout->row_count()-1));
        return widget;
    }

    /// Add a new data widget that exposes a raw variable in memory
    template <typename Type> detail::FormWidget<Type> *
    add_variable(const std::string &label, Type &value, bool editable = true) {
        return add_variable<Type>(label,
            [&](const Type & v) { value = v; },
            [&]() -> Type { return value; },
            editable
        );
    }

    /// Add a button with a custom callback
    Button *add_button(const std::string &label, const std::function<void()> &cb) {
        Button *button = new Button(m_window, label);
        button->set_callback(cb);
        button->set_fixed_height(25);
        if (m_layout->row_count() > 0)
            m_layout->append_row(m_variable_spacing);
        m_layout->append_row(0);
        m_layout->set_anchor(button, AdvancedGridLayout::Anchor(1, m_layout->row_count()-1, 3, 1));
        return button;
    }

    /// Add an arbitrary (optionally labeled) widget to the layout
    void add_widget(const std::string &label, Widget *widget) {
        m_layout->append_row(0);
        if (label == "") {
            m_layout->set_anchor(widget, AdvancedGridLayout::Anchor(1, m_layout->row_count()-1, 3, 1));
        } else {
            Label *label_w = new Label(m_window, label, m_label_font_name, m_label_font_size);
            m_layout->set_anchor(label_w, AdvancedGridLayout::Anchor(1, m_layout->row_count()-1));
            m_layout->set_anchor(widget, AdvancedGridLayout::Anchor(3, m_layout->row_count()-1));
        }
    }

    /// Cause all widgets to re-synchronize with the underlying variable state
    void refresh() {
        for (auto const &callback : m_refresh_callbacks)
            callback();
    }

    /// Access the currently active \ref Window instance
    Window *window() { return m_window; }
    void set_window(Window *window) {
        m_window = window;
        m_layout = dynamic_cast<AdvancedGridLayout *>(window->layout());
        if (m_layout == nullptr)
            throw std::runtime_error(
                "Internal error: window has an incompatible layout!");
    }

    /// Specify a fixed size for newly added widgets
    void set_fixed_size(const Vector2i &fw) { m_fixed_size = fw; }
    Vector2i fixed_size() { return m_fixed_size; }

    /* Set the font size / name of labels, group headers, and data widgets */
    const std::string &group_font_name() const { return m_group_font_name; }
    void set_group_font_name(const std::string &name) { m_group_font_name = name; }
    const std::string &label_font_name() const { return m_label_font_name; }
    void set_label_font_name(const std::string &name) { m_label_font_name = name; }
    int group_font_size() const { return m_group_font_size; }
    void set_group_font_size(int value) { m_group_font_size = value; }
    int label_font_size() const { return m_label_font_size; }
    void set_label_font_size(int value) { m_label_font_size = value; }
    int widget_font_size() const { return m_widget_font_size; }
    void set_widget_font_size(int value) { m_widget_font_size = value; }

protected:
    ref<Screen> m_screen;
    ref<Window> m_window;
    ref<AdvancedGridLayout> m_layout;
    std::vector<std::function<void()>> m_refresh_callbacks;
    std::string m_group_font_name = "sans-bold";
    std::string m_label_font_name = "sans";
    Vector2i m_fixed_size = Vector2i(0, 20);
    int m_group_font_size = 20;
    int m_label_font_size = 16;
    int m_widget_font_size = 16;
    int m_pre_group_spacing = 15;
    int m_post_group_spacing = 5;
    int m_variable_spacing = 5;
};

NAMESPACE_BEGIN(detail)

// template specialization unsupported
#ifndef DOXYGEN_SHOULD_SKIP_THIS

/* Various types of form widgets for different input types below */
template <> class FormWidget<bool, std::true_type> : public CheckBox {
public:
    FormWidget(Widget *p) : CheckBox(p, "") { set_fixed_width(20); }
    void set_value(bool v) { set_checked(v); }
    void set_editable(bool e) { set_enabled(e); }
    bool value() const { return checked(); }
};

template <typename T> class FormWidget<T, typename std::is_enum<T>::type> : public ComboBox {
public:
    FormWidget(Widget *p) : ComboBox(p) { }
    T value() const { return (T) selected_index(); }
    void set_value(T value) { set_selected_index((int) value); m_selected_index = (int) value; }
    void set_callback(const std::function<void(const T &)> &cb) {
        ComboBox::set_callback([cb](int v) { cb((T) v); });
    }
    void set_editable(bool e) { set_enabled(e); }
};

template <typename T> class FormWidget<T, typename std::is_integral<T>::type> : public IntBox<T> {
public:
    FormWidget(Widget *p) : IntBox<T>(p) { this->set_alignment(TextBox::Alignment::Right); }
};

template <typename T> class FormWidget<T, typename std::is_floating_point<T>::type> : public FloatBox<T> {
public:
    FormWidget(Widget *p) : FloatBox<T>(p) { this->set_alignment(TextBox::Alignment::Right); }
};

template <> class FormWidget<std::string, std::true_type> : public TextBox {
public:
    FormWidget(Widget *p) : TextBox(p) { set_alignment(TextBox::Alignment::Left); }
    void set_callback(const std::function<void(const std::string&)> &cb) {
        TextBox::set_callback([cb](const std::string &str) { cb(str); return true; });
    }
};

template <> class FormWidget<Color, std::true_type> : public ColorPicker {
public:
    FormWidget(Widget *p) : ColorPicker(p) { }
    void set_value(const Color &c) { set_color(c); }
    void set_editable(bool e) { set_enabled(e); }
    Color value() const { return color(); }
};

#endif // DOXYGEN_SHOULD_SKIP_THIS

NAMESPACE_END(detail)
NAMESPACE_END(nanogui)
