#ifdef NANOGUI_PYTHON

#include "python.h"

DECLARE_WIDGET(Label);
DECLARE_WIDGET(Popup);
DECLARE_WIDGET(MessageDialog);
DECLARE_WIDGET(VScrollPanel);
DECLARE_WIDGET(ComboBox);
DECLARE_WIDGET(ProgressBar);
DECLARE_WIDGET(Slider);

void register_basics(nb::module_ &m) {
    nb::class_<Label, Widget, PyLabel>(m, "Label", D(Label))
        .def(nb::init<Widget *, const std::string &, const std::string &, int>(),
             "parent"_a, "caption"_a, "font"_a = std::string("sans"),
             "font_size"_a = -1,
             D(Label, Label))
        .def(nb_method(Label, caption), D(Label, caption))
        .def(nb_method(Label, set_caption), D(Label, set_caption))
        .def(nb_method(Label, font), D(Label, font))
        .def(nb_method(Label, set_font), D(Label, set_font))
        .def(nb_method(Label, color), D(Label, color))
        .def(nb_method(Label, set_color), D(Label, set_color));


    nb::class_<Popup, Window, PyPopup> popup(m, "Popup", D(Popup));
    popup
        .def(nb::init<Widget *, Window *>(), "parent"_a, "parent_window"_a, D(Popup, Popup))
        .def(nb_method(Popup, anchor_pos), D(Popup, anchor_pos))
        .def(nb_method(Popup, set_anchor_pos), D(Popup, set_anchor_pos))
        .def(nb_method(Popup, anchor_offset), D(Popup, anchor_offset))
        .def(nb_method(Popup, set_anchor_offset), D(Popup, set_anchor_offset))
        .def(nb_method(Popup, anchor_size), D(Popup, anchor_size))
        .def(nb_method(Popup, set_anchor_size), D(Popup, set_anchor_size))
        .def("parent_window", (Window*(Popup::*)(void)) &Popup::parent_window, D(Popup, parent_window))
        .def(nb_method(Popup, side), D(Popup, side))
        .def(nb_method(Popup, set_side), D(Popup, set_side));

    nb::enum_<Popup::Side>(popup, "Side", D(Popup, Side))
        .value("Left", Popup::Side::Left)
        .value("Right", Popup::Side::Right)
        .export_values();

    nb::class_<MessageDialog, Window, PyMessageDialog> mdlg(m, "MessageDialog", D(MessageDialog));
    mdlg
        .def(nb::init<Widget *, MessageDialog::Type, const std::string&,
                      const std::string&, const std::string&, const std::string&, bool>(),
            "parent"_a, "type"_a, "title"_a = std::string("Untitled"),
            "message"_a = std::string("Message"), "button_text"_a = std::string("OK"),
            "alt_button_text"_a = std::string("Cancel"), "alt_button"_a = false,
            D(MessageDialog, MessageDialog))
        .def("message_label", (Label * (MessageDialog::*)()) &MessageDialog::message_label, D(MessageDialog, message_label))
        .def(nb_method(MessageDialog, callback), D(MessageDialog, callback))
        .def(nb_method(MessageDialog, set_callback), D(MessageDialog, set_callback));

    nb::enum_<MessageDialog::Type>(mdlg, "Type", D(MessageDialog, Type))
        .value("Information", MessageDialog::Type::Information)
        .value("Question", MessageDialog::Type::Question)
        .value("Warning", MessageDialog::Type::Warning);

    nb::class_<VScrollPanel, Widget, PyVScrollPanel>(m, "VScrollPanel", D(VScrollPanel))
        .def(nb::init<Widget *>(), "parent"_a, D(VScrollPanel, VScrollPanel))
        .def(nb_method(VScrollPanel, scroll), D(VScrollPanel, scroll))
        .def(nb_method(VScrollPanel, set_scroll), D(VScrollPanel, set_scroll));

    nb::class_<ComboBox, Widget, PyComboBox>(m, "ComboBox", D(ComboBox))
        .def(nb::init<Widget *>(), "parent"_a, D(ComboBox, ComboBox))
        .def(nb::init<Widget *, const std::vector<std::string> &>(),
             "parent"_a, "items"_a/*, D(ComboBox, ComboBox, 2)*/)
        .def(nb::init<Widget *, const std::vector<std::string> &,
                      const std::vector<std::string> &>(),
             "parent"_a, "items"_a, "items_short"_a/* ,D(ComboBox, ComboBox, 3)*/)
        .def(nb_method(ComboBox, callback), D(ComboBox, callback))
        .def(nb_method(ComboBox, set_callback), D(ComboBox, set_callback))
        .def(nb_method(ComboBox, selected_index), D(ComboBox, selected_index))
        .def(nb_method(ComboBox, set_selected_index), D(ComboBox, set_selected_index))
        .def("set_items", (void(ComboBox::*)(const std::vector<std::string>&)) &ComboBox::set_items, D(ComboBox, set_items))
        .def("set_items", (void(ComboBox::*)(const std::vector<std::string>&,
                          const std::vector<std::string>&)) &ComboBox::set_items/*, D(ComboBox, set_items, 2)*/)
        .def(nb_method(ComboBox, items), D(ComboBox, items))
        .def(nb_method(ComboBox, items_short), D(ComboBox, items_short));

    nb::class_<ProgressBar, Widget, PyProgressBar>(m, "ProgressBar", D(ProgressBar))
        .def(nb::init<Widget *>(), "parent"_a, D(ProgressBar, ProgressBar))
        .def(nb_method(ProgressBar, value), D(ProgressBar, value))
        .def(nb_method(ProgressBar, set_value), D(ProgressBar, set_value));

    nb::class_<Slider, Widget, PySlider>(m, "Slider", D(Slider))
        .def(nb::init<Widget *>(), "parent"_a, D(Slider, Slider))
        .def(nb_method(Slider, value), D(Slider, value))
        .def(nb_method(Slider, set_value), D(Slider, set_value))
        .def(nb_method(Slider, highlight_color), D(Slider, highlight_color))
        .def(nb_method(Slider, set_highlight_color), D(Slider, set_highlight_color))
        .def(nb_method(Slider, range), D(Slider, range))
        .def(nb_method(Slider, set_range), D(Slider, set_range))
        .def(nb_method(Slider, highlighted_range), D(Slider, highlighted_range))
        .def(nb_method(Slider, set_highlighted_range), D(Slider, set_highlighted_range))
        .def(nb_method(Slider, set_callback), D(Slider, set_callback))
        .def(nb_method(Slider, callback), D(Slider, callback))
        .def(nb_method(Slider, set_final_callback), D(Slider, set_final_callback))
        .def(nb_method(Slider, final_callback), D(Slider, final_callback));
}
#endif
