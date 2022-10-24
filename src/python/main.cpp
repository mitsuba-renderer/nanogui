#ifdef NANOGUI_PYTHON

#include "python.h"
#include <thread>
#include <mutex>
#include <condition_variable>
#include <nanobind/stl/pair.h>

#if defined(__APPLE__) || defined(__linux__)
#  include <signal.h>
#endif

#if defined(__APPLE__) || defined(__linux__)
namespace {
    class semaphore {
    public:
        semaphore(int count = 0) : count(count) { }

        void notify() {
            std::unique_lock<std::mutex> lck(mtx);
            ++count;
            cv.notify_one();
        }

        void wait() {
            std::unique_lock<std::mutex> lck(mtx);
            while (count == 0)
                cv.wait(lck);
            --count;
        }

    private:
        std::mutex mtx;
        std::condition_variable cv;
        int count;
    };
}
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

    m.def("init", &nanogui::init, D(init));
    m.def("shutdown", &nanogui::shutdown, D(shutdown));

    m.def("mainloop", [](float refresh) {
        nb::gil_scoped_release release;

        #if defined(__APPLE__) || defined(__linux__)
            sigint_handler_prev = signal(SIGINT, sigint_handler);
        #endif

        mainloop(refresh);

        #if defined(__APPLE__) || defined(__linux__)
            signal(SIGINT, sigint_handler_prev);
        #endif
    }, "refresh"_a = -1, D(mainloop));

    m.def("async", &nanogui::async, D(async));
    m.def("leave", &nanogui::leave, D(leave));
    m.def("test_10bit_edr_support", &test_10bit_edr_support, D(test_10bit_edr_support));
    m.def("active", &nanogui::active, D(active));
    m.def("file_dialog",
          (std::string(*)(
              const std::vector<std::pair<std::string, std::string>> &, bool)) &
              nanogui::file_dialog,
          D(file_dialog));
    m.def("file_dialog",
          (std::vector<std::string>(*)(
              const std::vector<std::pair<std::string, std::string>> &, bool,
              bool)) &
              nanogui::file_dialog,
          D(file_dialog, 2));
#if defined(__APPLE__)
        m.def("chdir_to_bundle_parent", &nanogui::chdir_to_bundle_parent);
    #endif
    m.def("utf8", [](int c) { return std::string(utf8(c).data()); }, D(utf8));
    m.def("load_image_directory", &nanogui::load_image_directory, D(load_image_directory));

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
}

#endif
