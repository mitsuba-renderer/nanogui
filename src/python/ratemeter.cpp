#ifdef NANOGUI_PYTHON

#include "python.h"
#include <nanogui/ratemeter.h>

void register_ratemeter(nb::module_ &m) {
    using RateMeterd = RateMeter<double>;
    nb::class_<RateMeterd>(m, "RateMeter", D(RateMeter))
        .def(nb::init<double>(), "tau"_a = 0.5, D(RateMeter, RateMeter))
        .def("reset", &RateMeterd::reset, D(RateMeter, reset))
        .def("tick", &RateMeterd::tick, "busy"_a = 0.0, D(RateMeter, tick))
        .def("rate", &RateMeterd::rate, D(RateMeter, rate))
        .def("interval", &RateMeterd::interval, D(RateMeter, interval))
        .def("busy", &RateMeterd::busy, D(RateMeter, busy))
        .def("tau", &RateMeterd::tau, D(RateMeter, tau))
        .def("__str__", [](const RateMeterd &m) {
            char buf[64];
            snprintf(buf, sizeof(buf), "%.1f ms (%.1f FPS)",
                     m.interval() * 1e3, m.rate());
            return std::string(buf);
        });
}

#endif
