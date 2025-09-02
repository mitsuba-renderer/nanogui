/*
    src/screen.cpp -- Top-level widget and interface between NanoGUI and GLFW

    A significant redesign of this code was contributed by Christian Schueller.

    NanoGUI was developed by Wenzel Jakob <wenzel.jakob@epfl.ch>.
    The widget drawing code is based on the NanoVG demo application
    by Mikko Mononen.

    All rights reserved. Use of this source code is governed by a
    BSD-style license that can be found in the LICENSE.txt file.
*/

#include <nanogui/chroma.h>
#include <nanogui/screen.h>
#include <nanogui/theme.h>
#include <nanogui/opengl.h>
#include <nanogui/window.h>
#include <nanogui/popup.h>
#include <nanogui/metal.h>
#include <nanogui/renderpass.h>
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
    if (env_sdr_white != nullptr)
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

#if defined(NANOGUI_USE_METAL)
    if (depth_buffer) {
        m_depth_stencil_texture = new Texture(
            stencil_buffer ? Texture::PixelFormat::DepthStencil
                           : Texture::PixelFormat::Depth,
            Texture::ComponentFormat::Float32,
            framebuffer_size(),
            Texture::InterpolationMode::Bilinear,
            Texture::InterpolationMode::Bilinear,
            Texture::WrapMode::ClampToEdge,
            1,
            Texture::TextureFlags::RenderTarget
        );
    }
#endif
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
    void *nswin = glfwGetCocoaWindow(window);
    metal_window_init(nswin, m_float_buffer);
    metal_window_set_size(nswin, m_fbsize);
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

#if defined(NANOGUI_USE_OPENGL) || defined(NANOGUI_USE_GLES)
    // Initialize color management resources if needed
    if (m_wants_color_management) {
        m_cm_texture = new Texture(
            pixel_format(),
            Texture::ComponentFormat::Float32,
            m_fbsize,
            Texture::InterpolationMode::Nearest,
            Texture::InterpolationMode::Nearest,
            Texture::WrapMode::ClampToEdge,
            1,
            Texture::TextureFlags::ShaderRead | Texture::TextureFlags::RenderTarget
        );

        if (m_stencil_buffer || m_depth_buffer) {
            m_cm_depth_texture = new Texture(
                m_stencil_buffer ? Texture::PixelFormat::DepthStencil : Texture::PixelFormat::Depth,
                Texture::ComponentFormat::UInt32,
                m_fbsize,
                Texture::InterpolationMode::Nearest,
                Texture::InterpolationMode::Nearest,
                Texture::WrapMode::ClampToEdge,
                1,
                Texture::TextureFlags::RenderTarget
            );
        }

        m_cm_render_pass = new RenderPass(
            {m_cm_texture},
            m_depth_buffer ? m_cm_depth_texture : nullptr,
            m_stencil_buffer ? m_cm_depth_texture : nullptr,
            nullptr,
            true
        );

        // Disable depth testing. We've only got a depth buffer in order to have a stencil buffer.
        m_cm_render_pass->set_depth_test(RenderPass::DepthTest::Always, true);

        m_dither_matrix = new Texture{
            Texture::PixelFormat::R,
            Texture::ComponentFormat::Float32,
            Vector2i{nanogui::DITHER_MATRIX_SIZE},
            Texture::InterpolationMode::Nearest,
            Texture::InterpolationMode::Nearest,
            Texture::WrapMode::Repeat,
        };

        const float dither_scale = has_float_buffer() ? 0.0f : (1.0f / (1u << bits_per_sample()));
        auto dither_matrix = nanogui::ditherMatrix(dither_scale);
        m_dither_matrix->upload((uint8_t*)dither_matrix.data());

#    if defined(NANOGUI_USE_OPENGL)
        std::string preamble = "#version 110\n";
#    elif defined(NANOGUI_USE_GLES)
        std::string preamble = "#version 100\nprecision highp float; precision highp sampler2D;\n";
#    endif
        auto vertex_shader = preamble + R"glsl(
            uniform vec2 ditherScale;

            attribute vec2 position;
            varying vec2 imageUv;
            varying vec2 ditherUv;

            void main() {
                vec2 pos = position * 0.5 + 0.5; // Convert from [-1, 1] to [0, 1]
                imageUv = pos;
                ditherUv = pos * ditherScale;

                gl_Position = vec4(position, 1.0, 1.0);
            }
        )glsl";
        auto fragment_shader = preamble + R"glsl(
            varying vec2 imageUv;
            varying vec2 ditherUv;

            uniform sampler2D framebufferTexture;
            uniform sampler2D ditherMatrix;

            uniform float displaySdrWhiteLevel;
            uniform float minLuminance;
            uniform float maxLuminance;

            uniform int outTransferFunction;
            uniform mat3 displayColorMatrix;
            uniform bool clipToUnitInterval;

            #define CM_TRANSFER_FUNCTION_BT1886     1
            #define CM_TRANSFER_FUNCTION_GAMMA22    2
            #define CM_TRANSFER_FUNCTION_GAMMA28    3
            #define CM_TRANSFER_FUNCTION_ST240      4
            #define CM_TRANSFER_FUNCTION_EXT_LINEAR 5
            #define CM_TRANSFER_FUNCTION_LOG_100    6
            #define CM_TRANSFER_FUNCTION_LOG_316    7
            #define CM_TRANSFER_FUNCTION_XVYCC      8
            #define CM_TRANSFER_FUNCTION_SRGB       9
            #define CM_TRANSFER_FUNCTION_EXT_SRGB   10
            #define CM_TRANSFER_FUNCTION_ST2084_PQ  11
            #define CM_TRANSFER_FUNCTION_ST428      12
            #define CM_TRANSFER_FUNCTION_HLG        13

            #define SRGB_POW 2.4
            #define SRGB_CUT 0.0031308
            #define SRGB_SCALE 12.92
            #define SRGB_ALPHA 1.055

            #define BT1886_POW (1.0 / 0.45)
            #define BT1886_CUT 0.018053968510807
            #define BT1886_SCALE 4.5
            #define BT1886_ALPHA (1.0 + 5.5 * BT1886_CUT)

            // See http://car.france3.mars.free.fr/HD/INA-%2026%20jan%2006/SMPTE%20normes%20et%20confs/s240m.pdf
            #define ST240_POW (1.0 / 0.45)
            #define ST240_CUT 0.0228
            #define ST240_SCALE 4.0
            #define ST240_ALPHA 1.1115

            #define ST428_POW 2.6
            #define ST428_SCALE (52.37 / 48.0)

            #define PQ_M1 0.1593017578125
            #define PQ_M2 78.84375
            #define PQ_INV_M1 (1.0 / PQ_M1)
            #define PQ_INV_M2 (1.0 / PQ_M2)
            #define PQ_C1 0.8359375
            #define PQ_C2 18.8515625
            #define PQ_C3 18.6875

            #define HLG_D_CUT (1.0 / 12.0)
            #define HLG_E_CUT 0.5
            #define HLG_A 0.17883277
            #define HLG_B 0.28466892
            #define HLG_C 0.55991073

            #define M_E 2.718281828459045

            vec3 mixb(vec3 a, vec3 b, bvec3 mask) {
                return mix(a, b, vec3(mask));
            }

            // The primary source for these transfer functions is https://www.itu.int/dms_pubrec/itu-r/rec/bt/R-REC-BT.1361-0-199802-W!!PDF-E.pdf
            // Outputs are assumed to have 1 == SDR White which is different for each transfer function.
            vec3 tfInvPQ(vec3 color) {
                vec3 E = pow(max(color.rgb, vec3(0.0)), vec3(PQ_INV_M2));
                return pow(
                    (max(E - PQ_C1, vec3(0.0))) / max(PQ_C2 - PQ_C3 * E, vec3(1e-5)),
                    vec3(PQ_INV_M1)
                );
            }

            vec3 tfInvHLG(vec3 color) {
                bvec3 isLow = lessThanEqual(color.rgb, vec3(HLG_E_CUT));
                vec3 lo = color.rgb * color.rgb / 3.0;
                vec3 hi = (exp((color.rgb - HLG_C) / HLG_A) + HLG_B) / 12.0;
                return mixb(hi, lo, isLow);
            }

            // Many transfer functions (including sRGB) follow the same pattern: a linear
            // segment for small values and a power function for larger values. The
            // following function implements this pattern from which sRGB, BT.1886, and
            // others can be derived by plugging in the right constants.
            vec3 tfInvLinPow(vec3 color, float gamma, float thres, float scale, float alpha) {
                bvec3 isLow = lessThanEqual(color.rgb, vec3(thres * scale));
                vec3 lo = color.rgb / scale;
                vec3 hi = pow((color.rgb + alpha - 1.0) / alpha, vec3(gamma));
                return mixb(hi, lo, isLow);
            }

            vec3 tfInvSRGB(vec3 color) {
                return tfInvLinPow(color, SRGB_POW, SRGB_CUT, SRGB_SCALE, SRGB_ALPHA);
            }

            vec3 tfInvExtSRGB(vec3 color) {
                // EXT sRGB is the sRGB transfer function mirrored around 0.
                return sign(color) * tfInvSRGB(abs(color));
            }

            vec3 tfInvBT1886(vec3 color) {
                return tfInvLinPow(color, BT1886_POW, BT1886_CUT, BT1886_SCALE, BT1886_ALPHA);
            }

            vec3 tfInvXVYCC(vec3 color) {
                // The inverse transfer function for XVYCC is the BT1886 transfer function mirrored around 0,
                // same as what EXT sRGB is to sRGB.
                return sign(color) * tfInvBT1886(abs(color));
            }

            vec3 tfInvST240(vec3 color) {
                return tfInvLinPow(color, ST240_POW, ST240_CUT, ST240_SCALE, ST240_ALPHA);
            }

            // Forward transfer functions corresponding to the inverse functions above.
            // Inputs are assumed to have 1 == 80 nits with a scale factor pre-applied to adjust for SDR white!
            vec3 tfPQ(vec3 color) {
                vec3 E = pow(max(color.rgb, vec3(0.0)), vec3(PQ_M1));
                return pow(
                    (vec3(PQ_C1) + PQ_C2 * E) / max(vec3(1.0) + PQ_C3 * E, vec3(1e-5)),
                    vec3(PQ_M2)
                );
            }

            vec3 tfHLG(vec3 color) {
                bvec3 isLow = lessThanEqual(color.rgb, vec3(HLG_D_CUT));
                vec3 lo = sqrt(max(color.rgb, vec3(0.0)) * 3.0);
                vec3 hi = HLG_A * log(max(12.0 * color.rgb - HLG_B, vec3(0.0001))) + HLG_C;
                return mixb(hi, lo, isLow);
            }

            vec3 tfLinPow(vec3 color, float gamma, float thres, float scale, float alpha) {
                bvec3 isLow = lessThanEqual(color.rgb, vec3(thres));
                vec3 lo = color.rgb * scale;
                vec3 hi = pow(color.rgb, vec3(1.0 / gamma)) * alpha - (alpha - 1.0);
                return mixb(hi, lo, isLow);
            }

            vec3 tfSRGB(vec3 color) {
                return tfLinPow(color, SRGB_POW, SRGB_CUT, SRGB_SCALE, SRGB_ALPHA);
            }

            vec3 tfExtSRGB(vec3 color) {
                // EXT sRGB is the sRGB transfer function mirrored around 0.
                return sign(color) * tfSRGB(abs(color));
            }

            vec3 tfBT1886(vec3 color) {
                return tfLinPow(color, BT1886_POW, BT1886_CUT, BT1886_SCALE, BT1886_ALPHA);
            }

            vec3 tfXVYCC(vec3 color) {
                // The transfer function for XVYCC is the BT1886 transfer function mirrored around 0,
                // same as what EXT sRGB is to sRGB.
                return sign(color) * tfBT1886(abs(color));
            }

            vec3 tfST240(vec3 color) {
                return tfLinPow(color, ST240_POW, ST240_CUT, ST240_SCALE, ST240_ALPHA);
            }

            vec3 toLinearRGB(vec3 color, int tf) {
                if (tf == CM_TRANSFER_FUNCTION_EXT_LINEAR) {
                    return color;
                } else if (tf == CM_TRANSFER_FUNCTION_ST2084_PQ) {
                    return tfInvPQ(color);
                } else if (tf == CM_TRANSFER_FUNCTION_GAMMA22) {
                    return pow(max(color, vec3(0.0)), vec3(2.2));
                } else if (tf == CM_TRANSFER_FUNCTION_GAMMA28) {
                    return pow(max(color, vec3(0.0)), vec3(2.8));
                } else if (tf == CM_TRANSFER_FUNCTION_HLG) {
                    return tfInvHLG(color);
                } else if (tf == CM_TRANSFER_FUNCTION_EXT_SRGB) {
                    return tfInvExtSRGB(color);
                } else if (tf == CM_TRANSFER_FUNCTION_BT1886) {
                    return tfInvBT1886(color);
                } else if (tf == CM_TRANSFER_FUNCTION_ST240) {
                    return tfInvST240(color);
                } else if (tf == CM_TRANSFER_FUNCTION_LOG_100) {
                    return mixb(exp((color - 1.0) * 2.0 * log(10.0)), vec3(0.0), lessThanEqual(color, vec3(0.0)));
                } else if (tf == CM_TRANSFER_FUNCTION_LOG_316) {
                    return mixb(exp((color - 1.0) * 2.5 * log(10.0)), vec3(0.0), lessThanEqual(color, vec3(0.0)));
                } else if (tf == CM_TRANSFER_FUNCTION_XVYCC) {
                    return tfInvXVYCC(color);
                } else if (tf == CM_TRANSFER_FUNCTION_ST428) {
                    return pow(max(color, vec3(0.0)), vec3(ST428_POW)) * ST428_SCALE;
                } else if (tf == CM_TRANSFER_FUNCTION_SRGB) {
                    return tfInvSRGB(color);
                } else {
                    return tfInvSRGB(color);
                }
            }

            vec3 fromLinearRGB(vec3 color, int tf) {
                if (tf == CM_TRANSFER_FUNCTION_EXT_LINEAR) {
                    return color;
                } else if (tf == CM_TRANSFER_FUNCTION_ST2084_PQ) {
                    return tfPQ(color);
                } else if (tf == CM_TRANSFER_FUNCTION_GAMMA22) {
                    return pow(max(color, vec3(0.0)), vec3(1.0 / 2.2));
                } else if (tf == CM_TRANSFER_FUNCTION_GAMMA28) {
                    return pow(max(color, vec3(0.0)), vec3(1.0 / 2.8));
                } else if (tf == CM_TRANSFER_FUNCTION_HLG) {
                    return tfHLG(color);
                } else if (tf == CM_TRANSFER_FUNCTION_EXT_SRGB) {
                    return tfExtSRGB(color);
                } else if (tf == CM_TRANSFER_FUNCTION_BT1886) {
                    return tfBT1886(color);
                } else if (tf == CM_TRANSFER_FUNCTION_ST240) {
                    return tfST240(color);
                } else if (tf == CM_TRANSFER_FUNCTION_LOG_100) {
                    return mixb(1.0 + log(color) / log(10.0) / 2.0, vec3(0.0), lessThanEqual(color, vec3(0.01)));
                } else if (tf == CM_TRANSFER_FUNCTION_LOG_316) {
                    return mixb(1.0 + log(color) / log(10.0) / 2.5, vec3(0.0), lessThanEqual(color, vec3(sqrt(10.0) / 1000.0)));
                } else if (tf == CM_TRANSFER_FUNCTION_XVYCC) {
                    return tfXVYCC(color);
                } else if (tf == CM_TRANSFER_FUNCTION_ST428) {
                    return pow(max(color, vec3(0.0)) / ST428_SCALE, vec3(1.0 / ST428_POW));
                } else if (tf == CM_TRANSFER_FUNCTION_SRGB) {
                    return tfSRGB(color);
                } else {
                    return tfSRGB(color);
                }
            }

            float transferWhiteLevel(int tf) {
                if (tf == CM_TRANSFER_FUNCTION_ST2084_PQ) {
                    return 10000.0;
                } else if (tf == CM_TRANSFER_FUNCTION_HLG) {
                    return 1000.0;
                } else if (tf == CM_TRANSFER_FUNCTION_BT1886) {
                    return 100.0;
                } else if (tf == CM_TRANSFER_FUNCTION_XVYCC) {
                    return 100.0;
                } else {
                    return 80.0;
                }
            }

            vec3 dither(vec3 color) {
                return color + texture2D(ditherMatrix, fract(ditherUv)).r;
            }

            void main() {
                vec4 color = texture2D(framebufferTexture, imageUv);

                // nanogui uses colors in extended sRGB with a scale that assumes SDR white corresponds to a value of 1. Hence, to convert to
                // absolute nits in the display's color space, we need to undo the extended sRGB transfer function, multiply by the SDR white
                // level of the display, apply the display's color matrix, and finally apply the display's transfer function.
                vec3 nits = displayColorMatrix * (displaySdrWhiteLevel * toLinearRGB(color.rgb, CM_TRANSFER_FUNCTION_EXT_SRGB));

                // Some displays perform strange tonemapping when provided with values outside of their luminance range. Make sure we don't
                // let this happen -- we strongly prefer hard clipping because we want the displayable colors to be preserved.
                if (maxLuminance > 0.0) {
                    nits = clamp(nits, vec3(minLuminance), vec3(maxLuminance));
                }

                // On Linux, some drivers only let us have an 8-bit framebuffer. When dealing with HDR content in such a situation,
                // dithering is essential to avoid banding artifacts.
                color.rgb = dither(fromLinearRGB(nits / transferWhiteLevel(outTransferFunction), outTransferFunction));

                if (clipToUnitInterval) {
                    color = clamp(color, vec4(0.0), vec4(1.0));
                }

                gl_FragColor = color;
            }
        )glsl";

        try {
            m_cm_shader = new Shader(nullptr, "color_management", vertex_shader, fragment_shader);
        } catch (const std::runtime_error& e) { fprintf(stderr, "Error creating color management shader: %s\n", e.what()); }

        uint32_t indices[3 * 2] = {0, 1, 2, 2, 3, 0};
        float positions[2 * 4] = {-1.f, -1.f, 1.f, -1.f, 1.f, 1.f, -1.f, 1.f};

        m_cm_shader->set_buffer("indices", VariableType::UInt32, {3 * 2}, indices);
        m_cm_shader->set_buffer("position", VariableType::Float32, {4, 2}, positions);
        m_cm_shader->set_texture("ditherMatrix", m_dither_matrix);
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

    if (m_cm_render_pass && m_cm_shader) {
        m_cm_render_pass->begin();
    }
#elif defined(NANOGUI_USE_METAL)
    void *nswin = glfwGetCocoaWindow(m_glfw_window);
    metal_window_set_size(nswin, m_fbsize);
    m_metal_drawable = metal_window_next_drawable(nswin);
    m_metal_texture = metal_drawable_texture(m_metal_drawable);
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
        metal_window_set_vsync(nswin, vsync);
        mnvgSetFlushWait(m_nvg_context, !vsync);
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
    metal_window_set_content_scale(nswin, m_pixel_ratio);
#endif
#endif

#if defined(NANOGUI_USE_OPENGL) || defined(NANOGUI_USE_GLES)
    CHK(glViewport(0, 0, m_fbsize[0], m_fbsize[1]));
#endif
}

void Screen::draw_teardown() {
#if defined(NANOGUI_USE_OPENGL) || defined(NANOGUI_USE_GLES)
    if (m_cm_render_pass && m_cm_shader) {
        m_cm_render_pass->end();

        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
        glViewport(0, 0, m_fbsize[0], m_fbsize[1]);
        CHK(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT));

        m_cm_shader->set_texture("framebufferTexture", m_cm_texture);

        float display_sdr_white_level =
            m_display_sdr_white_level_override ?
            m_display_sdr_white_level_override.value() :
            glfwGetWindowSdrWhiteLevel(m_glfw_window);

        m_cm_shader->set_uniform("displaySdrWhiteLevel", display_sdr_white_level);
        m_cm_shader->set_uniform("outTransferFunction", (int)glfwGetWindowTransfer(m_glfw_window));

        const auto display_chroma = chroma_from_wp_primaries(glfwGetWindowPrimaries(m_glfw_window));
        const auto display_color_matrix = inverse(chroma_to_rec709_matrix(display_chroma));
        m_cm_shader->set_uniform("displayColorMatrix", display_color_matrix);

        m_cm_shader->set_uniform("minLuminance", glfwGetWindowMinLuminance(m_glfw_window));
        m_cm_shader->set_uniform("maxLuminance", glfwGetWindowMaxLuminance(m_glfw_window));

        m_cm_shader->set_uniform("clipToUnitInterval", !m_float_buffer);

        m_cm_shader->set_uniform("ditherScale", (1.0f / DITHER_MATRIX_SIZE) * Vector2f(m_fbsize[0], m_fbsize[1]));
        m_cm_shader->set_texture("ditherMatrix", m_dither_matrix);

        m_cm_shader->begin();
        m_cm_shader->draw_array(Shader::PrimitiveType::Triangle, 0, 6, true);
        m_cm_shader->end();
    }

    glfwSwapBuffers(m_glfw_window);
#elif defined(NANOGUI_USE_METAL)
    mnvgSetColorTexture(m_nvg_context, nullptr);
    metal_present_and_release_drawable(m_metal_drawable);
    m_metal_texture = nullptr;
    m_metal_drawable = nullptr;
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
    m_redraw = true;
    draw_all();
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

#if defined(NANOGUI_USE_METAL)
    if (m_depth_stencil_texture)
        m_depth_stencil_texture->resize(fb_size);
#endif

#if defined(NANOGUI_USE_OPENGL) || defined(NANOGUI_USE_GLES)
    if (m_cm_texture) {
        m_cm_texture->resize(fb_size);
    }

    if (m_cm_depth_texture) {
        m_cm_depth_texture->resize(fb_size);
    }

    if (m_cm_render_pass) {
        m_cm_render_pass->resize(fb_size);
    }
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
    return applies_color_management() ? m_cm_render_pass->framebuffer_handle() : 0;
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
    return metal_window_layer(glfwGetCocoaWindow(m_glfw_window));
}
#endif

NAMESPACE_END(nanogui)
