#ifdef NANOGUI_PYTHON

#include "python.h"
#include <nanogui/chroma.h>
#include <nanobind/stl/array.h>

using namespace nanogui::ituth273;

void register_chroma(nb::module_ &m_) {
    auto m = m_.def_submodule("ituth273");

    nb::enum_<ColorPrimaries>(m, "ColorPrimaries")
        .value("BT709", ColorPrimaries::BT709)
        .value("Unspecified", ColorPrimaries::Unspecified)
        .value("BT470M", ColorPrimaries::BT470M)
        .value("BT470BG", ColorPrimaries::BT470BG)
        .value("SMTPE170M", ColorPrimaries::SMPTE170M)
        .value("SMTP240M", ColorPrimaries::SMPTE240M)
        .value("Film", ColorPrimaries::Film)
        .value("BT2020", ColorPrimaries::BT2020)
        .value("SMTPE428", ColorPrimaries::SMPTE428)
        .value("SMTPE431", ColorPrimaries::SMPTE431)
        .value("SMTPE432", ColorPrimaries::SMPTE432)
        .value("Weird", ColorPrimaries::Weird);


    m.def("chroma_to_rec709_matrix", &chroma_to_rec709_matrix);
    m.def("chroma", &chroma);
    m.def("from_screen", &from_screen);
};

#endif
