#ifdef NANOGUI_PYTHON

#include "python.h"

typedef FloatBox<double> DoubleBox;
typedef IntBox<int64_t> Int64Box;

DECLARE_WIDGET(TextBox);
DECLARE_WIDGET(DoubleBox);
DECLARE_WIDGET(Int64Box);

void register_textbox(py::module &m) {
    py::class_<TextBox, Widget, ref<TextBox>, PyTextBox> tbox(m, "TextBox", D(TextBox));
    tbox
        .def(py::init<Widget *, const std::string &>(), "parent"_a,
            "value"_a = std::string("Untitled"), D(TextBox, TextBox))
        .def("editable", &TextBox::editable, D(TextBox, editable))
        .def("set_editable", &TextBox::set_editable, D(TextBox, set_editable))
        .def("spinnable", &TextBox::spinnable, D(TextBox, spinnable))
        .def("set_spinnable", &TextBox::set_spinnable, D(TextBox, set_spinnable))
        .def("value", &TextBox::value, D(TextBox, value))
        .def("set_value", &TextBox::set_value, D(TextBox, set_value))
        .def("default_value", &TextBox::default_value, D(TextBox, default_value))
        .def("set_default_value", &TextBox::set_default_value, D(TextBox, set_default_value))
        .def("alignment", &TextBox::alignment, D(TextBox, alignment))
        .def("set_alignment", &TextBox::set_alignment, D(TextBox, set_alignment))
        .def("units", &TextBox::units, D(TextBox, units))
        .def("set_units", &TextBox::set_units, D(TextBox, set_units))
        .def("units_image", &TextBox::units_image, D(TextBox, units_image))
        .def("set_units_image", &TextBox::set_units_image, D(TextBox, set_units_image))
        .def("format", &TextBox::format, D(TextBox, format))
        .def("set_format", &TextBox::set_format, D(TextBox, set_format))
        .def("placeholder", &TextBox::placeholder, D(TextBox, placeholder))
        .def("set_placeholder", &TextBox::set_placeholder, D(TextBox, set_placeholder))
        .def("callback", &TextBox::callback, D(TextBox, callback))
        .def("set_callback", &TextBox::set_callback, D(TextBox, set_callback));

    py::enum_<TextBox::Alignment>(tbox, "Alignment", D(TextBox, Alignment))
        .value("Left", TextBox::Alignment::Left)
        .value("Center", TextBox::Alignment::Center)
        .value("Right", TextBox::Alignment::Right);

    py::class_<Int64Box, TextBox, ref<Int64Box>, PyInt64Box>(m, "IntBox", D(IntBox))
        .def(py::init<Widget *, int64_t>(), "parent"_a, "value"_a = (int64_t) 0, D(IntBox, IntBox))
        .def("value", &Int64Box::value, D(IntBox, value))
        .def("set_value", (void (Int64Box::*)(int64_t)) &Int64Box::set_value, D(IntBox, set_value))
        .def("set_callback", (void (Int64Box::*)(const std::function<void(int64_t)>&))
                &Int64Box::set_callback, D(IntBox, set_callback))
        .def("set_value_increment", &Int64Box::set_value_increment, D(IntBox, set_value_increment))
        .def("set_min_value", &Int64Box::set_min_value, D(IntBox, set_min_value))
        .def("set_max_value", &Int64Box::set_max_value, D(IntBox, set_max_value))
        .def("set_min_value", &Int64Box::set_min_max_values, D(IntBox, set_min_max_values));

    py::class_<DoubleBox, TextBox, ref<DoubleBox>, PyDoubleBox>(m, "FloatBox", D(FloatBox))
        .def(py::init<Widget *, double>(), "parent"_a, "value"_a = 0.0)
        .def("value", &DoubleBox::value, D(FloatBox, value))
        .def("set_value", (void (DoubleBox::*)(double)) &DoubleBox::set_value, D(FloatBox, set_value))
        .def("set_callback", (void (DoubleBox::*)(const std::function<void(double)>&))
                &DoubleBox::set_callback, D(FloatBox, set_callback))
        .def("set_value_increment", &DoubleBox::set_value_increment, D(FloatBox, set_value_increment))
        .def("set_min_value", &DoubleBox::set_min_value, D(FloatBox, set_min_value))
        .def("set_max_value", &DoubleBox::set_max_value, D(FloatBox, set_max_value))
        .def("set_min_value", &DoubleBox::set_min_max_values, D(FloatBox, set_min_max_values));
}

#endif
