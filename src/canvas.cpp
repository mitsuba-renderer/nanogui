/*
    nanogui/canvas.cpp -- Canvas widget for rendering full-fledged
    OpenGL content within its designated area. Very useful for
    displaying and manipulating 3D objects or scenes. Subclass it and
    overload `draw_contents` for rendering.

    NanoGUI was developed by Wenzel Jakob <wenzel.jakob@epfl.ch>.
    The widget drawing code is based on the NanoVG demo application
    by Mikko Mononen.

    All rights reserved. Use of this source code is governed by a
    BSD-style license that can be found in the LICENSE.txt file.
*/

#include <nanogui/screen.h>
#include <nanogui/canvas.h>
#include <nanogui/texture.h>
#include <nanogui/renderpass.h>
#include <nanogui/theme.h>
#include <nanogui/opengl.h>
#include "opengl_check.h"

NAMESPACE_BEGIN(nanogui)

Canvas::Canvas(Widget *parent, uint8_t samples,
               bool has_depth_buffer, bool has_stencil_buffer,
               bool clear)
    : Widget(parent), m_draw_border(true) {
    m_size = Vector2i(250, 250);
    m_border_color = m_theme->m_border_light;

#if defined(NANOGUI_USE_GLES)
    samples = 1;
#endif

    Screen *scr = screen();
    if (scr == nullptr)
        throw std::runtime_error("Canvas::Canvas(): could not find parent screen!");

    m_render_to_texture = samples != 1
        || (has_depth_buffer && !scr->has_depth_buffer())
        || (has_stencil_buffer && !scr->has_stencil_buffer());

    Object *color_texture = nullptr,
           *depth_texture = nullptr;

    if (has_stencil_buffer && !has_depth_buffer)
        throw std::runtime_error("Canvas::Canvas(): has_stencil implies has_depth!");

    if (!m_render_to_texture) {
        color_texture = scr;
        if (has_depth_buffer) {
#if defined(NANOGUI_USE_METAL)
            depth_texture = scr->depth_stencil_texture();
#else
            depth_texture = scr;
#endif
        }
    } else {
        color_texture = new Texture(
            scr->pixel_format(),
            scr->component_format(),
            m_size,
            Texture::InterpolationMode::Bilinear,
            Texture::InterpolationMode::Bilinear,
            Texture::WrapMode::ClampToEdge,
            samples,
            Texture::TextureFlags::RenderTarget
        );

#if defined(NANOGUI_USE_METAL)
        Texture *color_texture_resolved = nullptr;

        if (samples > 1) {
            color_texture_resolved = new Texture(
                scr->pixel_format(),
                scr->component_format(),
                m_size,
                Texture::InterpolationMode::Bilinear,
                Texture::InterpolationMode::Bilinear,
                Texture::WrapMode::ClampToEdge,
                1,
                Texture::TextureFlags::RenderTarget
            );

            m_render_pass_resolved = new RenderPass(
                { color_texture_resolved }
            );
        }
#endif

        depth_texture = new Texture(
            has_stencil_buffer ? Texture::PixelFormat::DepthStencil
                               : Texture::PixelFormat::Depth,
            Texture::ComponentFormat::Float32,
            m_size,
            Texture::InterpolationMode::Bilinear,
            Texture::InterpolationMode::Bilinear,
            Texture::WrapMode::ClampToEdge,
            samples,
            Texture::TextureFlags::RenderTarget
        );
    }

    m_render_pass = new RenderPass(
        { color_texture },
        depth_texture,
        has_stencil_buffer ? depth_texture : nullptr,
#if defined(NANOGUI_USE_METAL)
        m_render_pass_resolved,
#else
        nullptr,
#endif
        clear
    );
}

void Canvas::set_background_color(const Color &background_color) {
    m_render_pass->set_clear_color(0, background_color);
}

const Color& Canvas::background_color() const {
    return m_render_pass->clear_color(0);
}

void Canvas::draw_contents() { /* No-op. */ }

void Canvas::draw(NVGcontext *ctx) {
    Screen *scr = screen();
    if (scr == nullptr)
        throw std::runtime_error("Canvas::draw(): could not find parent screen!");

    float pixel_ratio = scr->pixel_ratio();

    Widget::draw(ctx);

    scr->nvg_flush();

    Vector2i fbsize = m_size;
    Vector2i offset = absolute_position();
    if (m_draw_border)
        fbsize -= 2;

#if defined(NANOGUI_USE_OPENGL) || defined(NANOGUI_USE_GLES)
    if (m_render_to_texture)
        offset = Vector2i(offset.x(), scr->size().y() - offset.y() - m_size.y());
#endif

    if (m_draw_border)
        offset += Vector2i(1, 1);

    fbsize = Vector2i(Vector2f(fbsize) * pixel_ratio);
    offset = Vector2i(Vector2f(offset) * pixel_ratio);

    if (m_render_to_texture) {
        m_render_pass->resize(fbsize);
#if defined(NANOGUI_USE_METAL)
        if (m_render_pass_resolved)
            m_render_pass_resolved->resize(fbsize);
#endif
    } else {
        m_render_pass->resize(scr->framebuffer_size());
        m_render_pass->set_viewport(offset, fbsize);
    }

    m_render_pass->begin();
    draw_contents();
    m_render_pass->end();

    if (m_draw_border) {
        nvgBeginPath(ctx);
        nvgStrokeWidth(ctx, 1.f);
        nvgStrokeColor(ctx, m_border_color);
        nvgRoundedRect(ctx, m_pos.x() + .5f, m_pos.y() + .5f,
                       m_size.x() - 1.f, m_size.y() - 1.f,
                       m_theme->m_window_corner_radius);
        nvgStroke(ctx);
    }

    if (m_render_to_texture) {
        RenderPass *rp = m_render_pass;
#if defined(NANOGUI_USE_METAL)
        if (m_render_pass_resolved)
            rp = m_render_pass_resolved;
#endif
        rp->blit_to(Vector2i(0, 0), fbsize, scr, offset);
    }
}

NAMESPACE_END(nanogui)
