#ifdef NANOGUI_PYTHON

#include "python.h"

class PyGLCanvas : public GLCanvas {
public:
    using GLCanvas::GLCanvas;
    NANOGUI_WIDGET_OVERLOADS(GLCanvas);

    void draw_gl() override {
        PYBIND11_OVERLOAD(void, GLCanvas, draw_gl);
    }
};

void register_glcanvas(py::module &m) {
    py::class_<GLCanvas, Widget, ref<GLCanvas>, PyGLCanvas> glcanvas(m, "GLCanvas", D(GLCanvas));
    glcanvas
        .def(py::init<Widget *>(), "parent"_a, D(GLCanvas, GLCanvas))
        .def("background_color", &GLCanvas::background_color, D(GLCanvas, background_color))
        .def("set_background_color", &GLCanvas::set_background_color, D(GLCanvas, set_background_color))
        .def("draw_border", &GLCanvas::draw_border, D(GLCanvas, draw_border))
        .def("set_draw_border", &GLCanvas::set_draw_border, D(GLCanvas, set_draw_border))
        .def("draw_gl", &GLCanvas::draw_gl, D(GLCanvas, draw_gl));
}

#endif
