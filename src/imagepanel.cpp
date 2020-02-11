/*
    src/imagepanel.cpp -- Image panel widget which shows a number of
    square-shaped icons

    NanoGUI was developed by Wenzel Jakob <wenzel.jakob@epfl.ch>.
    The widget drawing code is based on the NanoVG demo application
    by Mikko Mononen.

    All rights reserved. Use of this source code is governed by a
    BSD-style license that can be found in the LICENSE.txt file.
*/

#include <nanogui/imagepanel.h>
#include <nanogui/opengl.h>

NAMESPACE_BEGIN(nanogui)

ImagePanel::ImagePanel(Widget *parent)
    : Widget(parent), m_thumb_size(64), m_spacing(10), m_margin(10),
      m_mouse_index(-1) {}

Vector2i ImagePanel::grid_size() const {
    int n_cols = 1 + std::max(0,
        (int) ((m_size.x() - 2 * m_margin - m_thumb_size) /
        (float) (m_thumb_size + m_spacing)));
    int n_rows = ((int) m_images.size() + n_cols - 1) / n_cols;
    return Vector2i(n_cols, n_rows);
}

int ImagePanel::index_for_position(const Vector2i &p) const {
    Vector2f pp = (Vector2f(p - m_pos) - Vector2f(m_margin)) /
                  (float)(m_thumb_size + m_spacing);
    float icon_region = m_thumb_size / (float)(m_thumb_size + m_spacing);
    bool over_image = pp.x() - std::floor(pp.x()) < icon_region &&
                      pp.y() - std::floor(pp.y()) < icon_region;
    Vector2i grid_pos(pp), grid = grid_size();
    over_image &= grid_pos.x() >= 0 && grid_pos.y() >= 0 && pp.x() >= 0 &&
                  pp.y() >= 0 && grid_pos.x() < grid.x() &&
                  grid_pos.y() < grid.y();
    return over_image ? (grid_pos.x() + grid_pos.y() * grid.x()) : -1;
}

bool ImagePanel::mouse_motion_event(const Vector2i &p, const Vector2i & /* rel */,
                                    int /* button */, int /* modifiers */) {
    m_mouse_index = index_for_position(p);
    return true;
}

bool ImagePanel::mouse_button_event(const Vector2i &p, int /* button */, bool down,
                                    int /* modifiers */) {
    int index = index_for_position(p);
    if (index >= 0 && index < (int) m_images.size() && m_callback && down)
        m_callback(index);
    return true;
}

Vector2i ImagePanel::preferred_size(NVGcontext *) const {
    Vector2i grid = grid_size();
    return Vector2i(
        grid.x() * m_thumb_size + (grid.x() - 1) * m_spacing + 2*m_margin,
        grid.y() * m_thumb_size + (grid.y() - 1) * m_spacing + 2*m_margin
    );
}

void ImagePanel::draw(NVGcontext* ctx) {
    Vector2i grid = grid_size();

    for (size_t i = 0; i < m_images.size(); ++i) {
        Vector2i p = m_pos + Vector2i(m_margin) +
            Vector2i((int) i % grid.x(), (int) i / grid.x()) * (m_thumb_size + m_spacing);
        int imgw, imgh;

        nvgImageSize(ctx, m_images[i].first, &imgw, &imgh);
        float iw, ih, ix, iy;
        if (imgw < imgh) {
            iw = m_thumb_size;
            ih = iw * (float)imgh / (float)imgw;
            ix = 0;
            iy = -(ih - m_thumb_size) * 0.5f;
        } else {
            ih = m_thumb_size;
            iw = ih * (float)imgw / (float)imgh;
            ix = -(iw - m_thumb_size) * 0.5f;
            iy = 0;
        }

        NVGpaint img_paint = nvgImagePattern(
            ctx, p.x() + ix, p.y()+ iy, iw, ih, 0, m_images[i].first,
            m_mouse_index == (int)i ? 1.0 : 0.7);

        nvgBeginPath(ctx);
        nvgRoundedRect(ctx, p.x(), p.y(), m_thumb_size, m_thumb_size, 5);
        nvgFillPaint(ctx, img_paint);
        nvgFill(ctx);

        NVGpaint shadow_paint =
            nvgBoxGradient(ctx, p.x() - 1, p.y(), m_thumb_size + 2, m_thumb_size + 2, 5, 3,
                           nvgRGBA(0, 0, 0, 128), nvgRGBA(0, 0, 0, 0));
        nvgBeginPath(ctx);
        nvgRect(ctx, p.x()-5,p.y()-5, m_thumb_size+10,m_thumb_size+10);
        nvgRoundedRect(ctx, p.x(),p.y(), m_thumb_size,m_thumb_size, 6);
        nvgPathWinding(ctx, NVG_HOLE);
        nvgFillPaint(ctx, shadow_paint);
        nvgFill(ctx);

        nvgBeginPath(ctx);
        nvgRoundedRect(ctx, p.x()+0.5f,p.y()+0.5f, m_thumb_size-1,m_thumb_size-1, 4-0.5f);
        nvgStrokeWidth(ctx, 1.0f);
        nvgStrokeColor(ctx, nvgRGBA(255,255,255,80));
        nvgStroke(ctx);
    }
}

NAMESPACE_END(nanogui)
