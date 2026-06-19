#ifdef NANOGUI_PYTHON

#include "python.h"
#include <nanogui/ema.h>

void register_ema(nb::module_ &m) {
    using EMAf = EMA<float>;
    nb::class_<EMAf>(m, "EMA", D(EMA))
        .def(nb::init<float>(), "weight"_a = 0.983f, D(EMA, EMA))
        .def("reset", &EMAf::reset, D(EMA, reset))
        .def("put", &EMAf::put, "sample"_a, D(EMA, put))
        .def("value", &EMAf::value, D(EMA, value))
        .def("weight", &EMAf::weight, D(EMA, weight))
        .def("sample_count", &EMAf::sample_count, D(EMA, sample_count));
}

#endif