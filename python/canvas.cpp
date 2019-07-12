#ifdef NANOGUI_PYTHON

#include "python.h"

class PyCanvas : public Canvas {
public:
    using Canvas::Canvas;
    NANOGUI_WIDGET_OVERLOADS(Canvas);

    void draw_contents() override {
        PYBIND11_OVERLOAD(void, Canvas, draw_contents);
    }
};

void register_canvas(py::module &m) {
    py::class_<Canvas, Widget, ref<Canvas>, PyCanvas> canvas(m, "Canvas", D(Canvas));
    canvas
        .def(py::init<Widget *, uint8_t, bool, bool, bool>(),
             "parent"_a, "samples"_a = 4, "has_depth_buffer"_a = true,
             "has_stencil_buffer"_a = false,
             "clear"_a = true, D(Canvas, Canvas))
        .def("render_pass", &Canvas::render_pass, D(Canvas, render_pass))
        .def("draw_border", &Canvas::draw_border, D(Canvas, draw_border))
        .def("set_draw_border", &Canvas::set_draw_border, D(Canvas, set_draw_border))
        .def("border_color", &Canvas::border_color, D(Canvas, border_color))
        .def("set_border_color", &Canvas::set_border_color, D(Canvas, set_border_color))
        .def("background_color", &Canvas::background_color, D(Canvas, background_color))
        .def("set_background_color", &Canvas::set_background_color, D(Canvas, set_background_color))
        .def("draw_contents", &Canvas::draw_contents, D(Canvas, draw_contents));
}

#endif
