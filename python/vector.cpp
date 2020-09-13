#ifdef NANOGUI_PYTHON

#include "python.h"
#include <pybind11/operators.h>

#if defined(__clang__)
#  pragma clang diagnostic push
#  pragma clang diagnostic ignored "-Wself-assign-overloaded"
#  pragma clang diagnostic ignored "-Wunused-lambda-capture"
#endif

template <typename Array>
auto register_vector_type(py::module &m, const char *name) {
    constexpr size_t Size = Array::Size;
    using Value = typename Array::Value;

    auto array = py::class_<Array>(m, name, py::buffer_protocol());

    array.def(py::init<Value>())
         .def(py::init<const Array &>())
         .def(py::init([Size](const std::array<Value, Size> &arr) {
            Array a;
            for (size_t i = 0; i < Size; ++i)
                a[i] = arr[i];
            return a;
         }))
         .def(py::self == py::self)
         .def(py::self != py::self)
         .def(py::self + py::self)
         .def(py::self - py::self)
         .def(py::self * py::self)
         .def(Value() + py::self)
         .def(Value() - py::self)
         .def(Value() * py::self)
         .def(Value() / py::self)
         .def(py::self / py::self)
         .def(py::self += py::self)
         .def(py::self -= py::self)
         .def(py::self *= py::self)
         .def(py::self /= py::self)
         .def("__getitem__", [Size](const Array &a, size_t index) -> Value {
             if (index >= Size)
                 throw py::index_error();
             return a[index];
         }, "index"_a)
         .def("__setitem__", [Size](Array &a, size_t index, Value value) {
             if (index >= Size)
                 throw py::index_error();
             a[index] = value;
         }, "index"_a, "value"_a)
         .def_property("x", [](const Array &a) { return a.x(); },
                            [](Array &a, const Value &v) { a.x() = v; })
         .def_property("y", [](const Array &a) { return a.y(); },
                            [](Array &a, const Value &v) { a.y() = v; })
         .def_buffer([Size](Array &m) -> py::buffer_info {
              return py::buffer_info(
                  m.v,                                    // Pointer to buffer
                  sizeof(Value),                          // Size of one scalar
                  py::format_descriptor<Value>::format(), // Python struct-style format descriptor
                  1,                                      // Number of dimensions
                  { Size },                               // Buffer dimensions
                  { sizeof(Value) }
              );
         })
         .def("__repr__", [](const Array &a) {
             std::ostringstream oss;
             oss << a;
             return oss.str();
         });


    if constexpr (Size == 2)
        array.def(py::init<Value, Value>());

    if constexpr (Size == 3) {
        array.def(py::init<Value, Value, Value>());
        array.def_property("z", [](const Array &a) { return a.z(); },
                                [](Array &a, const Value &v) { a.z() = v; });
    }

    py::implicitly_convertible<py::sequence, Array>();
    py::implicitly_convertible<Value, Array>();
    return array;
}

#if defined(__clang__)
#  pragma clang diagnostic pop
#endif

void register_vector(py::module &m) {
    register_vector_type<Vector2i>(m, "Vector2i");
    register_vector_type<Vector2f>(m, "Vector2f");
    register_vector_type<Vector3f>(m, "Vector3f");

    py::class_<Matrix4f>(m, "Matrix4f", py::buffer_protocol())
        .def(py::init<>())
        .def(py::init<float>())
        .def("__matmul__", [](const Matrix4f &a, const Matrix4f &b) { return a * b; }, py::is_operator())
        .def("__getitem__",
             [](const Matrix4f &m, std::pair<size_t, size_t> index) -> float {
                 if (index.first >= 4 || index.second >= 4)
                     throw py::index_error();
                 return m.m[index.second][index.first];
             },
             "index"_a)
        .def("__setitem__",
             [](Matrix4f &m, std::pair<size_t, size_t> index, float value) {
                 if (index.first >= 4 || index.second >= 4)
                     throw py::index_error();
                 m.m[index.second][index.first] = value;
             },
             "index"_a, "value"_a)
        .def_static(
            "translate",
            [](const Vector3f &amount) { return Matrix4f::translate(amount); },
            "amount"_a)
        .def_static(
            "scale",
            [](const Vector3f &amount) { return Matrix4f::scale(amount); },
            "amount"_a)
        .def_static(
            "rotate",
            [](const Vector3f &axis, float angle) {
                return Matrix4f::rotate(axis, angle);
            },
            "axis"_a, "angle"_a)
        .def_static(
            "perspective",
            [](float fov, float near_, float far_, float aspect) {
                return Matrix4f::perspective(fov, near_, far_, aspect);
            },
            "fov"_a, "near"_a, "far"_a, "aspect"_a = 1.f)
        .def_static(
            "ortho",
            [](float left, float right, float bottom, float top, float near_, float far_) {
                return Matrix4f::ortho(left, right, bottom, top, near_, far_);
            },
            "left"_a, "right"_a, "bottom"_a, "top"_a, "near"_a, "far"_a)
        .def_static(
            "look_at",
            [](const Vector3f &origin, const Vector3f &target, const Vector3f &up) {
                return Matrix4f::look_at(origin, target, up);
            },
            "origin"_a, "target"_a, "up"_a)
        .def_buffer([](Matrix4f &m) -> py::buffer_info {
            return py::buffer_info(
                m.m,                                    // Pointer to buffer
                sizeof(float),                          // Size of one scalar
                py::format_descriptor<float>::format(), // Python struct-style
                                                        // format descriptor
                2,                                      // Number of dimensions
                { 4, 4 },                               // Buffer dimensions
                { sizeof(float), sizeof(float) * 4 });
        })
        .def("__repr__", [](const Matrix4f &m) {
            std::ostringstream oss;
            oss << m;
            return oss.str();
        });
}

#endif
