#include <iostream>

#include <nanogui/screen.h>
#include <nanogui/window.h>
#include <nanogui/layout.h>
#include <nanogui/label.h>
#include <nanogui/slider.h>
#include <nanogui/theme.h>
#include <nanogui/knob.h>

namespace nga {
using namespace nanogui;
using std::cout, std::endl;

class ExampleApplication : public Screen {
public:
    ExampleApplication() : Screen(Vector2i(1024, 768), "Knob example") {
        inc_ref();
        Window *window = new Window(this, "Knob example");
        window->set_position(Vector2i(15, 15));
        window->set_fixed_size(Vector2i(400, 650));

        Theme* th = new Theme(this->nvg_context());
        th->m_window_fill_focused =  m_theme->m_border_light;
        th->m_window_fill_unfocused =  m_theme->m_border_light;
        window->set_theme(th);

        window->set_layout(new BoxLayout(Orientation::Vertical,Alignment::Middle,40,20 ));

        new Label(window, "Vertical layout", "sans-bold");

        Slider *slider = new Slider(window); //provare argomento this
        slider->set_value(0.5f);
        slider->set_fixed_width(150);
        slider->set_highlighted_range({0.3f,0.5f});

        Knob* k = new Knob(window);
        k->set_value(0.25f);
        k->set_final_callback([](float f){ std::cout << "Knob k final value: " << f << endl; });
        k->set_callback([](float f){ std::cout << "Knob k value: " << f << endl; });
        k->set_range({35.0f,97.0f});
        k->set_highlighted_range({50.0f,60.0f});

        Knob* k2 = new Knob(window, 30);
        k2->set_value(0.45f);
        k2->set_notches(0);

        Knob* k3 = new Knob(window, 300);
        k3->set_value(0.45f);
        k3->set_notches(24);
        k3->set_final_callback([](float f){ std::cout << "Knob k3 final value: " << f << endl; });
        k3->set_callback([](float f){ std::cout << "Knob k3 value: " << f << endl; });
        k3->set_highlighted_range({0.90f,1.0f});

        perform_layout();
    }
};

}

int main()
{
    using std::cout, std::endl;
    cout << "Nano GUI Knob example" << endl;
    cout << "---------------------" << endl;

    try {
        nanogui::init();

        /* scoped variables */ {
            nanogui::ref<nga::ExampleApplication> app = new nga::ExampleApplication();
            app->dec_ref();
            app->draw_all();
            app->set_visible(true);
            nanogui::mainloop(1 / 60.f * 1000);
        }

        nanogui::shutdown();
    } catch (const std::exception &e) {
        std::string error_msg = std::string("Caught a fatal error: ") + std::string(e.what());
        std::cerr << error_msg << std::endl;
        return -1;
    } catch (...) {
        std::cerr << "Caught an unknown error!" << std::endl;
    }

    return 0;
}
