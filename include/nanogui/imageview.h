/*
    nanogui/imageview.h -- Widget used to display images.

    The image view widget was contributed by Stefan Ivanov.

    NanoGUI was developed by Wenzel Jakob <wenzel.jakob@epfl.ch>.
    The widget drawing code is based on the NanoVG demo application
    by Mikko Mononen.

    All rights reserved. Use of this source code is governed by a
    BSD-style license that can be found in the LICENSE.txt file.
*/
/** \file */

#pragma once

#include <nanogui/widget.h>
#include <nanogui/glutil.h>
#include <functional>

NAMESPACE_BEGIN(nanogui)

/**
 * \class ImageView imageview.h nanogui/imageview.h
 *
 * \brief Widget used to display images.
 */
class NANOGUI_EXPORT ImageView : public Widget {
public:
    ImageView(Widget* parent, GLuint image_id);
    ~ImageView();

    void bind_image(GLuint image_id);

    GLShader& image_shader() { return m_shader; }

    Vector2f position_f() const { return Vector2f(m_pos); }
    Vector2f size_f() const { return Vector2f(m_size); }

    const Vector2i& image_size() const { return m_image_size; }
    Vector2i scaled_image_size() const { return Vector2i(m_scale * Vector2f(m_image_size)); }
    Vector2f image_size_f() const { return Vector2f(m_image_size); }
    Vector2f scaled_image_size_f() const { return (m_scale * Vector2f(m_image_size)); }

    const Vector2f& offset() const { return m_offset; }
    void set_offset(const Vector2f& offset) { m_offset = offset; }
    float scale() const { return m_scale; }
    void set_scale(float scale) { m_scale = scale > 0.01f ? scale : 0.01f; }

    bool fixed_offset() const { return m_fixed_offset; }
    void set_fixed_offset(bool fixed_offset) { m_fixed_offset = fixed_offset; }
    bool fixed_scale() const { return m_fixed_scale; }
    void set_fixed_scale(bool fixed_scale) { m_fixed_scale = fixed_scale; }

    float zoom_sensitivity() const { return m_zoom_sensitivity; }
    void set_zoom_sensitivity(float zoom_sensitivity) { m_zoom_sensitivity = zoom_sensitivity; }

    float grid_threshold() const { return m_grid_threshold; }
    void set_grid_threshold(float grid_threshold) { m_grid_threshold = grid_threshold; }

    float pixel_info_threshold() const { return m_pixel_info_threshold; }
    void set_pixel_info_threshold(float pixel_info_threshold) { m_pixel_info_threshold = pixel_info_threshold; }

#ifndef DOXYGEN_SHOULD_SKIP_THIS
    void set_pixel_info_callback(const std::function<std::pair<std::string, Color>(const Vector2i&)>& callback) {
        m_pixel_info_callback = callback;
    }
    const std::function<std::pair<std::string, Color>(const Vector2i&)>& pixel_info_callback() const {
        return m_pixel_info_callback;
    }
#endif // DOXYGEN_SHOULD_SKIP_THIS

    void set_font_scale_factor(float font_scale_factor) { m_font_scale_factor = font_scale_factor; }
    float font_scale_factor() const { return m_font_scale_factor; }

    // Image transformation functions.

    /// Calculates the image coordinates of the given pixel position on the widget.
    Vector2f image_coordinate_at(const Vector2f& position) const;

    /**
     * Calculates the image coordinates of the given pixel position on the widget.
     * If the position provided corresponds to a coordinate outside the range of
     * the image, the coordinates are clamped to edges of the image.
     */
    Vector2f clamped_image_coordinate_at(const Vector2f& position) const;

    /// Calculates the position inside the widget for the given image coordinate.
    Vector2f position_for_coordinate(const Vector2f& image_coordinate) const;

    /**
     * Modifies the internal state of the image viewer widget so that the pixel at the provided
     * position on the widget has the specified image coordinate. Also clamps the values of offset
     * to the sides of the widget.
     */
    void set_image_coordinate_at(const Vector2f& position, const Vector2f& image_coordinate);

    /// Centers the image without affecting the scaling factor.
    void center();

    /// Centers and scales the image so that it fits inside the widgets.
    void fit();

    /// Set the scale while keeping the image centered
    void set_scale_centered(float scale);

    /// Moves the offset by the specified amount. Does bound checking.
    void move_offset(const Vector2f& delta);

    /**
     * Changes the scale factor by the provided amount modified by the zoom sensitivity member variable.
     * The scaling occurs such that the image coordinate under the focused position remains in
     * the same position before and after the scaling.
     */
    void zoom(int amount, const Vector2f& focus_position);

    bool keyboard_event(int key, int scancode, int action, int modifiers) override;
    bool keyboard_character_event(unsigned int codepoint) override;
    bool mouse_drag_event(const Vector2i &p, const Vector2i &rel, int button, int modifiers) override;
    bool scroll_event(const Vector2i &p, const Vector2f &rel) override;

    /// Function indicating whether the grid is currently visible.
    bool grid_visible() const;

    /// Function indicating whether the pixel information is currently visible.
    bool pixel_info_visible() const;

    /// Function indicating whether any of the overlays are visible.
    bool helpers_visible() const;

    Vector2i preferred_size(NVGcontext* ctx) const override;
    void perform_layout(NVGcontext* ctx) override;
    void draw(NVGcontext* ctx) override;

private:
    // Helper image methods.
    void update_image_parameters();

    // Helper drawing methods.
    void draw_widget_border(NVGcontext* ctx) const;
    void draw_image_border(NVGcontext* ctx) const;
    void draw_helpers(NVGcontext* ctx) const;
    static void draw_pixel_grid(NVGcontext* ctx, const Vector2f& upper_left_corner,
                              const Vector2f& lower_right_corner, float stride);
    void draw_pixel_info(NVGcontext* ctx, float stride) const;
    void write_pixel_info(NVGcontext* ctx, const Vector2f& cell_position,
                        const Vector2i& pixel, float stride, float font_size) const;

    // Image parameters.
    GLShader m_shader;
    GLuint m_image_id;
    Vector2i m_image_size;

    // Image display parameters.
    float m_scale;
    Vector2f m_offset;
    bool m_fixed_scale;
    bool m_fixed_offset;

    // Fine-tuning parameters.
    float m_zoom_sensitivity = 1.1f;

    // Image info parameters.
    float m_grid_threshold = -1;
    float m_pixel_info_threshold = -1;

    // Image pixel data display members.
    std::function<std::pair<std::string, Color>(const Vector2i&)> m_pixel_info_callback;
    float m_font_scale_factor = 0.2f;
};

NAMESPACE_END(nanogui)
