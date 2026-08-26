#ifdef NANOGUI_PYTHON

#include "python.h"
#include <nanogui/lines.h>
#include <nanobind/ndarray.h>

void register_lines(nb::module_ &m) {
    nb::class_<LineRenderer, Shader>(m, "LineRenderer", D(LineRenderer))
        .def(nb::init<RenderPass *, Shader::BlendMode>(),
             D(LineRenderer, LineRenderer),
             "render_pass"_a,
             "blend_mode"_a = Shader::BlendMode::None)
        .def("set_segments",
             [](LineRenderer &lines,
                nb::ndarray<float, nb::shape<-1, 2, 3>, nb::device::cpu,
                            nb::c_contig> endpoints) {
                 lines.set_segments((const Vector3f *) endpoints.data(),
                                    endpoints.shape(0));
             },
             D(LineRenderer, set_segments),
             "endpoints"_a)
        .def("set_box", &LineRenderer::set_box,
             D(LineRenderer, set_box),
             "min"_a, "max"_a)
        .def("set_mvp", &LineRenderer::set_mvp,
             D(LineRenderer, set_mvp),
             "mvp"_a)
        .def("set_width", &LineRenderer::set_width,
             D(LineRenderer, set_width),
             "width"_a)
        .def("width", &LineRenderer::width,
             D(LineRenderer, width))
        .def("set_color", &LineRenderer::set_color,
             D(LineRenderer, set_color),
             "color"_a)
        .def("draw", &LineRenderer::draw,
             D(LineRenderer, draw));
}

#endif
