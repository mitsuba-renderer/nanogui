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
        .def("set_texture_linear", &TexturedQuad::set_texture_linear,
             D(TexturedQuad, set_texture_linear),
             "linear"_a)
        .def("texture_linear", &TexturedQuad::texture_linear,
             D(TexturedQuad, texture_linear))
        .def("set_texture_exposure", &TexturedQuad::set_texture_exposure,
             D(TexturedQuad, set_texture_exposure),
             "exposure"_a)
        .def("texture_exposure", &TexturedQuad::texture_exposure,
             D(TexturedQuad, texture_exposure))
        .def("draw", &TexturedQuad::draw,
             D(TexturedQuad, draw));
}

#endif
