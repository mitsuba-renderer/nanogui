/*
    nanogui/glcanvas.cpp -- Canvas widget for rendering full-fledged
    OpenGL content within its designated area. Very useful for
    displaying and manipulating 3D objects or scenes. Subclass it and
    overload `draw_gl` for rendering.

    NanoGUI was developed by Wenzel Jakob <wenzel.jakob@epfl.ch>.
    The widget drawing code is based on the NanoVG demo application
    by Mikko Mononen.

    All rights reserved. Use of this source code is governed by a
    BSD-style license that can be found in the LICENSE.txt file.
*/

#include <nanogui/window.h>
#include <nanogui/screen.h>
#include <nanogui/glcanvas.h>
#include <nanogui/theme.h>
#include <nanogui/opengl.h>

NAMESPACE_BEGIN(nanogui)

GLCanvas::GLCanvas(Widget *parent)
    : Widget(parent), m_background_color(Vector4i(128, 128, 128, 255)),
      m_draw_border(true) {
    m_size = Vector2i(250, 250);
}

void GLCanvas::draw_widget_border(NVGcontext *ctx) const {
    nvgBeginPath(ctx);
    nvgStrokeWidth(ctx, 1.0f);
    nvgRoundedRect(ctx, m_pos.x() - 0.5f, m_pos.y() - 0.5f,
                   m_size.x() + 1, m_size.y() + 1, m_theme->m_window_corner_radius);
    nvgStrokeColor(ctx, m_theme->m_border_light);
    nvgRoundedRect(ctx, m_pos.x() - 1.0f, m_pos.y() - 1.0f,
                   m_size.x() + 2, m_size.y() + 2, m_theme->m_window_corner_radius);
    nvgStrokeColor(ctx, m_theme->m_border_dark);
    nvgStroke(ctx);
}

void GLCanvas::draw(NVGcontext *ctx) {
    Widget::draw(ctx);
    nvgEndFrame(ctx);

    if (m_draw_border)
        draw_widget_border(ctx);

    const Screen* screen = this->screen();
    assert(screen);

    float pixel_ratio = screen->pixel_ratio();
    Vector2f screen_size(screen->size());
    Vector2i position_in_screen = absolute_position();

    Vector2i size(Vector2f(m_size) * pixel_ratio),
             image_position(Vector2f(position_in_screen[0],
                                    screen_size[1] - position_in_screen[1] -
                                    (float) m_size[1]) * pixel_ratio);

    GLint stored_viewport[4];
    glGetIntegerv(GL_VIEWPORT, stored_viewport);

    glViewport(image_position[0], image_position[1], size[0] , size[1]);

    glEnable(GL_SCISSOR_TEST);
    glScissor(image_position[0], image_position[1], size[0], size[1]);
    glClearColor(m_background_color[0], m_background_color[1],
                 m_background_color[2], m_background_color[3]);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    draw_gl();

    glDisable(GL_SCISSOR_TEST);
    glViewport(stored_viewport[0], stored_viewport[1],
               stored_viewport[2], stored_viewport[3]);
}

NAMESPACE_END(nanogui)
