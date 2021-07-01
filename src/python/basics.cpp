#ifdef NANOGUI_PYTHON

#include "python.h"

DECLARE_WIDGET(Label);
DECLARE_WIDGET(Popup);
DECLARE_WIDGET(MessageDialog);
DECLARE_WIDGET(VScrollPanel);
DECLARE_WIDGET(ComboBox);
DECLARE_WIDGET(ProgressBar);
DECLARE_WIDGET(Slider);

void register_basics(py::module &m) {
    py::class_<Label, Widget, ref<Label>, PyLabel>(m, "Label", D(Label))
        .def(py::init<Widget *, const std::string &, const std::string &, int>(),
             "parent"_a, "caption"_a, "font"_a = std::string("sans"),
             "font_size"_a = -1,
             D(Label, Label))
        .def("caption", &Label::caption, D(Label, caption))
        .def("set_caption", &Label::set_caption, D(Label, set_caption))
        .def("font", &Label::font, D(Label, font))
        .def("set_font", &Label::set_font, D(Label, set_font))
        .def("color", &Label::color, D(Label, color))
        .def("set_color", &Label::set_color, D(Label, set_color));


    py::class_<Popup, Window, ref<Popup>, PyPopup> popup(m, "Popup", D(Popup));
    popup
        .def(py::init<Widget *, Window *>(), "parent"_a, "parent_window"_a, D(Popup, Popup))
        .def("anchor_pos", &Popup::anchor_pos, D(Popup, anchor_pos))
        .def("set_anchor_pos", &Popup::set_anchor_pos, D(Popup, set_anchor_pos))
        .def("anchor_offset", &Popup::anchor_offset, D(Popup, anchor_offset))
        .def("set_anchor_offset", &Popup::set_anchor_offset, D(Popup, set_anchor_offset))
        .def("anchor_size", &Popup::anchor_size, D(Popup, anchor_size))
        .def("set_anchor_size", &Popup::set_anchor_size, D(Popup, set_anchor_size))
        .def("parent_window", (Window*(Popup::*)(void)) &Popup::parent_window, D(Popup, parent_window))
        .def("side", &Popup::side, D(Popup, side))
        .def("set_side", &Popup::set_side, D(Popup, set_side));

    py::enum_<Popup::Side>(popup, "Side", D(Popup, Side))
        .value("Left", Popup::Side::Left)
        .value("Right", Popup::Side::Right)
        .export_values();

    py::class_<MessageDialog, Window, ref<MessageDialog>, PyMessageDialog> mdlg(m, "MessageDialog", D(MessageDialog));
    mdlg
        .def(py::init<Widget *, MessageDialog::Type, const std::string&,
                      const std::string&, const std::string&, const std::string&, bool>(),
            "parent"_a, "type"_a, "title"_a = std::string("Untitled"),
            "message"_a = std::string("Message"), "button_text"_a = std::string("OK"),
            "alt_button_text"_a = std::string("Cancel"), "alt_button"_a = false,
            D(MessageDialog, MessageDialog))
        .def("message_label", (Label * (MessageDialog::*)()) &MessageDialog::message_label, D(MessageDialog, message_label))
        .def("callback", &MessageDialog::callback, D(MessageDialog, callback))
        .def("set_callback", &MessageDialog::set_callback, D(MessageDialog, set_callback));

    py::enum_<MessageDialog::Type>(mdlg, "Type", D(MessageDialog, Type))
        .value("Information", MessageDialog::Type::Information)
        .value("Question", MessageDialog::Type::Question)
        .value("Warning", MessageDialog::Type::Warning);

    py::class_<VScrollPanel, Widget, ref<VScrollPanel>, PyVScrollPanel>(m, "VScrollPanel", D(VScrollPanel))
        .def(py::init<Widget *>(), "parent"_a, D(VScrollPanel, VScrollPanel))
        .def("scroll", &VScrollPanel::scroll, D(VScrollPanel, scroll))
        .def("set_scroll", &VScrollPanel::set_scroll, D(VScrollPanel, set_scroll));

    py::class_<ComboBox, Widget, ref<ComboBox>, PyComboBox>(m, "ComboBox", D(ComboBox))
        .def(py::init<Widget *>(), "parent"_a, D(ComboBox, ComboBox))
        .def(py::init<Widget *, const std::vector<std::string> &>(),
             "parent"_a, "items"_a/*, D(ComboBox, ComboBox, 2)*/)
        .def(py::init<Widget *, const std::vector<std::string> &,
                      const std::vector<std::string> &>(),
             "parent"_a, "items"_a, "items_short"_a/* ,D(ComboBox, ComboBox, 3)*/)
        .def("callback", &ComboBox::callback, D(ComboBox, callback))
        .def("set_callback", &ComboBox::set_callback, D(ComboBox, set_callback))
        .def("selected_index", &ComboBox::selected_index, D(ComboBox, selected_index))
        .def("set_selected_index", &ComboBox::set_selected_index, D(ComboBox, set_selected_index))
        .def("set_items", (void(ComboBox::*)(const std::vector<std::string>&)) &ComboBox::set_items, D(ComboBox, set_items))
        .def("set_items", (void(ComboBox::*)(const std::vector<std::string>&,
                          const std::vector<std::string>&)) &ComboBox::set_items/*, D(ComboBox, set_items, 2)*/)
        .def("items", &ComboBox::items, D(ComboBox, items))
        .def("items_short", &ComboBox::items_short, D(ComboBox, items_short));

    py::class_<ProgressBar, Widget, ref<ProgressBar>, PyProgressBar>(m, "ProgressBar", D(ProgressBar))
        .def(py::init<Widget *>(), "parent"_a, D(ProgressBar, ProgressBar))
        .def("value", &ProgressBar::value, D(ProgressBar, value))
        .def("set_value", &ProgressBar::set_value, D(ProgressBar, set_value));

    py::class_<Slider, Widget, ref<Slider>, PySlider>(m, "Slider", D(Slider))
        .def(py::init<Widget *>(), "parent"_a, D(Slider, Slider))
        .def("value", &Slider::value, D(Slider, value))
        .def("set_value", &Slider::set_value, D(Slider, set_value))
        .def("highlight_color", &Slider::highlight_color, D(Slider, highlight_color))
        .def("set_highlight_color", &Slider::set_highlight_color, D(Slider, set_highlight_color))
        .def("range", &Slider::range, D(Slider, range))
        .def("set_range", &Slider::set_range, D(Slider, set_range))
        .def("highlighted_range", &Slider::highlighted_range, D(Slider, highlighted_range))
        .def("set_highlighted_range", &Slider::set_highlighted_range, D(Slider, set_highlighted_range))
        .def("set_callback", &Slider::set_callback, D(Slider, set_callback))
        .def("callback", &Slider::callback, D(Slider, callback))
        .def("set_final_callback", &Slider::set_final_callback, D(Slider, set_final_callback))
        .def("final_callback", &Slider::final_callback, D(Slider, final_callback));
}
#endif
