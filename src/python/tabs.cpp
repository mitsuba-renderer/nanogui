#ifdef NANOGUI_PYTHON

#include "python.h"

DECLARE_WIDGET(TabWidgetBase);
DECLARE_WIDGET(TabWidget);

void register_tabs(py::module &m) {
    py::class_<TabWidgetBase, Widget, ref<TabWidgetBase>, PyTabWidgetBase>(m, "TabWidgetBase", D(TabWidgetBase))
        .def(py::init<Widget *>(), D(TabWidgetBase, TabWidgetBase))
        .def("tab_count", &TabWidgetBase::tab_count, D(TabWidgetBase, tab_count))
        .def("tab_id", &TabWidgetBase::tab_id, D(TabWidgetBase, tab_id))
        .def("tab_index", &TabWidgetBase::tab_index, D(TabWidgetBase, tab_index))
        .def("insert_tab", &TabWidgetBase::insert_tab, D(TabWidgetBase, insert_tab), "index"_a, "caption"_a)
        .def("append_tab", &TabWidgetBase::append_tab, D(TabWidgetBase, append_tab), "caption"_a)
        .def("remove_tab", &TabWidgetBase::remove_tab, D(TabWidgetBase, remove_tab), "id"_a)
        .def("selected_index", &TabWidgetBase::selected_index, D(TabWidgetBase, selected_index))
        .def("set_selected_index", &TabWidgetBase::set_selected_index, D(TabWidgetBase, set_selected_index), "id"_a)
        .def("selected_id", &TabWidgetBase::selected_id, D(TabWidgetBase, selected_id))
        .def("set_selected_id", &TabWidgetBase::set_selected_id, D(TabWidgetBase, set_selected_id), "id"_a)
        .def("tab_caption", &TabWidgetBase::tab_caption, D(TabWidgetBase, tab_caption), "id"_a)
        .def("set_tab_caption", &TabWidgetBase::set_tab_caption, D(TabWidgetBase, set_tab_caption), "id"_a, "caption"_a)
        .def("tabs_draggable", &TabWidgetBase::tabs_draggable, D(TabWidgetBase, tabs_draggable))
        .def("set_tabs_draggable", &TabWidgetBase::set_tabs_draggable, D(TabWidgetBase, set_tabs_draggable))
        .def("tabs_closeable", &TabWidgetBase::tabs_closeable, D(TabWidgetBase, tabs_closeable))
        .def("set_tabs_closeable", &TabWidgetBase::set_tabs_closeable, D(TabWidgetBase, set_tabs_closeable))
        .def("padding", &TabWidgetBase::padding, D(TabWidgetBase, padding))
        .def("set_padding", &TabWidgetBase::set_padding, D(TabWidgetBase, set_padding))
        .def("set_background_color", &TabWidgetBase::set_background_color, D(TabWidgetBase, set_background_color))
        .def("background_color", &TabWidgetBase::background_color, D(TabWidgetBase, background_color))
        .def("set_callback", &TabWidgetBase::set_callback, D(TabWidgetBase, set_callback))
        .def("callback", &TabWidgetBase::callback, D(TabWidgetBase, callback))
        .def("close_callback", &TabWidgetBase::close_callback, D(TabWidgetBase, close_callback))
        .def("set_close_callback", &TabWidgetBase::set_close_callback, D(TabWidgetBase, set_close_callback))
        .def("popup_callback", &TabWidgetBase::popup_callback, D(TabWidgetBase, popup_callback))
        .def("set_popup_callback", &TabWidgetBase::set_popup_callback, D(TabWidgetBase, set_popup_callback));

    py::class_<TabWidget, TabWidgetBase, ref<TabWidget>, PyTabWidget>(m, "TabWidget", D(TabWidget))
        .def(py::init<Widget *>(), D(TabWidget, TabWidget))
        .def("insert_tab", &TabWidget::insert_tab, D(TabWidget, insert_tab), "index"_a, "caption"_a, "widget"_a)
        .def("append_tab", &TabWidget::append_tab, D(TabWidget, append_tab), "caption"_a, "widget"_a)
        .def("remove_children", &TabWidget::remove_children, D(TabWidget, remove_children))
        .def("set_remove_children", &TabWidget::set_remove_children, D(TabWidget, set_remove_children), "id"_a);
}

#endif
