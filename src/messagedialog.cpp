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
#include <nanogui/label.h>
#include <nanogui/screen.h>
#include <nanogui/vscrollpanel.h>

NAMESPACE_BEGIN(nanogui)

MessageDialog::MessageDialog(Widget *parent, Type type, std::string_view title,
              std::string_view message,
              std::string_view button_text,
              std::string_view alt_button_text, bool alt_button,
              int max_width, int max_height) : Window(parent, title) {
    set_layout(new BoxLayout(Orientation::Vertical,
                            Alignment::Middle, 10, 10));
    set_modal(true);

    Widget *panel1 = new Widget(this);
    panel1->set_layout(new BoxLayout(Orientation::Horizontal,
                                     Alignment::Middle, 10, 15));
    int icon = 0;
    switch (type) {
        case Type::Information: icon = m_theme->m_message_information_icon; break;
        case Type::Question: icon = m_theme->m_message_question_icon; break;
        case Type::Warning: icon = m_theme->m_message_warning_icon; break;
    }
    Label *icon_label = new Label(panel1, std::string(utf8(icon).data()), "icons");
    icon_label->set_font_size(50);

    VScrollPanel *scroll_panel = new VScrollPanel(panel1);
    m_message_label = new Label(scroll_panel, message);

    Screen *screen = this->screen();
    if (screen) {
        Vector2i pref_size = m_message_label->preferred_size(screen->nvg_context());
        if (pref_size.x() > max_width)
            m_message_label->set_fixed_width(max_width);
        if (pref_size.y() > max_height)
            scroll_panel->set_fixed_height(max_height);
    }

    Widget *panel2 = new Widget(this);
    panel2->set_layout(new BoxLayout(Orientation::Horizontal,
                                     Alignment::Middle, 0, 15));

    if (alt_button) {
        Button *button = new Button(panel2, alt_button_text, m_theme->m_message_alt_button_icon);
        button->set_callback([&] { if (m_callback) m_callback(1); dispose(); });
    }
    Button *button = new Button(panel2, button_text, m_theme->m_message_primary_button_icon);
    button->set_callback([&] { if (m_callback) m_callback(0); dispose(); });
    center();
    request_focus();
}

NAMESPACE_END(nanogui)
