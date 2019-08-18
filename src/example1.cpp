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
#include <nanogui/icons.h>
#include <nanogui/messagedialog.h>
#include <nanogui/textbox.h>
#include <nanogui/slider.h>
#include <nanogui/imagepanel.h>
#include <nanogui/imageview.h>
#include <nanogui/vscrollpanel.h>
#include <nanogui/colorwheel.h>
#include <nanogui/colorpicker.h>
#include <nanogui/graph.h>
#include <nanogui/tabwidget.h>
#include <nanogui/texture.h>
#include <nanogui/shader.h>
#include <nanogui/renderpass.h>
#include <enoki/transform.h>
#include <stb_image.h>

using namespace nanogui;

class ExampleApplication : public Screen {
public:
    ExampleApplication() : Screen(Vector2i(1024, 768), "NanoGUI Test") {
        inc_ref();
        Window *window = new Window(this, "Button demo");
        window->set_position(Vector2i(15, 15));
        window->set_layout(new GroupLayout());

        /* No need to store a pointer, the data structure will be automatically
           freed when the parent window is deleted */
        new Label(window, "Push buttons", "sans-bold");

        Button *b = new Button(window, "Plain button");
        b->set_callback([] { std::cout << "pushed!" << std::endl; });
        b->set_tooltip("short tooltip");

        /* Alternative construction notation using variadic template */
        b = window->add<Button>("Styled", FA_ROCKET);
        b->set_background_color(Color(0, 0, 255, 25));
        b->set_callback([] { std::cout << "pushed!" << std::endl; });
        b->set_tooltip("This button has a fairly long tooltip. It is so long, in "
                "fact, that the shown text will span several lines.");

        new Label(window, "Toggle buttons", "sans-bold");
        b = new Button(window, "Toggle me");
        b->set_flags(Button::ToggleButton);
        b->set_change_callback([](bool state) { std::cout << "Toggle button state: " << state << std::endl; });

        new Label(window, "Radio buttons", "sans-bold");
        b = new Button(window, "Radio button 1");
        b->set_flags(Button::RadioButton);
        b = new Button(window, "Radio button 2");
        b->set_flags(Button::RadioButton);

        new Label(window, "A tool palette", "sans-bold");
        Widget *tools = new Widget(window);
        tools->set_layout(new BoxLayout(Orientation::Horizontal,
                                       Alignment::Middle, 0, 6));

        b = new ToolButton(tools, FA_CLOUD);
        b = new ToolButton(tools, FA_FAST_FORWARD);
        b = new ToolButton(tools, FA_COMPASS);
        b = new ToolButton(tools, FA_UTENSILS);

        new Label(window, "Popup buttons", "sans-bold");
        PopupButton *popup_btn = new PopupButton(window, "Popup", FA_FLASK);
        Popup *popup = popup_btn->popup();
        popup->set_layout(new GroupLayout());
        new Label(popup, "Arbitrary widgets can be placed here");
        new CheckBox(popup, "A check box");
        // popup right
        popup_btn = new PopupButton(popup, "Recursive popup", FA_CHART_PIE);
        Popup *popup_right = popup_btn->popup();
        popup_right->set_layout(new GroupLayout());
        new CheckBox(popup_right, "Another check box");
        // popup left
        popup_btn = new PopupButton(popup, "Recursive popup", FA_DNA);
        popup_btn->set_side(Popup::Side::Left);
        Popup *popup_left = popup_btn->popup();
        popup_left->set_layout(new GroupLayout());
        new CheckBox(popup_left, "Another check box");

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
            dlg->set_callback([](int result) { std::cout << "Dialog result: " << result << std::endl; });
        });
        b = new Button(tools, "Warn");
        b->set_callback([&] {
            auto dlg = new MessageDialog(this, MessageDialog::Type::Warning, "Title", "This is a warning message");
            dlg->set_callback([](int result) { std::cout << "Dialog result: " << result << std::endl; });
        });
        b = new Button(tools, "Ask");
        b->set_callback([&] {
            auto dlg = new MessageDialog(this, MessageDialog::Type::Warning, "Title", "This is a question message", "Yes", "No", true);
            dlg->set_callback([](int result) { std::cout << "Dialog result: " << result << std::endl; });
        });

#if defined(_WIN32)
        /// Executable is in the Debug/Release/.. subdirectory
        std::string resources_folder_path("../icons");
#else
        std::string resources_folder_path("./icons");
#endif
        std::vector<std::pair<int, std::string>> icons;

#if !defined(EMSCRIPTEN)
        try {
            icons = load_image_directory(m_nvg_context, resources_folder_path);
        } catch (const std::exception &e) {
            std::cerr << "Warning: " << e.what() << std::endl;
        }
#endif

        new Label(window, "Image panel & scroll panel", "sans-bold");
        PopupButton *image_panel_btn = new PopupButton(window, "Image Panel");
        image_panel_btn->set_icon(FA_IMAGES);
        popup = image_panel_btn->popup();
        VScrollPanel *vscroll = new VScrollPanel(popup);
        ImagePanel *img_panel = new ImagePanel(vscroll);
        img_panel->set_images(icons);
        popup->set_fixed_size(Vector2i(245, 150));

        auto image_window = new Window(this, "Selected image");
        image_window->set_position(Vector2i(710, 15));
        image_window->set_layout(new GroupLayout(3));

        // Create a Texture instance for each object
        for (auto& icon : icons) {
            Vector2i size;
            int n = 0;
            ImageHolder texture_data(
                stbi_load((icon.second + ".png").c_str(), &size.x(), &size.y(), &n, 0),
                stbi_image_free);
            assert(n == 4);

            Texture *tex = new Texture(
                Texture::PixelFormat::RGBA,
                Texture::ComponentFormat::UInt8,
                size,
                Texture::InterpolationMode::Trilinear,
                Texture::InterpolationMode::Nearest);

            tex->upload(texture_data.get());

            m_images.emplace_back(tex, std::move(texture_data));
        }

        ImageView *image_view = new ImageView(image_window);
        if (!m_images.empty())
            image_view->set_image(m_images[0].first);
        image_view->center();
        m_current_image = 0;

        img_panel->set_callback([this, image_view](int i) {
            std::cout << "Selected item " << i << std::endl;
            image_view->set_image(m_images[i].first);
            m_current_image = i;
        });

        image_view->set_pixel_callback(
            [this](const Vector2i& index, char **out, size_t size) {
                const Texture *texture = m_images[m_current_image].first.get();
                uint8_t *data = m_images[m_current_image].second.get();
                for (int ch = 0; ch < 4; ++ch) {
                    uint8_t value = data[(index.x() + index.y() * texture->size().x())*4 + ch];
                    snprintf(out[ch], size, "%i", (int) value);
                }
            }
        );

        new Label(window, "File dialog", "sans-bold");
        tools = new Widget(window);
        tools->set_layout(new BoxLayout(Orientation::Horizontal,
                                       Alignment::Middle, 0, 6));
        b = new Button(tools, "Open");
        b->set_callback([&] {
            std::cout << "File dialog result: " << file_dialog(
                    { {"png", "Portable Network Graphics"}, {"txt", "Text file"} }, false) << std::endl;
        });
        b = new Button(tools, "Save");
        b->set_callback([&] {
            std::cout << "File dialog result: " << file_dialog(
                    { {"png", "Portable Network Graphics"}, {"txt", "Text file"} }, true) << std::endl;
        });

        new Label(window, "Combo box", "sans-bold");
        new ComboBox(window, { "Combo box item 1", "Combo box item 2", "Combo box item 3"});
        new Label(window, "Check box", "sans-bold");
        CheckBox *cb = new CheckBox(window, "Flag 1",
            [](bool state) { std::cout << "Check box 1 state: " << state << std::endl; }
        );
        cb->set_checked(true);
        cb = new CheckBox(window, "Flag 2",
            [](bool state) { std::cout << "Check box 2 state: " << state << std::endl; }
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
            std::cout << "Final slider value: " << (int) (value * 100) << std::endl;
        });
        text_box->set_fixed_size(Vector2i(60,25));
        text_box->set_font_size(20);
        text_box->set_alignment(TextBox::Alignment::Right);

        window = new Window(this, "Misc. widgets");
        window->set_position(Vector2i(425,15));
        window->set_layout(new GroupLayout());

        TabWidget* tab_widget = window->add<TabWidget>();

        Widget* layer = new Widget(tab_widget);
        layer->set_layout(new GroupLayout());
        tab_widget->append_tab("Color Wheel", layer);

        // Use overloaded variadic add to fill the tab widget with Different tabs.
        layer->add<Label>("Color wheel widget", "sans-bold");
        layer->add<ColorWheel>();

        layer = new Widget(tab_widget);
        layer->set_layout(new GroupLayout());
        tab_widget->append_tab("Function Graph", layer);

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
        int plus_id = tab_widget->append_tab("+", new Widget(tab_widget));

        // A simple counter.
        int counter = 1;
        tab_widget->set_callback([tab_widget, this, counter, plus_id] (int id) mutable {
            if (id == plus_id) {
                // When the "+" tab has been clicked, simply add a new tab.
                std::string tab_name = "Dynamic " + std::to_string(counter);
                Widget* layer_dyn = new Widget(tab_widget);
                int new_id = tab_widget->insert_tab(tab_widget->tab_count() - 1,
                                                    tab_name, layer_dyn);
                layer_dyn->set_layout(new GroupLayout());
                layer_dyn->add<Label>("Function graph widget", "sans-bold");
                Graph *graph_dyn = layer_dyn->add<Graph>("Dynamic function");

                graph_dyn->set_header("E = 2.35e-3");
                graph_dyn->set_footer("Iteration " + std::to_string(new_id*counter));
                std::vector<float> &func_dyn = graph_dyn->values();
                func_dyn.resize(100);
                for (int i = 0; i < 100; ++i)
                    func_dyn[i] = 0.5f *
                        std::abs((0.5f * std::sin(i / 10.f + counter) +
                                  0.5f * std::cos(i / 23.f + 1 + counter)));
                ++counter;
                tab_widget->set_selected_id(new_id);

                // We must invoke the layout manager after adding tabs dynamically
                perform_layout();
            }
        });

        // A button to go back to the first tab and scroll the window.
        panel = window->add<Widget>();
        panel->add<Label>("Jump to tab: ");
        panel->set_layout(new BoxLayout(Orientation::Horizontal,
                                       Alignment::Middle, 0, 6));

        auto ib = panel->add<IntBox<int>>();
        ib->set_editable(true);

        b = panel->add<Button>("", FA_FORWARD);
        b->set_fixed_size(Vector2i(22, 22));
        ib->set_fixed_height(22);
        b->set_callback([tab_widget, ib] {
            int value = ib->value();
            if (value >= 0 && value < tab_widget->tab_count())
                tab_widget->set_selected_index(value);
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

        new Label(window, "Color picker :", "sans-bold");
        auto cp = new ColorPicker(window, {255, 120, 0, 255});
        cp->set_fixed_size({100, 20});
        cp->set_final_callback([](const Color &c) {
            std::cout << "ColorPicker final callback: ["
                      << c.r() << ", "
                      << c.g() << ", "
                      << c.b() << ", "
                      << c.w() << "]" << std::endl;
        });
        // setup a fast callback for the color picker widget on a new window
        // for demonstrative purposes
        window = new Window(this, "Color Picker Fast Callback");
        layout = new GridLayout(Orientation::Horizontal, 2,
                                 Alignment::Middle, 15, 5);
        layout->set_col_alignment(
            { Alignment::Maximum, Alignment::Fill });
        layout->set_spacing(0, 10);
        window->set_layout(layout);
        window->set_position(Vector2i(425, 500));
        new Label(window, "Combined: ");
        b = new Button(window, "ColorWheel", FA_INFINITY);
        new Label(window, "Red: ");
        auto red_int_box = new IntBox<int>(window);
        red_int_box->set_editable(false);
        new Label(window, "Green: ");
        auto green_int_box = new IntBox<int>(window);
        green_int_box->set_editable(false);
        new Label(window, "Blue: ");
        auto blue_int_box = new IntBox<int>(window);
        blue_int_box->set_editable(false);
        new Label(window, "Alpha: ");
        auto alpha_int_box = new IntBox<int>(window);

        cp->set_callback([b,red_int_box,blue_int_box,green_int_box,alpha_int_box](const Color &c) {
            b->set_background_color(c);
            b->set_text_color(c.contrasting_color());
            int red = (int) (c.r() * 255.0f);
            red_int_box->set_value(red);
            int green = (int) (c.g() * 255.0f);
            green_int_box->set_value(green);
            int blue = (int) (c.b() * 255.0f);
            blue_int_box->set_value(blue);
            int alpha = (int) (c.w() * 255.0f);
            alpha_int_box->set_value(alpha);
        });

        perform_layout();

        /* All NanoGUI widgets are initialized at this point. Now
           create shaders to draw the main window contents.

           NanoGUI comes with a simple wrapper around OpenGL 3, which
           eliminates most of the tedious and error-prone shader and buffer
           object management.
        */

        m_render_pass = new RenderPass({ this });
        m_render_pass->set_clear_color(0, Color(0.3f, 0.3f, 0.32f, 1.f));

        m_shader = new Shader(
            m_render_pass,

            /* An identifying name */
            "a_simple_shader",

#if defined(NANOGUI_USE_OPENGL)
            R"(/* Vertex shader */
            #version 330
            uniform mat4 mvp;
            in vec3 position;
            void main() {
                gl_Position = mvp * vec4(position, 1.0);
            })",

            /* Fragment shader */
            R"(#version 330
            out vec4 color;
            uniform float intensity;
            void main() {
                color = vec4(vec3(intensity), 1.0);
            })"
#elif defined(NANOGUI_USE_GLES)
            R"(/* Vertex shader */
            precision highp float;
            uniform mat4 mvp;
            attribute vec3 position;
            void main() {
                gl_Position = mvp * vec4(position, 1.0);
            })",

            /* Fragment shader */
            R"(precision highp float;
            uniform float intensity;
            void main() {
                gl_FragColor = vec4(vec3(intensity), 1.0);
            })"
#elif defined(NANOGUI_USE_METAL)
            R"(using namespace metal;
            struct VertexOut {
                float4 position [[position]];
            };

            vertex VertexOut vertex_main(const device packed_float3 *position,
                                         constant float4x4 &mvp,
                                         uint id [[vertex_id]]) {
                VertexOut vert;
                vert.position = mvp * float4(position[id], 1.f);
                return vert;
            })",

            /* Fragment shader */
            R"(using namespace metal;
            fragment float4 fragment_main(const constant float &intensity) {
                return float4(intensity);
            })"
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

        m_shader->set_buffer("indices", enoki::EnokiType::UInt32, 1, {3*2, 1, 1}, indices);
        m_shader->set_buffer("position", enoki::EnokiType::Float32, 2, {4, 3, 1}, positions);
        m_shader->set_uniform("intensity", 0.5f);
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
        Matrix4f mvp = enoki::scale<Matrix4f>(Vector3f(
                           (float) m_size.y() / (float) m_size.x() * 0.25f, 0.25f, 0.25f)) *
                       enoki::rotate<Matrix4f>(Vector3f(0, 0, 1), (float) glfwGetTime());

        m_shader->set_uniform("mvp", mvp);

        m_render_pass->resize(framebuffer_size());
        m_render_pass->begin();

        m_shader->begin();
        m_shader->draw_array(Shader::PrimitiveType::Triangle, 0, 6, true);
        m_shader->end();

        m_render_pass->end();
    }
private:
    ProgressBar *m_progress;
    ref<Shader> m_shader;
    ref<RenderPass> m_render_pass;

    using ImageHolder = std::unique_ptr<uint8_t[], void(*)(void*)>;
    std::vector<std::pair<ref<Texture>, ImageHolder>> m_images;
    int m_current_image;
};

int main(int /* argc */, char ** /* argv */) {
    try {
        nanogui::init();

        /* scoped variables */ {
            ref<ExampleApplication> app = new ExampleApplication();
            app->dec_ref();
            app->draw_all();
            app->set_visible(true);
            nanogui::mainloop(1 / 60.f * 1000);
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
