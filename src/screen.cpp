/*
    src/screen.cpp -- Top-level widget and interface between NanoGUI and GLFW

    A significant redesign of this code was contributed by Christian Schueller.

    NanoGUI was developed by Wenzel Jakob <wenzel.jakob@epfl.ch>.
    The widget drawing code is based on the NanoVG demo application
    by Mikko Mononen.

    All rights reserved. Use of this source code is governed by a
    BSD-style license that can be found in the LICENSE.txt file.
*/

#include <nanogui/screen.h>
#include <nanogui/theme.h>
#include <nanogui/opengl.h>
#include <nanogui/window.h>
#include <nanogui/popup.h>
#include <map>
#include <iostream>

#if defined(EMSCRIPTEN)
#  include <emscripten/emscripten.h>
#  include <emscripten/html5.h>
#endif

#if defined(_WIN32)
#  define NOMINMAX
#  undef APIENTRY

#  define WIN32_LEAN_AND_MEAN
#  include <windows.h>

#  define GLFW_EXPOSE_NATIVE_WGL
#  define GLFW_EXPOSE_NATIVE_WIN32
#  include <GLFW/glfw3native.h>
#endif

/* Allow enforcing the GL2 implementation of NanoVG */
#if defined(NANOGUI_USE_OPENGL)
#  define NANOVG_GL3_IMPLEMENTATION
#else
#  define NANOVG_GLES2_IMPLEMENTATION
#endif

#include <nanovg_gl.h>

NAMESPACE_BEGIN(nanogui)

std::map<GLFWwindow *, Screen *> __nanogui_screens;

#if defined(NANOGUI_GLAD)
static bool glad_initialized = false;
#endif

/* Calculate pixel ratio for hi-dpi devices. */
static float get_pixel_ratio(GLFWwindow *window) {
#if defined(_WIN32)
    HWND hwnd = glfwGetWin32Window(window);
    HMONITOR monitor = MonitorFromWindow(hwnd, MONITOR_DEFAULTTONEAREST);
    /* The following function only exists on Windows 8.1+, but we don't want to make that a dependency */
    static HRESULT (WINAPI *GetDpiForMonitor_)(HMONITOR, UINT, UINT*, UINT*) = nullptr;
    static bool GetDpiForMonitor_tried = false;

    if (!GetDpiForMonitor_tried) {
        auto shcore = LoadLibrary(TEXT("shcore"));
        if (shcore)
            GetDpiForMonitor_ = (decltype(GetDpiForMonitor_)) GetProcAddress(shcore, "GetDpiForMonitor");
        GetDpiForMonitor_tried = true;
    }

    if (GetDpiForMonitor_) {
        uint32_t dpi_x, dpi_y;
        if (GetDpiForMonitor_(monitor, 0 /* effective DPI */, &dpi_x, &dpi_y) == S_OK)
            return dpi_x / 96.0;
    }
    return 1.f;
#elif defined(__linux__)
    (void) window;

    float ratio = 1.0f;
    FILE *fp;
    /* Try to read the pixel ratio from KDEs config */
    auto currentDesktop = std::getenv("XDG_CURRENT_DESKTOP");
    if (currentDesktop && currentDesktop == std::string("KDE")) {
        fp = popen("kreadconfig5 --group KScreen --key ScaleFactor", "r");
        if (!fp)
            return 1;

        if (fscanf(fp, "%f", &ratio) != 1)
            return 1;
    } else {
        /* Try to read the pixel ratio from GTK */
        fp = popen("gsettings get org.gnome.desktop.interface scaling-factor", "r");
        if (!fp)
            return 1;

        int ratioInt = 1;
        if (fscanf(fp, "uint32 %i", &ratioInt) != 1)
            return 1;
        ratio = ratioInt;
    }
    if (pclose(fp) != 0)
        return 1;
    return ratio >= 1 ? ratio : 1;
#elif defined(EMSCRIPTEN)
    return emscripten_get_device_pixel_ratio();
#else
    Vector2i fb_size, size;
    glfwGetFramebufferSize(window, &fb_size[0], &fb_size[1]);
    glfwGetWindowSize(window, &size[0], &size[1]);
    return (float)fb_size[0] / (float)size[0];
#endif
}

Screen::Screen()
    : Widget(nullptr), m_glfw_window(nullptr), m_nvg_context(nullptr),
      m_cursor(Cursor::Arrow), m_background(0.3f, 0.3f, 0.32f, 1.f),
      m_shutdown_glfw(false), m_fullscreen(false), m_redraw(false) {
    memset(m_cursors, 0, sizeof(GLFWcursor *) * (int) Cursor::CursorCount);
}

Screen::Screen(const Vector2i &size, const std::string &caption, bool resizable,
               bool fullscreen, int color_bits, int alpha_bits, int depth_bits,
               int stencil_bits, int n_samples,
               unsigned int gl_major, unsigned int gl_minor)
    : Widget(nullptr), m_glfw_window(nullptr), m_nvg_context(nullptr),
      m_cursor(Cursor::Arrow), m_background(0.3f, 0.3f, 0.32f, 1.f), m_caption(caption),
      m_shutdown_glfw(false), m_fullscreen(fullscreen), m_redraw(false) {
    memset(m_cursors, 0, sizeof(GLFWcursor *) * (int) Cursor::CursorCount);

#if defined(NANOGUI_USE_OPENGL)
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);

    /* Request a forward compatible OpenGL gl_major.gl_minor core profile context.
       Default value is an OpenGL 3.3 core profile context. */
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, gl_major);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, gl_minor);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#else
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
    glfwWindowHint(GLFW_CONTEXT_CREATION_API, GLFW_EGL_CONTEXT_API);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
#endif

    glfwWindowHint(GLFW_SAMPLES, n_samples);
    glfwWindowHint(GLFW_RED_BITS, color_bits);
    glfwWindowHint(GLFW_GREEN_BITS, color_bits);
    glfwWindowHint(GLFW_BLUE_BITS, color_bits);
    glfwWindowHint(GLFW_ALPHA_BITS, alpha_bits);
    glfwWindowHint(GLFW_STENCIL_BITS, stencil_bits);
    glfwWindowHint(GLFW_DEPTH_BITS, depth_bits);
    glfwWindowHint(GLFW_VISIBLE, GL_FALSE);
    glfwWindowHint(GLFW_RESIZABLE, resizable ? GL_TRUE : GL_FALSE);

    if (fullscreen) {
        GLFWmonitor *monitor = glfwGetPrimaryMonitor();
        const GLFWvidmode *mode = glfwGetVideoMode(monitor);
        m_glfw_window = glfwCreateWindow(mode->width, mode->height,
                                       caption.c_str(), monitor, nullptr);
    } else {
        m_glfw_window = glfwCreateWindow(size.x(), size.y(),
                                       caption.c_str(), nullptr, nullptr);
    }

    if (!m_glfw_window) {
#if defined(NANOGUI_USE_OPENGL)
        throw std::runtime_error("Could not create an OpenGL " +
                                 std::to_string(gl_major) + "." +
                                 std::to_string(gl_minor) + " context!");
#else
        (void) gl_major; (void) gl_minor;
        throw std::runtime_error("Could not create an GLES 2 context!");
#endif
    }

    glfwMakeContextCurrent(m_glfw_window);

#if defined(NANOGUI_GLAD)
    if (!glad_initialized) {
        glad_initialized = true;
        if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
            throw std::runtime_error("Could not initialize GLAD!");
        glGetError(); // pull and ignore unhandled errors like GL_INVALID_ENUM
    }
#endif

    glfwGetFramebufferSize(m_glfw_window, &m_fbsize[0], &m_fbsize[1]);
    glViewport(0, 0, m_fbsize[0], m_fbsize[1]);
    glClearColor(m_background[0], m_background[1], m_background[2], m_background[3]);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    glfwSwapInterval(0);
    glfwSwapBuffers(m_glfw_window);

#if defined(__APPLE__)
    /* Poll for events once before starting a potentially
       lengthy loading process. This is needed to be
       classified as "interactive" by other software such
       as i_term2 */

    glfwPollEvents();
#endif

    /* Propagate GLFW events to the appropriate Screen instance */
    glfwSetCursorPosCallback(m_glfw_window,
        [](GLFWwindow *w, double x, double y) {
            auto it = __nanogui_screens.find(w);
            if (it == __nanogui_screens.end())
                return;
            Screen *s = it->second;
            if (!s->m_process_events)
                return;
            s->cursor_pos_callback_event(x, y);
        }
    );

    glfwSetMouseButtonCallback(m_glfw_window,
        [](GLFWwindow *w, int button, int action, int modifiers) {
            auto it = __nanogui_screens.find(w);
            if (it == __nanogui_screens.end())
                return;
            Screen *s = it->second;
            if (!s->m_process_events)
                return;
            s->mouse_button_callback_event(button, action, modifiers);
        }
    );

    glfwSetKeyCallback(m_glfw_window,
        [](GLFWwindow *w, int key, int scancode, int action, int mods) {
            auto it = __nanogui_screens.find(w);
            if (it == __nanogui_screens.end())
                return;
            Screen *s = it->second;
            if (!s->m_process_events)
                return;
            s->key_callback_event(key, scancode, action, mods);
        }
    );

    glfwSetCharCallback(m_glfw_window,
        [](GLFWwindow *w, unsigned int codepoint) {
            auto it = __nanogui_screens.find(w);
            if (it == __nanogui_screens.end())
                return;
            Screen *s = it->second;
            if (!s->m_process_events)
                return;
            s->char_callback_event(codepoint);
        }
    );

    glfwSetDropCallback(m_glfw_window,
        [](GLFWwindow *w, int count, const char **filenames) {
            auto it = __nanogui_screens.find(w);
            if (it == __nanogui_screens.end())
                return;
            Screen *s = it->second;
            if (!s->m_process_events)
                return;
            s->drop_callback_event(count, filenames);
        }
    );

    glfwSetScrollCallback(m_glfw_window,
        [](GLFWwindow *w, double x, double y) {
            auto it = __nanogui_screens.find(w);
            if (it == __nanogui_screens.end())
                return;
            Screen *s = it->second;
            if (!s->m_process_events)
                return;
            s->scroll_callback_event(x, y);
        }
    );

    /* React to framebuffer size events -- includes window
       size events and also catches things like dragging
       a window from a Retina-capable screen to a normal
       screen on Mac OS X */
    glfwSetFramebufferSizeCallback(m_glfw_window,
        [](GLFWwindow* w, int width, int height) {
            auto it = __nanogui_screens.find(w);
            if (it == __nanogui_screens.end())
                return;
            Screen *s = it->second;

            if (!s->m_process_events)
                return;

            s->resize_callback_event(width, height);
        }
    );

    // notify when the screen has lost focus (e.g. application switch)
    glfwSetWindowFocusCallback(m_glfw_window,
        [](GLFWwindow *w, int focused) {
            auto it = __nanogui_screens.find(w);
            if (it == __nanogui_screens.end())
                return;

            Screen *s = it->second;
            // focus_event: 0 when false, 1 when true
            s->focus_event(focused != 0);
        }
    );
    initialize(m_glfw_window, true);
}

void Screen::initialize(GLFWwindow *window, bool shutdown_glfw) {
    m_glfw_window = window;
    m_shutdown_glfw = shutdown_glfw;
    glfwGetWindowSize(m_glfw_window, &m_size[0], &m_size[1]);
    glfwGetFramebufferSize(m_glfw_window, &m_fbsize[0], &m_fbsize[1]);

    m_pixel_ratio = get_pixel_ratio(window);

#if defined(_WIN32) || defined(__linux__) || defined(EMSCRIPTEN)
    if (m_pixel_ratio != 1 && !m_fullscreen)
        glfwSetWindowSize(window, m_size.x() * m_pixel_ratio, m_size.y() * m_pixel_ratio);
    #if defined(EMSCRIPTEN)
        emscripten_set_element_css_size("#canvas", m_size.x(), m_size.y());
    #endif
#endif

#if defined(NANOGUI_GLAD)
    if (!glad_initialized) {
        glad_initialized = true;
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
            throw std::runtime_error("Could not initialize GLAD!");
        glGetError(); // pull and ignore unhandled errors like GL_INVALID_ENUM
    }
#endif

    /* Detect framebuffer properties and set up compatible NanoVG context */
    GLint n_stencil_bits = 0, n_samples = 0;
#if defined(NANOGUI_USE_OPENGL)
    glGetFramebufferAttachmentParameteriv(GL_DRAW_FRAMEBUFFER,
        GL_STENCIL, GL_FRAMEBUFFER_ATTACHMENT_STENCIL_SIZE, &n_stencil_bits);
#else
    n_stencil_bits = 8;
#endif
    glGetIntegerv(GL_SAMPLES, &n_samples);

    int flags = 0;
    if (n_stencil_bits >= 8)
       flags |= NVG_STENCIL_STROKES;
    if (n_samples <= 1)
       flags |= NVG_ANTIALIAS;
#if !defined(NDEBUG)
    flags |= NVG_DEBUG;
#endif

#if defined(NANOGUI_USE_OPENGL)
    m_nvg_context = nvgCreateGL3(flags);
#else
    m_nvg_context = nvgCreateGLES2(flags);
#endif

    if (m_nvg_context == nullptr)
        throw std::runtime_error("Could not initialize NanoVG!");

    m_visible = glfwGetWindowAttrib(window, GLFW_VISIBLE) != 0;
    set_theme(new Theme(m_nvg_context));
    m_mouse_pos = Vector2i(0);
    m_mouse_state = m_modifiers = 0;
    m_drag_active = false;
    m_last_interaction = glfwGetTime();
    m_process_events = true;
    m_redraw = true;
    __nanogui_screens[m_glfw_window] = this;

    for (int i=0; i < (int) Cursor::CursorCount; ++i)
        m_cursors[i] = glfwCreateStandardCursor(GLFW_ARROW_CURSOR + i);

    /// Fixes retina display-related font rendering issue (#185)
    nvgBeginFrame(m_nvg_context, m_size[0], m_size[1], m_pixel_ratio);
    nvgEndFrame(m_nvg_context);
}

Screen::~Screen() {
    __nanogui_screens.erase(m_glfw_window);
    for (int i=0; i < (int) Cursor::CursorCount; ++i) {
        if (m_cursors[i])
            glfwDestroyCursor(m_cursors[i]);
    }
    if (m_nvg_context) {
#if defined(NANOGUI_USE_OPENGL)
        nvgDeleteGL3(m_nvg_context);
#else
        nvgDeleteGLES2(m_nvg_context);
#endif
    }
    if (m_glfw_window && m_shutdown_glfw)
        glfwDestroyWindow(m_glfw_window);
}

void Screen::set_visible(bool visible) {
    if (m_visible != visible) {
        m_visible = visible;

        if (visible)
            glfwShowWindow(m_glfw_window);
        else
            glfwHideWindow(m_glfw_window);
    }
}

void Screen::set_caption(const std::string &caption) {
    if (caption != m_caption) {
        glfwSetWindowTitle(m_glfw_window, caption.c_str());
        m_caption = caption;
    }
}

void Screen::set_size(const Vector2i &size) {
    Widget::set_size(size);

#if defined(_WIN32) || defined(__linux__) || defined(EMSCRIPTEN)
    glfwSetWindowSize(m_glfw_window, size.x() * m_pixel_ratio, size.y() * m_pixel_ratio);
#else
    glfwSetWindowSize(m_glfw_window, size.x(), size.y());
#endif
}

void Screen::draw_all() {
    if (!m_redraw)
        return;
    m_redraw = false;

    glfwMakeContextCurrent(m_glfw_window);
    glfwGetFramebufferSize(m_glfw_window, &m_fbsize[0], &m_fbsize[1]);
    glfwGetWindowSize(m_glfw_window, &m_size[0], &m_size[1]);

    glClearColor(m_background[0], m_background[1], m_background[2], m_background[3]);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

#if defined(_WIN32) || defined(__linux__) || defined(EMSCRIPTEN)
    m_size = Vector2i(m_size / m_pixel_ratio);
    m_fbsize = Vector2i(m_size * m_pixel_ratio);
#else
    /* Recompute pixel ratio on OSX */
    if (m_size[0])
        m_pixel_ratio = (float) m_fbsize[0] / (float) m_size[0];
#endif

    glViewport(0, 0, m_fbsize[0], m_fbsize[1]);

    draw_contents();
    draw_widgets();

    glfwSwapBuffers(m_glfw_window);
}

void Screen::draw_widgets() {
    nvgBeginFrame(m_nvg_context, m_size[0], m_size[1], m_pixel_ratio);

    draw(m_nvg_context);

    double elapsed = glfwGetTime() - m_last_interaction;

    if (elapsed > 0.5f) {
        /* Draw tooltips */
        const Widget *widget = find_widget(m_mouse_pos);
        if (widget && !widget->tooltip().empty()) {
            int tooltip_width = 150;

            float bounds[4];
            nvgFontFace(m_nvg_context, "sans");
            nvgFontSize(m_nvg_context, 15.0f);
            nvgTextAlign(m_nvg_context, NVG_ALIGN_LEFT | NVG_ALIGN_TOP);
            nvgTextLineHeight(m_nvg_context, 1.1f);
            Vector2i pos = widget->absolute_position() +
                           Vector2i(widget->width() / 2, widget->height() + 10);

            nvgTextBounds(m_nvg_context, pos.x(), pos.y(),
                            widget->tooltip().c_str(), nullptr, bounds);
            int h = (bounds[2] - bounds[0]) / 2;
            if (h > tooltip_width / 2) {
                nvgTextAlign(m_nvg_context, NVG_ALIGN_CENTER | NVG_ALIGN_TOP);
                nvgTextBoxBounds(m_nvg_context, pos.x(), pos.y(), tooltip_width,
                                widget->tooltip().c_str(), nullptr, bounds);

                h = (bounds[2] - bounds[0]) / 2;
            }
            nvgGlobalAlpha(m_nvg_context,
                           std::min(1.0, 2 * (elapsed - 0.5f)) * 0.8);

            nvgBeginPath(m_nvg_context);
            nvgFillColor(m_nvg_context, Color(0, 255));
            nvgRoundedRect(m_nvg_context, bounds[0] - 4 - h, bounds[1] - 4,
                           (int) (bounds[2] - bounds[0]) + 8,
                           (int) (bounds[3] - bounds[1]) + 8, 3);

            int px = (int) ((bounds[2] + bounds[0]) / 2) - h;
            nvgMoveTo(m_nvg_context, px, bounds[1] - 10);
            nvgLineTo(m_nvg_context, px + 7, bounds[1] + 1);
            nvgLineTo(m_nvg_context, px - 7, bounds[1] + 1);
            nvgFill(m_nvg_context);

            nvgFillColor(m_nvg_context, Color(255, 255));
            nvgFontBlur(m_nvg_context, 0.0f);
            nvgTextBox(m_nvg_context, pos.x() - h, pos.y(), tooltip_width,
                       widget->tooltip().c_str(), nullptr);
        }
    }

    nvgEndFrame(m_nvg_context);
}

bool Screen::keyboard_event(int key, int scancode, int action, int modifiers) {
    if (m_focus_path.size() > 0) {
        for (auto it = m_focus_path.rbegin() + 1; it != m_focus_path.rend(); ++it)
            if ((*it)->focused() && (*it)->keyboard_event(key, scancode, action, modifiers))
                return true;
    }

    return false;
}

bool Screen::keyboard_character_event(unsigned int codepoint) {
    if (m_focus_path.size() > 0) {
        for (auto it = m_focus_path.rbegin() + 1; it != m_focus_path.rend(); ++it)
            if ((*it)->focused() && (*it)->keyboard_character_event(codepoint))
                return true;
    }
    return false;
}

bool Screen::resize_event(const Vector2i& size) {
    if (m_resize_callback) {
        m_resize_callback(size);
        return true;
    }
    return false;
}

void Screen::redraw() {
    if (!m_redraw) {
        m_redraw = true;
        glfwPostEmptyEvent();
    }
}

void Screen::cursor_pos_callback_event(double x, double y) {
    Vector2i p((int) x, (int) y);

#if defined(_WIN32) || defined(__linux__) || defined(EMSCRIPTEN)
    p = Vector2i(Vector2f(p) / m_pixel_ratio);
#endif

    m_last_interaction = glfwGetTime();
    try {
        p -= Vector2i(1, 2);

        bool ret = false;
        if (!m_drag_active) {
            Widget *widget = find_widget(p);
            if (widget != nullptr && widget->cursor() != m_cursor) {
                m_cursor = widget->cursor();
                glfwSetCursor(m_glfw_window, m_cursors[(int) m_cursor]);
            }
        } else {
            ret = m_drag_widget->mouse_drag_event(
                p - m_drag_widget->parent()->absolute_position(), p - m_mouse_pos,
                m_mouse_state, m_modifiers);
        }

        if (!ret)
            ret = mouse_motion_event(p, p - m_mouse_pos, m_mouse_state, m_modifiers);

        m_mouse_pos = p;
        m_redraw |= ret;
    } catch (const std::exception &e) {
        std::cerr << "Caught exception in event handler: " << e.what() << std::endl;
    }
}

void Screen::mouse_button_callback_event(int button, int action, int modifiers) {
    m_modifiers = modifiers;
    m_last_interaction = glfwGetTime();
    try {
        if (m_focus_path.size() > 1) {
            const Window *window =
                dynamic_cast<Window *>(m_focus_path[m_focus_path.size() - 2]);
            if (window && window->modal()) {
                if (!window->contains(m_mouse_pos))
                    return;
            }
        }

        if (action == GLFW_PRESS)
            m_mouse_state |= 1 << button;
        else
            m_mouse_state &= ~(1 << button);

        auto drop_widget = find_widget(m_mouse_pos);
        if (m_drag_active && action == GLFW_RELEASE &&
            drop_widget != m_drag_widget)
            m_drag_widget->mouse_button_event(
                m_mouse_pos - m_drag_widget->parent()->absolute_position(), button,
                false, m_modifiers);

        if (drop_widget != nullptr && drop_widget->cursor() != m_cursor) {
            m_cursor = drop_widget->cursor();
            glfwSetCursor(m_glfw_window, m_cursors[(int) m_cursor]);
        }

        if (action == GLFW_PRESS && (button == GLFW_MOUSE_BUTTON_1 || button == GLFW_MOUSE_BUTTON_2)) {
            m_drag_widget = find_widget(m_mouse_pos);
            if (m_drag_widget == this)
                m_drag_widget = nullptr;
            m_drag_active = m_drag_widget != nullptr;
            if (!m_drag_active)
                update_focus(nullptr);
        } else {
            m_drag_active = false;
            m_drag_widget = nullptr;
        }

        m_redraw |= mouse_button_event(m_mouse_pos, button,
                                       action == GLFW_PRESS, m_modifiers);
    } catch (const std::exception &e) {
        std::cerr << "Caught exception in event handler: " << e.what() << std::endl;
    }
}

void Screen::key_callback_event(int key, int scancode, int action, int mods) {
    m_last_interaction = glfwGetTime();
    try {
        m_redraw |= keyboard_event(key, scancode, action, mods);
    } catch (const std::exception &e) {
        std::cerr << "Caught exception in event handler: " << e.what() << std::endl;
    }
}

void Screen::char_callback_event(unsigned int codepoint) {
    m_last_interaction = glfwGetTime();
    try {
        m_redraw |= keyboard_character_event(codepoint);
    } catch (const std::exception &e) {
        std::cerr << "Caught exception in event handler: " << e.what() << std::endl;
    }
}

void Screen::drop_callback_event(int count, const char **filenames) {
    std::vector<std::string> arg(count);
    for (int i = 0; i < count; ++i)
        arg[i] = filenames[i];
    m_redraw |= drop_event(arg);
}

void Screen::scroll_callback_event(double x, double y) {
    m_last_interaction = glfwGetTime();
    try {
        if (m_focus_path.size() > 1) {
            const Window *window =
                dynamic_cast<Window *>(m_focus_path[m_focus_path.size() - 2]);
            if (window && window->modal()) {
                if (!window->contains(m_mouse_pos))
                    return;
            }
        }
        m_redraw |= scroll_event(m_mouse_pos, Vector2f(x, y));
    } catch (const std::exception &e) {
        std::cerr << "Caught exception in event handler: " << e.what() << std::endl;
    }
}

void Screen::resize_callback_event(int, int) {
    Vector2i fb_size, size;
    glfwGetFramebufferSize(m_glfw_window, &fb_size[0], &fb_size[1]);
    glfwGetWindowSize(m_glfw_window, &size[0], &size[1]);

#if defined(_WIN32) || defined(__linux__) || defined(EMSCRIPTEN)
    size = Vector2i(Vector2f(size) / m_pixel_ratio);
#endif

    if (m_fbsize == Vector2i(0, 0) || size == Vector2i(0, 0))
        return;

    m_fbsize = fb_size; m_size = size;
    m_last_interaction = glfwGetTime();

    try {
        m_redraw |= resize_event(m_size);
    } catch (const std::exception &e) {
        std::cerr << "Caught exception in event handler: " << e.what() << std::endl;
    }
}

void Screen::update_focus(Widget *widget) {
    for (auto w: m_focus_path) {
        if (!w->focused())
            continue;
        w->focus_event(false);
    }
    m_focus_path.clear();
    Widget *window = nullptr;
    while (widget) {
        m_focus_path.push_back(widget);
        if (dynamic_cast<Window *>(widget))
            window = widget;
        widget = widget->parent();
    }
    for (auto it = m_focus_path.rbegin(); it != m_focus_path.rend(); ++it)
        (*it)->focus_event(true);

    if (window)
        move_window_to_front((Window *) window);
}

void Screen::dispose_window(Window *window) {
    if (std::find(m_focus_path.begin(), m_focus_path.end(), window) != m_focus_path.end())
        m_focus_path.clear();
    if (m_drag_widget == window)
        m_drag_widget = nullptr;
    remove_child(window);
}

void Screen::center_window(Window *window) {
    if (window->size() == 0) {
        window->set_size(window->preferred_size(m_nvg_context));
        window->perform_layout(m_nvg_context);
    }
    window->set_position((m_size - window->size()) / 2);
}

void Screen::move_window_to_front(Window *window) {
    m_children.erase(std::remove(m_children.begin(), m_children.end(), window), m_children.end());
    m_children.push_back(window);
    /* Brute force topological sort (no problem for a few windows..) */
    bool changed = false;
    do {
        size_t base_index = 0;
        for (size_t index = 0; index < m_children.size(); ++index)
            if (m_children[index] == window)
                base_index = index;
        changed = false;
        for (size_t index = 0; index < m_children.size(); ++index) {
            Popup *pw = dynamic_cast<Popup *>(m_children[index]);
            if (pw && pw->parent_window() == window && index < base_index) {
                move_window_to_front(pw);
                changed = true;
                break;
            }
        }
    } while (changed);
}

NAMESPACE_END(nanogui)
