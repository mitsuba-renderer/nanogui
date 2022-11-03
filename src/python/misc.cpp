#ifdef NANOGUI_PYTHON

#include "python.h"
#include <nanobind/stl/pair.h>

DECLARE_WIDGET(ColorWheel);
DECLARE_WIDGET(ColorPicker);
DECLARE_WIDGET(Graph);
DECLARE_WIDGET(ImagePanel);

void register_misc(nb::module_ &m) {
    nb::class_<ColorWheel, Widget, PyColorWheel>(m, "ColorWheel", D(ColorWheel))
        .def(nb::init<Widget *>(), "parent"_a, D(ColorWheel, ColorWheel))
        .def(nb::init<Widget *, const Color &>(), "parent"_a, "Color"_a)
        .def(nb_method(ColorWheel, color), D(ColorWheel, color))
        .def(nb_method(ColorWheel, set_color), D(ColorWheel, set_color))
        .def(nb_method(ColorWheel, callback), D(ColorWheel, callback))
        .def(nb_method(ColorWheel, set_callback), D(ColorWheel, set_callback));

    nb::class_<ColorPicker, PopupButton, PyColorPicker>(m, "ColorPicker", D(ColorPicker))
        .def(nb::init<Widget *>(), "parent"_a, D(ColorPicker, ColorPicker))
        .def(nb::init<Widget *, const Color &>(), "parent"_a, "Color"_a)
        .def(nb_method(ColorPicker, color), D(ColorPicker, color))
        .def(nb_method(ColorPicker, set_color), D(ColorPicker, set_color))
        .def(nb_method(ColorPicker, callback), D(ColorPicker, callback))
        .def(nb_method(ColorPicker, set_callback), D(ColorPicker, set_callback))
        .def(nb_method(ColorPicker, final_callback), D(ColorPicker, final_callback))
        .def(nb_method(ColorPicker, set_final_callback), D(ColorPicker, set_final_callback));

    nb::class_<Graph, Widget, PyGraph>(m, "Graph", D(Graph))
        .def(nb::init<Widget *, const std::string &>(), "parent"_a,
             "caption"_a = std::string("Untitled"), D(Graph, Graph))
        .def(nb_method(Graph, caption), D(Graph, caption))
        .def(nb_method(Graph, set_caption), D(Graph, set_caption))
        .def(nb_method(Graph, header), D(Graph, header))
        .def(nb_method(Graph, set_header), D(Graph, set_header))
        .def(nb_method(Graph, footer), D(Graph, footer))
        .def(nb_method(Graph, set_footer), D(Graph, set_footer))
        .def(nb_method(Graph, background_color), D(Graph, background_color))
        .def(nb_method(Graph, set_background_color), D(Graph, set_background_color))
        .def(nb_method(Graph, fill_color), D(Graph, fill_color))
        .def(nb_method(Graph, set_fill_color), D(Graph, set_fill_color))
        .def(nb_method(Graph, stroke_color), D(Graph, stroke_color))
        .def(nb_method(Graph, set_stroke_color), D(Graph, set_stroke_color))
        .def(nb_method(Graph, text_color), D(Graph, text_color))
        .def(nb_method(Graph, set_text_color), D(Graph, set_text_color))
        .def("values", (std::vector<float> &(Graph::*)(void)) &Graph::values, D(Graph, values))
        .def(nb_method(Graph, set_values), D(Graph, set_values));

    nb::class_<ImagePanel, Widget, PyImagePanel>(m, "ImagePanel", D(ImagePanel))
        .def(nb::init<Widget *>(), "parent"_a, D(ImagePanel, ImagePanel))
        .def(nb_method(ImagePanel, images), D(ImagePanel, images))
        .def(nb_method(ImagePanel, set_images), D(ImagePanel, set_images))
        .def(nb_method(ImagePanel, callback), D(ImagePanel, callback))
        .def(nb_method(ImagePanel, set_callback), D(ImagePanel, set_callback));
}

#endif
