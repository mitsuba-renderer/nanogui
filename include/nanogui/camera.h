/*
    nanogui/camera.h -- Self-contained turntable camera controller

    NanoGUI was developed by Wenzel Jakob <wenzel.jakob@epfl.ch>.
    The widget drawing code is based on the NanoVG demo application
    by Mikko Mononen.

    All rights reserved. Use of this source code is governed by a
    BSD-style license that can be found in the LICENSE.txt file.
*/

/**
 * \file nanogui/camera.h
 *
 * \brief Self-contained turntable camera controller
 */

#pragma once

#include <nanogui/vector.h>
#include <functional>

NAMESPACE_BEGIN(nanogui)

/**
 * \brief Camera state
 *
 * This convenience class wraps the view matrix read and written by the
 * \ref CameraController class below.
 */
struct NANOGUI_EXPORT CameraState {
    Vector3f origin { 0.f, 0.f, 1.f };
    Vector3f target { 0.f };
    Vector3f up { 0.f, 1.f, 0.f };

    CameraState() = default;

    CameraState(const Vector3f &origin, const Vector3f &target, const Vector3f &up)
        : origin(origin), target(target), up(up) { }

    /// Distance from the origin to the target
    float distance() const { return norm(target - origin); }

    /// Unit vector from the origin to the target
    Vector3f forward() const { return normalize(target - origin); }

    /// Unit vector pointing to the right of the view direction
    Vector3f right() const { return normalize(cross(forward(), up)); }

    /// Orthonormal (forward, right, up) basis of the camera
    void basis(Vector3f &forward, Vector3f &right, Vector3f &up) const;

    /// World-to-camera transformation, see \ref Matrix4f::look_at()
    Matrix4f view_matrix() const {
        return Matrix4f::look_at(origin, target, up);
    }
};

/**
 * \brief Self-contained turntable camera controller
 *
 * This class ships a simple and self-contained camera controller to facilitate
 * building demo applications that need 3D viewport navigation. Its internal
 * representation uses a pivot, distance, and azimuth/elevation/roll angles
 * relative to an 'up' axis.
 *
 * Dragging the mouse with LMB held orbits around the pivot, MMB/RMB pan, and
 * the scroll wheel zooms.
 *
 * A \ref Screen subclass can simply forward its mouse and keyboard events and
 * call \ref update() once per frame. The callback set with \ref set_callback()
 * runs whenever the camera changes.
 *
 * Pressing WASD or the cursor keys switches to first person navigation, in
 * which case dragging LMB turns the camera, and the wheel changes the speed.
 * Q and E move down and up along the world up axis, and holding Shift
 * multiplies the speed by \ref fly_boost.
 *
 * The \ref frame() method frames a given bounding box with a smooth camera
 * transition from the current view and switches back to orbit mode.
 *
 * Motion speeds are relative to the distance to the pivot. Panning is designed
 * grab a 3D scene position and maintain it below the moving cursor. This
 * requires the application to provide a \ref set_depth_callback() to obtain the
 * distance from the camera.
 *
 * Several adjustable fields control the motion sensitivity. Negative values
 * flip the associated direction. Applications that desire different key
 * bindings can translate buttons, modifiers, and key codes before forwarding
 * events to the controller.
 */
class NANOGUI_EXPORT CameraController {
public:
    /**
     * \brief Create a controller
     *
     * \param state
     *     Initial camera state
     *
     * \param world_up
     *     Up direction of the scene, around which the orbit yaw rotates
     */
    CameraController(const CameraState &state, const Vector3f &world_up);

    /// Return the current camera state
    CameraState state() const { return to_state(m_turntable); }

    /**
     * \brief Replace the camera state, which ends a drag or frame animation
     *
     * The roll of \c state relative to \ref world_up() is preserved by
     * later orbits. States with non-finite components or a zero distance
     * are ignored. With \c notify cleared, the callback stays silent, which
     * suits corrections that leave the view unchanged, such as moving the
     * orbit target along the view axis.
     */
    void set_state(const CameraState &state, bool notify = true);

    /**
     * \brief Set the callback invoked whenever the camera changes
     *
     * The callback receives a copy of the new camera state and may keep it
     * beyond the call, e.g. as application state read by another thread.
     */
    void set_callback(const std::function<void(CameraState)> &callback) {
        m_callback = callback;
    }

    /// Return the callback invoked whenever the camera changes
    const std::function<void(CameraState)> &callback() const { return m_callback; }

    /**
     * \brief Set the callback invoked when the left mouse button is clicked
     *
     * A press and release with at most \ref drag_threshold pixels of motion in
     * between counts as a click, and such motion leaves the camera untouched.
     * The callback receives the release position and whether the press was a
     * double-click (see \ref MOD_DOUBLE_CLICK).
     */
    void set_click_callback(const std::function<void(Vector2i, bool)> &callback) {
        m_click_callback = callback;
    }

    /// Return the callback invoked when the left mouse button is clicked
    const std::function<void(Vector2i, bool)> &click_callback() const {
        return m_click_callback;
    }

    /**
     * \brief Set a callback that reports the scene depth under the cursor
     *
     * The 3D panning feature requires knowing the 3D depth associated with a
     * given screen-space position. This function can be used to register a
     * callback that provides this information given a normalized [0, 1]^2
     * coordinate. Without this, panning falls back to the pivot distance.
     */
    void set_depth_callback(const std::function<float(Vector2f)> &callback) {
        m_depth_callback = callback;
    }

    /// Return the registered depth callback
    const std::function<float(Vector2f)> &depth_callback() const {
        return m_depth_callback;
    }

    /// Per-axis motion in pixels beyond which a click becomes a drag
    float drag_threshold = 3.f;

    /// Up direction of the scene
    const Vector3f &world_up() const { return m_world_up; }

    /**
     * \brief Set the up direction of the scene
     *
     * With \c snap set, the closest signed world axis is used instead.
     * Non-finite or zero vectors are ignored.
     */
    void set_world_up(const Vector3f &world_up, bool snap = false);

    /**
     * \brief Set the projection matrix of the view and the viewport size
     *
     * Panning and \ref frame() relate cursor motion and framing to the scene
     * through the projection. \c size is the viewport size in the units of
     * the event positions (typically \ref Widget::size()). Panning is disabled
     * until this method is called.
     */
    void set_projection(const Matrix4f &projection, const Vector2i &size) {
        m_projection = projection;
        m_size = size;
    }

    /// Return the projection matrix set with \ref set_projection()
    const Matrix4f &projection() const { return m_projection; }

    /// Return the viewport size set with \ref set_projection()
    const Vector2i &viewport_size() const { return m_size; }

    /// Reference length of the scene, which bounds the range of the zoom
    /// distance. Must be positive.
    float scene_scale = 1.f;

    /// Degrees per pixel of orbit or look drag (negative values flip the direction)
    float orbit_speed = 0.4f;

    /// Multiplier on the pan drag. At the default of 1, the grabbed point
    /// stays under the cursor (negative values flip the direction).
    float pan_speed = 1.f;

    /// Distance factor per scroll wheel notch (values > 1 invert the zoom)
    float zoom_step = 0.8f;

    /// Scroll offset that counts as one notch on touchpads and similar devices
    float zoom_precise_notch = 12.f;

    /// Fly speed in multiples of the pivot distance per second
    float fly_speed = 0.5f;

    /// Seconds over which a held key ramps up to full speed
    float fly_ramp = 0.5f;

    /// Speed multiple applied while Shift is held in first person navigation
    float fly_boost = 4.f;

    /// Handle mouse button presses, see \ref Widget::mouse_button_event()
    bool mouse_button_event(const Vector2i &p, int button, bool down, int modifiers);

    /**
     * \brief Handle mouse motion, see \ref Screen::mouse_motion_event_f()
     *
     * \c button is the bitmask of the pressed buttons. A drag ends when its
     * button is no longer in it.
     *
     * Panning matches the cursor: the point at the pivot depth stays under
     * the mouse, based on the view set with \ref set_projection().
     * Orthographic projections pan independently of the pivot distance.
     */
    bool mouse_motion_event(const Vector2f &p, const Vector2f &rel, int button,
                            int modifiers);

    /// Handle scrolling, see \ref Widget::scroll_event()
    bool scroll_event(const Vector2i &p, const Vector2f &rel, int flags);

    /// Handle key presses, see \ref Widget::keyboard_event()
    bool keyboard_event(int key, int scancode, int action, int modifiers);

    /// Handle focus changes, see \ref Widget::focus_event()
    void focus_event(bool focused);

    /**
     * \brief Advance the fly motion and frame animation
     *
     * Call this once per frame. Returns \c true if the camera changed.
     */
    bool update();

    /// Is a frame animation in progress?
    bool animating() const { return m_animating; }

    /**
     * \brief Is first person navigation active?
     *
     * The navigation keys enter it, \ref frame() and \ref set_state()
     * leave it.
     */
    bool fly_mode() const { return m_fly; }
    void set_fly_mode(bool value) { m_fly = value; }

    /**
     * \brief Animate the camera so that a bounding box fills the view
     *
     * Starts a transition of \c duration seconds from the current camera to
     * one that frames the box along the current view direction, spanning the
     * field of view divided by \c margin along its larger extent.
     *
     * The field of view and the principal point offset are derived from the
     * projection matrix set with \ref set_projection(). Returns \c false
     * without starting an animation for an orthographic projection, where
     * moving the eye does not change the framing.
     */
    bool frame(const Vector3f &min, const Vector3f &max, float margin = 1.2f,
               float duration = 0.7f);

private:
    /// Turntable state
    struct Turntable {
        Vector3f pivot = Vector3f(0.f);
        // Angles are in radians, roll within [-pi/2, pi/2].
        float distance = 1.f, azimuth = 0.f, elevation = 0.f, roll = 0.f;
    };

    /// Basis (forward, right, up) of the given angles, before roll
    void basis(float azimuth, float elevation, Vector3f &forward,
               Vector3f &right, Vector3f &up) const;
    CameraState to_state(const Turntable &t) const;
    bool from_state(const CameraState &state, Turntable &t) const;
    void commit(const Turntable &t, bool notify = true);

    // Current turntable state, the source of truth between events
    Turntable m_turntable;

    // Projection and viewport size of the view, see set_projection()
    Matrix4f m_projection { 1.f };
    Vector2i m_size { 0, 0 };

    // Scene up and the horizontal axes of azimuth 0 and 90 degrees (defaults: +Y up)
    Vector3f m_world_up = Vector3f(0.f, 1.f, 0.f),
             m_axis[2] = { Vector3f(1.f, 0.f, 0.f),
                           Vector3f(0.f, 0.f, -1.f) };

    // Callback invoked whenever the camera changes
    std::function<void(CameraState)> m_callback;

    // Callback invoked when the left mouse button is clicked
    std::function<void(Vector2i, bool)> m_click_callback;

    // Callback that reports the scene depth under the cursor
    std::function<float(Vector2f)> m_depth_callback;

    // Pending click: armed by a press, cancelled by motion beyond drag_threshold
    bool m_click_armed = false, m_click_double = false;
    Vector2f m_click_pos;

    // Button held during the current drag, or -1
    int m_drag_button = -1;

    // Turntable state captured when the drag began
    Turntable m_drag_start;

    // Total mouse offset since the drag began
    Vector2f m_drag_offset;

    // Pan anchor depth captured when the drag began (0: use the pivot distance)
    float m_pan_depth = 0.f;

    // Is first person navigation active?
    bool m_fly = false;

    // Bit mask of held navigation keys
    int m_keys = 0;

    // Is the Shift speed boost active?
    bool m_boost = false;

    // Time of the last update() call
    double m_last_update = -1.0;

    // Seconds the navigation keys have been held, for the acceleration ramp
    float m_held = 0.f;

    // Is a frame animation in progress?
    bool m_animating = false;

    // Start time and duration of the frame animation
    double m_start = 0.0;
    float m_duration = 0.f;

    // Center of the framed bounding box
    Vector3f m_center;

    // Eye-to-center directions at the start and end, scaled to unit depth
    // (interpolating them moves the center on a straight screen-space path)
    Vector3f m_dir0, m_dir1;

    // Center depth at the start and end, interpolated geometrically
    float m_z0 = 0.f, m_z1 = 0.f;
};

NAMESPACE_END(nanogui)
