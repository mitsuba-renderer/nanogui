#ifdef NANOGUI_PYTHON

#include "python.h"
#include <nanobind/operators.h>
#include <nanobind/tensor.h>
#include <nanobind/stl/array.h>
#include <nanobind/stl/pair.h>

#if defined(__clang__)
#  pragma clang diagnostic push
#  pragma clang diagnostic ignored "-Wself-assign-overloaded"
#  pragma clang diagnostic ignored "-Wunused-lambda-capture"
#endif

template <typename Array>
auto register_vector_type(nb::module_ &m, const char *name) {
    constexpr size_t Size = Array::Size;
    using Value = typename Array::Value;

    auto array = nb::class_<Array>(m, name);

    array.def(nb::init<Value>())
         .def(nb::init<const Array &>())
         .def("__init__", [](Array &a, const std::array<Value, Size> &arr) {
            new (&a) Array();
            for (size_t i = 0; i < Array::Size; ++i)
                a[i] = arr[i];
         })
         .def(nb::self == nb::self)
         .def(nb::self != nb::self)
         .def(nb::self + nb::self)
         .def(nb::self - nb::self)
         .def(nb::self * nb::self)
         .def(Value() + nb::self)
         .def(Value() - nb::self)
         .def(Value() * nb::self)
         .def(Value() / nb::self)
         .def(nb::self / nb::self)
         .def(nb::self += nb::self)
         .def(nb::self -= nb::self)
         .def(nb::self *= nb::self)
         .def(nb::self /= nb::self)
         .def("__getitem__", [](const Array &a, size_t index) -> Value {
             if (index >= Array::Size)
                 throw nb::index_error();
             return a[index];
         }, "index"_a)
         .def("__setitem__", [](Array &a, size_t index, Value value) {
             if (index >= Array::Size)
                 throw nb::index_error();
             a[index] = value;
         }, "index"_a, "value"_a)
         .def_property("x", [](const Array &a) { return a.x(); },
                            [](Array &a, const Value &v) { a.x() = v; })
         .def_property("y", [](const Array &a) { return a.y(); },
                            [](Array &a, const Value &v) { a.y() = v; })
        .def("__dlpack__", [](nb::handle_t<Array> self) {
            const Array &a = nb::cast<const Array &>(self);
            const size_t shape[1] = { Array::Size };
            return nb::tensor<float>((void *) a.data(), 1, shape, self);
         })
         .def("__repr__", [](const Array &a) {
             std::ostringstream oss;
             oss << a;
             return oss.str();
         });


    if constexpr (Size == 2)
        array.def(nb::init<Value, Value>());

    if constexpr (Size == 3) {
        array.def(nb::init<Value, Value, Value>());
        array.def_property("z", [](const Array &a) { return a.z(); },
                                [](Array &a, const Value &v) { a.z() = v; });
    }

    nb::detail::implicitly_convertible(
        [](PyTypeObject *, PyObject *src,
           nb::detail::cleanup_list *) noexcept -> bool {
            return PySequence_Check(src) || PyNumber_Check(src);
        },
        &typeid(Array));

    return array;
}

#if defined(__clang__)
#  pragma clang diagnostic pop
#endif

void register_vector(nb::module_ &m) {
    register_vector_type<Vector2i>(m, "Vector2i");
    register_vector_type<Vector2f>(m, "Vector2f");
    register_vector_type<Vector3f>(m, "Vector3f");

    nb::class_<Matrix4f>(m, "Matrix4f")
        .def(nb::init<>())
        .def(nb::init<float>())
        .def_property_readonly("T", &Matrix4f::T)
        .def("__matmul__", [](const Matrix4f &a, const Matrix4f &b) { return a * b; }, nb::is_operator())
        .def("__getitem__",
             [](const Matrix4f &m, std::pair<size_t, size_t> index) -> float {
                 if (index.first >= 4 || index.second >= 4)
                     throw nb::index_error();
                 return m.m[index.second][index.first];
             },
             "index"_a)
        .def("__setitem__",
             [](Matrix4f &m, std::pair<size_t, size_t> index, float value) {
                 if (index.first >= 4 || index.second >= 4)
                     throw nb::index_error();
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
        .def("__dlpack__", [](const Matrix4f &m) {
            Matrix4f *t = new Matrix4f(m.T());
            const size_t shape[2] = { 4, 4 };

            nb::capsule owner(t, [](void *p) noexcept {
               delete (Matrix4f *) p;
            });

            return nb::tensor<float>(&t->m, 2, shape, owner);
         })
        .def("__repr__", [](const Matrix4f &m) {
            std::ostringstream oss;
            oss << m;
            return oss.str();
        });
}

#endif
