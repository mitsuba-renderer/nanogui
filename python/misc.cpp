#ifdef NANOGUI_PYTHON

#include "python.h"

DECLARE_WIDGET(ColorWheel);
DECLARE_WIDGET(ColorPicker);
DECLARE_WIDGET(Graph);
DECLARE_WIDGET(ImageView);
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

    py::class_<ImageView, Widget, ref<ImageView>, PyImageView>(m, "ImageView", D(ImageView))
        .def(py::init<Widget *, GLuint>(), D(ImageView, ImageView))
        .def("bind_image", &ImageView::bind_image, D(ImageView, bind_image))
        .def("image_shader", &ImageView::image_shader, D(ImageView, image_shader))
        .def("scaled_image_size", &ImageView::scaled_image_size, D(ImageView, scaled_image_size))
        .def("offset", &ImageView::offset, D(ImageView, offset))
        .def("set_offset", &ImageView::set_offset, D(ImageView, set_offset))
        .def("scale", &ImageView::scale, D(ImageView, scale))
        .def("fixed_offset", &ImageView::fixed_offset, D(ImageView, fixed_offset))
        .def("set_fixed_offset", &ImageView::set_fixed_offset, D(ImageView, set_fixed_offset))
        .def("fixed_scale", &ImageView::fixed_scale, D(ImageView, fixed_scale))
        .def("set_fixed_scale", &ImageView::set_fixed_scale, D(ImageView, set_fixed_scale))
        .def("zoom_sensitivity", &ImageView::zoom_sensitivity, D(ImageView, zoom_sensitivity))
        .def("set_zoom_sensitivity", &ImageView::set_zoom_sensitivity, D(ImageView, set_zoom_sensitivity))
        .def("grid_threshold", &ImageView::grid_threshold, D(ImageView, grid_threshold))
        .def("set_grid_threshold", &ImageView::set_grid_threshold, D(ImageView, set_grid_threshold))
        .def("pixel_info_threshold", &ImageView::pixel_info_threshold, D(ImageView, pixel_info_threshold))
        .def("set_pixel_info_threshold", &ImageView::set_pixel_info_threshold, D(ImageView, set_pixel_info_threshold))
        .def("set_pixel_info_callback", &ImageView::set_pixel_info_callback, D(ImageView, set_pixel_info_callback))
        .def("pixel_info_callback", &ImageView::pixel_info_callback, D(ImageView, pixel_info_callback))
        .def("set_font_scale_factor", &ImageView::set_font_scale_factor, D(ImageView, set_font_scale_factor))
        .def("font_scale_factor", &ImageView::font_scale_factor, D(ImageView, font_scale_factor))
        .def("image_coordinate_at", &ImageView::image_coordinate_at, D(ImageView, image_coordinate_at))
        .def("clamped_image_coordinate_at", &ImageView::clamped_image_coordinate_at, D(ImageView, clamped_image_coordinate_at))
        .def("position_for_coordinate", &ImageView::position_for_coordinate, D(ImageView, position_for_coordinate))
        .def("set_image_coordinate_at", &ImageView::set_image_coordinate_at, D(ImageView, set_image_coordinate_at))
        .def("center",&ImageView::center, D(ImageView, center))
        .def("fit", &ImageView::fit, D(ImageView, fit))
        .def("set_scale_centered", &ImageView::set_scale_centered, D(ImageView, set_scale_centered))
        .def("move_offset", &ImageView::move_offset, D(ImageView, move_offset))
        .def("zoom", &ImageView::zoom, D(ImageView, zoom))
        .def("grid_visible", &ImageView::grid_visible, D(ImageView, grid_visible))
        .def("pixel_info_visible", &ImageView::pixel_info_visible, D(ImageView, pixel_info_visible))
        .def("helpers_visible", &ImageView::helpers_visible, D(ImageView, helpers_visible));

    py::class_<ImagePanel, Widget, ref<ImagePanel>, PyImagePanel>(m, "ImagePanel", D(ImagePanel))
        .def(py::init<Widget *>(), "parent"_a, D(ImagePanel, ImagePanel))
        .def("images", &ImagePanel::images, D(ImagePanel, images))
        .def("set_images", &ImagePanel::set_images, D(ImagePanel, set_images))
        .def("callback", &ImagePanel::callback, D(ImagePanel, callback))
        .def("set_callback", &ImagePanel::set_callback, D(ImagePanel, set_callback));
}

#endif
