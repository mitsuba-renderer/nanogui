#ifdef NANOGUI_PYTHON

#include "python.h"
#include <thread>
#include <mutex>
#include <condition_variable>
#include <nanobind/stl/pair.h>

#if defined(__APPLE__) || defined(__linux__)
#  include <signal.h>
#endif

extern void register_vector(nb::module_ &m);
extern void register_glfw(nb::module_ &m);
extern void register_entypo(nb::module_ &m);
extern void register_eigen(nb::module_ &m);
extern void register_widget(nb::module_ &m);
extern void register_layout(nb::module_ &m);
extern void register_basics(nb::module_ &m);
extern void register_button(nb::module_ &m);
extern void register_tabs(nb::module_ &m);
extern void register_textbox(nb::module_ &m);
extern void register_textarea(nb::module_ &m);
extern void register_theme(nb::module_ &m);
extern void register_canvas(nb::module_ &m);
extern void register_formhelper(nb::module_ &m);
extern void register_misc(nb::module_ &m);
extern void register_nanovg(nb::module_ &m);
extern void register_render(nb::module_ &m);
extern void register_quad(nb::module_ &m);
extern void register_chroma(nb::module_ &m);
extern void register_ema(nb::module_ &m);

#if defined(__APPLE__) || defined(__linux__)
static void (*sigint_handler_prev)(int) = nullptr;
static void sigint_handler(int sig) {
    nanogui::leave();
    signal(sig, sigint_handler_prev);
    raise(sig);
}
#endif

NB_MODULE(nanogui_ext, m_) {
    (void) m_;
    nb::module_ m = nb::module_::import_("nanogui");
    m.attr("__doc__") = "NanoGUI plugin";

#if defined(NANOGUI_USE_OPENGL)
    m.attr("api") = "opengl";
#elif defined(NANOGUI_USE_GLES) && NANOGUI_GLES_VERSION == 2
    m.attr("api") = "gles2";
#elif defined(NANOGUI_USE_GLES) && NANOGUI_GLES_VERSION == 3
    m.attr("api") = "gles3";
#elif defined(NANOGUI_USE_METAL)
    m.attr("api") = "metal";
#endif

    m.def("init", &nanogui::init, D(init), "color_management"_a = false);
    m.def("shutdown", &nanogui::shutdown, D(shutdown));

    nb::enum_<RunMode>(m, "RunMode", D(RunMode))
        .value("Stopped", RunMode::Stopped, D(RunMode, Stopped))
        .value("VSync", RunMode::VSync, D(RunMode, VSync))
        .value("Eager", RunMode::Eager, D(RunMode, Eager))
        .value("Lazy", RunMode::Lazy, D(RunMode, Lazy));

    nb::enum_<FileDialogType>(m, "FileDialogType", D(FileDialogType))
        .value("Open", FileDialogType::Open, D(FileDialogType, Open))
        .value("OpenMultiple", FileDialogType::OpenMultiple, D(FileDialogType, OpenMultiple))
        .value("Save", FileDialogType::Save, D(FileDialogType, Save))
        .value("PickFolder", FileDialogType::PickFolder, D(FileDialogType, PickFolder))
        .value("PickFolderMultiple", FileDialogType::PickFolderMultiple, D(FileDialogType, PickFolderMultiple));

    m.def("run", [](RunMode run_mode) {
        nb::gil_scoped_release release;

        #if defined(__APPLE__) || defined(__linux__)
            sigint_handler_prev = signal(SIGINT, sigint_handler);
        #endif

        run(run_mode);

        #if defined(__APPLE__) || defined(__linux__)
            signal(SIGINT, sigint_handler_prev);
        #endif
    }, "run_mode"_a = RunMode::VSync, D(run));

    m.def("async", &nanogui::async, D(async));
    m.def("set_run_mode", &nanogui::set_run_mode);
    m.def("run_mode", &nanogui::run_mode);
    m.def("leave", &nanogui::leave, D(leave));
    m.def("test_10bit_edr_support", &test_10bit_edr_support, D(test_10bit_edr_support));
    m.def("active", &nanogui::active, D(active));
    m.def("file_dialog", nanogui::file_dialog, "widget"_a, "type"_a,
          "filters"_a = nb::list(), "default_path"_a = nb::str(""),
          D(file_dialog), nb::call_guard<nb::gil_scoped_release>());
    #if defined(__APPLE__)
        m.def("chdir_to_bundle_parent", &nanogui::chdir_to_bundle_parent);
    #endif
    m.def("utf8", [](int c) { return std::string(utf8(c).data()); }, D(utf8));
    m.def("load_image_directory", &nanogui::load_image_directory, D(load_image_directory),
          nb::call_guard<nb::gil_scoped_release>());

    nb::enum_<Cursor>(m, "Cursor", D(Cursor))
        .value("Arrow", Cursor::Arrow)
        .value("IBeam", Cursor::IBeam)
        .value("Crosshair", Cursor::Crosshair)
        .value("Hand", Cursor::Hand)
        .value("HResize", Cursor::HResize)
        .value("VResize", Cursor::VResize);

    nb::enum_<Alignment>(m, "Alignment", D(Alignment))
        .value("Minimum", Alignment::Minimum)
        .value("Middle", Alignment::Middle)
        .value("Maximum", Alignment::Maximum)
        .value("Fill", Alignment::Fill);

    nb::enum_<Orientation>(m, "Orientation", D(Orientation))
        .value("Horizontal", Orientation::Horizontal)
        .value("Vertical", Orientation::Vertical);

    register_vector(m);
    register_glfw(m);
    register_entypo(m);
    register_eigen(m);
    register_widget(m);
    register_layout(m);
    register_basics(m);
    register_button(m);
    register_tabs(m);
    register_textbox(m);
    register_textarea(m);
    register_theme(m);
    register_canvas(m);
    register_formhelper(m);
    register_misc(m);
    register_nanovg(m);
    register_render(m);
    register_quad(m);
    register_chroma(m);
    register_ema(m);
}

#endif
