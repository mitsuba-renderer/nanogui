#ifdef NANOGUI_PYTHON

#include "python.h"
#include <nanogui/quad.h>

void register_quad(nb::module_ &m) {
    nb::class_<TexturedQuad, Shader>(m, "TexturedQuad", D(TexturedQuad))
        .def(nb::init<RenderPass *, Shader::BlendMode>(),
             D(TexturedQuad, TexturedQuad),
             "render_pass"_a,
             "blend_mode"_a = Shader::BlendMode::None)
        .def("set_texture", &TexturedQuad::set_texture,
             D(TexturedQuad, set_texture),
             "texture"_a)
        .def("set_mvp", &TexturedQuad::set_mvp,
             D(TexturedQuad, set_mvp),
             "mvp"_a)
        .def("set_linear", &TexturedQuad::set_linear,
             D(TexturedQuad, set_linear),
             "linear"_a)
        .def("linear", &TexturedQuad::linear,
             D(TexturedQuad, linear))
        .def("set_depth_from_alpha", &TexturedQuad::set_depth_from_alpha,
             D(TexturedQuad, set_depth_from_alpha),
             "enabled"_a)
        .def("depth_from_alpha", &TexturedQuad::depth_from_alpha,
             D(TexturedQuad, depth_from_alpha))
        .def("set_depth_projection", &TexturedQuad::set_depth_projection,
             D(TexturedQuad, set_depth_projection),
             "projection"_a, "scale"_a = 1.f)
        .def("set_texture_exposure", &TexturedQuad::set_texture_exposure,
             D(TexturedQuad, set_texture_exposure),
             "exposure"_a)
        .def("texture_exposure", &TexturedQuad::texture_exposure,
             D(TexturedQuad, texture_exposure))
        .def("draw", &TexturedQuad::draw,
             D(TexturedQuad, draw));
}

#endif
