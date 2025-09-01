/*
    nanogui/nanogui.cpp -- Basic initialization and utility routines

    NanoGUI was developed by Wenzel Jakob <wenzel.jakob@epfl.ch>.
    The widget drawing code is based on the NanoVG demo application
    by Mikko Mononen.

    All rights reserved. Use of this source code is governed by a
    BSD-style license that can be found in the LICENSE.txt file.
*/

#include <nanogui/screen.h>
#include <nanogui/opengl.h>
#include <nanogui/metal.h>
#include <unordered_map>

#include <mutex>
#include <stdexcept>

#if defined(__APPLE__)
#  define GLFW_EXPOSE_NATIVE_COCOA
#elif defined(_WIN32)
#  define GLFW_EXPOSE_NATIVE_WIN32
#else
#  define GLFW_EXPOSE_NATIVE_WAYLAND
#  define GLFW_EXPOSE_NATIVE_X11
#endif

#include <nfd_glfw3.h>

#if !defined(_WIN32)
#  include <dirent.h>
#endif

#if defined(EMSCRIPTEN)
#  include <emscripten/emscripten.h>
#endif

NAMESPACE_BEGIN(nanogui)

extern std::unordered_map<GLFWwindow *, Screen *> __nanogui_screens;

#if defined(__APPLE__)
  extern void disable_saved_application_state_osx();
#endif

void init(bool color_management) {
    #if !defined(_WIN32)
        /* Avoid locale-related number parsing issues */
        setlocale(LC_NUMERIC, "C");
    #endif

    #if defined(__APPLE__)
        disable_saved_application_state_osx();
        glfwInitHint(GLFW_COCOA_CHDIR_RESOURCES, GLFW_FALSE);
    #endif

    glfwSetErrorCallback(
        [](int error, const char *descr) {
            if (error == GLFW_NOT_INITIALIZED)
                return; /* Ignore */
            fprintf(stderr, "GLFW error %i: %s", error, descr);
        }
    );

    if (color_management) {
        glfwInitHint(GLFW_WAYLAND_COLOR_MANAGEMENT, GLFW_TRUE);
    }

    if (!glfwInit())
        throw std::runtime_error("Could not initialize GLFW!");

#if defined(NANOGUI_USE_METAL)
    metal_init();
#endif

    glfwSetTime(0);

    nfdresult_t rv = NFD_Init();
    if (rv != NFD_OKAY)
        throw std::runtime_error("Could not initialize NFD!");
}

#if defined(EMSCRIPTEN)
static double emscripten_last = 0;
static float emscripten_refresh = 0;
#endif

std::mutex m_async_mutex;
std::vector<std::function<void()>> m_async_functions;
static RunMode current_run_mode = RunMode::Stopped;


static void mainloop_iteration() {
    int num_screens = 0;

    #if defined(EMSCRIPTEN)
        double emscripten_now = glfwGetTime();
        bool emscripten_redraw = false;
        if (float((emscripten_now - emscripten_last) * 1000) > emscripten_refresh) {
            emscripten_redraw = true;
            emscripten_last = emscripten_now;
        }
    #endif

    /* Run async functions */ {
        std::lock_guard<std::mutex> guard(m_async_mutex);
        for (auto &f : m_async_functions)
            f();
        m_async_functions.clear();
    }

    for (auto kv : __nanogui_screens) {
        Screen *screen = kv.second;
        if (!screen->visible()) {
            continue;
        } else if (glfwWindowShouldClose(screen->glfw_window())) {
            screen->set_visible(false);
            continue;
        }
        #if defined(EMSCRIPTEN)
            if (emscripten_redraw || screen->tooltip_fade_in_progress())
                screen->redraw();
        #endif
        screen->draw_all();
        num_screens++;
    }

    if (num_screens == 0) {
        // Give up if there was nothing to draw
        current_run_mode = RunMode::Stopped;
        return;
    }

    #if !defined(EMSCRIPTEN)
        // Wait for mouse/keyboard or empty refresh events
        if (current_run_mode == RunMode::Lazy)
            glfwWaitEvents();
        else
            glfwPollEvents();
    #endif
}

void run(RunMode run_mode) {
    if (current_run_mode != RunMode::Stopped)
        throw std::runtime_error("Main loop is already active!");

#if defined(EMSCRIPTEN)
    emscripten_refresh = refresh;
    /* The following will throw an exception and enter the main
       loop within Emscripten. This means that none of the code below
       (or in the caller, for that matter) will be executed */
    emscripten_set_main_loop(mainloop_iteration, 0, 1);
#endif

    current_run_mode = run_mode;

    try {
        while (current_run_mode != RunMode::Stopped)
            mainloop_iteration();

        // Process events once more
        glfwPollEvents();
    } catch (const std::exception &e) {
        fprintf(stderr, "Caught exception in main loop: %s", e.what());
        leave();
    }
}

void async(const std::function<void()> &func) {
    std::lock_guard<std::mutex> guard(m_async_mutex);
    m_async_functions.push_back(func);
}

RunMode run_mode() { return current_run_mode; }
void set_run_mode(RunMode run_mode) { current_run_mode = run_mode; }

std::pair<bool, bool> test_10bit_edr_support() {
#if defined(NANOGUI_USE_METAL)
    return metal_10bit_edr_support();
#else
    return { false, false };
#endif
}


void shutdown() {
    NFD_Quit();

    glfwTerminate();

#if defined(NANOGUI_USE_METAL)
    metal_shutdown();
#endif
}

#if defined(__clang__)
#  define NANOGUI_FALLTHROUGH [[clang::fallthrough]];
#elif defined(__GNUG__)
#  define NANOGUI_FALLTHROUGH __attribute__ ((fallthrough));
#else
#  define NANOGUI_FALLTHROUGH
#endif

std::string utf8(uint32_t c) {
    char seq[8];
    int n = 0;
    if (c < 0x80) n = 1;
    else if (c < 0x800) n = 2;
    else if (c < 0x10000) n = 3;
    else if (c < 0x200000) n = 4;
    else if (c < 0x4000000) n = 5;
    else if (c <= 0x7fffffff) n = 6;
    seq[n] = '\0';
    switch (n) {
        case 6: seq[5] = 0x80 | (c & 0x3f); c = c >> 6; c |= 0x4000000; NANOGUI_FALLTHROUGH
        case 5: seq[4] = 0x80 | (c & 0x3f); c = c >> 6; c |= 0x200000;  NANOGUI_FALLTHROUGH
        case 4: seq[3] = 0x80 | (c & 0x3f); c = c >> 6; c |= 0x10000;   NANOGUI_FALLTHROUGH
        case 3: seq[2] = 0x80 | (c & 0x3f); c = c >> 6; c |= 0x800;     NANOGUI_FALLTHROUGH
        case 2: seq[1] = 0x80 | (c & 0x3f); c = c >> 6; c |= 0xc0;      NANOGUI_FALLTHROUGH
        case 1: seq[0] = c;
    }
    return std::string(seq, seq + n);
}

static std::unordered_map<std::string, int> icon_cache;

int __nanogui_get_image(NVGcontext *ctx, const std::string &name, uint8_t *data, uint32_t size) {
    auto it = icon_cache.find(name);
    if (it != icon_cache.end())
        return it->second;
    int icon_id = nvgCreateImageMem(ctx, 0, data, size);
    if (icon_id == 0)
        throw std::runtime_error("Unable to load resource data.");
    icon_cache[name] = icon_id;
    return icon_id;
}

std::vector<std::pair<int, std::string>>
load_image_directory(NVGcontext *ctx, const std::string &path) {
    std::vector<std::pair<int, std::string> > result;
#if !defined(_WIN32)
    DIR *dp = opendir(path.c_str());
    if (!dp)
        throw std::runtime_error("Could not open image directory!");
    struct dirent *ep;
    while ((ep = readdir(dp))) {
        const char *fname = ep->d_name;
#else
    WIN32_FIND_DATA ffd;
    std::string search_path = path + "/*.*";
    HANDLE handle = FindFirstFileA(search_path.c_str(), &ffd);
    if (handle == INVALID_HANDLE_VALUE)
        throw std::runtime_error("Could not open image directory!");
    do {
        const char *fname = ffd.cFileName;
#endif
        if (strstr(fname, "png") == nullptr)
            continue;
        std::string full_name = path + "/" + std::string(fname);
        int img = nvgCreateImage(ctx, full_name.c_str(), 0);
        if (img == 0)
            throw std::runtime_error("Could not open image data!");
        result.push_back(
            std::make_pair(img, full_name.substr(0, full_name.length() - 4)));
#if !defined(_WIN32)
    }
    closedir(dp);
#else
    } while (FindNextFileA(handle, &ffd) != 0);
    FindClose(handle);
#endif
    return result;
}

template <typename Func> struct scope_guard {
    scope_guard(Func &&func) : func(std::move(func)) { };
    ~scope_guard() { func(); }
    scope_guard(const Func &) = delete;
    scope_guard() = delete;
    scope_guard& operator=(const Func &) = delete;
    scope_guard& operator=(Func&&) = delete;

private:
    Func func;
};

std::vector<std::string>
file_dialog(Widget *parent,
            FileDialogType type,
            const std::vector<std::pair<std::string, std::string>> &filters,
            const std::string &default_path) {

    std::vector<nfdu8filteritem_t> nfd_filters;
    for (const auto& ftype: filters)
        nfd_filters.push_back({ftype.second.c_str(), ftype.first.c_str()});
    const char *nfd_default_path = default_path.empty() ? nullptr: default_path.c_str();

    const nfdpathset_t* out_paths{};
    nfdwindowhandle_t parent_window{};
    nfdu8char_t* out_path{};
    nfdresult_t result{};
    nfdpathsetenum_t enumerator = {};
    bool enumerator_set = false;

    auto cleanup = scope_guard([&]() {
        if (out_paths)
            NFD_PathSet_Free(out_paths);
        if (out_path)
            NFD_FreePathU8(out_path);
        if (enumerator_set)
            NFD_PathSet_FreeEnum(&enumerator);
    });

    if (!NFD_GetNativeWindowFromGLFWWindow(parent->screen()->glfw_window(), &parent_window))
        throw std::runtime_error("nanogui::file_dialog(): could not obtain native window handle");

    switch (type) {
        case FileDialogType::Save: {
                nfdsavedialogu8args_t args = {};
                args.filterList = nfd_filters.data();
                args.filterCount = nfd_filters.size();
                args.defaultPath = nfd_default_path;
                args.parentWindow = parent_window;
                result = NFD_SaveDialogU8_With(&out_path, &args);
            }
            break;

        case FileDialogType::Open:
        case FileDialogType::OpenMultiple: {
                nfdopendialogu8args_t args = {};
                args.filterList = nfd_filters.data();
                args.filterCount = nfd_filters.size();
                args.defaultPath = nfd_default_path;
                args.parentWindow = parent_window;

                if (type == FileDialogType::OpenMultiple)
                    result = NFD_OpenDialogMultipleU8_With(&out_paths, &args);
                else
                    result = NFD_OpenDialogU8_With(&out_path, &args);
            }
            break;

        case FileDialogType::PickFolder:
        case FileDialogType::PickFolderMultiple: {
                nfdpickfolderu8args_t args = {};
                args.defaultPath = nfd_default_path;
                args.parentWindow = parent_window;

                if (!filters.empty())
                    throw std::runtime_error("nanogui::file_dialog(): filters are not supported for folder selection.");

                if (type == FileDialogType::PickFolderMultiple)
                    result = NFD_PickFolderMultipleU8_With(&out_paths, &args);
                else
                    result = NFD_PickFolderU8_With(&out_path, &args);
            }
            break;
    }

    if (result == NFD_CANCEL)
        return {};
    else if (result != NFD_OKAY)
        throw std::runtime_error("nanogui::file_dialog(): dialog error: " + std::string(NFD_GetError()));

    if (!NFD_PathSet_GetEnum(out_paths, &enumerator))
        throw std::runtime_error("nanogui::file_dialog(): could not obtain enumerator: " + std::string(NFD_GetError()));
    enumerator_set = true;

    std::vector<std::string> paths;

    switch (type) {
        case FileDialogType::Save:
        case FileDialogType::Open:
        case FileDialogType::PickFolder:
            paths.emplace_back(out_path);
            break;

        case FileDialogType::PickFolderMultiple:
        case FileDialogType::OpenMultiple:
            while (NFD_PathSet_EnumNextU8(&enumerator, &out_path) && out_path)
                paths.emplace_back(out_path);
            break;
    }

    return paths;
}

static void (*object_inc_ref_py)(PyObject *) noexcept = nullptr;
static void (*object_dec_ref_py)(PyObject *) noexcept = nullptr;

Object::~Object() { }

void Object::inc_ref() const noexcept {
    uintptr_t value = m_state.load(std::memory_order_relaxed);

    while (true) {
        if (value & 1) {
            if (!m_state.compare_exchange_weak(value,
                                               value + 2,
                                               std::memory_order_relaxed,
                                               std::memory_order_relaxed))
                continue;
        } else {
            object_inc_ref_py((PyObject *) value);
        }

        break;
    }
}

void Object::dec_ref() const noexcept {
    uintptr_t value = m_state.load(std::memory_order_relaxed);

    while (true) {
        if (value & 1) {
            if (value == 1) {
                fprintf(stderr,
                        "Object::dec_ref(%p): reference count underflow!",
                        this);
                abort();
            } else if (value == 3) {
                delete this;
            } else {
                if (!m_state.compare_exchange_weak(value,
                                                   value - 2,
                                                   std::memory_order_relaxed,
                                                   std::memory_order_relaxed))
                    continue;
            }
        } else {
            object_dec_ref_py((PyObject *) value);
        }
        break;
    }
}

void Object::set_self_py(PyObject *o) noexcept {
    uintptr_t value = m_state.load(std::memory_order_relaxed);
    if (value & 1) {
        value >>= 1;
        for (uintptr_t i = 0; i < value; ++i)
            object_inc_ref_py(o);

        uintptr_t o_i = (uintptr_t) o;
        if (o_i & 1) {
            fprintf(stderr, "Object::set_self_py(%p): invalid pointer alignment!", this);
            abort();
        }

        m_state.store(o_i);
    } else {
        fprintf(stderr,
                "Object::set_self_py(%p): a Python object was already present!",
                this);
        abort();
    }
}

PyObject *Object::self_py() const noexcept {
    uintptr_t value = m_state.load(std::memory_order_relaxed);
    if (value & 1)
        return nullptr;
    else
        return (PyObject *) value;
}

void object_init_py(void (*object_inc_ref_py_)(PyObject *) noexcept,
                    void (*object_dec_ref_py_)(PyObject *) noexcept) {
    object_inc_ref_py = object_inc_ref_py_;
    object_dec_ref_py = object_dec_ref_py_;
}

dither_matrix_t ditherMatrix(float scale) {
    // 8x8 Bayer dithering matrix scaled to [-0.5f, 0.5f] / 255
    dither_matrix_t thresholdMatrix = {
        {0, 32, 8, 40, 2, 34, 10, 42,
         48, 16, 56, 24, 50, 18, 58, 26,
         12, 44, 4, 36, 14, 46, 6, 38,
         60, 28, 52, 20, 62, 30, 54, 22,
         3, 35, 11, 43, 1, 33, 9, 41,
         51, 19, 59, 27, 49, 17, 57, 25,
         15, 47, 7, 39, 13, 45, 5, 37,
         63, 31, 55, 23, 61, 29, 53, 21}
    };

    for (size_t i = 0; i < DITHER_MATRIX_SIZE * DITHER_MATRIX_SIZE; ++i) {
        thresholdMatrix[i] = (thresholdMatrix[i] / DITHER_MATRIX_SIZE / DITHER_MATRIX_SIZE - 0.5f) * scale;
    }

    return thresholdMatrix;
}

NAMESPACE_END(nanogui)

