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
#include <nanogui/metal.h>
#include <nanogui/shader.h>

#include <algorithm>
#include <cstdlib>
#include <iostream>

#if defined(EMSCRIPTEN)
#  include <emscripten/emscripten.h>
#  include <emscripten/html5.h>
#endif

#if defined(_WIN32)
#  ifndef NOMINMAX
#    define NOMINMAX
#  endif
#  undef APIENTRY

#  ifndef WIN32_LEAN_AND_MEAN
#    define WIN32_LEAN_AND_MEAN
# endif
#  include <windows.h>

#  define GLFW_EXPOSE_NATIVE_WGL
#  define GLFW_EXPOSE_NATIVE_WIN32
#  include <GLFW/glfw3native.h>
#endif

/* Allow enforcing the GL2 implementation of NanoVG */

#if defined(NANOGUI_USE_OPENGL) || defined(NANOGUI_USE_GLES)
#  if defined(NANOGUI_USE_OPENGL)
#    define NANOVG_GL3_IMPLEMENTATION
#  elif defined(NANOGUI_USE_GLES)
#    define NANOVG_GLES2_IMPLEMENTATION
#  endif
#  include <nanovg_gl.h>
#  include "opengl_check.h"
#elif defined(NANOGUI_USE_METAL)
#  include <nanovg_mtl.h>
#endif

#if defined(__APPLE__)
#  define GLFW_EXPOSE_NATIVE_COCOA 1
#  include <GLFW/glfw3native.h>
#endif

#if !defined(GL_RGBA_FLOAT_MODE)
#  define GL_RGBA_FLOAT_MODE 0x8820
#endif

NAMESPACE_BEGIN(nanogui)

std::vector<std::pair<GLFWwindow *, Screen *>> __nanogui_screens;

#if defined(NANOGUI_GLAD)
static bool glad_initialized = false;
#endif

/* Calculate pixel ratio for hi-dpi devices. */
static float get_pixel_ratio(GLFWwindow *window) {
#if defined(EMSCRIPTEN)
    return emscripten_get_device_pixel_ratio();
#else
    float xscale, yscale;
    glfwGetWindowContentScale(window, &xscale, &yscale);
    return xscale;
#endif
}

#if defined(EMSCRIPTEN)
static EM_BOOL nanogui_emscripten_resize_callback(int eventType, const EmscriptenUiEvent *, void *) {
    double ratio = emscripten_get_device_pixel_ratio();

    int w1, h1;
    emscripten_get_canvas_element_size("#canvas", &w1, &h1);

    double w2, h2;
    emscripten_get_element_css_size("#canvas", &w2, &h2);

    double w3 = w2 * ratio, h3 = h2 * ratio;

    if (w1 != (int) w3 || h1 != (int) h3)
        emscripten_set_canvas_element_size("#canvas", w3, h3);

    for (auto kv: __nanogui_screens) {
        Screen *screen = kv.second;
        screen->resize_event(Vector2i((int) w2, (int) h2));
        screen->redraw();
    }

    return true;
}
#endif

Screen::Screen()
    : Widget(nullptr), m_glfw_window(nullptr), m_nvg_context(nullptr),
      m_cursor(Cursor::Arrow), m_background(0.3f, 0.3f, 0.32f, 1.f),
      m_shutdown_glfw(false), m_fullscreen(false), m_depth_buffer(false),
      m_stencil_buffer(false), m_float_buffer(false), m_redraw(false),
      m_last_run_mode(RunMode::Stopped) {
    memset(m_cursors, 0, sizeof(GLFWcursor *) * (size_t) Cursor::CursorCount);
#if defined(NANOGUI_USE_OPENGL)
    GLint n_stencil_bits = 0, n_depth_bits = 0;
    GLboolean float_mode;
    CHK(glGetFramebufferAttachmentParameteriv(GL_DRAW_FRAMEBUFFER,
        GL_DEPTH, GL_FRAMEBUFFER_ATTACHMENT_DEPTH_SIZE, &n_depth_bits));
    CHK(glGetFramebufferAttachmentParameteriv(GL_DRAW_FRAMEBUFFER,
        GL_STENCIL, GL_FRAMEBUFFER_ATTACHMENT_STENCIL_SIZE, &n_stencil_bits));
    CHK(glGetBooleanv(GL_RGBA_FLOAT_MODE, &float_mode));
    m_depth_buffer = n_depth_bits > 0;
    m_stencil_buffer = n_stencil_bits > 0;
    m_float_buffer = (bool) float_mode;
#endif
}

Screen::Screen(const Vector2i &size, std::string_view caption, bool resizable,
               bool maximized, bool fullscreen, bool depth_buffer, bool stencil_buffer,
               bool float_buffer, unsigned int gl_major, unsigned int gl_minor)
    : Widget(nullptr), m_glfw_window(nullptr), m_nvg_context(nullptr),
      m_cursor(Cursor::Arrow), m_background(0.3f, 0.3f, 0.32f, 1.f), m_caption(caption),
      m_shutdown_glfw(false), m_fullscreen(fullscreen), m_depth_buffer(depth_buffer),
      m_stencil_buffer(stencil_buffer), m_float_buffer(float_buffer), m_redraw(false),
      m_last_run_mode(RunMode::Stopped) {
    memset(m_cursors, 0, sizeof(GLFWcursor *) * (int) Cursor::CursorCount);

#if defined(__APPLE__)
    auto [capability10bit, capabilityEdr] = test_10bit_edr_support();
    if (!capability10bit && !capabilityEdr)
        m_float_buffer = false;
#elif defined(__linux__)
    // We do not support wide color / HDR on non-Wayland Linux systems
    if (glfwGetPlatform() != GLFW_PLATFORM_WAYLAND)
        m_float_buffer = false;
#endif

#if defined(NANOGUI_USE_OPENGL)
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);

    /* Request a forward compatible OpenGL gl_major.gl_minor core profile context.
       Default value is an OpenGL 3.3 core profile context. */
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, gl_major);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, gl_minor);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#elif defined(NANOGUI_USE_GLES)
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
    glfwWindowHint(GLFW_CONTEXT_CREATION_API, GLFW_EGL_CONTEXT_API);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, NANOGUI_GLES_VERSION);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
#elif defined(NANOGUI_USE_METAL)
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_COCOA_RETINA_FRAMEBUFFER, GLFW_TRUE);
    glfwWindowHint(GLFW_COCOA_GRAPHICS_SWITCHING, GLFW_TRUE);
    m_stencil_buffer = stencil_buffer = false;
#else
#  error Did not select a graphics API!
#endif

    int color_bits = 8, depth_bits = 0, stencil_bits = 0;

    if (stencil_buffer && !depth_buffer)
        throw std::runtime_error(
            "Screen::Screen(): stencil_buffer = True requires depth_buffer = True");
    if (depth_buffer)
        depth_bits = 32;
    if (stencil_buffer) {
        depth_bits = 24;
        stencil_bits = 8;
    }
    if (m_float_buffer)
        color_bits = 16;

    glfwWindowHint(GLFW_RED_BITS, color_bits);
    glfwWindowHint(GLFW_GREEN_BITS, color_bits);
    glfwWindowHint(GLFW_BLUE_BITS, color_bits);
    glfwWindowHint(GLFW_ALPHA_BITS, color_bits);
    glfwWindowHint(GLFW_STENCIL_BITS, stencil_bits);
    glfwWindowHint(GLFW_DEPTH_BITS, depth_bits);

#if (defined(NANOGUI_USE_OPENGL) || defined(NANOGUI_USE_GLES) || defined(NANOGUI_USE_METAL)) && defined(GLFW_FLOATBUFFER)
    glfwWindowHint(GLFW_FLOATBUFFER, m_float_buffer ? GL_TRUE : GL_FALSE);
#else
    m_float_buffer = false;
#endif

    glfwWindowHint(GLFW_VISIBLE, GL_FALSE);
    glfwWindowHint(GLFW_RESIZABLE, resizable ? GL_TRUE : GL_FALSE);
    glfwWindowHint(GLFW_MAXIMIZED, maximized ? GL_TRUE : GL_FALSE);
    glfwWindowHint(GLFW_SCALE_TO_MONITOR, GLFW_TRUE);

    glfwWindowHintString(GLFW_X11_CLASS_NAME, m_caption.c_str());
    glfwWindowHintString(GLFW_X11_INSTANCE_NAME, m_caption.c_str());
    glfwWindowHintString(GLFW_WAYLAND_APP_ID, m_caption.c_str());

    for (int i = 0; i < 2; ++i) {
        if (fullscreen) {
            GLFWmonitor *monitor = glfwGetPrimaryMonitor();
            const GLFWvidmode *mode = glfwGetVideoMode(monitor);
            m_glfw_window = glfwCreateWindow(mode->width, mode->height,
                                             m_caption.c_str(), monitor, nullptr);
        } else {
            m_glfw_window = glfwCreateWindow(size.x(), size.y(),
                                             m_caption.c_str(), nullptr, nullptr);
        }

        if (m_glfw_window == nullptr && m_float_buffer) {
            m_float_buffer = false;
#if defined(GLFW_FLOATBUFFER)
            glfwWindowHint(GLFW_FLOATBUFFER, GL_FALSE);
#endif

            glfwWindowHint(GLFW_RED_BITS, 10);
            glfwWindowHint(GLFW_GREEN_BITS, 10);
            glfwWindowHint(GLFW_BLUE_BITS, 10);
            glfwWindowHint(GLFW_ALPHA_BITS, 2);
        } else {
            break;
        }
    }

    if (!m_glfw_window) {
        (void) gl_major; (void) gl_minor;
#if defined(NANOGUI_USE_OPENGL)
        throw std::runtime_error("Could not create an OpenGL " +
                                 std::to_string(gl_major) + "." +
                                 std::to_string(gl_minor) + " context!");
#elif defined(NANOGUI_USE_GLES)
        throw std::runtime_error("Could not create a GLES 2 context!");
#elif defined(NANOGUI_USE_METAL)
        throw std::runtime_error(
            "Could not create a GLFW window for rendering using Metal!");
#endif
    }

#if defined(NANOGUI_USE_OPENGL) || defined(NANOGUI_USE_GLES)
    glfwMakeContextCurrent(m_glfw_window);
#endif

    glfwSetInputMode(m_glfw_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

#if defined(NANOGUI_GLAD)
    if (!glad_initialized) {
        glad_initialized = true;
        if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
            throw std::runtime_error("Could not initialize GLAD!");
        glGetError(); // pull and ignore unhandled errors like GL_INVALID_ENUM
    }
#endif

#if defined(NANOGUI_USE_OPENGL) || defined(NANOGUI_USE_GLES)
    m_bits_per_sample = glfwGetWindowAttrib(m_glfw_window, GLFW_RED_BITS);
    if (m_float_buffer && m_bits_per_sample < 16) {
        fprintf(stderr, "Could not allocate floating point framebuffer.\n");
        m_float_buffer = false;
    }
#else
    m_bits_per_sample = m_float_buffer ? 16 : 8;
#endif

    float display_sdr_white_level = glfwGetWindowSdrWhiteLevel(m_glfw_window);
    uint32_t display_transfer_function = glfwGetWindowTransfer(m_glfw_window);
    uint32_t display_primaries = glfwGetWindowPrimaries(m_glfw_window);

#ifdef _WIN32
    // On Windows, it is expensive to query the display SDR level, so we do it once on startup and cache the value.
    m_display_sdr_white_level_override = display_sdr_white_level;
#endif

    const char* env_sdr_white = std::getenv("NANOGUI_CM_SDR_WHITE_LEVEL");
    if (env_sdr_white != nullptr && strlen(env_sdr_white) > 0)
        m_display_sdr_white_level_override = display_sdr_white_level = std::stof(env_sdr_white);

    m_wants_color_management = display_primaries != 1 ||
                               display_transfer_function != 10 ||
                               display_sdr_white_level != 80.0f;

    glfwGetFramebufferSize(m_glfw_window, &m_fbsize[0], &m_fbsize[1]);

#if defined(NANOGUI_USE_OPENGL) || defined(NANOGUI_USE_GLES)
    CHK(glViewport(0, 0, m_fbsize[0], m_fbsize[1]));
    CHK(glClearColor(m_background[0], m_background[1],
                     m_background[2], m_background[3]));
    CHK(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT |
                GL_STENCIL_BUFFER_BIT));
    glfwSwapBuffers(m_glfw_window);
#endif

#if defined(__APPLE__)
    /* Poll for events once before starting a potentially
       lengthy loading process. This is needed to be
       classified as "interactive" by other software such
       as iTerm2 */

    glfwPollEvents();
#endif

    /* Propagate GLFW events to the appropriate Screen instance */
    glfwSetCursorPosCallback(m_glfw_window,
        [](GLFWwindow *w, double x, double y) {
            for (auto kv: __nanogui_screens) {
                if (w == kv.first) {
                    kv.second->cursor_pos_callback_event(x, y);
                    break;
                }
            }
        }
    );

    glfwSetMouseButtonCallback(m_glfw_window,
        [](GLFWwindow *w, int button, int action, int modifiers) {
            for (auto kv: __nanogui_screens) {
                if (w == kv.first) {
                    kv.second->mouse_button_callback_event(button, action, modifiers);
                    break;
                }
            }
        }
    );

    glfwSetKeyCallback(m_glfw_window,
        [](GLFWwindow *w, int key, int scancode, int action, int mods) {
            for (auto kv: __nanogui_screens) {
                if (w == kv.first) {
                    kv.second->key_callback_event(key, scancode, action, mods);
                    break;
                }
            }
        }
    );

    glfwSetCharCallback(m_glfw_window,
        [](GLFWwindow *w, unsigned int codepoint) {
            for (auto kv: __nanogui_screens) {
                if (w == kv.first) {
                    kv.second->char_callback_event(codepoint);
                    break;
                }
            }
        }
    );

    glfwSetDropCallback(m_glfw_window,
        [](GLFWwindow *w, int count, const char **filenames) {
            for (auto kv: __nanogui_screens) {
                if (w == kv.first) {
                    kv.second->drop_callback_event(count, filenames);
                    break;
                }
            }
        }
    );

    glfwSetScrollCallback(m_glfw_window,
        [](GLFWwindow *w, double x, double y) {
            for (auto kv: __nanogui_screens) {
                if (w == kv.first) {
                    kv.second->scroll_callback_event(x, y);
                    break;
                }
            }
        }
    );

    /* React to framebuffer size events -- includes window
       size events and also catches things like dragging
       a window from a Retina-capable screen to a normal
       screen on Mac OS X */
    glfwSetFramebufferSizeCallback(m_glfw_window,
        [](GLFWwindow* w, int width, int height) {
            for (auto kv: __nanogui_screens) {
                if (w == kv.first) {
                    kv.second->resize_callback_event(width, height);
                    break;
                }
            }
        }
    );

    // notify when the screen has lost focus (e.g. application switch)
    glfwSetWindowFocusCallback(m_glfw_window,
        [](GLFWwindow *w, int focused) {
            for (auto kv: __nanogui_screens) {
                if (w == kv.first) {
                    kv.second->focus_event(focused != 0);
                    break;
                }
            }
        }
    );

    glfwSetWindowContentScaleCallback(m_glfw_window,
        [](GLFWwindow* w, float, float) {
            for (auto kv: __nanogui_screens) {
                if (w == kv.first) {
                    Screen *s = kv.second;
                    s->m_pixel_ratio = get_pixel_ratio(w);
                    s->resize_callback_event(s->m_size.x(), s->m_size.y());
                    break;
                }
            }
        }
    );

    // notify when the screen was maximized or restored
    glfwSetWindowMaximizeCallback(m_glfw_window,
        [](GLFWwindow *w, int maximized) {
            for (auto kv: __nanogui_screens) {
                if (w == kv.first) {
                    kv.second->maximize_event(maximized != 0);
                    break;
                }
            }
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

#if defined(EMSCRIPTEN)
    double w, h;
    emscripten_get_element_css_size("#canvas", &w, &h);
    double ratio = emscripten_get_device_pixel_ratio(),
           w2 = w * ratio, h2 = h * ratio;

    if (w != m_size[0] || h != m_size[1]) {
        /* The canvas element is configured as width/height: auto, expand to
           the available space instead of using the specified window resolution */
        nanogui_emscripten_resize_callback(0, nullptr, nullptr);
        emscripten_set_resize_callback(nullptr, nullptr, false,
                                       nanogui_emscripten_resize_callback);
    } else if (w != w2 || h != h2) {
        /* Configure for rendering on a high-DPI display */
        emscripten_set_canvas_element_size("#canvas", (int) w2, (int) h2);
        emscripten_set_element_css_size("#canvas", w, h);
    }
    m_fbsize = Vector2i((int) w2, (int) h2);
    m_size = Vector2i((int) w, (int) h);
#elif defined(_WIN32) || defined(__linux__)
    if (glfwGetPlatform() != GLFW_PLATFORM_WAYLAND && m_pixel_ratio != 1 && !m_fullscreen)
        glfwSetWindowSize(window, m_size.x() * m_pixel_ratio,
                                  m_size.y() * m_pixel_ratio);
#endif

#if defined(NANOGUI_GLAD)
    if (!glad_initialized) {
        glad_initialized = true;
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
            throw std::runtime_error("Could not initialize GLAD!");
        glGetError(); // pull and ignore unhandled errors like GL_INVALID_ENUM
    }
#endif

    int flags = NVG_ANTIALIAS;
    if (m_stencil_buffer)
       flags |= NVG_STENCIL_STROKES;
#if !defined(NDEBUG)
    flags |= NVG_DEBUG;
#endif

#if defined(NANOGUI_USE_OPENGL)
    m_nvg_context = nvgCreateGL3(flags);
#elif defined(NANOGUI_USE_GLES)
    m_nvg_context = nvgCreateGLES2(flags);
#elif defined(NANOGUI_USE_METAL)
    m_nswin = glfwGetCocoaWindow(window);
    metal_window_init(m_nswin, m_float_buffer);
    metal_window_set_size(m_nswin, m_fbsize);
    m_nvg_context = nvgCreateMTL(metal_layer(),
                                 metal_command_queue(),
                                 flags | NVG_DOUBLE_BUFFER);
#endif

    if (!m_nvg_context)
        throw std::runtime_error("Could not initialize NanoVG!");

    m_visible = glfwGetWindowAttrib(window, GLFW_VISIBLE) != 0;
    set_theme(new Theme(m_nvg_context));
    m_mouse_pos = Vector2i(0);
    m_mouse_state = m_modifiers = 0;
    m_drag_active = false;
    m_last_interaction = glfwGetTime();
    m_redraw = true;
    __nanogui_screens.emplace_back(m_glfw_window, this);

    for (size_t i = 0; i < (size_t) Cursor::CursorCount; ++i)
        m_cursors[i] = glfwCreateStandardCursor(GLFW_ARROW_CURSOR + (int) i);

    if (m_stencil_buffer || m_depth_buffer) {
        m_depth_stencil_texture = new Texture(
            m_stencil_buffer ? Texture::PixelFormat::DepthStencil
                             : Texture::PixelFormat::Depth,
            Texture::ComponentFormat::UInt32,
            m_fbsize,
            Texture::InterpolationMode::Nearest,
            Texture::InterpolationMode::Nearest,
            Texture::WrapMode::ClampToEdge,
            1,
            Texture::TextureFlags::RenderTarget
        );
    }

#if defined(NANOGUI_USE_OPENGL) || defined(NANOGUI_USE_GLES)
    // Initialize color management resources if needed
    if (m_wants_color_management) {
        m_color_texture = new Texture(
            pixel_format(),
            Texture::ComponentFormat::Float32,
            m_fbsize,
            Texture::InterpolationMode::Nearest,
            Texture::InterpolationMode::Nearest,
            Texture::WrapMode::ClampToEdge,
            1,
            Texture::TextureFlags::ShaderRead | Texture::TextureFlags::RenderTarget
        );

        m_color_pass = new ColorPass(
            m_color_texture,
            m_depth_buffer ? m_depth_stencil_texture : nullptr,
            m_stencil_buffer ? m_depth_stencil_texture : nullptr,
            bits_per_sample(),
            m_float_buffer
        );
    }
#endif

    /// Fixes retina display-related font rendering issue (#185)
    nvgBeginFrame(m_nvg_context, m_size[0], m_size[1], m_pixel_ratio);
    nvgEndFrame(m_nvg_context);
}

Screen::~Screen() {
    auto it = std::find_if(
        __nanogui_screens.begin(), __nanogui_screens.end(),
        [w = m_glfw_window](const auto &kv) { return kv.first == w; });
    if (it != __nanogui_screens.end())
        __nanogui_screens.erase(it);
    else
        fprintf(stderr, "NanoGUI: could not unregister screen\n");

    for (size_t i = 0; i < (size_t) Cursor::CursorCount; ++i) {
        if (m_cursors[i])
            glfwDestroyCursor(m_cursors[i]);
    }

    if (m_nvg_context) {
#if defined(NANOGUI_USE_OPENGL)
        nvgDeleteGL3(m_nvg_context);
#elif defined(NANOGUI_USE_GLES)
        nvgDeleteGLES2(m_nvg_context);
#elif defined(NANOGUI_USE_METAL)
        metal_sync();
        nvgDeleteMTL(m_nvg_context);
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

void Screen::set_caption(std::string_view caption) {
    if (caption != m_caption) {
        m_caption = caption;
        glfwSetWindowTitle(m_glfw_window, m_caption.c_str());
    }
}

void Screen::move_window(const Vector2i &rel) {
    // Wayland does not support moving windows, so we do nothing
    if (glfwGetPlatform() == GLFW_PLATFORM_WAYLAND)
        return;

    Vector2i pos;
    glfwGetWindowPos(m_glfw_window, &pos[0], &pos[1]);
    pos += rel;

#if defined(_WIN32) || defined(__linux__) || defined(EMSCRIPTEN)
    auto scaled_size = m_size * m_pixel_ratio;
#else
    auto scaled_size = m_size;
#endif

    int monitor_count = 0;
    GLFWmonitor **monitors = glfwGetMonitors(&monitor_count);
    if (monitor_count > 0) {
        Vector2i work_pos{std::numeric_limits<int>::max()};
        Vector2i work_size{std::numeric_limits<int>::min()};
        for (int i = 0; i < monitor_count; ++i) {
            Vector2i monitor_work_pos, monitor_work_size;
            glfwGetMonitorWorkarea(monitors[i], &monitor_work_pos[0],
                                   &monitor_work_pos[1], &monitor_work_size[0],
                                   &monitor_work_size[1]);
            work_pos = min(work_pos, monitor_work_pos);
            work_size = max(work_size, monitor_work_size);
        }

        pos = max(min(pos, work_pos + work_size - scaled_size), work_pos);
    }

#if defined(_WIN32) || defined(__linux__) || defined(EMSCRIPTEN)
    glfwSetWindowPos(m_glfw_window, pos.x() * m_pixel_ratio,
                     pos.y() * m_pixel_ratio);
#else
    glfwSetWindowPos(m_glfw_window, pos.x(), pos.y());
#endif
}

void Screen::set_size(const Vector2i &size) {
    Widget::set_size(size);

    auto targetSize = size;
#if defined(_WIN32) || defined(__linux__) || defined(EMSCRIPTEN)
    if (glfwGetPlatform() != GLFW_PLATFORM_WAYLAND)
        targetSize = Vector2i(size.x() * m_pixel_ratio, size.y() * m_pixel_ratio);
#endif

    glfwSetWindowSize(m_glfw_window, targetSize.x(), targetSize.y());
}

void Screen::clear() {
#if defined(NANOGUI_USE_OPENGL) || defined(NANOGUI_USE_GLES)
    CHK(glClearColor(m_background[0], m_background[1], m_background[2], m_background[3]));
    CHK(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT));
#elif defined(NANOGUI_USE_METAL)
    mnvgClearWithColor(m_nvg_context, m_background);
#endif
}

void Screen::draw_setup() {
#if defined(NANOGUI_USE_OPENGL) || defined(NANOGUI_USE_GLES)
    glfwMakeContextCurrent(m_glfw_window);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, framebuffer_handle());

    if (m_color_pass)
        m_color_pass->begin();

#elif defined(NANOGUI_USE_METAL)
    metal_window_set_size(m_nswin, m_fbsize);
    metal_window_next_drawable(m_nswin, &m_metal_drawable, &m_metal_texture);
    mnvgSetColorTexture(m_nvg_context, m_metal_texture);
#endif

#if !defined(EMSCRIPTEN)
    glfwGetFramebufferSize(m_glfw_window, &m_fbsize[0], &m_fbsize[1]);
    glfwGetWindowSize(m_glfw_window, &m_size[0], &m_size[1]);
#else
    emscripten_get_canvas_element_size("#canvas", &m_size[0], &m_size[1]);
    m_fbsize = m_size;
#endif

    RunMode run_mode = nanogui::run_mode();
    if (run_mode != m_last_run_mode) {
#if !defined(NANOGUI_USE_METAL)
        int interval = 0;
        if (run_mode != RunMode::Eager) {
            bool swap_control = glfwExtensionSupported("WGL_EXT_swap_control_tear") ||
                                glfwExtensionSupported("GLX_EXT_swap_control_tear");
            interval = swap_control ? -1 : 1;
        }
        glfwSwapInterval(interval);
#else
        bool vsync = run_mode == RunMode::VSync;
        metal_window_set_vsync(m_nswin, vsync);
#endif
        m_last_run_mode = run_mode;
    }

#if defined(_WIN32) || defined(__linux__) || defined(EMSCRIPTEN)
    if (glfwGetPlatform() != GLFW_PLATFORM_WAYLAND) {
        m_fbsize = m_size;
        m_size = Vector2i(Vector2f(m_size) / m_pixel_ratio);
    }
#else
    /* Recompute pixel ratio on OSX */
    if (m_size[0])
        m_pixel_ratio = (float) m_fbsize[0] / (float) m_size[0];
#if defined(NANOGUI_USE_METAL)
    metal_window_set_content_scale(m_nswin, m_pixel_ratio);
#endif
#endif

#if defined(NANOGUI_USE_OPENGL) || defined(NANOGUI_USE_GLES)
    CHK(glViewport(0, 0, m_fbsize[0], m_fbsize[1]));
#endif
}

void Screen::draw_teardown() {
#if defined(NANOGUI_USE_OPENGL) || defined(NANOGUI_USE_GLES)
    if (m_color_pass) {
        m_color_pass->end();

        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
        glViewport(0, 0, m_fbsize[0], m_fbsize[1]);
        CHK(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT));

        m_color_pass->configure(m_glfw_window, m_display_sdr_white_level_override);
        m_color_pass->draw_quad();
    }

    glfwSwapBuffers(m_glfw_window);
#elif defined(NANOGUI_USE_METAL)
    mnvgSetColorTexture(m_nvg_context, nullptr);
    metal_present_and_release_drawable(m_metal_drawable);
    m_metal_drawable = m_metal_texture = nullptr;
#endif
}

void Screen::draw_all() {
    if (run_mode() != RunMode::Lazy || m_redraw) {
#if defined(NANOGUI_USE_METAL)
        void *pool = autorelease_init();
#endif

        draw_setup();
        draw_contents();
        draw_widgets();
        draw_teardown();

#if defined(NANOGUI_USE_METAL)
        autorelease_release(pool);
#endif
        m_redraw = false;
    }
}

void Screen::draw_contents() {
    clear();
}

void Screen::nvg_flush() {
    NVGparams *params = nvgInternalParams(m_nvg_context);
    params->renderFlush(params->userPtr);
    params->renderViewport(params->userPtr, m_size[0], m_size[1], m_pixel_ratio);
}

void Screen::draw_widgets() {
    nvgBeginFrame(m_nvg_context, m_size[0], m_size[1], m_pixel_ratio);

    draw(m_nvg_context);

    double elapsed = glfwGetTime() - m_last_interaction;

    if (elapsed > 0.2f) {
        /* Draw tooltips */
        const Widget *widget = find_widget(m_mouse_pos);
        while (widget && widget->tooltip().empty())
            widget = widget->parent();

        if (widget && !widget->tooltip().empty()) {
            int tooltip_width = 180;

            float bounds[4];
            nvgFontFace(m_nvg_context, "sans");
            nvgFontSize(m_nvg_context, 15.0f);
            nvgTextAlign(m_nvg_context, NVG_ALIGN_LEFT | NVG_ALIGN_TOP);
            nvgTextLineHeight(m_nvg_context, 1.1f);
            Vector2i pos = widget->absolute_position() +
                           Vector2i(widget->width() / 2, widget->height() + 10);

            std::string_view tooltip = widget->tooltip();
            nvgTextBounds(m_nvg_context, pos.x(), pos.y(),
                          tooltip.data(), tooltip.data() + tooltip.size(), bounds);

            int h = (bounds[2] - bounds[0]) / 2;
            if (h > tooltip_width / 2) {
                nvgTextAlign(m_nvg_context, NVG_ALIGN_CENTER | NVG_ALIGN_TOP);
                nvgTextBoxBounds(m_nvg_context, pos.x(), pos.y(), tooltip_width,
                                tooltip.data(), tooltip.data() + tooltip.size(), bounds);

                h = (bounds[2] - bounds[0]) / 2;
            }
            int shift = 0;

            if (pos.x() - h - 8 < 0) {
                /* Keep tooltips on screen */
                shift = pos.x() - h - 8;
                pos.x() -= shift;
                bounds[0] -= shift;
                bounds[2] -= shift;
            }

            nvgGlobalAlpha(m_nvg_context, 0.8f);

            nvgBeginPath(m_nvg_context);
            nvgFillColor(m_nvg_context, Color(0, 255));
            nvgRoundedRect(m_nvg_context, bounds[0] - 4 - h, bounds[1] - 4,
                           (int) (bounds[2] - bounds[0]) + 8,
                           (int) (bounds[3] - bounds[1]) + 8, 3);

            int px = (int) ((bounds[2] + bounds[0]) / 2) - h + shift;
            nvgMoveTo(m_nvg_context, px, bounds[1] - 10);
            nvgLineTo(m_nvg_context, px + 7, bounds[1] + 1);
            nvgLineTo(m_nvg_context, px - 7, bounds[1] + 1);
            nvgFill(m_nvg_context);

            nvgFillColor(m_nvg_context, Color(255, 255));
            nvgFontBlur(m_nvg_context, 0.0f);
            nvgTextBox(m_nvg_context, pos.x() - h, pos.y(), tooltip_width,
                       tooltip.data(), tooltip.data() + tooltip.size());
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
    if (m_resize_callback)
        m_resize_callback(size);
    return true;
}

void Screen::redraw() {
    glfwPostEmptyEvent();
    m_redraw = true;
}

void Screen::cursor_pos_callback_event(double x, double y) {
#if defined(_WIN32) || defined(__linux__) || defined(EMSCRIPTEN)
    if (glfwGetPlatform() != GLFW_PLATFORM_WAYLAND) {
        x /= m_pixel_ratio;
        y /= m_pixel_ratio;
    }
#endif

    x -= 1; y -= 2;

    Vector2i p((int) x, (int) y);
    Vector2f p_f((float) x, (float) y);

    m_last_interaction = glfwGetTime();
    try {

        bool ret = false;
        if (!m_drag_active) {
            Widget *widget = find_widget(p);
            while (widget && widget->cursor() == Cursor::Arrow)
                widget = widget->parent();

            m_cursor = widget ? widget->cursor() : Cursor::Arrow;
            glfwSetCursor(m_glfw_window, m_cursors[(int)m_cursor]);
        } else {
            ret = m_drag_widget->mouse_drag_event(
                p - m_drag_widget->parent()->absolute_position(), p - m_mouse_pos,
                m_mouse_state, m_modifiers);
        }

        if (!ret) {
            ret = mouse_motion_event(p, p - m_mouse_pos, m_mouse_state, m_modifiers);
            ret = mouse_motion_event_f(p, p_f - m_mouse_pos_f, m_mouse_state, m_modifiers);
        }

        m_mouse_pos = p;
        m_mouse_pos_f = p_f;
        m_redraw |= ret;
    } catch (const std::exception &e) {
        std::cerr << "Caught exception in event handler: " << e.what() << std::endl;
    }
}

bool Screen::mouse_motion_event_f(const Vector2f &, const Vector2f &, int, int) {
    return false;
}

void Screen::mouse_button_callback_event(int button, int action, int modifiers) {
    m_modifiers = modifiers;
    m_last_interaction = glfwGetTime();

    #if defined(__APPLE__)
        if (button == GLFW_MOUSE_BUTTON_1 && modifiers == GLFW_MOD_CONTROL)
            button = GLFW_MOUSE_BUTTON_2;
    #endif

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
            drop_widget != m_drag_widget) {
            m_redraw |= m_drag_widget->mouse_button_event(
                m_mouse_pos - m_drag_widget->parent()->absolute_position(), button,
                false, m_modifiers);
        }

        if (drop_widget != nullptr && drop_widget->cursor() != m_cursor) {
            Widget *widget = drop_widget;
            while (widget && widget->cursor() == Cursor::Arrow)
                widget = widget->parent();

            m_cursor = widget ? widget->cursor() : Cursor::Arrow;
            glfwSetCursor(m_glfw_window, m_cursors[(int) m_cursor]);
        }

        bool btn12 = button == GLFW_MOUSE_BUTTON_1 || button == GLFW_MOUSE_BUTTON_2;

        if (!m_drag_active && action == GLFW_PRESS && btn12) {
            m_drag_widget = find_widget(m_mouse_pos);
            if (m_drag_widget == this)
                m_drag_widget = nullptr;
            m_drag_active = m_drag_widget != nullptr;
            if (!m_drag_active)
                update_focus(nullptr);
        } else if (m_drag_active && action == GLFW_RELEASE && btn12) {
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
#if defined(EMSCRIPTEN)
    return;
#endif

    Vector2i fb_size, size;
    glfwGetFramebufferSize(m_glfw_window, &fb_size[0], &fb_size[1]);
    glfwGetWindowSize(m_glfw_window, &size[0], &size[1]);
    if (fb_size == Vector2i(0, 0) || size == Vector2i(0, 0))
        return;
    m_fbsize = fb_size; m_size = size;

#if defined(_WIN32) || defined(__linux__) || defined(EMSCRIPTEN)
    if (glfwGetPlatform() != GLFW_PLATFORM_WAYLAND)
        m_size = Vector2i(Vector2f(m_size) / m_pixel_ratio);
#endif

    m_last_interaction = glfwGetTime();

    if (m_depth_stencil_texture)
        m_depth_stencil_texture->resize(fb_size);

#if defined(NANOGUI_USE_OPENGL) || defined(NANOGUI_USE_GLES)
    if (m_color_texture)
        m_color_texture->resize(fb_size);

    if (m_color_pass)
        m_color_pass->resize(fb_size);
#endif

    try {
        resize_event(m_size);
    } catch (const std::exception &e) {
        std::cerr << "Caught exception in event handler: " << e.what() << std::endl;
    }

    redraw();
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

bool Screen::maximize_event(bool /*maximized*/) {
    return false;
}

void Screen::dispose_widget(Widget *widget) {
    if (std::find(m_focus_path.begin(), m_focus_path.end(), widget) != m_focus_path.end())
        m_focus_path.clear();

    if (m_drag_widget == widget) {
        m_drag_widget = nullptr;
        m_drag_active = false;
    }

    for (auto child : widget->children())
        dispose_widget(child);
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

bool Screen::tooltip_fade_in_progress() const {
    double elapsed = glfwGetTime() - m_last_interaction;
    if (elapsed < 0.25f || elapsed > 1.25f)
        return false;
    /* Temporarily increase the frame rate to fade in the tooltip */
    const Widget *widget = find_widget(m_mouse_pos);
    return widget && !widget->tooltip().empty();
}

#if defined(NANOGUI_USE_OPENGL) || defined(NANOGUI_USE_GLES)
uint32_t Screen::framebuffer_handle() const {
    return applies_color_management() ? m_color_pass->framebuffer_handle() : 0;
}
#endif

Texture::PixelFormat Screen::pixel_format() const {
#if defined(NANOGUI_USE_METAL)
    if (!m_float_buffer)
        return Texture::PixelFormat::BGRA;
#endif
    return Texture::PixelFormat::RGBA;
}

Texture::ComponentFormat Screen::component_format() const {
    if (m_float_buffer)
        return Texture::ComponentFormat::Float16;
    else
        return Texture::ComponentFormat::UInt8;
}

#if defined(NANOGUI_USE_METAL)
void *Screen::metal_layer() const {
    return metal_window_layer(m_nswin);
}
#endif

NAMESPACE_END(nanogui)
