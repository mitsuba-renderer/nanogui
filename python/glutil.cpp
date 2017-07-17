#ifdef NANOGUI_PYTHON

#include "python.h"
#include <pybind11/numpy.h>

static void upload_attrib_py(GLShader &sh, const std::string &name, py::array M, int version) {
    if (M.ndim() != 2)
        throw py::type_error("upload_attrib(): expects 2D array");

    M = py::array::ensure(M, py::array::f_style);

    py::dtype dtype = M.dtype();
    GLuint gl_type;
    bool integral = true;

    if (dtype.kind() == 'i') {
        switch (dtype.itemsize()) {
            case 1: gl_type = GL_BYTE; break;
            case 2: gl_type = GL_SHORT; break;
            case 4: gl_type = GL_INT; break;
            default: throw py::type_error("upload_attrib(): Invalid integer type!");
        }
    } else if (dtype.kind() == 'u') {
        switch (dtype.itemsize()) {
            case 1: gl_type = GL_UNSIGNED_BYTE; break;
            case 2: gl_type = GL_UNSIGNED_SHORT; break;
            case 4: gl_type = GL_UNSIGNED_INT; break;
            default: throw py::type_error("upload_attrib(): Invalid unsigned integer type!");
        }
    } else if (dtype.kind() == 'f') {
        switch (dtype.itemsize()) {
            case 2: gl_type = GL_HALF_FLOAT; break;
            case 4: gl_type = GL_FLOAT; break;
            case 8: gl_type = GL_DOUBLE; break;
            default: throw py::type_error("upload_attrib(): Invalid floating point type!");
        }
        integral = false;
    } else {
        throw py::type_error("upload_attrib(): Invalid type!");
    }

    sh.upload_attrib(name, (size_t) M.shape(0), (size_t) M.shape(1),
                     M.itemsize(), gl_type, integral, M.data(), version);
}

static void set_uniform_py(GLShader &sh, const std::string &name, py::object arg, bool warn = true) {
    GLuint id = sh.uniform(name, warn);
    py::array value_ = py::array::ensure(arg);
    auto dtype = value_.dtype();
    if (dtype.kind() == 'f') {
        auto value = py::array_t<float, py::array::forcecast | py::array::c_style>(value_);
        if (value.ndim() == 0 || (value.ndim() == 1 && value.shape(0) == 1))
            glUniform1fv(id, 1, value.data());
        else if (value.ndim() == 1 && value.shape(0) == 2)
            glUniform2fv(id, 1, value.data());
        else if (value.ndim() == 1 && value.shape(0) == 3)
            glUniform3fv(id, 1, value.data());
        else if (value.ndim() == 1 && value.shape(0) == 4)
            glUniform4fv(id, 1, value.data());
        else if (value.ndim() == 2 && value.shape(0) == 3 && value.shape(1) == 3)
            glUniformMatrix3fv(id, 1, GL_TRUE, value.data());
        else if (value.ndim() == 2 && value.shape(0) == 4 && value.shape(1) == 4)
            glUniformMatrix4fv(id, 1, GL_TRUE, value.data());
        else
            throw py::type_error("set_uniform(): invalid dimension/size!");
    } else if (dtype.kind() == 'i') {
        auto value = py::array_t<int, py::array::forcecast>(value_);

        if (value.ndim() == 0 || (value.ndim() == 1 && value.shape(0) == 1))
            glUniform1iv(id, 1, value.data());
        else if (value.ndim() == 1 && value.shape(0) == 2)
            glUniform2iv(id, 1, value.data());
        else if (value.ndim() == 1 && value.shape(0) == 3)
            glUniform3iv(id, 1, value.data());
        else if (value.ndim() == 1 && value.shape(0) == 4)
            glUniform4iv(id, 1, value.data());
        else
            throw py::type_error("set_uniform(): invalid dimension/size!");
    } else if (dtype.kind() == 'u') {
        auto value = py::array_t<unsigned int, py::array::forcecast>(value_);

        if (value.ndim() == 0 || (value.ndim() == 1 && value.shape(0) == 1))
            glUniform1uiv(id, 1, value.data());
        else if (value.ndim() == 1 && value.shape(0) == 2)
            glUniform2uiv(id, 1, value.data());
        else if (value.ndim() == 1 && value.shape(0) == 3)
            glUniform3uiv(id, 1, value.data());
        else if (value.ndim() == 1 && value.shape(0) == 4)
            glUniform4uiv(id, 1, value.data());
        else
            throw py::type_error("set_uniform(): invalid dimension/size!");
    }
}

void register_glutil(py::module &m) {
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
             "value"_a, "warn"_a = true);

    py::class_<Arcball>(m, "Arcball", D(Arcball))
        .def(py::init<float>(), "speed_factor"_a = 2.f, D(Arcball, Arcball))
        .def(py::init<const Quaternion4f &>(), D(Arcball, Arcball, 2))
        .def("state", &Arcball::state, D(Arcball, state))
        .def("set_state", &Arcball::set_state, D(Arcball, set_state))
        .def("size", &Arcball::size, D(Arcball, size))
        .def("set_size", &Arcball::set_size, D(Arcball, set_size))
        .def("speed_factor", &Arcball::speed_factor, D(Arcball, speed_factor))
        .def("set_speed_factor", &Arcball::set_speed_factor, D(Arcball, set_speed_factor))
        .def("active", &Arcball::active, D(Arcball, active))
        .def("button", &Arcball::button, "pos"_a, "pressed"_a, D(Arcball, button))
        .def("motion", &Arcball::motion, "pos"_a, D(Arcball, motion))
        .def("matrix", &Arcball::matrix, D(Arcball, matrix));

    m.def("translate", &enoki::translate<Matrix4f, Vector3f>, "v"_a);
    m.def("scale", &enoki::scale<Matrix4f, Vector3f>, "v"_a);
    m.def("rotate", &enoki::rotate<Matrix4f, Vector3f>, "axis"_a, "angle"_a);
    m.def("perspective", &enoki::perspective<Matrix4f>, "fov"_a, "near"_a, "far"_a);
    m.def("frustum", &enoki::frustum<Matrix4f>, "left"_a, "right"_a,
          "bottom"_a, "top"_a, "near"_a, "far"_a);
    m.def("frustum", &enoki::frustum<Matrix4f>, "left"_a, "right"_a,
          "bottom"_a, "top"_a, "near"_a, "far"_a);
    m.def("ortho", &enoki::ortho<Matrix4f>, "left"_a, "right"_a,
          "bottom"_a, "top"_a, "near"_a, "far"_a);
    m.def("look_at", &enoki::look_at<Matrix4f, Vector3f, Vector3f>, "origin"_a,
          "target"_a, "up"_a);

    #define C(name) gl.attr(#name) = py::int_(GL_##name);
    py::module gl = m.def_submodule("gl", "OpenGL bindings");

    gl.def("Enable", [](GLenum cap) { glEnable(cap); }, "cap"_a);
    gl.def("Disable", [](GLenum cap) { glDisable(cap); }, "cap"_a);
    gl.def("BlendFunc", [](GLenum sfactor,
                           GLenum dfactor) { glBlendFunc(sfactor, dfactor); },
           "sfactor"_a, "dfactor"_a);
    gl.def("Scissor", [](GLint x, GLint y, GLsizei w, GLsizei h) { glScissor(x, y, w, h); });
    gl.def("Cull", [](GLenum mode) { glCullFace(mode); });
    gl.def("PointSize", [](GLfloat size) { glPointSize(size); });
    gl.def("LineWidth", [](GLfloat size) { glLineWidth(size); });

    /* Primitive types */
    C(POINTS); C(LINE_STRIP); C(LINE_LOOP); C(LINES); C(LINE_STRIP_ADJACENCY);
    C(LINES_ADJACENCY); C(TRIANGLE_STRIP); C(TRIANGLE_FAN); C(TRIANGLES);
    C(TRIANGLE_STRIP_ADJACENCY); C(TRIANGLES_ADJACENCY);

    /* Depth testing */
    C(DEPTH_TEST); C(NEVER); C(LESS); C(EQUAL); C(LEQUAL); C(GREATER);
    C(NOTEQUAL); C(GEQUAL); C(ALWAYS);

    /* Blend functions */
    C(BLEND); C(ZERO); C(ONE); C(SRC_COLOR); C(DST_COLOR);
    C(ONE_MINUS_DST_COLOR); C(SRC_ALPHA); C(ONE_MINUS_SRC_ALPHA);
    C(DST_ALPHA); C(ONE_MINUS_DST_ALPHA); C(CONSTANT_COLOR);
    C(ONE_MINUS_CONSTANT_COLOR); C(CONSTANT_ALPHA);
    C(ONE_MINUS_CONSTANT_ALPHA);

    /* Culling functions */
    C(FRONT); C(BACK); C(FRONT_AND_BACK);

    /* Remaining gl_enable/glDisable enums */
    C(SCISSOR_TEST); C(STENCIL_TEST); C(PROGRAM_POINT_SIZE);
    C(LINE_SMOOTH); C(POLYGON_SMOOTH); C(CULL_FACE);
}

#endif
