#pragma once

#if defined(_MSC_VER)
#  pragma warning (disable:5033) // 'register' is no longer a supported storage class
#endif

#include <nanogui/nanogui.h>
#include <nanogui/opengl.h>

#include <nanobind/stl/string.h>
#include <nanobind/stl/vector.h>
#include <nanobind/stl/function.h>
#include <nanogui/python.h>
#include "py_doc.h"

#define D(...) DOC(nanogui, __VA_ARGS__ )

#define DECLARE_LAYOUT(Name) \
    class Py##Name : public Name { \
    public: \
        using Name::Name; \
        NANOGUI_LAYOUT_OVERLOADS(Name); \
    }

#define DECLARE_WIDGET(Name) \
    class Py##Name : public Name { \
    public: \
        using Name::Name; \
        NANOGUI_WIDGET_OVERLOADS(Name); \
    }

#define DECLARE_SCREEN(Name) \
    class Py##Name : public Name { \
    public: \
        using Name::Name; \
        NANOGUI_WIDGET_OVERLOADS(Name); \
        NANOGUI_SCREEN_OVERLOADS(Name); \
    }

using namespace nanogui;

namespace nb = nanobind;
using namespace nanobind::literals;

extern int widget_tp_traverse(PyObject *, visitproc, void *);
extern int widget_tp_clear(PyObject *);
