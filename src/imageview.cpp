/*
    nanogui/imageview.cpp -- Widget used to display images.

    NanoGUI was developed by Wenzel Jakob <wenzel.jakob@epfl.ch>.
    The widget drawing code is based on the NanoVG demo application
    by Mikko Mononen.

    All rights reserved. Use of this source code is governed by a
    BSD-style license that can be found in the LICENSE.txt file.
*/

#include <nanogui/imageview.h>
#include <nanogui/renderpass.h>
#include <nanogui/shader.h>
#include <nanogui/texture.h>
#include <nanogui/screen.h>
#include <nanogui/opengl.h>
#include <nanogui_resources.h>

NAMESPACE_BEGIN(nanogui)

ImageView::ImageView(Widget *parent) : Canvas(parent, 1, false, false, false) {
    render_pass()->set_clear_color(0, Color(0.3f, 0.3f, 0.32f, 1.f));

    m_image_shader = new Shader(
        render_pass(),
        /* An identifying name */
        "a_simple_shader",
        NANOGUI_SHADER(imageview_vertex),
        NANOGUI_SHADER(imageview_fragment),
        Shader::BlendMode::AlphaBlend
    );

    const float positions[] = {
        0.f, 0.f, 1.f, 0.f, 0.f, 1.f,
        1.f, 0.f, 1.f, 1.f, 0.f, 1.f
    };

    m_image_shader->set_buffer("position", VariableType::Float32, { 6, 2 },
                               positions);
    m_render_pass->set_cull_mode(RenderPass::CullMode::Disabled);

    m_image_border_color = m_theme->m_border_dark;
    m_draw_image_border = true;
    m_image_background_color = Color(0.f, 0.f, 0.f, 0.f);
}

void ImageView::set_image(Texture *image) {
    if (image->mag_interpolation_mode() != Texture::InterpolationMode::Nearest)
        throw std::runtime_error(
            "ImageView::set_image(): interpolation mode must be set to 'Nearest'!");
    m_image_shader->set_texture("image", image);
    m_image = image;
}

float ImageView::scale() const {
    return std::pow(2.f, m_scale / 5.f);
}

void ImageView::set_scale(float scale) {
    m_scale = std::log2(scale) * 5.f;
}

void ImageView::center() {
    if (!m_image)
        return;
    m_offset = Vector2i(.5f * (Vector2f(m_size) * screen()->pixel_ratio() - Vector2f(m_image->size()) * scale()));
}

void ImageView::reset() {
    m_scale = 0.f;
    center();
}

Vector2f ImageView::pos_to_pixel(const Vector2f &p) const {
    Vector2f p2 = p;
    if (m_draw_border)
        p2 -= 1.f;
    return (p2 * screen()->pixel_ratio() - m_offset) / scale();
}

Vector2f ImageView::pixel_to_pos(const Vector2f &p) const {
    Vector2i pos = (p * scale() + m_offset) / screen()->pixel_ratio();
    if (m_draw_border)
        pos += 1.f;
    return pos;
}

bool ImageView::keyboard_event(int key, int /* scancode */, int action, int /* modifiers */) {
    if (!m_enabled || !m_image)
        return false;

    if (action == GLFW_PRESS) {
        if (key == GLFW_KEY_R) {
            reset();
            return true;
        }
    }
    return false;
}

bool ImageView::mouse_drag_event(const Vector2i & /* p */, const Vector2i &rel,
                                 int /* button */, int /* modifiers */) {
    if (!m_enabled || !m_image)
        return false;

    m_offset += rel * screen()->pixel_ratio();

    return true;
}

bool ImageView::scroll_event(const Vector2i &p, const Vector2f &rel) {
    if (!m_enabled || !m_image)
        return false;

    Vector2f p1 = pos_to_pixel(p - m_pos);
    m_scale += rel.y();

    // Restrict scaling to a reasonable range
    m_scale = std::max(
        m_scale, std::min(0.f, std::log2(40.f / std::max(m_image->size().x(),
                                                         m_image->size().y())) * 5.f));
    m_scale = std::min(m_scale, 45.f);

    Vector2f p2 = pos_to_pixel(p - m_pos);
    m_offset += (p2 - p1) * scale();
    return true;
}

void ImageView::draw(NVGcontext *ctx) {
    if (!m_enabled || !m_image)
        return;

    Canvas::draw(ctx);

    Vector2i top_left = Vector2i(pixel_to_pos(Vector2f(0.f, 0.f))),
             size     = Vector2i(pixel_to_pos(Vector2f(m_image->size())) - Vector2f(top_left));

    if (m_draw_image_border) {
        nvgBeginPath(ctx);
        nvgStrokeWidth(ctx, 1.f);
        nvgStrokeColor(ctx, m_image_border_color);
        nvgRect(ctx, m_pos.x() - .5f + top_left.x(),
                     m_pos.y() - .5f + top_left.y(),
                     size.x() + 1.f, size.y() + 1.f);
        nvgStroke(ctx);
    }

    nvgSave(ctx);
    nvgIntersectScissor(ctx, m_pos.x(), m_pos.y(), m_size.x(), m_size.y());

    if (scale() > 100 && m_pixel_callback) {
        float font_size = scale() / 10.f;
        float alpha = std::min(1.f, (scale() - 100) / 100.f);
        nvgFontSize(ctx, font_size);
        nvgFontFace(ctx, "sans-bold");
        nvgTextAlign(ctx, NVG_ALIGN_CENTER | NVG_ALIGN_MIDDLE);

        Vector2i start = max(Vector2i(0), Vector2i(pos_to_pixel(Vector2f(0.f, 0.f))) - 1),
                 end   = min(Vector2i(pos_to_pixel(Vector2f(m_size))) + 1, m_image->size() - 1);

        char text_buf[80],
            *text[4] = { text_buf, text_buf + 20, text_buf + 40, text_buf + 60 };

        for (int y = start.y(); y <= end.y(); ++y) {
            for (int x = start.x(); x <= end.x(); ++x) {
                Vector2i pos = Vector2i(pixel_to_pos(Vector2f(x + .5f, y + .5f)));

                m_pixel_callback(Vector2i(x, y), text, 20);

                for (int ch = 0; ch < 4; ++ch) {
                    Color col(0.f, 0.f, 0.f, alpha);
                    nvgFillColor(ctx, col);
                    nvgFontBlur(ctx, 2);
                    float xpos = m_pos.x() + pos.x(),
                          ypos = m_pos.y() + pos.y() + (ch - 1.5f) * font_size;
                    nvgText(ctx, xpos, ypos, text[ch], nullptr);
                    col = Color(0.3f, 0.3f, 0.3f, alpha);
                    if (ch == 3)
                        col[0] = col[1] = col[2] = 1.f;
                    else
                        col[ch] = 1.f;
                    nvgFillColor(ctx, col);
                    nvgFontBlur(ctx, 0);
                    nvgText(ctx, xpos, ypos, text[ch], nullptr);
                }
            }
        }
    }

    nvgRestore(ctx);
}

void ImageView::draw_contents() {
    if (!m_image)
        return;

    /* Ensure that 'offset' is a multiple of the pixel ratio */
    float pixel_ratio = screen()->pixel_ratio();
    m_offset = (Vector2f(Vector2i(m_offset / pixel_ratio)) * pixel_ratio);

    Vector2f bound1 = Vector2f(m_size) * pixel_ratio,
             bound2 = -Vector2f(m_image->size()) * scale();

    if ((m_offset.x() >= bound1.x()) != (m_offset.x() < bound2.x()))
        m_offset.x() = std::max(std::min(m_offset.x(), bound1.x()), bound2.x());
    if ((m_offset.y() >= bound1.y()) != (m_offset.y() < bound2.y()))
        m_offset.y() = std::max(std::min(m_offset.y(), bound1.y()), bound2.y());

    Vector2i viewport_size = render_pass()->viewport().second;

    float scale = std::pow(2.f, m_scale / 5.f);

    Matrix4f matrix_background =
        Matrix4f::scale(Vector3f(m_image->size().x() * scale / 20.f,
                                 m_image->size().y() * scale / 20.f, 1.f));

    Matrix4f matrix_image =
        Matrix4f::ortho(0.f, viewport_size.x(), viewport_size.y(), 0.f, -1.f, 1.f) *
        Matrix4f::translate(Vector3f(m_offset.x(), (int) m_offset.y(), 0.f)) *
        Matrix4f::scale(Vector3f(m_image->size().x() * scale,
                                 m_image->size().y() * scale, 1.f));

    m_image_shader->set_uniform("matrix_image",      Matrix4f(matrix_image));
    m_image_shader->set_uniform("matrix_background", Matrix4f(matrix_background));
    m_image_shader->set_uniform("background_color",  m_image_background_color);

    m_image_shader->begin();
    m_image_shader->draw_array(Shader::PrimitiveType::Triangle, 0, 6, false);
    m_image_shader->end();
}

NAMESPACE_END(nanogui)
