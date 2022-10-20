#ifdef NANOGUI_PYTHON

#include "python.h"

void register_eigen(nb::module_ &m) {
    nb::class_<Color>(m, "Color", D(Color))
        .def(nb::init<int, int, int, int>(), D(Color, Color, 7))
        .def(nb::init<int, int>(), D(Color, Color, 5))
        .def(nb::init<float, float, float, float>(), D(Color, Color, 7))
        .def(nb::init<float, float>(), D(Color, Color, 5))
        .def("contrasting_color", &Color::contrasting_color,
             D(Color, contrasting_color))
        .def_property("r", [](const Color &c) { return c.r(); },
                      [](Color &c, float v) { c.r() = v; }, D(Color, r))
        .def_property("g", [](const Color &c) { return c.g(); },
                      [](Color &c, float v) { c.g() = v; }, D(Color, g))
        .def_property("b", [](const Color &c) { return c.b(); },
                      [](Color &c, float v) { c.b() = v; }, D(Color, b))
        .def_property("w", [](const Color &c) { return c.w(); },
                      [](Color &c, float v) { c.w() = v; }, "Return a reference to the alpha channel.")
        .def("__repr__", [](const Color &c) {
            std::ostringstream oss;
            oss << c;
            return oss.str();
        });
}
#endif
