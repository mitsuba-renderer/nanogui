#ifdef NANOGUI_PYTHON

#include "python.h"
#include <nanogui/camera.h>
#include <nanobind/stl/function.h>
#include <nanobind/stl/tuple.h>

/// Cyclic GC support: the callbacks may close the loop back to their owner
static int camera_tp_traverse(PyObject *self, visitproc visit, void *arg) {
    return widget_tp_traverse_base(self, visit, arg, Py_TYPE(self));
}

static int camera_tp_clear(PyObject *self) {
    CameraController *c = nb::inst_ptr<CameraController>(self);
    c->set_callback(nullptr);
    c->set_click_callback(nullptr);
    return 0;
}

void register_camera(nb::module_ &m) {
    nb::class_<CameraState>(m, "CameraState", D(CameraState))
        .def(nb::init<>())
        .def(nb::init<const Vector3f &, const Vector3f &, const Vector3f &>(),
             "origin"_a, "target"_a, "up"_a)
        .def_rw("origin", &CameraState::origin)
        .def_rw("target", &CameraState::target)
        .def_rw("up", &CameraState::up)
        .def("distance", &CameraState::distance, D(CameraState, distance))
        .def("forward", &CameraState::forward, D(CameraState, forward))
        .def("right", &CameraState::right, D(CameraState, right))
        .def("basis", [](const CameraState &c) {
                 Vector3f forward, right, up;
                 c.basis(forward, right, up);
                 return std::make_tuple(forward, right, up);
             }, D(CameraState, basis))
        .def("view_matrix", &CameraState::view_matrix, D(CameraState, view_matrix))
        .def("__repr__", [](const CameraState &c) {
                 std::ostringstream oss;
                 oss << "CameraState[origin=" << c.origin << ", target="
                     << c.target << ", up=" << c.up << "]";
                 return oss.str();
             });

    PyType_Slot camera_type_slots[] = {
        { Py_tp_traverse, (void *) camera_tp_traverse },
        { Py_tp_clear, (void *) camera_tp_clear },
        { 0, nullptr }
    };

    nb::class_<CameraController>(m, "CameraController", D(CameraController),
                                 nb::type_slots(camera_type_slots))
        .def(nb::init<const CameraState &, const Vector3f &>(),
             D(CameraController, CameraController), "state"_a, "world_up"_a)
        .def("state", &CameraController::state, D(CameraController, state))
        .def("set_state", &CameraController::set_state,
             D(CameraController, set_state), "state"_a)
        .def("set_callback", &CameraController::set_callback,
             D(CameraController, set_callback), "callback"_a.none())
        .def("callback", &CameraController::callback,
             D(CameraController, callback))
        .def("set_click_callback", &CameraController::set_click_callback,
             D(CameraController, set_click_callback), "callback"_a.none())
        .def("click_callback", &CameraController::click_callback,
             D(CameraController, click_callback))
        .def_rw("drag_threshold", &CameraController::drag_threshold,
                D(CameraController, drag_threshold))
        .def("world_up", &CameraController::world_up,
             D(CameraController, world_up))
        .def("set_world_up", &CameraController::set_world_up,
             D(CameraController, set_world_up), "world_up"_a, "snap"_a = false)
        .def_rw("scene_scale", &CameraController::scene_scale,
                D(CameraController, scene_scale))
        .def_rw("orbit_speed", &CameraController::orbit_speed,
                D(CameraController, orbit_speed))
        .def_rw("pan_speed", &CameraController::pan_speed,
                D(CameraController, pan_speed))
        .def_rw("zoom_step", &CameraController::zoom_step,
                D(CameraController, zoom_step))
        .def_rw("fly_speed", &CameraController::fly_speed,
                D(CameraController, fly_speed))
        .def_rw("fly_ramp", &CameraController::fly_ramp,
                D(CameraController, fly_ramp))
        .def("mouse_button_event", &CameraController::mouse_button_event,
             D(CameraController, mouse_button_event),
             "p"_a, "button"_a, "down"_a, "modifiers"_a)
        .def("mouse_motion_event", &CameraController::mouse_motion_event,
             D(CameraController, mouse_motion_event),
             "p"_a, "rel"_a, "button"_a, "modifiers"_a)
        .def("scroll_event", &CameraController::scroll_event,
             D(CameraController, scroll_event), "p"_a, "rel"_a)
        .def("keyboard_event", &CameraController::keyboard_event,
             D(CameraController, keyboard_event),
             "key"_a, "scancode"_a, "action"_a, "modifiers"_a)
        .def("focus_event", &CameraController::focus_event,
             D(CameraController, focus_event), "focused"_a)
        .def("update", &CameraController::update,
             D(CameraController, update))
        .def("animating", &CameraController::animating,
             D(CameraController, animating))
        .def("fly_mode", &CameraController::fly_mode,
             D(CameraController, fly_mode))
        .def("set_fly_mode", &CameraController::set_fly_mode, "value"_a)
        .def("frame", &CameraController::frame,
             D(CameraController, frame), "min"_a, "max"_a, "projection"_a,
             "margin"_a = 1.2f, "duration"_a = 0.7f);
}

#endif
