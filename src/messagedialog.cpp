/*
    src/messagedialog.cpp -- Simple "OK" or "Yes/No"-style modal dialogs

    NanoGUI was developed by Wenzel Jakob <wenzel.jakob@epfl.ch>.
    The widget drawing code is based on the NanoVG demo application
    by Mikko Mononen.

    All rights reserved. Use of this source code is governed by a
    BSD-style license that can be found in the LICENSE.txt file.
*/

#include <nanogui/messagedialog.h>
#include <nanogui/layout.h>
#include <nanogui/button.h>
#include <nanogui/entypo.h>
#include <nanogui/label.h>

NAMESPACE_BEGIN(nanogui)

MessageDialog::MessageDialog(Widget *parent, Type type, const std::string &title,
              const std::string &message,
              const std::string &button_text,
              const std::string &alt_button_text, bool alt_button) : Window(parent, title) {
    set_layout(new BoxLayout(Orientation::Vertical,
                            Alignment::Middle, 10, 10));
    set_modal(true);

    Widget *panel1 = new Widget(this);
    panel1->set_layout(new BoxLayout(Orientation::Horizontal,
                                    Alignment::Middle, 10, 15));
    int icon = 0;
    switch (type) {
        case Type::Information: icon = ENTYPO_ICON_CIRCLED_INFO; break;
        case Type::Question: icon = ENTYPO_ICON_CIRCLED_HELP; break;
        case Type::Warning: icon = ENTYPO_ICON_WARNING; break;
    }
    Label *icon_label = new Label(panel1, std::string(utf8(icon).data()), "icons");
    icon_label->set_font_size(50);
    m_message_label = new Label(panel1, message);
    m_message_label->set_fixed_width(200);
    Widget *panel2 = new Widget(this);
    panel2->set_layout(new BoxLayout(Orientation::Horizontal,
                                    Alignment::Middle, 0, 15));

    if (alt_button) {
        Button *button = new Button(panel2, alt_button_text, ENTYPO_ICON_CIRCLED_CROSS);
        button->set_callback([&] { if (m_callback) m_callback(1); dispose(); });
    }
    Button *button = new Button(panel2, button_text, ENTYPO_ICON_CHECK);
    button->set_callback([&] { if (m_callback) m_callback(0); dispose(); });
    center();
    request_focus();
}

NAMESPACE_END(nanogui)
