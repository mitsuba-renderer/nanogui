/*
    nanogui/imageview.cpp -- Widget used to display images.

    The image view widget was contributed by Stefan Ivanov.

    NanoGUI was developed by Wenzel Jakob <wenzel.jakob@epfl.ch>.
    The widget drawing code is based on the NanoVG demo application
    by Mikko Mononen.

    All rights reserved. Use of this source code is governed by a
    BSD-style license that can be found in the LICENSE.txt file.
*/

#include <nanogui/imageview.h>
#include <nanogui/window.h>
#include <nanogui/screen.h>
#include <nanogui/theme.h>
#include <cmath>

NAMESPACE_BEGIN(nanogui)

namespace {
    std::vector<std::string> tokenize(const std::string &string,
                                      const std::string &delim = "\n",
                                      bool include_empty = false) {
        std::string::size_type last_pos = 0, pos = string.find_first_of(delim, last_pos);
        std::vector<std::string> tokens;

        while (last_pos != std::string::npos) {
            std::string substr = string.substr(last_pos, pos - last_pos);
            if (!substr.empty() || include_empty)
                tokens.push_back(std::move(substr));
            last_pos = pos;
            if (last_pos != std::string::npos) {
                last_pos += 1;
                pos = string.find_first_of(delim, last_pos);
            }
        }

        return tokens;
    }

#if defined(NANOGUI_USE_OPENGL)
    constexpr char const *const default_image_view_vertex_shader =
        R"(#version 330
        uniform vec2 scale_factor;
        uniform vec2 position;
        in vec2 vertex;
        out vec2 uv;
        void main() {
            uv = vertex;
            vec2 scaled_vertex = (vertex * scale_factor) + position;
            gl_Position  = vec4(2.0*scaled_vertex.x - 1.0,
                                1.0 - 2.0*scaled_vertex.y,
                                0.0, 1.0);

        })";

    constexpr char const *const default_image_view_fragment_shader =
        R"(#version 330
        uniform sampler2D image;
        out vec4 color;
        in vec2 uv;
        void main() {
            color = texture(image, uv);
        })";

}
#else // GLES2
    constexpr char const *const default_image_view_vertex_shader =
        R"(precision highp float;
        uniform vec2 scale_factor;
        uniform vec2 position;
        attribute vec2 vertex;
        varying vec2 uv;
        void main() {
            uv = vertex;
            vec2 scaled_vertex = (vertex * scale_factor) + position;
            gl_Position  = vec4(2.0*scaled_vertex.x - 1.0,
                                1.0 - 2.0*scaled_vertex.y,
                                0.0, 1.0);

        })";

    constexpr char const *const default_image_view_fragment_shader =
        R"(precision highp float;
        uniform sampler2D image;
        varying vec2 uv;
        void main() {
            gl_FragColor = texture2D(image, uv);
        })";

}
#endif

ImageView::ImageView(Widget* parent, GLuint image_id)
    : Widget(parent), m_image_id(image_id), m_scale(1.0f), m_offset(0),
    m_fixed_scale(false), m_fixed_offset(false), m_pixel_info_callback(nullptr) {
    update_image_parameters();
    m_shader.init("ImageViewShader", default_image_view_vertex_shader,
                 default_image_view_fragment_shader);

    uint32_t indices[3*2] = {
        0, 1, 2,
        2, 3, 1

    };

    float vertices[2*4] = {
        0.f, 0.f,
        1.f, 0.f,
        0.f, 1.f,
        1.f, 1.f
    };

    m_shader.bind();
    m_shader.upload_indices(indices, 3, 2);
    m_shader.upload_attrib("vertex", vertices, 2, 4);
}

ImageView::~ImageView() {
    m_shader.free();
}

void ImageView::bind_image(GLuint image_id) {
    m_image_id = image_id;
    update_image_parameters();
    fit();
}

Vector2f ImageView::image_coordinate_at(const Vector2f& position) const {
    auto image_position = position - m_offset;
    return image_position / m_scale;
}

Vector2f ImageView::clamped_image_coordinate_at(const Vector2f& position) const {
    auto image_coordinate = image_coordinate_at(position);
    return enoki::min(enoki::max(Vector2f(0), image_coordinate), image_size_f());
}

Vector2f ImageView::position_for_coordinate(const Vector2f& image_coordinate) const {
    return m_scale*image_coordinate + m_offset;
}

void ImageView::set_image_coordinate_at(const Vector2f& position, const Vector2f& image_coordinate) {
    // Calculate where the new offset must be in order to satisfy the image position equation.
    // Round the floating point values to balance out the floating point to integer conversions.
    m_offset = position - (image_coordinate * m_scale);

    // Clamp offset so that the image remains near the screen.
    m_offset = max(min(m_offset, size_f()), -scaled_image_size_f());
}

void ImageView::center() {
    m_offset = (size_f() - scaled_image_size_f()) * .5f;
}

void ImageView::fit() {
    // Calculate the appropriate scaling factor.
    m_scale = hmin(size_f() / image_size_f());
    center();
}

void ImageView::set_scale_centered(float scale) {
    auto center_position = size_f() * 0.5f;
    auto p = image_coordinate_at(center_position);
    m_scale = scale;
    set_image_coordinate_at(center_position, p);
}

void ImageView::move_offset(const Vector2f& delta) {
    // Apply the delta to the offset.
    m_offset += delta;

    // Prevent the image from going out of bounds.
    auto scaled_size = scaled_image_size_f();
    if (m_offset.x() + scaled_size.x() < 0)
        m_offset.x() = -scaled_size.x();
    if (m_offset.x() > size_f().x())
        m_offset.x() = size_f().x();
    if (m_offset.y() + scaled_size.y() < 0)
        m_offset.y() = -scaled_size.y();
    if (m_offset.y() > size_f().y())
        m_offset.y() = size_f().y();
}

void ImageView::zoom(int amount, const Vector2f& focus_position) {
    auto focused_coordinate = image_coordinate_at(focus_position);
    float scale_factor = std::pow(m_zoom_sensitivity, amount);
    m_scale = std::max(0.01f, scale_factor * m_scale);
    set_image_coordinate_at(focus_position, focused_coordinate);
}

bool ImageView::mouse_drag_event(const Vector2i& p, const Vector2i& rel, int button, int /*modifiers*/) {
    if ((button & (1 << GLFW_MOUSE_BUTTON_LEFT)) != 0 && !m_fixed_offset) {
        set_image_coordinate_at(p + rel, image_coordinate_at(p));
        return true;
    }
    return false;
}

bool ImageView::grid_visible() const {
    return (m_grid_threshold != -1) && (m_scale > m_grid_threshold);
}

bool ImageView::pixel_info_visible() const {
    return m_pixel_info_callback && (m_pixel_info_threshold != -1) && (m_scale > m_pixel_info_threshold);
}

bool ImageView::helpers_visible() const {
    return grid_visible() || pixel_info_visible();
}

bool ImageView::scroll_event(const Vector2i& p, const Vector2f& rel) {
    if (m_fixed_scale)
        return false;
    float v = rel.y();
    if (std::abs(v) < 1)
        v = std::copysign(1.f, v);
    zoom(v, p - position());
    return true;
}

bool ImageView::keyboard_event(int key, int /*scancode*/, int action, int modifiers) {
    if (action) {
        switch (key) {
        case GLFW_KEY_LEFT:
            if (!m_fixed_offset) {
                if (GLFW_MOD_CONTROL & modifiers)
                    move_offset(Vector2f(30, 0));
                else
                    move_offset(Vector2f(10, 0));
                return true;
            }
            break;
        case GLFW_KEY_RIGHT:
            if (!m_fixed_offset) {
                if (GLFW_MOD_CONTROL & modifiers)
                    move_offset(Vector2f(-30, 0));
                else
                    move_offset(Vector2f(-10, 0));
                return true;
            }
            break;
        case GLFW_KEY_DOWN:
            if (!m_fixed_offset) {
                if (GLFW_MOD_CONTROL & modifiers)
                    move_offset(Vector2f(0, -30));
                else
                    move_offset(Vector2f(0, -10));
                return true;
            }
            break;
        case GLFW_KEY_UP:
            if (!m_fixed_offset) {
                if (GLFW_MOD_CONTROL & modifiers)
                    move_offset(Vector2f(0, 30));
                else
                    move_offset(Vector2f(0, 10));
                return true;
            }
            break;
        }
    }
    return false;
}

bool ImageView::keyboard_character_event(unsigned int codepoint) {
    switch (codepoint) {
    case '-':
        if (!m_fixed_scale) {
            zoom(-1, size_f() * .5f);
            return true;
        }
        break;
    case '+':
        if (!m_fixed_scale) {
            zoom(1, size_f() * .5f);
            return true;
        }
        break;
    case 'c':
        if (!m_fixed_offset) {
            center();
            return true;
        }
        break;
    case 'f':
        if (!m_fixed_offset && !m_fixed_scale) {
            fit();
            return true;
        }
        break;
    case '1': case '2': case '3': case '4': case '5':
    case '6': case '7': case '8': case '9':
        if (!m_fixed_scale) {
            set_scale_centered(1 << (codepoint - '1'));
            return true;
        }
        break;
    default:
        return false;
    }
    return false;
}

Vector2i ImageView::preferred_size(NVGcontext* /*ctx*/) const {
    return m_image_size;
}

void ImageView::perform_layout(NVGcontext* ctx) {
    Widget::perform_layout(ctx);
    center();
}

void ImageView::draw(NVGcontext* ctx) {
    Widget::draw(ctx);
    nvgEndFrame(ctx); // Flush the NanoVG draw stack, not necessary to call nvgBeginFrame afterwards.

    draw_image_border(ctx);

    // Calculate several variables that need to be send to OpenGL in order for the image to be
    // properly displayed inside the widget.
    const Screen* screen = dynamic_cast<const Screen*>(this->window()->parent());
    assert(screen);
    Vector2f screen_size(screen->size());
    Vector2f scale_factor = m_scale * image_size_f() / screen_size;
    Vector2f position_in_screen(absolute_position());
    Vector2f position_after_offset = position_in_screen + m_offset;
    Vector2f image_position = position_after_offset / screen_size;
    glEnable(GL_SCISSOR_TEST);
    float r = screen->pixel_ratio();
    glScissor(position_in_screen.x() * r,
              (screen_size.y() - position_in_screen.y() - size().y()) * r,
              size().x() * r, size().y() * r);
    m_shader.bind();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_image_id);
    m_shader.set_uniform("image", 0);
    m_shader.set_uniform("scale_factor", scale_factor);
    m_shader.set_uniform("position", image_position);
    m_shader.draw_indexed(GL_TRIANGLES, 0, 2);
    glDisable(GL_SCISSOR_TEST);

    if (helpers_visible())
        draw_helpers(ctx);

    draw_widget_border(ctx);
}

void ImageView::update_image_parameters() {
#if defined(NANOGUI_USE_OPENGL)
    // Query the width of the OpenGL texture.
    glBindTexture(GL_TEXTURE_2D, m_image_id);
    GLint w, h;
    glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &w);
    glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &h);
    m_image_size = Vector2i(w, h);
#else
    std::cout << "Warning: ImageVIew::update_image_parameters(): not supported on GLES2!" << std::endl;;
#endif
}

void ImageView::draw_widget_border(NVGcontext* ctx) const {
    nvgBeginPath(ctx);
    nvgStrokeWidth(ctx, 1);
    nvgRoundedRect(ctx, m_pos.x() + 0.5f, m_pos.y() + 0.5f, m_size.x() - 1,
                   m_size.y() - 1, 0);
    nvgStrokeColor(ctx, m_theme->m_window_popup);
    nvgStroke(ctx);

    nvgBeginPath(ctx);
    nvgRoundedRect(ctx, m_pos.x() + 0.5f, m_pos.y() + 0.5f, m_size.x() - 1,
                   m_size.y() - 1, m_theme->m_button_corner_radius);
    nvgStrokeColor(ctx, m_theme->m_border_dark);
    nvgStroke(ctx);
}

void ImageView::draw_image_border(NVGcontext* ctx) const {
    nvgSave(ctx);
    nvgBeginPath(ctx);
    nvgScissor(ctx, m_pos.x(), m_pos.y(), m_size.x(), m_size.y());
    nvgStrokeWidth(ctx, 1.0f);
    Vector2i border_position = m_pos + Vector2i(m_offset);
    Vector2i border_size(scaled_image_size_f());
    nvgRect(ctx, border_position.x() - 0.5f, border_position.y() - 0.5f,
            border_size.x() + 1, border_size.y() + 1);
    nvgStrokeColor(ctx, Color(1.0f, 1.0f, 1.0f, 1.0f));
    nvgStroke(ctx);
    nvgResetScissor(ctx);
    nvgRestore(ctx);
}

void ImageView::draw_helpers(NVGcontext* ctx) const {
    // We need to apply m_pos after the transformation to account for the position of the widget
    // relative to the parent.
    Vector2f upper_left_corner = position_for_coordinate(Vector2f(0)) + position_f();
    Vector2f lower_right_corner = position_for_coordinate(image_size_f()) + position_f();
    if (grid_visible())
        draw_pixel_grid(ctx, upper_left_corner, lower_right_corner, m_scale);
    if (pixel_info_visible())
        draw_pixel_info(ctx, m_scale);
}

void ImageView::draw_pixel_grid(NVGcontext* ctx, const Vector2f& upper_left_corner,
                              const Vector2f& lower_right_corner, float stride) {
    nvgBeginPath(ctx);

    // Draw the vertical grid lines
    float current_x = upper_left_corner.x();
    while (current_x <= lower_right_corner.x()) {
        nvgMoveTo(ctx, std::round(current_x), std::round(upper_left_corner.y()));
        nvgLineTo(ctx, std::round(current_x), std::round(lower_right_corner.y()));
        current_x += stride;
    }

    // Draw the horizontal grid lines
    float current_y = upper_left_corner.y();
    while (current_y <= lower_right_corner.y()) {
        nvgMoveTo(ctx, std::round(upper_left_corner.x()), std::round(current_y));
        nvgLineTo(ctx, std::round(lower_right_corner.x()), std::round(current_y));
        current_y += stride;
    }

    nvgStrokeWidth(ctx, 1.0f);
    nvgStrokeColor(ctx, Color(1.0f, 1.0f, 1.0f, 0.2f));
    nvgStroke(ctx);
}

void ImageView::draw_pixel_info(NVGcontext* ctx, float stride) const {
    // Extract the image coordinates at the two corners of the widget.
    Vector2i top_left(floor(clamped_image_coordinate_at(Vector2f(0))));
    Vector2i bottom_right(ceil(clamped_image_coordinate_at(size_f())));

    // Extract the positions for where to draw the text.
    Vector2f current_cell_position =
        (position_f() + position_for_coordinate(top_left));

    float x_initial_position = current_cell_position.x();
    int x_initial_index = top_left.x();

    // Properly scale the pixel information for the given stride.
    auto font_size = stride * m_font_scale_factor;
    static constexpr float max_font_size = 30.0f;
    font_size = font_size > max_font_size ? max_font_size : font_size;
    nvgBeginPath(ctx);
    nvgFontSize(ctx, font_size);
    nvgTextAlign(ctx, NVG_ALIGN_CENTER | NVG_ALIGN_TOP);
    nvgFontFace(ctx, "sans");
    while (top_left.y() != bottom_right.y()) {
        while (top_left.x() != bottom_right.x()) {
            write_pixel_info(ctx, current_cell_position, top_left, stride, font_size);
            current_cell_position.x() += stride;
            ++top_left.x();
        }
        current_cell_position.x() = x_initial_position;
        current_cell_position.y() += stride;
        ++top_left.y();
        top_left.x() = x_initial_index;
    }
}

void ImageView::write_pixel_info(NVGcontext* ctx, const Vector2f& cell_position,
                                 const Vector2i& pixel, float stride, float font_size) const {
    auto pixel_data = m_pixel_info_callback(pixel);
    auto pixel_data_rows = tokenize(pixel_data.first);

    // If no data is provided for this pixel then simply return.
    if (pixel_data_rows.empty())
        return;

    nvgFillColor(ctx, pixel_data.second);
    float y_offset = (stride - font_size * pixel_data_rows.size()) * .5f;
    for (size_t i = 0; i != pixel_data_rows.size(); ++i) {
        nvgText(ctx, cell_position.x() + stride / 2, cell_position.y() + y_offset,
                pixel_data_rows[i].data(), nullptr);
        y_offset += font_size;
    }
}

NAMESPACE_END(nanogui)
