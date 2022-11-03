#ifdef NANOGUI_PYTHON

#include "python.h"

typedef FloatBox<double> DoubleBox;
typedef IntBox<int64_t> Int64Box;

DECLARE_WIDGET(TextBox);
DECLARE_WIDGET(DoubleBox);
DECLARE_WIDGET(Int64Box);

void register_textbox(nb::module_ &m) {
    nb::class_<TextBox, Widget, PyTextBox> tbox(m, "TextBox", D(TextBox));
    tbox
        .def(nb::init<Widget *, const std::string &>(), "parent"_a,
            "value"_a = std::string("Untitled"), D(TextBox, TextBox))
        .def(nb_method(TextBox, editable), D(TextBox, editable))
        .def(nb_method(TextBox, set_editable), D(TextBox, set_editable))
        .def(nb_method(TextBox, spinnable), D(TextBox, spinnable))
        .def(nb_method(TextBox, set_spinnable), D(TextBox, set_spinnable))
        .def(nb_method(TextBox, value), D(TextBox, value))
        .def(nb_method(TextBox, set_value), D(TextBox, set_value))
        .def(nb_method(TextBox, default_value), D(TextBox, default_value))
        .def(nb_method(TextBox, set_default_value), D(TextBox, set_default_value))
        .def(nb_method(TextBox, alignment), D(TextBox, alignment))
        .def(nb_method(TextBox, set_alignment), D(TextBox, set_alignment))
        .def(nb_method(TextBox, units), D(TextBox, units))
        .def(nb_method(TextBox, set_units), D(TextBox, set_units))
        .def(nb_method(TextBox, units_image), D(TextBox, units_image))
        .def(nb_method(TextBox, set_units_image), D(TextBox, set_units_image))
        .def(nb_method(TextBox, format), D(TextBox, format))
        .def(nb_method(TextBox, set_format), D(TextBox, set_format))
        .def(nb_method(TextBox, placeholder), D(TextBox, placeholder))
        .def(nb_method(TextBox, set_placeholder), D(TextBox, set_placeholder))
        .def(nb_method(TextBox, callback), D(TextBox, callback))
        .def(nb_method(TextBox, set_callback), D(TextBox, set_callback));

    nb::enum_<TextBox::Alignment>(tbox, "Alignment", D(TextBox, Alignment))
        .value("Left", TextBox::Alignment::Left)
        .value("Center", TextBox::Alignment::Center)
        .value("Right", TextBox::Alignment::Right);

    nb::class_<Int64Box, TextBox, PyInt64Box>(m, "IntBox", D(IntBox))
        .def(nb::init<Widget *, int64_t>(), "parent"_a, "value"_a = (int64_t) 0, D(IntBox, IntBox))
        .def(nb_method(Int64Box, value), D(IntBox, value))
        .def("set_value", (void (Int64Box::*)(int64_t)) &Int64Box::set_value, D(IntBox, set_value))
        .def("set_callback", (void (Int64Box::*)(const std::function<void(int64_t)>&))
                &Int64Box::set_callback, D(IntBox, set_callback))
        .def(nb_method(Int64Box, set_value_increment), D(IntBox, set_value_increment))
        .def(nb_method(Int64Box, set_min_value), D(IntBox, set_min_value))
        .def(nb_method(Int64Box, set_max_value), D(IntBox, set_max_value))
        .def(nb_method(Int64Box, set_min_value), D(IntBox, set_min_max_values));

    nb::class_<DoubleBox, TextBox, PyDoubleBox>(m, "FloatBox", D(FloatBox))
        .def(nb::init<Widget *, double>(), "parent"_a, "value"_a = 0.0)
        .def(nb_method(DoubleBox, value), D(FloatBox, value))
        .def("set_value", (void (DoubleBox::*)(double)) &DoubleBox::set_value, D(FloatBox, set_value))
        .def("set_callback", (void (DoubleBox::*)(const std::function<void(double)>&))
                &DoubleBox::set_callback, D(FloatBox, set_callback))
        .def(nb_method(DoubleBox, set_value_increment), D(FloatBox, set_value_increment))
        .def(nb_method(DoubleBox, set_min_value), D(FloatBox, set_min_value))
        .def(nb_method(DoubleBox, set_max_value), D(FloatBox, set_max_value))
        .def(nb_method(DoubleBox, set_min_value), D(FloatBox, set_min_max_values));
}

#endif
