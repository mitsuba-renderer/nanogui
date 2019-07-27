#ifdef NANOGUI_PYTHON

#include "python.h"

DECLARE_WIDGET(TextArea);

void register_textarea(py::module &m) {
    py::class_<TextArea, Widget, ref<TextArea>, PyTextArea>(m, "TextArea", D(TextArea))
        .def(py::init<Widget *>(), D(TextArea, TextArea))
        .def("set_font", &TextArea::set_font, D(TextArea, set_font))
        .def("font", &TextArea::font, D(TextArea, font))
        .def("set_foreground_color", &TextArea::set_foreground_color, D(TextArea, set_foreground_color))
        .def("foreground_color", &TextArea::foreground_color, D(TextArea, foreground_color))
        .def("set_background_color", &TextArea::set_background_color, D(TextArea, set_background_color))
        .def("background_color", &TextArea::background_color, D(TextArea, background_color))
        .def("set_selection_color", &TextArea::set_selection_color, D(TextArea, set_selection_color))
        .def("selection_color", &TextArea::selection_color, D(TextArea, selection_color))
        .def("set_padding", &TextArea::set_padding, D(TextArea, set_padding))
        .def("padding", &TextArea::padding, D(TextArea, padding))
        .def("set_selectable", &TextArea::set_selectable, D(TextArea, set_selectable))
        .def("is_selectable", &TextArea::is_selectable, D(TextArea, is_selectable))
        .def("append", &TextArea::append, D(TextArea, append))
        .def("append_line", &TextArea::append_line, D(TextArea, append_line))
        .def("clear", &TextArea::clear, D(TextArea, clear));
}

#endif
