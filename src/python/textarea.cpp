#ifdef NANOGUI_PYTHON

#include "python.h"

DECLARE_WIDGET(TextArea);

void register_textarea(nb::module_ &m) {
    nb::class_<TextArea, Widget, PyTextArea>(m, "TextArea", D(TextArea))
        .def(nb::init<Widget *>(), D(TextArea, TextArea))
        .def(nb_method(TextArea, set_font), D(TextArea, set_font))
        .def(nb_method(TextArea, font), D(TextArea, font))
        .def(nb_method(TextArea, set_foreground_color), D(TextArea, set_foreground_color))
        .def(nb_method(TextArea, foreground_color), D(TextArea, foreground_color))
        .def(nb_method(TextArea, set_background_color), D(TextArea, set_background_color))
        .def(nb_method(TextArea, background_color), D(TextArea, background_color))
        .def(nb_method(TextArea, set_selection_color), D(TextArea, set_selection_color))
        .def(nb_method(TextArea, selection_color), D(TextArea, selection_color))
        .def(nb_method(TextArea, set_padding), D(TextArea, set_padding))
        .def(nb_method(TextArea, padding), D(TextArea, padding))
        .def(nb_method(TextArea, set_selectable), D(TextArea, set_selectable))
        .def(nb_method(TextArea, is_selectable), D(TextArea, is_selectable))
        .def(nb_method(TextArea, append), D(TextArea, append))
        .def(nb_method(TextArea, append_line), D(TextArea, append_line))
        .def(nb_method(TextArea, clear), D(TextArea, clear));
}

#endif
