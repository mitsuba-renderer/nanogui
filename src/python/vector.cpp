#ifdef NANOGUI_PYTHON

#include "python.h"
#include <nanobind/operators.h>
#include <nanobind/ndarray.h>
#include <nanobind/stl/array.h>
#include <nanobind/stl/pair.h>

#if defined(__clang__)
#  pragma clang diagnostic push
#  pragma clang diagnostic ignored "-Wself-assign-overloaded"
#  pragma clang diagnostic ignored "-Wunused-lambda-capture"
#endif

template <typename Vector>
auto register_vector_type(nb::module_ &m, const char *name) {
    constexpr size_t Size = Vector::Size;
    using Value = typename Vector::Value;

    auto type = nb::class_<Vector>(m, name);

    type.def(nb::init<Value>())
        .def(nb::init<const Vector &>())
        .def("__init__", [](Vector *v, const nb::sequence &arr) {
           new (v) Vector(0);
           for (size_t i = 0, size = std::min(Vector::Size, nb::len(arr)); i < size; ++i)
               (*v)[i] = nb::cast<Value>(arr[i]);
        })
        .def("__len__", [](const Vector &) { return Size; })
        .def(-nb::self)
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
        .def("__getitem__", [](const Vector &a, size_t index) -> Value {
            if (index >= Vector::Size)
                throw nb::index_error();
            return a[index];
         }, "index"_a)
         .def("__setitem__", [](Vector &a, size_t index, Value value) {
             if (index >= Vector::Size)
                 throw nb::index_error();
             a[index] = value;
         }, "index"_a, "value"_a)
         .def_prop_rw("x", [](const Vector &a) { return a.x(); },
                           [](Vector &a, const Value &v) { a.x() = v; })
         .def_prop_rw("y", [](const Vector &a) { return a.y(); },
                           [](Vector &a, const Value &v) { a.y() = v; })
         .def("__dlpack__", [](nb::handle_t<Vector> self) {
             const Vector &a = nb::cast<const Vector &>(self);
             const size_t shape[1] = { Vector::Size };
             return nb::ndarray<float>((void *) a.data(), 1, shape, self);
          })
          .def("__repr__", [](const Vector &a) {
              std::ostringstream oss;
              oss << a;
              return oss.str();
          });


    if constexpr (Size == 2)
        type.def(nb::init<Value, Value>());

    if constexpr (Size == 3) {
        type.def(nb::init<Value, Value, Value>());
        type.def_prop_rw("z", [](const Vector &a) { return a.z(); },
                              [](Vector &a, const Value &v) { a.z() = v; });
    }

    if constexpr (Size == 4) {
        type.def(nb::init<Value, Value, Value, Value>());
        type.def_prop_rw("z", [](const Vector &a) { return a.z(); },
                              [](Vector &a, const Value &v) { a.z() = v; });
        type.def_prop_rw("w", [](const Vector &a) { return a.w(); },
                              [](Vector &a, const Value &v) { a.w() = v; });
    }

    nb::detail::implicitly_convertible(
        [](PyTypeObject *, PyObject *src,
           nb::detail::cleanup_list *) noexcept -> bool {
            return PySequence_Check(src) || PyNumber_Check(src);
        },
        &typeid(Vector));

    return type;
}

template <typename Matrix>
auto register_matrix_type(nb::module_ &m, const char *name) {
    using Vector = typename Matrix::Column;
    using Value = typename Matrix::Value;
    constexpr size_t Size = Matrix::Size;

    auto type = nb::class_<Matrix>(m, name)
        .def(nb::init<>())
        .def(nb::init<float>())
        .def(nb::init<const Matrix &>());

    type.def("__init__",
             [](Matrix *m, const nb::ndarray<Value, nb::f_contig, nb::shape<Size, Size>, nb::ro, nb::device::cpu> &array) {
                new (m) Matrix();
                memcpy(m->m, array.data(), sizeof(Value)*Size*Size);
             })
        .def("__init__",
             [](Matrix *m, const nb::ndarray<Value, nb::c_contig, nb::shape<Size, Size>, nb::ro, nb::device::cpu> &array) {
                new (m) Matrix();
                memcpy(m->m, array.data(), sizeof(Value)*Size*Size);
                *m = m->T();
             })
        .def_prop_ro("T", &Matrix::T)
        .def("__matmul__", [](const Matrix &a, const Matrix &b) { return a * b; }, nb::is_operator())
        .def("__len__", [](const Matrix &) { return Size; })
        .def("__getitem__", [](const Matrix &m, size_t index) -> const Vector& {
            if (index >= Vector::Size)
                throw nb::index_error();
            return m.m[index];
        })
        .def("__setitem__", [](Matrix &m, size_t index, const Vector &value) {
            if (index >= Vector::Size)
                throw nb::index_error();
            m.m[index] = value;
        })
        .def("__dlpack__", [](const Matrix *self) {
            Matrix *t = new Matrix(self->T());

            nb::capsule owner(t, [](void *p) noexcept {
                delete (Matrix *) p;
            });

            return nb::ndarray<float>(&t->m, {Size, Size}, owner);
         })
        .def("__repr__", [](const Matrix &m) {
            std::ostringstream oss;
            oss << m;
            return oss.str();
        })
        .def_static("scale", nb::overload_cast<const Vector3f &>(&Matrix::scale))
        .def_static("rotate",
                    [](const Vector3f &axis, float angle) {
                        return Matrix::rotate(axis, angle);
                    },
                    "axis"_a, "angle"_a);
    return type;
}

#if defined(__clang__)
#  pragma clang diagnostic pop
#endif

void register_vector(nb::module_ &m) {
    register_vector_type<Vector2i>(m, "Vector2i");
    register_vector_type<Vector2f>(m, "Vector2f");
    register_vector_type<Vector3f>(m, "Vector3f");
    register_vector_type<Vector4f>(m, "Vector4f");

    register_matrix_type<Matrix3f>(m, "Matrix3f");
    register_matrix_type<Matrix4f>(m, "Matrix4f")
        .def_static("translate", &Matrix4f::translate)
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
            "origin"_a, "target"_a, "up"_a);
}

#endif
