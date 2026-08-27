"""Python counterpart of the C++ ``nanogui::FormHelper`` class.

This is a reimplementation rather than a binding. Widget callbacks must
cross the language boundary exactly once so that the cyclic garbage
collector can recover them through the ``callback()`` getters (see
``widget_tp_traverse`` in ``src/python/widget.cpp``). Adapters that a C++
implementation would express as lambdas are Python closures here, which
the collector can traverse.
"""

import nanogui as ng

Anchor = ng.AdvancedGridLayout.Anchor


class _FormComboBox(ng.ComboBox):
    """ComboBox that re-applies the tracked value when the item list
    changes (``set_selected_index`` ignores calls while it is empty)"""

    def __init__(self, parent, getter):
        super().__init__(parent)
        self._getter = getter

    def set_items(self, *args):
        super().set_items(*args)
        self.set_selected_index(self._getter())


class FormHelper:
    """Convenience class to create simple AntTweakBar-style layouts that
    expose variables of various types using NanoGUI widgets"""

    def __init__(self, screen):
        self._screen = screen
        self._window = None
        self._layout = None
        self._refresh_callbacks = []
        self._fixed_size = ng.Vector2i(0, 20)
        self._group_font_name = "sans-bold"
        self._label_font_name = "sans"
        self._group_font_size = 20
        self._label_font_size = 16
        self._widget_font_size = 16
        self._pre_group_spacing = 15
        self._post_group_spacing = 5
        self._variable_spacing = 5

    def add_window(self, pos, title="Untitled"):
        """Add a new top-level window"""
        self._window = window = ng.Window(self._screen, title)
        self._layout = layout = ng.AdvancedGridLayout([10, 0, 10, 0], [], 10)
        layout.set_col_stretch(2, 1)
        window.set_position(ng.Vector2i(pos[0], pos[1]))
        window.set_layout(layout)
        window.set_visible(True)
        return window

    def set_window(self, window):
        """Set the active window and continue placing widgets in it"""
        layout = window.layout()
        if not isinstance(layout, ng.AdvancedGridLayout):
            raise RuntimeError("FormHelper.set_window(): the window has "
                               "an incompatible layout!")
        self._window = window
        self._layout = layout

    def window(self):
        """The currently active window"""
        return self._window

    def add_group(self, caption):
        """Add a new group that may contain several sub-widgets"""
        layout = self._layout
        label = ng.Label(self._window, caption, self._group_font_name,
                         self._group_font_size)
        if layout.row_count() > 0:
            layout.append_row(self._pre_group_spacing)
        layout.append_row(0)
        layout.set_anchor(label, Anchor(0, layout.row_count() - 1, 4, 1))
        layout.append_row(self._post_group_spacing)
        return label

    def _label(self, caption):
        return ng.Label(self._window, caption, self._label_font_name,
                        self._label_font_size)

    def _install(self, label, widget, current, apply, getter):
        """Shared plumbing: fonts, sizing, refresh hookup, grid placement"""
        widget.set_font_size(self._widget_font_size)
        fs = widget.fixed_size()
        widget.set_fixed_size(ng.Vector2i(fs[0] or self._fixed_size[0],
                                          fs[1] or self._fixed_size[1]))

        def refresh():
            value = getter()
            if value != current():
                apply(value)
        self._refresh_callbacks.append(refresh)

        layout = self._layout
        if layout.row_count() > 0:
            layout.append_row(self._variable_spacing)
        layout.append_row(0)
        row = layout.row_count() - 1
        layout.set_anchor(label, Anchor(1, row))
        layout.set_anchor(widget, Anchor(3, row))
        return widget

    def add_bool_variable(self, label, setter, getter, editable=True):
        """Add a checkbox controlled by custom getter/setter functions"""
        label_w = self._label(label)
        widget = ng.CheckBox(self._window, "")
        widget.set_fixed_width(20)
        widget.set_checked(getter())
        widget.set_callback(setter)
        widget.set_enabled(editable)
        return self._install(label_w, widget, widget.checked,
                             widget.set_checked, getter)

    def add_int_variable(self, label, setter, getter, editable=True):
        """Add an integer box controlled by custom getter/setter functions"""
        label_w = self._label(label)
        widget = ng.IntBox(self._window)
        widget.set_alignment(ng.TextBox.Alignment.Right)
        widget.set_value(getter())
        widget.set_callback(setter)
        widget.set_editable(editable)
        return self._install(label_w, widget, widget.value,
                             widget.set_value, getter)

    def add_double_variable(self, label, setter, getter, editable=True):
        """Add a float box controlled by custom getter/setter functions"""
        label_w = self._label(label)
        widget = ng.FloatBox(self._window)
        widget.set_alignment(ng.TextBox.Alignment.Right)
        widget.set_value(getter())
        widget.set_callback(setter)
        widget.set_editable(editable)
        return self._install(label_w, widget, widget.value,
                             widget.set_value, getter)

    def add_string_variable(self, label, setter, getter, editable=True):
        """Add a text box controlled by custom getter/setter functions"""
        label_w = self._label(label)
        widget = ng.TextBox(self._window)
        widget.set_alignment(ng.TextBox.Alignment.Left)
        widget.set_value(getter())

        def apply(value):
            # TextBox callbacks report whether to accept the edit
            setter(value)
            return True
        widget.set_callback(apply)
        widget.set_editable(editable)
        return self._install(label_w, widget, widget.value,
                             widget.set_value, getter)

    def add_color_variable(self, label, setter, getter, editable=True):
        """Add a color picker controlled by custom getter/setter functions"""
        label_w = self._label(label)
        widget = ng.ColorPicker(self._window)
        widget.set_color(getter())
        widget.set_callback(setter)
        widget.set_enabled(editable)
        return self._install(label_w, widget, widget.color,
                             widget.set_color, getter)

    def add_enum_variable(self, label, setter, getter, editable=True):
        """Add a combo box controlled by custom getter/setter functions
        that map the selection to an integer index"""
        label_w = self._label(label)
        widget = _FormComboBox(self._window, getter)
        widget.set_selected_index(getter())
        widget.set_callback(setter)
        widget.set_enabled(editable)
        return self._install(label_w, widget, widget.selected_index,
                             widget.set_selected_index, getter)

    def add_button(self, label, cb):
        """Add a button with a custom callback"""
        widget = ng.Button(self._window, label)
        widget.set_callback(cb)
        widget.set_fixed_height(25)
        layout = self._layout
        if layout.row_count() > 0:
            layout.append_row(self._variable_spacing)
        layout.append_row(0)
        layout.set_anchor(widget, Anchor(1, layout.row_count() - 1, 3, 1))
        return widget

    def add_widget(self, label, widget):
        """Add an arbitrary (optionally labeled) widget to the layout"""
        layout = self._layout
        layout.append_row(0)
        row = layout.row_count() - 1
        if not label:
            layout.set_anchor(widget, Anchor(1, row, 3, 1))
        else:
            layout.set_anchor(self._label(label), Anchor(1, row))
            layout.set_anchor(widget, Anchor(3, row))

    def refresh(self):
        """Cause all widgets to re-synchronize with the underlying state"""
        for callback in self._refresh_callbacks:
            callback()

    def fixed_size(self):
        return self._fixed_size

    def set_fixed_size(self, fw):
        self._fixed_size = ng.Vector2i(fw[0], fw[1])

    def group_font_name(self):
        return self._group_font_name

    def set_group_font_name(self, name):
        self._group_font_name = name

    def label_font_name(self):
        return self._label_font_name

    def set_label_font_name(self, name):
        self._label_font_name = name

    def group_font_size(self):
        return self._group_font_size

    def set_group_font_size(self, value):
        self._group_font_size = value

    def label_font_size(self):
        return self._label_font_size

    def set_label_font_size(self, value):
        self._label_font_size = value

    def widget_font_size(self):
        return self._widget_font_size

    def set_widget_font_size(self, value):
        self._widget_font_size = value
