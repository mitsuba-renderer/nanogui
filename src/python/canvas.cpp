#ifdef NANOGUI_PYTHON

#include "python.h"

class PyCanvas : public Canvas {
public:
    using Canvas::Canvas;
    NANOGUI_WIDGET_OVERLOADS(Canvas);

    void draw_contents() override {
        PYBIND11_OVERLOAD(void, Canvas, draw_contents);
    }
};

class PyImageView : public ImageView {
public:
    using ImageView::ImageView;
    NANOGUI_WIDGET_OVERLOADS(ImageView);

    void draw_contents() override {
        PYBIND11_OVERLOAD(void, ImageView, draw_contents);
    }
};

void register_canvas(py::module &m) {
    py::class_<Canvas, Widget, ref<Canvas>, PyCanvas>(m, "Canvas", D(Canvas))
        .def(py::init<Widget *, uint8_t, bool, bool, bool>(),
             "parent"_a, "samples"_a = 4, "has_depth_buffer"_a = true,
             "has_stencil_buffer"_a = false,
             "clear"_a = true, D(Canvas, Canvas))
        .def("render_pass", &Canvas::render_pass, D(Canvas, render_pass))
        .def("draw_border", &Canvas::draw_border, D(Canvas, draw_border))
        .def("set_draw_border", &Canvas::set_draw_border, D(Canvas, set_draw_border))
        .def("border_color", &Canvas::border_color, D(Canvas, border_color))
        .def("set_border_color", &Canvas::set_border_color, D(Canvas, set_border_color))
        .def("background_color", &Canvas::background_color, D(Canvas, background_color))
        .def("set_background_color", &Canvas::set_background_color, D(Canvas, set_background_color))
        .def("draw_contents", &Canvas::draw_contents, D(Canvas, draw_contents));

    py::class_<ImageView, Canvas, ref<ImageView>, PyImageView>(m, "ImageView", D(ImageView))
        .def(py::init<Widget *>(), D(ImageView, ImageView))
        .def("image", py::overload_cast<>(&ImageView::image, py::const_), D(ImageView, image))
        .def("set_image", &ImageView::set_image, D(ImageView, set_image))
        .def("reset", &ImageView::reset, D(ImageView, reset))
        .def("center", &ImageView::center, D(ImageView, center))
        .def("offset", &ImageView::offset, D(ImageView, offset))
        .def("set_offset", &ImageView::set_offset, D(ImageView, set_offset))
        .def("scale", &ImageView::scale, D(ImageView, scale))
        .def("set_scale", &ImageView::set_scale, D(ImageView, set_scale))
        .def("pos_to_pixel", &ImageView::pos_to_pixel, D(ImageView, pos_to_pixel))
        .def("pixel_to_pos", &ImageView::pixel_to_pos, D(ImageView, pixel_to_pos))
        .def("set_pixel_callback",
             [](ImageView &img,
                const std::function<std::array<std::string, 4>(const Vector2i &)> &func) {
                img.set_pixel_callback([func](const Vector2i &p, char **out, size_t size) {
                    auto str = func(p);
                    for (int i = 0; i < 4; ++i)
                        strncpy(out[i], str[i].c_str(), size);
                });
             },
             D(ImageView, set_pixel_callback));
}

#endif
