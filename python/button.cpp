#ifdef NANOGUI_PYTHON

#include "python.h"

DECLARE_WIDGET(Button);
DECLARE_WIDGET(ToolButton);
DECLARE_WIDGET(PopupButton);
DECLARE_WIDGET(CheckBox);

void register_button(py::module &m) {
    py::class_<Button, Widget, ref<Button>, PyButton> button(m, "Button", D(Button));
    button
        .def(py::init<Widget *, const std::string &, int>(),
             "parent"_a, "caption"_a = std::string("Untitled"), "icon"_a = 0, D(Button, Button))
        .def("caption", &Button::caption, D(Button, caption))
        .def("set_caption", &Button::set_caption, D(Button, set_caption))
        .def("background_color", &Button::background_color, D(Button, background_color))
        .def("set_background_color", &Button::set_background_color, D(Button, set_background_color))
        .def("text_color", &Button::text_color, D(Button, text_color))
        .def("set_text_color", &Button::set_text_color, D(Button, set_text_color))
        .def("icon", &Button::icon, D(Button, icon))
        .def("set_icon", &Button::set_icon, D(Button, set_icon))
        .def("flags", &Button::flags, D(Button, flags))
        .def("set_flags", &Button::set_flags, D(Button, set_flags))
        .def("icon_position", &Button::icon_position, D(Button, icon_position))
        .def("set_icon_position", &Button::set_icon_position, D(Button, set_icon_position))
        .def("pushed", &Button::pushed, D(Button, pushed))
        .def("set_pushed", &Button::set_pushed, D(Button, set_pushed))
        .def("callback", &Button::callback, D(Button, callback))
        .def("set_callback", &Button::set_callback, D(Button, set_callback))
        .def("change_callback", &Button::change_callback, D(Button, change_callback))
        .def("set_change_callback", &Button::set_change_callback, D(Button, set_change_callback))
        .def("button_group", &Button::button_group, D(Button, button_group))
        .def("set_button_group", &Button::set_button_group, D(Button, set_button_group));

    py::enum_<Button::IconPosition>(button, "IconPosition", D(Button, IconPosition))
        .value("Left", Button::IconPosition::Left)
        .value("LeftCentered", Button::IconPosition::LeftCentered)
        .value("RightCentered", Button::IconPosition::RightCentered)
        .value("Right", Button::IconPosition::Right);

    py::enum_<Button::Flags>(button, "Flags", D(Button, Flags))
        .value("NormalButton", Button::Flags::NormalButton)
        .value("RadioButton", Button::Flags::RadioButton)
        .value("ToggleButton", Button::Flags::ToggleButton)
        .value("PopupButton", Button::Flags::PopupButton)
        .value("MenuButton", Button::Flags::MenuButton);

    py::class_<ToolButton, Button, ref<ToolButton>, PyToolButton>(m, "ToolButton", D(ToolButton))
        .def(py::init<Widget *,int, const std::string &>(),
             "parent"_a, "icon"_a, "caption"_a = std::string(""),
             D(ToolButton, ToolButton));

    py::class_<PopupButton, Button, ref<PopupButton>, PyPopupButton> popup_btn(m, "PopupButton", D(PopupButton));
    popup_btn
        .def(py::init<Widget *, const std::string&, int>(),
                "parent"_a, "caption"_a = std::string("Untitled"),
                "button_icon"_a = 0, D(PopupButton, PopupButton))
        .def("popup", (Popup*(PopupButton::*)(void)) &PopupButton::popup, D(PopupButton, popup))
        .def("chevron_icon", &PopupButton::chevron_icon, D(PopupButton, chevron_icon))
        .def("set_chevron_icon", &PopupButton::set_chevron_icon, D(PopupButton, set_chevron_icon))
        .def("side", &PopupButton::side, D(PopupButton, side))
        .def("set_side", &PopupButton::set_side, D(PopupButton, set_side));

    py::class_<CheckBox, Widget, ref<CheckBox>, PyCheckBox>(m, "CheckBox", D(CheckBox))
        .def(py::init<Widget *, const std::string &>(), "parent"_a,
             "caption"_a = std::string("Untitled"),
             D(CheckBox, CheckBox))
        .def(py::init<Widget *, const std::string &, const std::function<void(bool)>&>(),
             "parent"_a, "caption"_a, "callback"_a,
             D(CheckBox, CheckBox))
        .def("caption", &CheckBox::caption, D(CheckBox, caption))
        .def("set_caption", &CheckBox::set_caption, D(CheckBox, set_caption))
        .def("checked", &CheckBox::checked, D(CheckBox, checked))
        .def("set_checked", &CheckBox::set_checked, D(CheckBox, set_checked))
        .def("pushed", &CheckBox::pushed, D(CheckBox, pushed))
        .def("set_pushed", &CheckBox::set_pushed, D(CheckBox, set_pushed))
        .def("callback", &CheckBox::callback, D(CheckBox, callback))
        .def("set_callback", &CheckBox::set_callback, D(CheckBox, set_callback));
}

#endif
