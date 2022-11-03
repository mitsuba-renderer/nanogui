#ifdef NANOGUI_PYTHON

#include "python.h"

DECLARE_WIDGET(TabWidgetBase);
DECLARE_WIDGET(TabWidget);

void register_tabs(nb::module_ &m) {
    nb::class_<TabWidgetBase, Widget, PyTabWidgetBase>(m, "TabWidgetBase", D(TabWidgetBase))
        .def(nb::init<Widget *>(), D(TabWidgetBase, TabWidgetBase))
        .def(nb_method(TabWidgetBase, tab_count), D(TabWidgetBase, tab_count))
        .def(nb_method(TabWidgetBase, tab_id), D(TabWidgetBase, tab_id))
        .def(nb_method(TabWidgetBase, tab_index), D(TabWidgetBase, tab_index))
        .def(nb_method(TabWidgetBase, insert_tab), D(TabWidgetBase, insert_tab), "index"_a, "caption"_a)
        .def(nb_method(TabWidgetBase, append_tab), D(TabWidgetBase, append_tab), "caption"_a)
        .def(nb_method(TabWidgetBase, remove_tab), D(TabWidgetBase, remove_tab), "id"_a)
        .def(nb_method(TabWidgetBase, selected_index), D(TabWidgetBase, selected_index))
        .def(nb_method(TabWidgetBase, set_selected_index), D(TabWidgetBase, set_selected_index), "id"_a)
        .def(nb_method(TabWidgetBase, selected_id), D(TabWidgetBase, selected_id))
        .def(nb_method(TabWidgetBase, set_selected_id), D(TabWidgetBase, set_selected_id), "id"_a)
        .def(nb_method(TabWidgetBase, tab_caption), D(TabWidgetBase, tab_caption), "id"_a)
        .def(nb_method(TabWidgetBase, set_tab_caption), D(TabWidgetBase, set_tab_caption), "id"_a, "caption"_a)
        .def(nb_method(TabWidgetBase, tabs_draggable), D(TabWidgetBase, tabs_draggable))
        .def(nb_method(TabWidgetBase, set_tabs_draggable), D(TabWidgetBase, set_tabs_draggable))
        .def(nb_method(TabWidgetBase, tabs_closeable), D(TabWidgetBase, tabs_closeable))
        .def(nb_method(TabWidgetBase, set_tabs_closeable), D(TabWidgetBase, set_tabs_closeable))
        .def(nb_method(TabWidgetBase, padding), D(TabWidgetBase, padding))
        .def(nb_method(TabWidgetBase, set_padding), D(TabWidgetBase, set_padding))
        .def(nb_method(TabWidgetBase, set_background_color), D(TabWidgetBase, set_background_color))
        .def(nb_method(TabWidgetBase, background_color), D(TabWidgetBase, background_color))
        .def(nb_method(TabWidgetBase, set_callback), D(TabWidgetBase, set_callback))
        .def(nb_method(TabWidgetBase, callback), D(TabWidgetBase, callback))
        .def(nb_method(TabWidgetBase, close_callback), D(TabWidgetBase, close_callback))
        .def(nb_method(TabWidgetBase, set_close_callback), D(TabWidgetBase, set_close_callback))
        .def(nb_method(TabWidgetBase, popup_callback), D(TabWidgetBase, popup_callback))
        .def(nb_method(TabWidgetBase, set_popup_callback), D(TabWidgetBase, set_popup_callback));

    nb::class_<TabWidget, TabWidgetBase, PyTabWidget>(m, "TabWidget", D(TabWidget))
        .def(nb::init<Widget *>(), D(TabWidget, TabWidget))
        .def(nb_method(TabWidget, insert_tab), D(TabWidget, insert_tab), "index"_a, "caption"_a, "widget"_a)
        .def(nb_method(TabWidget, append_tab), D(TabWidget, append_tab), "caption"_a, "widget"_a)
        .def(nb_method(TabWidget, remove_children), D(TabWidget, remove_children))
        .def(nb_method(TabWidget, set_remove_children), D(TabWidget, set_remove_children), "id"_a);
}

#endif
