#ifdef NANOGUI_PYTHON

#include "python.h"

DECLARE_WIDGET(ColorWheel);
DECLARE_WIDGET(ColorPicker);
DECLARE_WIDGET(Graph);
DECLARE_WIDGET(ImagePanel);

void register_misc(py::module &m) {
    py::class_<ColorWheel, Widget, ref<ColorWheel>, PyColorWheel>(m, "ColorWheel", D(ColorWheel))
        .def(py::init<Widget *>(), "parent"_a, D(ColorWheel, ColorWheel))
        .def(py::init<Widget *, const Color &>(), "parent"_a, "Color"_a)
        .def("color", &ColorWheel::color, D(ColorWheel, color))
        .def("set_color", &ColorWheel::set_color, D(ColorWheel, set_color))
        .def("callback", &ColorWheel::callback, D(ColorWheel, callback))
        .def("set_callback", &ColorWheel::set_callback, D(ColorWheel, set_callback));

    py::class_<ColorPicker, PopupButton, ref<ColorPicker>, PyColorPicker>(m, "ColorPicker", D(ColorPicker))
        .def(py::init<Widget *>(), "parent"_a, D(ColorPicker, ColorPicker))
        .def(py::init<Widget *, const Color &>(), "parent"_a, "Color"_a)
        .def("color", &ColorPicker::color, D(ColorPicker, color))
        .def("set_color", &ColorPicker::set_color, D(ColorPicker, set_color))
        .def("callback", &ColorPicker::callback, D(ColorPicker, callback))
        .def("set_callback", &ColorPicker::set_callback, D(ColorPicker, set_callback))
        .def("final_callback", &ColorPicker::final_callback, D(ColorPicker, final_callback))
        .def("set_final_callback", &ColorPicker::set_final_callback, D(ColorPicker, set_final_callback));

    py::class_<Graph, Widget, ref<Graph>, PyGraph>(m, "Graph", D(Graph))
        .def(py::init<Widget *, const std::string &>(), "parent"_a,
             "caption"_a = std::string("Untitled"), D(Graph, Graph))
        .def("caption", &Graph::caption, D(Graph, caption))
        .def("set_caption", &Graph::set_caption, D(Graph, set_caption))
        .def("header", &Graph::header, D(Graph, header))
        .def("set_header", &Graph::set_header, D(Graph, set_header))
        .def("footer", &Graph::footer, D(Graph, footer))
        .def("set_footer", &Graph::set_footer, D(Graph, set_footer))
        .def("background_color", &Graph::background_color, D(Graph, background_color))
        .def("set_background_color", &Graph::set_background_color, D(Graph, set_background_color))
        .def("fill_color", &Graph::fill_color, D(Graph, fill_color))
        .def("set_fill_color", &Graph::set_fill_color, D(Graph, set_fill_color))
        .def("stroke_color", &Graph::stroke_color, D(Graph, stroke_color))
        .def("set_stroke_color", &Graph::set_stroke_color, D(Graph, set_stroke_color))
        .def("text_color", &Graph::text_color, D(Graph, text_color))
        .def("set_text_color", &Graph::set_text_color, D(Graph, set_text_color))
        .def("values", (std::vector<float> &(Graph::*)(void)) &Graph::values, D(Graph, values))
        .def("set_values", &Graph::set_values, D(Graph, set_values));

    py::class_<ImagePanel, Widget, ref<ImagePanel>, PyImagePanel>(m, "ImagePanel", D(ImagePanel))
        .def(py::init<Widget *>(), "parent"_a, D(ImagePanel, ImagePanel))
        .def("images", &ImagePanel::images, D(ImagePanel, images))
        .def("set_images", &ImagePanel::set_images, D(ImagePanel, set_images))
        .def("callback", &ImagePanel::callback, D(ImagePanel, callback))
        .def("set_callback", &ImagePanel::set_callback, D(ImagePanel, set_callback));
}

#endif
