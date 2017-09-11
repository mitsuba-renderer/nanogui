/*
    src/example4.cpp -- C++ version of an example application that shows
    how to use the OpenGL widget. For a Python implementation, see
    '../python/example4.py'.

    NanoGUI was developed by Wenzel Jakob <wenzel.jakob@epfl.ch>.
    The widget drawing code is based on the NanoVG demo application
    by Mikko Mononen.

    All rights reserved. Use of this source code is governed by a
    BSD-style license that can be found in the LICENSE.txt file.
*/

#include <nanogui/screen.h>
#include <nanogui/layout.h>
#include <nanogui/window.h>
#include <nanogui/button.h>
#include <nanogui/glcanvas.h>

#if defined(_WIN32)
#  if defined(APIENTRY)
#    undef APIENTRY
#  endif
#  include <windows.h>
#endif

using nanogui::Vector3f;
using nanogui::Vector2i;

class MyGLCanvas : public nanogui::GLCanvas {
public:
    MyGLCanvas(Widget *parent) : nanogui::GLCanvas(parent), m_rotation(Vector3f(0.25f, 0.5f, 0.33f)) {
        using namespace nanogui;

        m_shader.init(
            /* An identifying name */
            "a_simple_shader",

#if defined(NANOGUI_USE_OPENGL)
            /* Vertex shader */
            "#version 330\n"
            "uniform mat4 model_view_proj;\n"
            "in vec3 position;\n"
            "in vec3 color;\n"
            "out vec4 frag_color;\n"
            "void main() {\n"
            "    frag_color = vec4(color, 1.0);\n"
            "    gl_Position = model_view_proj * vec4(position, 1.0);\n"
            "}",

            /* Fragment shader */
            "#version 330\n"
            "out vec4 color;\n"
            "in vec4 frag_color;\n"
            "void main() {\n"
            "    color = frag_color;\n"
            "}"
#else // GLES2
            /* Vertex shader */
            "precision highp float;\n"
            "uniform mat4 model_view_proj;\n"
            "attribute vec3 position;\n"
            "attribute vec3 color;\n"
            "varying vec4 frag_color;\n"
            "void main() {\n"
            "    frag_color = vec4(color, 1.0);\n"
            "    gl_Position = model_view_proj * vec4(position, 1.0);\n"
            "}",

            /* Fragment shader */
            "precision highp float;\n"
            "varying vec4 frag_color;\n"
            "void main() {\n"
            "    gl_FragColor = frag_color;\n"
            "}"
#endif
        );

        uint32_t indices[3*12] = {
            0, 1, 3,
            3, 2, 1,
            3, 2, 6,
            6, 7, 3,
            7, 6, 5,
            5, 4, 7,
            4, 5, 1,
            1, 0, 4,
            4, 0, 3,
            3, 7, 4,
            5, 6, 2,
            2, 1, 5
        };

        float positions[3*8] = {
            -1.f,  1.f,  1.f,
            -1.f,  1.f, -1.f,
             1.f,  1.f, -1.f,
             1.f,  1.f,  1.f,
            -1.f, -1.f,  1.f,
            -1.f, -1.f, -1.f,
             1.f, -1.f, -1.f,
             1.f, -1.f,  1.f
        };

        float colors[3*8] = {
            1.f, 0.f, 0.f,
            0.f, 1.f, 0.f,
            1.f, 1.f, 0.f,
            0.f, 0.f, 1.f,
            1.f, 0.f, 1.f,
            0.f, 1.f, 1.f,
            .5f, .5f, .5f,
            1.f, 0.f, .5f
        };

        m_shader.bind();
        m_shader.upload_indices(indices, 3, 12);
        m_shader.upload_attrib("position", positions, 3, 8);
        m_shader.upload_attrib("color", colors, 3, 8);
    }

    ~MyGLCanvas() {
        m_shader.free();
    }

    void set_rotation(Vector3f v_rotation) {
        m_rotation = v_rotation;
    }

    virtual void draw_gl() override {
        using namespace nanogui;

        m_shader.bind();

        using Matrix4f = enoki::Matrix<float, 4>;

        float f_time = (float)glfwGetTime();
        Matrix4f mvp =
            enoki::rotate<Matrix4f>(Vector3f(1, 0, 0), m_rotation[0] * f_time) *
            enoki::rotate<Matrix4f>(Vector3f(0, 1, 0), m_rotation[2] * f_time) *
            enoki::rotate<Matrix4f>(Vector3f(0, 0, 1), m_rotation[3] * f_time);

        m_shader.set_uniform("model_view_proj", enoki::scale<Matrix4f>(Vector3f(.5f)) * mvp);

        glEnable(GL_DEPTH_TEST);
        /* Draw 12 triangles starting at index 0 */
        m_shader.draw_indexed(GL_TRIANGLES, 0, 12);
        glDisable(GL_DEPTH_TEST);
    }

private:
    nanogui::GLShader m_shader;
    Vector3f m_rotation;
};


class ExampleApplication : public nanogui::Screen {
public:
    ExampleApplication() : nanogui::Screen(Vector2i(800, 600), "NanoGUI Test", false) {
        using namespace nanogui;

        Window *window = new Window(this, "GLCanvas Demo");
        window->set_position(Vector2i(15, 15));
        window->set_layout(new GroupLayout());

        m_canvas = new MyGLCanvas(window);
        m_canvas->set_background_color({100, 100, 100, 255});
        m_canvas->set_size({400, 400});

        Widget *tools = new Widget(window);
        tools->set_layout(new BoxLayout(Orientation::Horizontal,
                                       Alignment::Middle, 0, 5));

        Button *b0 = new Button(tools, "Random Color");
        b0->set_callback([this]() {
            m_canvas->set_background_color(
                Vector4i(rand() % 256, rand() % 256, rand() % 256, 255));
        });

        Button *b1 = new Button(tools, "Random Rotation");
        b1->set_callback([this]() {
            m_canvas->set_rotation(Vector3f(float(rand() % 100) / 100.f,
                                            float(rand() % 100) / 100.f,
                                            float(rand() % 100) / 100.f));
        });

        perform_layout();
    }

    virtual bool keyboard_event(int key, int scancode, int action, int modifiers) {
        if (Screen::keyboard_event(key, scancode, action, modifiers))
            return true;
        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
            set_visible(false);
            return true;
        }
        return false;
    }

    virtual void draw(NVGcontext *ctx) {
        /* Draw the user interface */
        Screen::draw(ctx);
    }
private:
    MyGLCanvas *m_canvas;
};

int main(int /* argc */, char ** /* argv */) {
    try {
        nanogui::init();

        /* scoped variables */ {
            nanogui::ref<ExampleApplication> app = new ExampleApplication();
            app->draw_all();
            app->set_visible(true);
            nanogui::mainloop();
        }

        nanogui::shutdown();
    } catch (const std::runtime_error &e) {
        std::string error_msg = std::string("Caught a fatal error: ") + std::string(e.what());
        #if defined(_WIN32)
            MessageBoxA(nullptr, error_msg.c_str(), NULL, MB_ICONERROR | MB_OK);
        #else
            std::cerr << error_msg << std::endl;
        #endif
        return -1;
    }

    return 0;
}
