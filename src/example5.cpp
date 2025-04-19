/*
    src/example5.cpp -- A few more widget examples

    NanoGUI was developed by Wenzel Jakob <wenzel.jakob@epfl.ch>.
    The widget drawing code is based on the NanoVG demo application
    by Mikko Mononen.

    All rights reserved. Use of this source code is governed by a
    BSD-style license that can be found in the LICENSE.txt file.
*/

#include <nanogui/screen.h>
#include <nanogui/window.h>
#include <nanogui/borderlayout.h>
#include <nanogui/button.h>
#include <nanogui/combobox.h>
#include <nanogui/label.h>
#include <nanogui/popup.h>
#include <iostream>

using namespace nanogui;

class ExampleApplication : public Screen {
public:
    ExampleApplication() : Screen(Vector2i(1024, 768), "NanoGUI More test") {
        inc_ref();

        // Simple border layout example
        {
            Window *win = new Window(this, "Simple BorderLayout");
            win->set_position({ 10, 10 });
            BorderLayout * bl = new BorderLayout();
            win->set_layout(bl);
            Button *north = new Button(win, "North");
            bl->set_side(north, BorderLayout::North);
            Button *south = new Button(win, "South");
            bl->set_side(south, BorderLayout::South);
            Button *east = new Button(win, "East");
            bl->set_side(east, BorderLayout::East);
            Button *west = new Button(win, "West");
            bl->set_side(west, BorderLayout::West);
            Button *center = new Button(win, "Center");
            center->set_fixed_size({ 150, 150 });
            south->set_fixed_height(100);
        }

        // A slightly more complicated border layout example
        {
            Window * win = new Window(this, "Complex BorderLayout");
            win->set_position({ 300, 10 });
            BorderLayout *bl = new BorderLayout();
            win->set_layout(bl);
            Button *north1 = new Button(win, "North#1");
            bl->set_side(north1, BorderLayout::North);
            Button *east1 = new Button(win, "East#1");
            bl->set_side(east1, BorderLayout::East);
            Button *east2 = new Button(win, "East#2");
            bl->set_side(east2, BorderLayout::East);
            Button *north2 = new Button(win, "North#2");
            bl->set_side(north2, BorderLayout::North);
            Button *south = new Button(win, "South");
            bl->set_side(south, BorderLayout::South);
            Button *east3 = new Button(win, "East#3");
            bl->set_side(east3, BorderLayout::East);
            Button *center = new Button(win, "Center");
            center->set_fixed_size({ 150, 150 });
            south->set_fixed_height(100);
        }

        // Test window for popup location
        {
            Window * win = new Window(this, "Popups");
            win->set_position({ 700, 10 });
            BorderLayout *bl = new BorderLayout();
            win->set_layout(bl);

            // Add some dummy space to the left and right (to demonstrate the difference between Popup::Left and Popup::LeftInside)
            Widget *dummy_left_panel = new Widget(win);
            bl->set_side(dummy_left_panel, BorderLayout::West);
            dummy_left_panel->set_fixed_width(50);
            Widget *dummy_right_panel = new Widget(win);
            bl->set_side(dummy_right_panel, BorderLayout::East);
            dummy_right_panel->set_fixed_width(50);

            // Main container
            Widget *container = new Widget(win);
            container->set_layout(new BoxLayout(Orientation::Vertical, Alignment::Fill, 8, 8));

            // Create a standard combo box
            std::vector<std::string> fruits{ "Apple", "Banana", "Lemon", "Strawberry" };
            new Label(container, "ComboBox:");
            /* ComboBox * combo0 = */ new ComboBox(container, fruits);
            /* ComboBox * combo0l = */ (new ComboBox(container, fruits))->set_side(Popup::Left);
            new Label(container, "ComboBox(inside):");
            /* ComboBox * combo1 = */ (new ComboBox(container, fruits))->set_side(Popup::RightInside);
            /* ComboBox * combo1l = */ (new ComboBox(container, fruits))->set_side(Popup::LeftInside);
        }

        perform_layout();
    }
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
    } catch (const std::exception &e) {
        std::string error_msg = std::string("Caught a fatal error: ") + std::string(e.what());
        #if defined(_WIN32)
            MessageBoxA(nullptr, error_msg.c_str(), NULL, MB_ICONERROR | MB_OK);
        #else
            std::cerr << error_msg << std::endl;
        #endif
        return -1;
    } catch (...) {
        std::cerr << "Caught an unknown error!" << std::endl;
    }

    return 0;
}
