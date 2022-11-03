#ifdef NANOGUI_PYTHON

#include "python.h"
#include <nanobind/stl/array.h>

class PyCanvas : public Canvas {
public:
    using Canvas::Canvas;
    NANOGUI_WIDGET_OVERLOADS(Canvas);

    void draw_contents() override {
        NB_OVERRIDE(void, Canvas, draw_contents);
    }
};

class PyImageView : public ImageView {
public:
    using ImageView::ImageView;
    NANOGUI_WIDGET_OVERLOADS(ImageView);

    void draw_contents() override {
        NB_OVERRIDE(void, ImageView, draw_contents);
    }
};

void register_canvas(nb::module_ &m) {
    nb::class_<Canvas, Widget, PyCanvas>(m, "Canvas", D(Canvas))
        .def(nb::init<Widget *, uint8_t, bool, bool, bool>(),
             "parent"_a, "samples"_a = 4, "has_depth_buffer"_a = true,
             "has_stencil_buffer"_a = false,
             "clear"_a = true, D(Canvas, Canvas))
        .def(nb_method(Canvas, render_pass), D(Canvas, render_pass))
        .def(nb_method(Canvas, draw_border), D(Canvas, draw_border))
        .def(nb_method(Canvas, set_draw_border), D(Canvas, set_draw_border))
        .def(nb_method(Canvas, border_color), D(Canvas, border_color))
        .def(nb_method(Canvas, set_border_color), D(Canvas, set_border_color))
        .def(nb_method(Canvas, background_color), D(Canvas, background_color))
        .def(nb_method(Canvas, set_background_color), D(Canvas, set_background_color))
        .def(nb_method(Canvas, draw_contents), D(Canvas, draw_contents));

    nb::class_<ImageView, Canvas, PyImageView>(m, "ImageView", D(ImageView))
        .def(nb::init<Widget *>(), D(ImageView, ImageView))
        .def("image", nb::overload_cast<>(&ImageView::image, nb::const_), D(ImageView, image))
        .def(nb_method(ImageView, set_image), D(ImageView, set_image))
        .def(nb_method(ImageView, reset), D(ImageView, reset))
        .def(nb_method(ImageView, center), D(ImageView, center))
        .def(nb_method(ImageView, offset), D(ImageView, offset))
        .def(nb_method(ImageView, set_offset), D(ImageView, set_offset))
        .def(nb_method(ImageView, scale), D(ImageView, scale))
        .def(nb_method(ImageView, set_scale), D(ImageView, set_scale))
        .def(nb_method(ImageView, pos_to_pixel), D(ImageView, pos_to_pixel))
        .def(nb_method(ImageView, pixel_to_pos), D(ImageView, pixel_to_pos))
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
