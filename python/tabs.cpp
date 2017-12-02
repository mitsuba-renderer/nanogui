#ifdef NANOGUI_PYTHON

#include "python.h"

DECLARE_WIDGET(StackedWidget);
DECLARE_WIDGET(TabHeader);
DECLARE_WIDGET(TabWidget);

void register_tabs(py::module &m) {
    py::class_<StackedWidget, Widget, ref<StackedWidget>, PyStackedWidget>(m, "StackedWidget", D(StackedWidget))
        .def(py::init<Widget *>(), D(StackedWidget, StackedWidget))
        .def("selected_index", &StackedWidget::selected_index, D(StackedWidget, selected_index))
        .def("set_selected_index", &StackedWidget::set_selected_index, D(StackedWidget, set_selected_index));

    py::class_<TabHeader, Widget, ref<TabHeader>, PyTabHeader>(m, "TabHeader", D(TabHeader))
        .def(py::init<Widget *, const std::string &>(), D(TabHeader, TabHeader))
        .def("set_font", &TabHeader::set_font, D(TabHeader, set_font))
        .def("font", &TabHeader::font, D(TabHeader, font))
        .def("overflowing", &TabHeader::overflowing, D(TabHeader, overflowing))
        .def("callback", &TabHeader::callback, D(TabHeader, callback))
        .def("set_callback", &TabHeader::set_callback, D(TabHeader, set_callback))
        .def("active_tab", &TabHeader::active_tab, D(TabHeader, active_tab))
        .def("set_active_tab", &TabHeader::set_active_tab, D(TabHeader, set_active_tab))
        .def("add_tab", (void (TabHeader::*)(const std::string &)) &TabHeader::add_tab, D(TabHeader, add_tab))
        .def("add_tab", (void (TabHeader::*)(int index, const std::string &)) &TabHeader::add_tab, D(TabHeader, add_tab, 2))
        .def("remove_tab", (int (TabHeader::*)(const std::string &)) &TabHeader::remove_tab, D(TabHeader, remove_tab))
        .def("remove_tab", (void (TabHeader::*)(int index)) &TabHeader::remove_tab, D(TabHeader, remove_tab, 2))
        .def("tab_label_at", &TabHeader::tab_label_at, D(TabHeader, tab_label_at))
        .def("tab_index", &TabHeader::tab_index, D(TabHeader, tab_index))
        .def("ensure_tab_visible", &TabHeader::ensure_tab_visible, D(TabHeader, ensure_tab_visible));

    py::class_<TabWidget, Widget, ref<TabWidget>, PyTabWidget>(m, "TabWidget", D(TabWidget))
        .def(py::init<Widget *>(), D(TabWidget, TabWidget))
        .def("set_active_tab", &TabWidget::set_active_tab, D(TabWidget, set_active_tab))
        .def("active_tab", &TabWidget::active_tab, D(TabWidget, active_tab))
        .def("tab_count", &TabWidget::tab_count, D(TabWidget, tab_count))
        .def("set_callback", &TabWidget::set_callback, D(TabWidget, set_callback))
        .def("callback", &TabWidget::callback, D(TabWidget, callback))
        .def("add_tab", (void (TabWidget::*)(const std::string &, Widget *)) &TabWidget::add_tab, D(TabWidget, add_tab))
        .def("add_tab", (void (TabWidget::*)(int index, const std::string &, Widget *)) &TabWidget::add_tab, D(TabWidget, add_tab, 2))
        .def("create_tab", (Widget *(TabWidget::*)(const std::string &)) &TabWidget::create_tab, D(TabWidget, create_tab))
        .def("create_tab", (Widget *(TabWidget::*)(int index, const std::string &)) &TabWidget::create_tab, D(TabWidget, create_tab, 2))
        .def("remove_tab", (bool (TabWidget::*)(const std::string &)) &TabWidget::remove_tab, D(TabWidget, remove_tab))
        .def("remove_tab", (void (TabWidget::*)(int index)) &TabWidget::remove_tab, D(TabWidget, remove_tab, 2))
        .def("tab_label_at", &TabWidget::tab_label_at, D(TabWidget, tab_label_at))
        .def("tab_index", &TabWidget::tab_index, D(TabWidget, tab_index))
        .def("tab", (Widget * (TabWidget::*)(const std::string &)) &TabWidget::tab, D(TabWidget, tab))
        .def("tab", (Widget * (TabWidget::*)(int)) &TabWidget::tab, D(TabWidget, tab, 2))
        .def("ensure_tab_visible", &TabWidget::ensure_tab_visible, D(TabWidget, ensure_tab_visible));
}

#endif
