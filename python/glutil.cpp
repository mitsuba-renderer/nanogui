#ifdef NANOGUI_PYTHON

#include "python.h"
#include <pybind11/numpy.h>

void register_glutil(py::module &m) {
#if 0
    py::class_<GLShader>(m, "GLShader", D(GLShader))
        .def(py::init<>())
        .def("init", &GLShader::init, "name"_a,
             "vertex_str"_a, "fragment_str"_a,
             "geometry_str"_a = "", D(GLShader, init))
        .def("init_from_files", &GLShader::init_from_files, "name"_a,
             "vertex_fname"_a, "fragment_fname"_a,
             "geometry_fname"_a = "", D(GLShader, init_from_files))
        .def("name", &GLShader::name, D(GLShader, name))
        .def("define", &GLShader::define, "key"_a, "value"_a,
             D(GLShader, define))
        .def("bind", &GLShader::bind, D(GLShader, bind))
        .def("free", &GLShader::free, D(GLShader, free))
        .def("attrib", &GLShader::attrib, "name"_a,
             "warn"_a = true, D(GLShader, attrib))
        .def("uniform", &GLShader::uniform, "name"_a,
             "warn"_a = true, D(GLShader, uniform))
        .def("upload_attrib", &upload_attrib_py, "name"_a,
             "M"_a, "version"_a = -1)
        .def("upload_indices", [](GLShader &sh, py::array M, int version) {
                upload_attrib_py(sh, "indices", M, version);
             }, "M"_a, "version"_a = -1)
        .def("invalidate_attribs", &GLShader::invalidate_attribs,
             D(GLShader, invalidate_attribs))
        .def("free_attrib", &GLShader::free_attrib,
             D(GLShader, free_attrib))
        .def("has_attrib", &GLShader::has_attrib,
             D(GLShader, has_attrib))
        .def("attrib_version", &GLShader::attrib_version,
             D(GLShader, attrib_version))
        .def("reset_attrib_version", &GLShader::reset_attrib_version,
             D(GLShader, reset_attrib_version))
        .def("share_attrib", &GLShader::share_attrib,
             D(GLShader, share_attrib), "other_shader"_a,
             "name"_a, "as"_a = "")
        .def("draw_array", &GLShader::draw_array,
             D(GLShader, draw_array), "type"_a,
             "offset"_a, "count"_a)
        .def("draw_indexed", &GLShader::draw_indexed,
             D(GLShader, draw_indexed), "type"_a,
             "offset"_a, "count"_a)
        .def("set_uniform", &set_uniform_py, "name"_a,
             "value"_a, "warn"_a = true)
        .def("attrib_buffer", &GLShader::attrib_buffer, D(GLShader, attrib_buffer));

    py::class_<GLShader::Buffer>(m, "Buffer", D(GLShader, Buffer))
        .def(py::init<>())
        .def_readonly("id", &GLShader::Buffer::id, D(GLShader, Buffer, id))
        .def_readonly("attrib_id", &GLShader::Buffer::attrib_id, D(GLShader, Buffer, attrib_id))
        .def_readonly("gl_type", &GLShader::Buffer::gl_type, D(GLShader, Buffer, gl_type))
        .def_readonly("dim", &GLShader::Buffer::dim, D(GLShader, Buffer, dim))
        .def_readonly("comp_size", &GLShader::Buffer::comp_size, D(GLShader, Buffer, comp_size))
        .def_readonly("size", &GLShader::Buffer::size, D(GLShader, Buffer, size))
        .def_readonly("version", &GLShader::Buffer::version, D(GLShader, Buffer, version))
        .def_readonly("integral", &GLShader::Buffer::integral, D(GLShader, Buffer, integral));

    py::class_<Arcball>(m, "Arcball", D(Arcball))
        .def(py::init<float>(), "speed_factor"_a = 2.f, D(Arcball, Arcball))
        .def(py::init<const Quaternion4f &>(), D(Arcball, Arcball, 2))
        .def("state", (Quaternion4f& (Arcball::*)()) &Arcball::state, D(Arcball, state))
        .def("set_state", &Arcball::set_state, D(Arcball, set_state))
        .def("size", &Arcball::size, D(Arcball, size))
        .def("set_size", &Arcball::set_size, D(Arcball, set_size))
        .def("speed_factor", &Arcball::speed_factor, D(Arcball, speed_factor))
        .def("set_speed_factor", &Arcball::set_speed_factor, D(Arcball, set_speed_factor))
        .def("active", &Arcball::active, D(Arcball, active))
        .def("button", &Arcball::button, "pos"_a, "pressed"_a, D(Arcball, button))
        .def("motion", &Arcball::motion, "pos"_a, D(Arcball, motion))
        .def("matrix", &Arcball::matrix, D(Arcball, matrix))
        .def("active_state", &Arcball::active_state, D(Arcball, active_state))
        .def("interrupt", &Arcball::interrupt, D(Arcball, interrupt));
#endif
}

#endif
