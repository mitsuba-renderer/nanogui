#ifdef NANOGUI_PYTHON

#include "python.h"

DECLARE_WIDGET(Button);
DECLARE_WIDGET(ToolButton);
DECLARE_WIDGET(PopupButton);
DECLARE_WIDGET(CheckBox);

void register_button(nb::module_ &m) {
    nb::class_<Button, Widget, PyButton> button(m, "Button", D(Button));

    button
        .def(nb::init<Widget *, const std::string &, int>(),
             "parent"_a, "caption"_a = std::string("Untitled"), "icon"_a = 0, D(Button, Button))
        .def(nb_method(Button, caption), D(Button, caption))
        .def(nb_method(Button, set_caption), D(Button, set_caption))
        .def(nb_method(Button, background_color), D(Button, background_color))
        .def(nb_method(Button, set_background_color), D(Button, set_background_color))
        .def(nb_method(Button, text_color), D(Button, text_color))
        .def(nb_method(Button, set_text_color), D(Button, set_text_color))
        .def(nb_method(Button, icon), D(Button, icon))
        .def(nb_method(Button, set_icon), D(Button, set_icon))
        .def(nb_method(Button, flags), D(Button, flags))
        .def(nb_method(Button, set_flags), D(Button, set_flags))
        .def(nb_method(Button, icon_position), D(Button, icon_position))
        .def(nb_method(Button, set_icon_position), D(Button, set_icon_position))
        .def(nb_method(Button, pushed), D(Button, pushed))
        .def(nb_method(Button, set_pushed), D(Button, set_pushed))
        .def(nb_method(Button, callback), D(Button, callback))
        .def(nb_method(Button, set_callback), D(Button, set_callback))
        .def(nb_method(Button, change_callback), D(Button, change_callback))
        .def(nb_method(Button, set_change_callback), D(Button, set_change_callback))
        .def(nb_method(Button, button_group), D(Button, button_group))
        .def(nb_method(Button, set_button_group), D(Button, set_button_group));

    nb::enum_<Button::IconPosition>(button, "IconPosition", D(Button, IconPosition))
        .value("Left", Button::IconPosition::Left)
        .value("LeftCentered", Button::IconPosition::LeftCentered)
        .value("RightCentered", Button::IconPosition::RightCentered)
        .value("Right", Button::IconPosition::Right);

    nb::enum_<Button::Flags>(button, "Flags", D(Button, Flags))
        .value("NormalButton", Button::Flags::NormalButton)
        .value("RadioButton", Button::Flags::RadioButton)
        .value("ToggleButton", Button::Flags::ToggleButton)
        .value("PopupButton", Button::Flags::PopupButton)
        .value("MenuButton", Button::Flags::MenuButton);

    nb::class_<ToolButton, Button, PyToolButton>(m, "ToolButton", D(ToolButton))
        .def(nb::init<Widget *,int, const std::string &>(),
             "parent"_a, "icon"_a, "caption"_a = std::string(""),
             D(ToolButton, ToolButton));

    nb::class_<PopupButton, Button, PyPopupButton> popup_btn(m, "PopupButton", D(PopupButton));
    popup_btn
        .def(nb::init<Widget *, const std::string&, int>(),
                "parent"_a, "caption"_a = std::string("Untitled"),
                "button_icon"_a = 0, D(PopupButton, PopupButton))
        .def("popup", (Popup*(PopupButton::*)(void)) &PopupButton::popup, D(PopupButton, popup))
        .def(nb_method(PopupButton, chevron_icon), D(PopupButton, chevron_icon))
        .def(nb_method(PopupButton, set_chevron_icon), D(PopupButton, set_chevron_icon))
        .def(nb_method(PopupButton, side), D(PopupButton, side))
        .def(nb_method(PopupButton, set_side), D(PopupButton, set_side));

    nb::class_<CheckBox, Widget, PyCheckBox>(m, "CheckBox", D(CheckBox))
        .def(nb::init<Widget *, const std::string &>(), "parent"_a,
             "caption"_a = std::string("Untitled"),
             D(CheckBox, CheckBox))
        .def(nb::init<Widget *, const std::string &, const std::function<void(bool)>&>(),
             "parent"_a, "caption"_a, "callback"_a,
             D(CheckBox, CheckBox))
        .def(nb_method(CheckBox, caption), D(CheckBox, caption))
        .def(nb_method(CheckBox, set_caption), D(CheckBox, set_caption))
        .def(nb_method(CheckBox, checked), D(CheckBox, checked))
        .def(nb_method(CheckBox, set_checked), D(CheckBox, set_checked))
        .def(nb_method(CheckBox, pushed), D(CheckBox, pushed))
        .def(nb_method(CheckBox, set_pushed), D(CheckBox, set_pushed))
        .def(nb_method(CheckBox, callback), D(CheckBox, callback))
        .def(nb_method(CheckBox, set_callback), D(CheckBox, set_callback));
}

#endif
