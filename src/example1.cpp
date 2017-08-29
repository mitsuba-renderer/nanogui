/*
    src/example1.cpp -- C++ version of an example application that shows
    how to use the various widget classes. For a Python implementation, see
    '../python/example1.py'.

    NanoGUI was developed by Wenzel Jakob <wenzel.jakob@epfl.ch>.
    The widget drawing code is based on the NanoVG demo application
    by Mikko Mononen.

    All rights reserved. Use of this source code is governed by a
    BSD-style license that can be found in the LICENSE.txt file.
*/

#include <nanogui/opengl.h>
#include <nanogui/glutil.h>
#include <nanogui/screen.h>
#include <nanogui/window.h>
#include <nanogui/layout.h>
#include <nanogui/label.h>
#include <nanogui/checkbox.h>
#include <nanogui/button.h>
#include <nanogui/toolbutton.h>
#include <nanogui/popupbutton.h>
#include <nanogui/combobox.h>
#include <nanogui/progressbar.h>
#include <nanogui/entypo.h>
#include <nanogui/messagedialog.h>
#include <nanogui/textbox.h>
#include <nanogui/slider.h>
#include <nanogui/imagepanel.h>
#include <nanogui/imageview.h>
#include <nanogui/vscrollpanel.h>
#include <nanogui/colorwheel.h>
#include <nanogui/graph.h>
#include <nanogui/tabwidget.h>
#include <iostream>
#include <string>

// Includes for the GLTexture class.
#include <cstdint>
#include <memory>
#include <utility>

#if defined(__GNUG__)
/// Uh oh, stb-image needs some refactoring..
#  pragma GCC diagnostic ignored "-Wmissing-field-initializers"
#  pragma GCC diagnostic ignored "-Wshift-negative-value"
#  if !defined(__clang__)
#    pragma GCC diagnostic ignored "-Wmisleading-indentation"
#    pragma GCC diagnostic ignored "-Wmaybe-uninitialized"
#  endif
#endif
#if defined(_WIN32)
#  pragma warning(push)
#  pragma warning(disable: 4457 4456 4005 4312)
#endif

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#if defined(_WIN32)
#  pragma warning(pop)
#endif
#if defined(_WIN32)
#  if defined(APIENTRY)
#    undef APIENTRY
#  endif
#  include <windows.h>
#endif

using std::cout;
using std::cerr;
using std::endl;
using std::string;
using std::vector;
using std::pair;
using std::to_string;

class GLTexture {
public:
    using handle_type = std::unique_ptr<uint8_t[], void(*)(void*)>;
    GLTexture() = default;
    GLTexture(const std::string& texture_name)
        : m_texture_name(texture_name), m_texture_id(0) {}

    GLTexture(const std::string& texture_name, GLint texture_id)
        : m_texture_name(texture_name), m_texture_id(texture_id) {}

    GLTexture(const GLTexture& other) = delete;
    GLTexture(GLTexture&& other) noexcept
        : m_texture_name(std::move(other.m_texture_name)),
        m_texture_id(other.m_texture_id) {
        other.m_texture_id = 0;
    }
    GLTexture& operator=(const GLTexture& other) = delete;
    GLTexture& operator=(GLTexture&& other) noexcept {
        m_texture_name = std::move(other.m_texture_name);
        std::swap(m_texture_id, other.m_texture_id);
        return *this;
    }
    ~GLTexture() noexcept {
        if (m_texture_id)
            glDeleteTextures(1, &m_texture_id);
    }

    GLuint texture() const { return m_texture_id; }
    const std::string& texture_name() const { return m_texture_name; }

    /**
    *  Load a file in memory and create an OpenGL texture.
    *  Returns a handle type (an std::unique_ptr) to the loaded pixels.
    */
    handle_type load(const std::string& file_name) {
        if (m_texture_id) {
            glDeleteTextures(1, &m_texture_id);
            m_texture_id = 0;
        }
        int force_channels = 0;
        int w, h, n;
        handle_type texture_data(stbi_load(file_name.c_str(), &w, &h, &n, force_channels), stbi_image_free);
        if (!texture_data)
            throw std::invalid_argument("Could not load texture data from file " + file_name);
        glGenTextures(1, &m_texture_id);
        glBindTexture(GL_TEXTURE_2D, m_texture_id);
        GLint internal_format;
        GLint format;
        switch (n) {
#if defined(NANOGUI_USE_OPENGL)
            case 1: internal_format = GL_R8; format = GL_RED; break;
            case 2: internal_format = GL_RG8; format = GL_RG; break;
            case 3: internal_format = GL_RGB8; format = GL_RGB; break;
            case 4: internal_format = GL_RGBA8; format = GL_RGBA; break;
            default: internal_format = 0; format = 0; break;
#else
            case 1: internal_format = GL_LUMINANCE; format = GL_LUMINANCE; break;
            case 2: internal_format = GL_LUMINANCE_ALPHA; format = GL_LUMINANCE_ALPHA; break;
            case 3: internal_format = GL_RGB; format = GL_RGB; break;
            case 4: internal_format = GL_RGBA; format = GL_RGBA; break;
            default: internal_format = 0; format = 0; break;
#endif
        }
        glTexImage2D(GL_TEXTURE_2D, 0, internal_format, w, h, 0, format, GL_UNSIGNED_BYTE, texture_data.get());
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        return texture_data;
    }

private:
    std::string m_texture_name;
    GLuint m_texture_id;
};

class ExampleApplication : public nanogui::Screen {
public:
    ExampleApplication() : nanogui::Screen(nanogui::Vector2i(1024, 768), "NanoGUI Test") {
        using namespace nanogui;

        Window *window = new Window(this, "Button demo");
        window->set_position(Vector2i(15, 15));
        window->set_layout(new GroupLayout());

        /* No need to store a pointer, the data structure will be automatically
           freed when the parent window is deleted */
        new Label(window, "Push buttons", "sans-bold");

        Button *b = new Button(window, "Plain button");
        b->set_callback([] { cout << "pushed!" << endl; });
        b->set_tooltip("short tooltip");

        /* Alternative construction notation using variadic template */
        b = window->add<Button>("Styled", ENTYPO_ICON_ROCKET);
        b->set_background_color(Color(0, 0, 255, 25));
        b->set_callback([] { cout << "pushed!" << endl; });
        b->set_tooltip("This button has a fairly long tooltip. It is so long, in "
                "fact, that the shown text will span several lines.");

        new Label(window, "Toggle buttons", "sans-bold");
        b = new Button(window, "Toggle me");
        b->set_flags(Button::ToggleButton);
        b->set_change_callback([](bool state) { cout << "Toggle button state: " << state << endl; });

        new Label(window, "Radio buttons", "sans-bold");
        b = new Button(window, "Radio button 1");
        b->set_flags(Button::RadioButton);
        b = new Button(window, "Radio button 2");
        b->set_flags(Button::RadioButton);

        new Label(window, "A tool palette", "sans-bold");
        Widget *tools = new Widget(window);
        tools->set_layout(new BoxLayout(Orientation::Horizontal,
                                       Alignment::Middle, 0, 6));

        b = new ToolButton(tools, ENTYPO_ICON_CLOUD);
        b = new ToolButton(tools, ENTYPO_ICON_FF);
        b = new ToolButton(tools, ENTYPO_ICON_COMPASS);
        b = new ToolButton(tools, ENTYPO_ICON_INSTALL);

        new Label(window, "Popup buttons", "sans-bold");
        PopupButton *popup_btn = new PopupButton(window, "Popup", ENTYPO_ICON_EXPORT);
        Popup *popup = popup_btn->popup();
        popup->set_layout(new GroupLayout());
        new Label(popup, "Arbitrary widgets can be placed here");
        new CheckBox(popup, "A check box");
        popup_btn = new PopupButton(popup, "Recursive popup", ENTYPO_ICON_FLASH);
        popup = popup_btn->popup();
        popup->set_layout(new GroupLayout());
        new CheckBox(popup, "Another check box");

        window = new Window(this, "Basic widgets");
        window->set_position(Vector2i(200, 15));
        window->set_layout(new GroupLayout());

        new Label(window, "Message dialog", "sans-bold");
        tools = new Widget(window);
        tools->set_layout(new BoxLayout(Orientation::Horizontal,
                                       Alignment::Middle, 0, 6));
        b = new Button(tools, "Info");
        b->set_callback([&] {
            auto dlg = new MessageDialog(this, MessageDialog::Type::Information, "Title", "This is an information message");
            dlg->set_callback([](int result) { cout << "Dialog result: " << result << endl; });
        });
        b = new Button(tools, "Warn");
        b->set_callback([&] {
            auto dlg = new MessageDialog(this, MessageDialog::Type::Warning, "Title", "This is a warning message");
            dlg->set_callback([](int result) { cout << "Dialog result: " << result << endl; });
        });
        b = new Button(tools, "Ask");
        b->set_callback([&] {
            auto dlg = new MessageDialog(this, MessageDialog::Type::Warning, "Title", "This is a question message", "Yes", "No", true);
            dlg->set_callback([](int result) { cout << "Dialog result: " << result << endl; });
        });

        vector<pair<int, string>>
            icons = load_image_directory(m_nvg_context, "icons");
        #if defined(_WIN32)
            string resources_folder_path("../resources/");
        #else
            string resources_folder_path("./");
        #endif

        new Label(window, "Image panel & scroll panel", "sans-bold");
        PopupButton *image_panel_btn = new PopupButton(window, "Image Panel");
        image_panel_btn->set_icon(ENTYPO_ICON_FOLDER);
        popup = image_panel_btn->popup();
        VScrollPanel *vscroll = new VScrollPanel(popup);
        ImagePanel *img_panel = new ImagePanel(vscroll);
        img_panel->set_images(icons);
        popup->set_fixed_size(Vector2i(245, 150));

        auto image_window = new Window(this, "Selected image");
        image_window->set_position(Vector2i(710, 15));
        image_window->set_layout(new GroupLayout());

        // Load all of the images by creating a GLTexture object and saving the pixel data.
        for (auto& icon : icons) {
            GLTexture texture(icon.second);
            auto data = texture.load(resources_folder_path + icon.second + ".png");
            m_images_data.emplace_back(std::move(texture), std::move(data));
        }

        // Set the first texture
        auto image_view = new ImageView(image_window, m_images_data[0].first.texture());
        m_current_image = 0;
        // Change the active textures.
        img_panel->set_callback([this, image_view](int i) {
            image_view->bind_image(m_images_data[i].first.texture());
            m_current_image = i;
            cout << "Selected item " << i << '\n';
        });
        image_view->set_grid_threshold(20);
        image_view->set_pixel_info_threshold(20);
        image_view->set_pixel_info_callback(
            [this, image_view](const Vector2i& index) -> pair<string, Color> {
            auto& image_data = m_images_data[m_current_image].second;
            auto& texture_size = image_view->image_size();
            string string_data;
            uint16_t channel_sum = 0;
            for (int i = 0; i != 4; ++i) {
                auto& channel_data = image_data[4*index.y()*texture_size.x() + 4*index.x() + i];
                channel_sum += channel_data;
                string_data += (to_string(static_cast<int>(channel_data)) + "\n");
            }
            float intensity = static_cast<float>(255 - (channel_sum / 4)) / 255.0f;
            float color_scale = intensity > 0.5f ? (intensity + 1) / 2 : intensity / 2;
            Color text_color = Color(color_scale, 1.0f);
            return { string_data, text_color };
        });

        new Label(window, "File dialog", "sans-bold");
        tools = new Widget(window);
        tools->set_layout(new BoxLayout(Orientation::Horizontal,
                                       Alignment::Middle, 0, 6));
        b = new Button(tools, "Open");
        b->set_callback([&] {
            cout << "File dialog result: " << file_dialog(
                    { {"png", "Portable Network Graphics"}, {"txt", "Text file"} }, false) << endl;
        });
        b = new Button(tools, "Save");
        b->set_callback([&] {
            cout << "File dialog result: " << file_dialog(
                    { {"png", "Portable Network Graphics"}, {"txt", "Text file"} }, true) << endl;
        });

        new Label(window, "Combo box", "sans-bold");
        new ComboBox(window, { "Combo box item 1", "Combo box item 2", "Combo box item 3"});
        new Label(window, "Check box", "sans-bold");
        CheckBox *cb = new CheckBox(window, "Flag 1",
            [](bool state) { cout << "Check box 1 state: " << state << endl; }
        );
        cb->set_checked(true);
        cb = new CheckBox(window, "Flag 2",
            [](bool state) { cout << "Check box 2 state: " << state << endl; }
        );
        new Label(window, "Progress bar", "sans-bold");
        m_progress = new ProgressBar(window);

        new Label(window, "Slider and text box", "sans-bold");

        Widget *panel = new Widget(window);
        panel->set_layout(new BoxLayout(Orientation::Horizontal,
                                       Alignment::Middle, 0, 20));

        Slider *slider = new Slider(panel);
        slider->set_value(0.5f);
        slider->set_fixed_width(80);

        TextBox *text_box = new TextBox(panel);
        text_box->set_fixed_size(Vector2i(60, 25));
        text_box->set_value("50");
        text_box->set_units("%");
        slider->set_callback([text_box](float value) {
            text_box->set_value(std::to_string((int) (value * 100)));
        });
        slider->set_final_callback([&](float value) {
            cout << "Final slider value: " << (int) (value * 100) << endl;
        });
        text_box->set_fixed_size(Vector2i(60,25));
        text_box->set_font_size(20);
        text_box->set_alignment(TextBox::Alignment::Right);

        window = new Window(this, "Misc. widgets");
        window->set_position(Vector2i(425,15));
        window->set_layout(new GroupLayout());

        TabWidget* tab_widget = window->add<TabWidget>();

        Widget* layer = tab_widget->create_tab("Color Wheel");
        layer->set_layout(new GroupLayout());

        // Use overloaded variadic add to fill the tab widget with Different tabs.
        layer->add<Label>("Color wheel widget", "sans-bold");
        layer->add<ColorWheel>();

        layer = tab_widget->create_tab("Function Graph");
        layer->set_layout(new GroupLayout());

        layer->add<Label>("Function graph widget", "sans-bold");

        Graph *graph = layer->add<Graph>("Some Function");

        graph->set_header("E = 2.35e-3");
        graph->set_footer("Iteration 89");
        std::vector<float> &func = graph->values();
        func.resize(100);
        for (int i = 0; i < 100; ++i)
            func[i] = 0.5f * (0.5f * std::sin(i / 10.f) +
                              0.5f * std::cos(i / 23.f) + 1);

        // Dummy tab used to represent the last tab button.
        tab_widget->create_tab("+");

        // A simple counter.
        int counter = 1;
        tab_widget->set_callback([tab_widget, this, counter] (int index) mutable {
            if (index == (tab_widget->tab_count()-1)) {
                // When the "+" tab has been clicked, simply add a new tab.
                string tab_name = "Dynamic " + to_string(counter);
                Widget* layer_dyn = tab_widget->create_tab(index, tab_name);
                layer_dyn->set_layout(new GroupLayout());
                layer_dyn->add<Label>("Function graph widget", "sans-bold");
                Graph *graph_dyn = layer_dyn->add<Graph>("Dynamic function");

                graph_dyn->set_header("E = 2.35e-3");
                graph_dyn->set_footer("Iteration " + to_string(index*counter));
                std::vector<float> &func_dyn = graph_dyn->values();
                func_dyn.resize(100);
                for (int i = 0; i < 100; ++i)
                    func_dyn[i] = 0.5f *
                        std::abs((0.5f * std::sin(i / 10.f + counter) +
                                  0.5f * std::cos(i / 23.f + 1 + counter)));
                ++counter;
                // We must invoke perform layout from the screen instance to keep everything in order.
                // This is essential when creating tabs dynamically.
                perform_layout();
                // Ensure that the newly added header is visible on screen
                tab_widget->ensure_tab_visible(index);

            }
        });
        tab_widget->set_active_tab(0);

        // A button to go back to the first tab and scroll the window.
        panel = window->add<Widget>();
        panel->add<Label>("Jump to tab: ");
        panel->set_layout(new BoxLayout(Orientation::Horizontal,
                                       Alignment::Middle, 0, 6));

        auto ib = panel->add<IntBox<int>>();
        ib->set_editable(true);

        b = panel->add<Button>("", ENTYPO_ICON_FORWARD);
        b->set_fixed_size(Vector2i(22, 22));
        ib->set_fixed_height(22);
        b->set_callback([tab_widget, ib] {
            int value = ib->value();
            if (value >= 0 && value < tab_widget->tab_count()) {
                tab_widget->set_active_tab(value);
                tab_widget->ensure_tab_visible(value);
            }
        });

        window = new Window(this, "Grid of small widgets");
        window->set_position(Vector2i(425, 300));
        GridLayout *layout =
            new GridLayout(Orientation::Horizontal, 2,
                           Alignment::Middle, 15, 5);
        layout->set_col_alignment(
            { Alignment::Maximum, Alignment::Fill });
        layout->set_spacing(0, 10);
        window->set_layout(layout);

        /* FP widget */ {
            new Label(window, "Floating point :", "sans-bold");
            text_box = new TextBox(window);
            text_box->set_editable(true);
            text_box->set_fixed_size(Vector2i(100, 20));
            text_box->set_value("50");
            text_box->set_units("GiB");
            text_box->set_default_value("0.0");
            text_box->set_font_size(16);
            text_box->set_format("[-]?[0-9]*\\.?[0-9]+");
        }

        /* Positive integer widget */ {
            new Label(window, "Positive integer :", "sans-bold");
            auto int_box = new IntBox<int>(window);
            int_box->set_editable(true);
            int_box->set_fixed_size(Vector2i(100, 20));
            int_box->set_value(50);
            int_box->set_units("Mhz");
            int_box->set_default_value("0");
            int_box->set_font_size(16);
            int_box->set_format("[1-9][0-9]*");
            int_box->set_spinnable(true);
            int_box->set_min_value(1);
            int_box->set_value_increment(2);
        }

        /* Checkbox widget */ {
            new Label(window, "Checkbox :", "sans-bold");

            cb = new CheckBox(window, "Check me");
            cb->set_font_size(16);
            cb->set_checked(true);
        }

        new Label(window, "Combo box :", "sans-bold");
        ComboBox *cobo =
            new ComboBox(window, { "Item 1", "Item 2", "Item 3" });
        cobo->set_font_size(16);
        cobo->set_fixed_size(Vector2i(100,20));

        new Label(window, "Color button :", "sans-bold");
        popup_btn = new PopupButton(window, "", 0);
        popup_btn->set_background_color(Color(255, 120, 0, 255));
        popup_btn->set_font_size(16);
        popup_btn->set_fixed_size(Vector2i(100, 20));
        popup = popup_btn->popup();
        popup->set_layout(new GroupLayout());

        ColorWheel *colorwheel = new ColorWheel(popup);
        colorwheel->set_color(popup_btn->background_color());

        Button *color_btn = new Button(popup, "Pick");
        color_btn->set_fixed_size(Vector2i(100, 25));
        Color c = colorwheel->color();
        color_btn->set_background_color(c);

        colorwheel->set_callback([color_btn](const Color &value) {
            color_btn->set_background_color(value);
        });

        color_btn->set_change_callback([color_btn, popup_btn](bool pushed) {
            if (pushed) {
                popup_btn->set_background_color(color_btn->background_color());
                popup_btn->set_pushed(false);
            }
        });

        perform_layout();

        /* All NanoGUI widgets are initialized at this point. Now
           create an OpenGL shader to draw the main window contents.

           NanoGUI comes with a simple wrapper around OpenGL 3, which
           eliminates most of the tedious and error-prone shader and buffer
           object management.
        */

        m_shader.init(
            /* An identifying name */
            "a_simple_shader",

#if defined(NANOGUI_USE_OPENGL)
            /* Vertex shader */
            "#version 330\n"
            "uniform mat4 model_view_proj;\n"
            "in vec3 position;\n"
            "void main() {\n"
            "    gl_Position = model_view_proj * vec4(position, 1.0);\n"
            "}",

            /* Fragment shader */
            "#version 330\n"
            "out vec4 color;\n"
            "uniform float intensity;\n"
            "void main() {\n"
            "    color = vec4(vec3(intensity), 1.0);\n"
            "}"
#else // GLES2
            /* Vertex shader */
            "uniform mat4 model_view_proj;\n"
            "attribute vec3 position;\n"
            "void main() {\n"
            "    gl_Position = model_view_proj * vec4(position, 1.0);\n"
            "}",

            /* Fragment shader */
            "uniform float intensity;\n"
            "void main() {\n"
            "    gl_FragColor = vec4(vec3(intensity), 1.0);\n"
            "}"
#endif
        );

        uint32_t indices[3*2] = {
            0, 1, 2,
            2, 3, 0
        };

        float positions[3*4] = {
            -1.f, -1.f, 0.f,
            1.f, -1.f, 0.f,
            1.f, 1.f, 0.f,
            -1.f, 1.f, 0.f
        };

        m_shader.bind();
        m_shader.upload_indices(indices, 3, 2);
        m_shader.upload_attrib("position", positions, 3, 4);
        m_shader.set_uniform("intensity", 0.5f);
    }

    ~ExampleApplication() {
        m_shader.free();
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
        /* Animate the scrollbar */
        m_progress->set_value(std::fmod((float) glfwGetTime() / 10, 1.0f));

        /* Draw the user interface */
        Screen::draw(ctx);
    }

    virtual void draw_contents() {
        using namespace nanogui;

        /* Draw the window contents using OpenGL */
        m_shader.bind();

        Matrix4f mvp =
            enoki::scale<Matrix4f>(Vector3f((float) m_size.y() / (float) m_size.x() * 0.25f, 0.25f, 0.25f)) *
            enoki::rotate<Matrix4f>(Vector3f(0, 0, 1), (float) glfwGetTime());

        m_shader.set_uniform("model_view_proj", mvp);

        /* Draw 2 triangles starting at index 0 */
        m_shader.draw_indexed(GL_TRIANGLES, 0, 2);
    }
private:
    nanogui::ProgressBar *m_progress;
    nanogui::GLShader m_shader;

    using images_data_type = vector<pair<GLTexture, GLTexture::handle_type>>;
    images_data_type m_images_data;
    int m_current_image;
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
            std::cerr << error_msg << endl;
        #endif
        return -1;
    }

    return 0;
}
