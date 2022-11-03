#ifdef NANOGUI_PYTHON

#include "python.h"
#include <nanobind/make_iterator.h>

DECLARE_WIDGET(Widget);
DECLARE_SCREEN(Screen);
DECLARE_WIDGET(Window);

/// Cyclic GC support: find all callback getter functions and traverse them recursively
int widget_tp_traverse_base(PyObject *self, visitproc visit, void *arg, PyTypeObject *tp) {
    PyObject *dict = tp->tp_dict;
    PyObject *key, *value;
    Py_ssize_t pos = 0;

    const char *suffix = "callback";
    size_t suffix_len = strlen(suffix);

    while (PyDict_Next(dict, &pos, &key, &value)) {
        if (!PyUnicode_Check(key))
            continue;

        const char *name = PyUnicode_AsUTF8AndSize(key, nullptr);
        size_t name_len = strlen(name);

        if (suffix_len > name_len)
            continue;

        if (strncmp(name, "set_", 4) == 0 ||
            strcmp(name + name_len - suffix_len, suffix) != 0)
            continue;


#if PY_VERSION_HEX < 0x03090000
        PyObject *func = PyObject_GetAttr(self, key),
                 *result = nullptr;
        if (func) {
            result = _PyObject_Vectorcall(func, nullptr, 0, nullptr);
            Py_DECREF(func);
        }
#else
        PyObject *args[] = { self };
        PyObject *result = PyObject_VectorcallMethod(
            key, args, 1 | PY_VECTORCALL_ARGUMENTS_OFFSET, nullptr);
#endif

        if (!result) {
            PyErr_Clear();
            continue;
        }

        Py_VISIT(result);
        Py_DECREF(result);
    }

    if (strcmp(tp->tp_name, "nanogui.Widget") != 0) {
        PyTypeObject *base = (PyTypeObject *) PyType_GetSlot(tp, Py_tp_base);
        if (base) {
            int rv = widget_tp_traverse_base(self, visit, arg, base);
            if (rv)
                return rv;
        }
    }

    return 0;
}

// Main widget traversal method that may be invoked by Python's cyclic GC
int widget_tp_traverse(PyObject *self, visitproc visit, void *arg) {

    Widget *w = nb::inst_ptr<Widget>(self);

    // Visit nested children
    for (Widget *wc : w->children()) {
        PyObject *o = wc->self_py();
        Py_VISIT(o);
    }

    // Visit callback functions (which can also produce reference cycles)
    return widget_tp_traverse_base(self, visit, arg, Py_TYPE(self));
}

int widget_tp_clear(PyObject *self) {
    Widget *w = nb::inst_ptr<Widget>(self);

    int count = w->child_count();
    for (int i = 0; i < count; ++i)
        w->remove_child_at(count - 1 - i);

    return 0;
}

void register_widget(nb::module_ &m) {
    object_init_py(
        [](PyObject *o) noexcept {
            nb::gil_scoped_acquire guard;
            Py_INCREF(o);
        },
        [](PyObject *o) noexcept {
            nb::gil_scoped_acquire guard;
            Py_DECREF(o);
        });

    nb::class_<Object>(
        m, "Object",
        nb::intrusive_ptr<Object>(
            [](Object *o, PyObject *po) noexcept { o->set_self_py(po); }));

    PyType_Slot widget_type_slots[] = {
        { Py_tp_traverse, (void *) widget_tp_traverse },
        { Py_tp_clear, (void *) widget_tp_clear },
        { 0, nullptr }
    };

    nb::class_<Widget, Object, PyWidget>(
        m, "Widget", D(Widget), nb::type_slots(widget_type_slots))
        .def(nb::init<Widget *>(), D(Widget, Widget))
        .def("parent", (Widget *(Widget::*)(void)) &Widget::parent, D(Widget, parent))
        .def(nb_method(Widget, set_parent), D(Widget, set_parent))
        .def("layout", (Layout *(Widget::*)(void)) &Widget::layout, D(Widget, layout))
        .def(nb_method(Widget, set_layout), D(Widget, set_layout))
        .def("theme", (Theme *(Widget::*)(void)) &Widget::theme, D(Widget, theme))
        .def(nb_method(Widget, set_theme), D(Widget, set_theme))
        .def(nb_method(Widget, position), D(Widget, position))
        .def(nb_method(Widget, set_position), D(Widget, set_position))
        .def(nb_method(Widget, absolute_position), D(Widget, absolute_position))
        .def(nb_method(Widget, size), D(Widget, size))
        .def(nb_method(Widget, set_size), D(Widget, set_size))
        .def(nb_method(Widget, width), D(Widget, width))
        .def(nb_method(Widget, set_width), D(Widget, set_width))
        .def(nb_method(Widget, height), D(Widget, height))
        .def(nb_method(Widget, set_height), D(Widget, set_height))
        .def(nb_method(Widget, fixed_size), D(Widget, fixed_size))
        .def(nb_method(Widget, set_fixed_size), D(Widget, set_fixed_size))
        .def(nb_method(Widget, fixed_width), D(Widget, fixed_width))
        .def(nb_method(Widget, set_fixed_width), D(Widget, set_fixed_width))
        .def(nb_method(Widget, fixed_height), D(Widget, fixed_height))
        .def(nb_method(Widget, set_fixed_height), D(Widget, set_fixed_height))
        .def(nb_method(Widget, visible), D(Widget, visible))
        .def(nb_method(Widget, set_visible), D(Widget, set_visible))
        .def(nb_method(Widget, visible_recursive), D(Widget, visible_recursive))
        .def("children", (const std::vector<Widget *>&(Widget::*)(void)) &Widget::children,
             D(Widget, children), nb::rv_policy::reference)
        .def("add_child", (void (Widget::*) (int, Widget *)) &Widget::add_child, D(Widget, add_child))
        .def("add_child", (void (Widget::*) (Widget *)) &Widget::add_child, D(Widget, add_child, 2))
        .def(nb_method(Widget, child_count), D(Widget, child_count))
        .def("__len__", &Widget::child_count, D(Widget, child_count))
        .def("__iter__", [](const Widget &w) {
                return nb::make_iterator(nb::type<Widget>(), "iterator", w.children().begin(), w.children().end());
            }, nb::keep_alive<0, 1>())
        .def(nb_method(Widget, child_index), D(Widget, child_index))
        .def("__getitem__", (Widget* (Widget::*)(int)) &Widget::child_at, D(Widget, child_at))
        .def(nb_method(Widget, remove_child_at), D(Widget, remove_child_at))
        .def(nb_method(Widget, remove_child), D(Widget, remove_child))
        .def("__delitem__", &Widget::remove_child_at, D(Widget, remove_child_at))
        .def(nb_method(Widget, enabled), D(Widget, enabled))
        .def(nb_method(Widget, set_enabled), D(Widget, set_enabled))
        .def(nb_method(Widget, focused), D(Widget, focused))
        .def(nb_method(Widget, set_focused), D(Widget, set_focused))
        .def(nb_method(Widget, request_focus), D(Widget, request_focus))
        .def(nb_method(Widget, tooltip), D(Widget, tooltip))
        .def(nb_method(Widget, set_tooltip), D(Widget, set_tooltip))
        .def(nb_method(Widget, font_size), D(Widget, font_size))
        .def(nb_method(Widget, set_font_size), D(Widget, set_font_size))
        .def(nb_method(Widget, has_font_size), D(Widget, has_font_size))
        .def(nb_method(Widget, cursor), D(Widget, cursor))
        .def(nb_method(Widget, set_cursor), D(Widget, set_cursor))
        .def("find_widget", (Widget *(Widget::*)(const Vector2i &)) &Widget::find_widget, D(Widget, find_widget))
        .def(nb_method(Widget, contains), D(Widget, contains))
        .def(nb_method(Widget, mouse_button_event), "p"_a, "button"_a,
             "down"_a, "modifiers"_a, D(Widget, mouse_button_event))
        .def(nb_method(Widget, mouse_motion_event), "p"_a, "rel"_a,
             "button"_a, "modifiers"_a, D(Widget, mouse_motion_event))
        .def(nb_method(Widget, mouse_drag_event), "p"_a, "rel"_a,
             "button"_a, "modifiers"_a, D(Widget, mouse_drag_event))
        .def(nb_method(Widget, mouse_enter_event), "p"_a, "enter"_a,
             D(Widget, mouse_enter_event))
        .def(nb_method(Widget, scroll_event), "p"_a, "rel"_a,
             D(Widget, scroll_event))
        .def(nb_method(Widget, focus_event), "focused"_a, D(Widget, focus_event))
        .def(nb_method(Widget, keyboard_event), "key"_a, "scancode"_a,
             "action"_a, "modifiers"_a, D(Widget, keyboard_event))
        .def(nb_method(Widget, keyboard_character_event),
             D(Widget, keyboard_character_event))
        .def(nb_method(Widget, preferred_size), D(Widget, preferred_size))
        .def(nb_method(Widget, perform_layout), D(Widget, perform_layout))
        .def("screen", nb::overload_cast<>(&Widget::screen, nb::const_), D(Widget, screen))
        .def("window", nb::overload_cast<>(&Widget::window, nb::const_), D(Widget, window))
        .def(nb_method(Widget, draw), D(Widget, draw));

    nb::class_<Window, Widget, PyWindow>(m, "Window", D(Window))
        .def(nb::init<Widget *, const std::string>(), "parent"_a,
             "title"_a = std::string("Untitled"), D(Window, Window))
        .def(nb_method(Window, title), D(Window, title))
        .def(nb_method(Window, set_title), D(Window, set_title))
        .def(nb_method(Window, modal), D(Window, modal))
        .def(nb_method(Window, set_modal), D(Window, set_modal))
        .def(nb_method(Window, dispose), D(Window, dispose))
        .def(nb_method(Window, button_panel), D(Window, button_panel))
        .def(nb_method(Window, center), D(Window, center));

    nb::class_<Screen, Widget, PyScreen>(m, "Screen", D(Screen))
        .def(nb::init<const Vector2i &, const std::string &, bool, bool, bool,
                      bool, bool, unsigned int, unsigned int>(),
            "size"_a, "caption"_a = "Unnamed", "resizable"_a = true, "fullscreen"_a = false,
            "depth_buffer"_a = true, "stencil_buffer"_a = true,
            "float_buffer"_a = false, "gl_major"_a = 3, "gl_minor"_a = 2, D(Screen, Screen))
        .def(nb_method(Screen, caption), D(Screen, caption))
        .def(nb_method(Screen, set_caption), D(Screen, set_caption))
        .def(nb_method(Screen, background), D(Screen, background))
        .def(nb_method(Screen, set_background), D(Screen, set_background))
        .def(nb_method(Screen, set_visible), D(Screen, set_visible))
        .def(nb_method(Screen, set_size), D(Screen, set_size))
        .def(nb_method(Screen, framebuffer_size), D(Screen, framebuffer_size))
        .def("perform_layout", (void(Screen::*)(void)) &Screen::perform_layout, D(Screen, perform_layout))
        .def(nb_method(Screen, redraw), D(Screen, redraw))
        .def(nb_method(Screen, clear), D(Screen, clear))
        .def(nb_method(Screen, draw_all), D(Screen, draw_all))
        .def(nb_method(Screen, draw_contents), D(Screen, draw_contents))
        .def(nb_method(Screen, resize_event), "size"_a, D(Screen, resize_event))
        .def(nb_method(Screen, resize_callback))
        .def(nb_method(Screen, set_resize_callback))
        .def(nb_method(Screen, drop_event), D(Screen, drop_event))
        .def(nb_method(Screen, mouse_pos), D(Screen, mouse_pos))
        .def(nb_method(Screen, pixel_ratio), D(Screen, pixel_ratio))
        .def(nb_method(Screen, has_depth_buffer), D(Screen, has_depth_buffer))
        .def(nb_method(Screen, has_stencil_buffer), D(Screen, has_stencil_buffer))
        .def(nb_method(Screen, has_float_buffer), D(Screen, has_float_buffer))
        .def(nb_method(Screen, glfw_window), D(Screen, glfw_window),
                nb::rv_policy::reference)
        .def(nb_method(Screen, nvg_context), D(Screen, nvg_context),
                nb::rv_policy::reference)
        .def(nb_method(Screen, pixel_format), D(Screen, pixel_format))
        .def(nb_method(Screen, component_format), D(Screen, component_format))
        .def(nb_method(Screen, nvg_flush), D(Screen, nvg_flush))
#if defined(NANOGUI_USE_METAL)
        .def(nb_method(Screen, metal_layer))
        .def(nb_method(Screen, metal_texture))
        .def(nb_method(Screen, depth_stencil_texture))
#endif
        ;
}
#endif
