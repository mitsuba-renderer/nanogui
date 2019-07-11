/*
    NanoGUI was developed by Wenzel Jakob <wenzel.jakob@epfl.ch>.
    The widget drawing code is based on the NanoVG demo application
    by Mikko Mononen.

    All rights reserved. Use of this source code is governed by a
    BSD-style license that can be found in the LICENSE.txt file.
*/
/**
 * \file nanogui/arcball.h
 *
 * \brief Defines the arcball helper class to interactively rotate objects on-screen.
 */

#pragma once

#include <nanogui/widget.h>
#include <enoki/quaternion.h>

NAMESPACE_BEGIN(nanogui)

/**
 * \struct Arcball glutil.h nanogui/glutil.h
 *
 * \brief Arcball helper class to interactively rotate objects on-screen.
 *
 * The Arcball class enables fluid interaction by representing rotations using
 * a quaternion, and is setup to be used in conjunction with the existing
 * mouse callbacks defined in \ref nanogui::Widget.  The Arcball operates by
 * maintaining an "active" state which is typically controlled using a mouse
 * button click / release.  A click pressed would call \ref Arcball::button
 * with ``down = true``, and a click released with ``down = false``.  The high
 * level mechanics are:
 *
 * 1. The Arcball is made active by calling \ref Arcball::button with a
 *    specified click location, and ``down = true``.
 *
 * 2. As the user holds the mouse button down and drags, calls to
 *    \ref Arcball::motion are issued.  Internally, the Arcball keeps track of
 *    how far the rotation is from the start click.  During the active state,
 *    \ref m_quat is not updated, call \ref Arcball::matrix to get the current
 *    rotation for use in drawing updates.  Receiving the rotation as a matrix
 *    will usually be more convenient for traditional pipelines, however you
 *    can also acquire the associated rotation quaternion \ref Arcball::active_state.
 *
 * 3. The user releases the mouse button, and a call to \ref Arcball::button
 *    with ``down = false``.  The Arcball is no longer active, and its internal
 *    \ref m_quat is updated.
 *
 * A very simple \ref nanogui::Screen derived class to illustrate usage:
 *
 * \rst
 * .. code-block:: cpp
 *
 *    class ArcballScreen : public nanogui::Screen {
 *    public:
 *        // Creating a 400x400 window
 *        ArcballScreen() : nanogui::Screen({400, 400}, "ArcballDemo") {
 *            m_arcball.set_size(m_size);// Note 1
 *        }
 *
 *        virtual bool mouse_button_event(const Vector2i &p, int button, bool down, int modifiers) override {
 *            // In this example, we are using the left mouse button
 *            // to control the arcball motion
 *            if (button == GLFW_MOUSE_BUTTON_1) {
 *                m_arcball.button(p, down);// Note 2
 *                return true;
 *            }
 *            return false;
 *        }
 *
 *        virtual bool mouse_motion_event(const Vector2i &p, const Vector2i &rel, int button, int modifiers) override {
 *            if (button == GLFW_MOUSE_BUTTON_1) {
 *                m_arcball.motion(p);// Note 2
 *                return true;
 *            }
 *            return false;
 *        }
 *
 *        virtual void draw_contents() override {
 *            // Option 1: acquire a 4x4 homogeneous rotation matrix
 *            Matrix4f rotation = m_arcball.matrix();
 *            // Option 2: acquire an equivalent quaternion
 *            Quaternion4f rotation = m_arcball.active_state();
 *            // ... do some drawing with the current rotation ...
 *        }
 *
 *    protected:
 *        nanogui::Arcball m_arcball;
 *    };
 *
 * **Note 1**
 *     The user is responsible for setting the size with
 *     :func:`Arcball::set_size <nanogui::Arcball::set_size>`, this does **not**
 *     need to be the same as the Screen dimensions (e.g., you are using the
 *     Arcball to control a specific ``glViewport``).
 *
 * **Note 2**
 *     Be aware that the input vector ``p`` to
 *     :func:`Widget::mouse_button_event <nanogui::Widget::mouse_button_event>`
 *     and :func:`Widget::mouse_motion_event <nanogui::Widget::mouse_motion_event>`
 *     are in the coordinates of the Screen dimensions (top left is ``(0, 0)``,
 *     bottom right is ``(width, height)``).  If you are using the Arcball to
 *     control a subregion of the Screen, you will want to transform the input
 *     ``p`` before calling :func:`Arcball::button <nanogui::Arcball::button>`
 *     or :func:`Arcball::motion <nanogui::Arcball::motion>`.  For example, if
 *     controlling the right half of the screen, you might create
 *     ``Vector2i adjusted_click(p.x() - (m_size.x() / 2), p.y())``, and then
 *     call ``m_arcball.motion(adjusted_click)``.
 * \endrst
 */
struct Arcball {
    /**
     * \brief The default constructor.
     *
     * \rst
     * .. note::
     *
     *    Make sure to call :func:`Arcball::set_size <nanogui::Arcball::set_size>`
     *    after construction.
     * \endrst
     *
     * \param speed_factor
     *     The speed at which the Arcball rotates (default: ``2.0``).  See also
     *     \ref m_speed_factor.
     */
    Arcball(float speed_factor = 2.0f)
        : m_active(false), m_last_pos(0), m_size(0),
          m_quat(enoki::identity<Quaternion4f>()),
          m_incr(enoki::identity<Quaternion4f>()),
          m_speed_factor(speed_factor) { }

    /**
     * Constructs an Arcball based off of the specified rotation.
     *
     * \rst
     * .. note::
     *
     *    Make sure to call :func:`Arcball::set_size <nanogui::Arcball::set_size>`
     *    after construction.
     * \endrst
     */
     Arcball(const Quaternion4f &quat)
        : m_active(false), m_last_pos(0), m_size(0),
          m_quat(quat),
          m_incr(enoki::identity<Quaternion4f>()),
          m_speed_factor(2.0f) { }

    /**
     * \brief Returs the current internal rotation state of the Arcball
     *
     * Use \ref Arcball::matrix for drawing loops. This method will not return
     * any updates while \ref m_active is ``true``.
     */
    Quaternion4f &state() { return m_quat; }

    /// ``const`` version of \ref Arcball::state.
    const Quaternion4f &state() const { return m_quat; }

    /// Sets the rotation of this Arcball.  The Arcball will be marked as **not** active.
    void set_state(const Quaternion4f &state) {
        m_active = false;
        m_last_pos = 0;
        m_quat = state;
        m_incr = enoki::identity<Quaternion4f>();
    }

    /**
     * \brief Sets the screen size in pixels associated with the Arcball widget
     *
     * The size of the Arcball and the positions provided in \ref
     * Arcball::button and \ref Arcball::motion are in the same units
     * (usually pixels in screen-space).
     */
    void set_size(Vector2i size) { m_size = size; }

    /// Returns the current size of this Arcball.
    const Vector2i &size() const { return m_size; }

    /// Sets the speed at which this Arcball rotates.  See also \ref m_speed_factor.
    void set_speed_factor(float speed_factor) { m_speed_factor = speed_factor; }

    /// Returns the current speed at which this Arcball rotates.
    float speed_factor() const { return m_speed_factor; }

    /// Returns whether or not this Arcball is currently active.
    bool active() const { return m_active; }

    /**
     * \brief Signals a state change from active to non-active, or vice-versa.
     *
     * \param pos
     *     The click location, should be in the same coordinate system as
     *     specified by \ref m_size.
     *
     * \param pressed
     *     When ``true``, this Arcball becomes active.  When ``false``, this
     *     Arcball becomes non-active, and its internal \ref m_quat is updated
     *     with the final rotation.
     */
    void button(Vector2i pos, bool pressed) {
        m_active = pressed;
        m_last_pos = pos;
        if (!m_active)
            m_quat = normalize(m_incr * m_quat);
        m_incr = enoki::identity<Quaternion4f>();
    }

    /**
     * \brief When active, updates \ref m_incr corresponding to the specified
     *        position.
     *
     * \param pos
     *     Where the mouse has been dragged
     */
    bool motion(Vector2i pos) {
        if (!m_active)
            return false;

        /* Based on the rotation controller form AntTweakBar */
        float inv_min_dim = 1.0f / hmin(m_size);
        float w = (float) m_size.x(), h = (float) m_size.y();

        float ox = (m_speed_factor * (2*m_last_pos.x() - w) + w) - w - 1.0f;
        float tx = (m_speed_factor * (2*pos.x()        - w) + w) - w - 1.0f;
        float oy = (m_speed_factor * (h - 2*m_last_pos.y()) + h) - h - 1.0f;
        float ty = (m_speed_factor * (h - 2*pos.y())        + h) - h - 1.0f;

        ox *= inv_min_dim; oy *= inv_min_dim;
        tx *= inv_min_dim; ty *= inv_min_dim;

        Vector3f v0(ox, oy, 1.0f), v1(tx, ty, 1.0f);
        if (enoki::squared_norm(v0) > 1e-4f && enoki::squared_norm(v1) > 1e-4f) {
            v0 = enoki::normalize(v0);
            v1 = enoki::normalize(v1);
            Vector3f axis = enoki::cross(v0, v1);
            float sa = enoki::norm(axis),
                  ca = enoki::dot(v0, v1),
                  angle = std::atan2(sa, ca);
            if (tx*tx + ty*ty > 1.0f)
                angle *= 1.0f + 0.2f * (std::sqrt(tx*tx + ty*ty) - 1.0f);
            m_incr = enoki::rotate<Quaternion4f>(enoki::normalize(axis), angle);
            if (!std::isfinite(enoki::abs(m_incr)))
                m_incr = enoki::identity<Quaternion4f>();
        }
        return true;
    }

    /**
     * Returns the current rotation *including* the active motion, suitable for
     * use with typical homogeneous matrix transformations.  The upper left 3x3
     * block is the rotation matrix, with 0-0-0-1 as the right-most column /
     * bottom row.
     */
    Matrix4f matrix() const {
        return enoki::quat_to_matrix<Matrix4f>(m_incr * m_quat);
    }

    /// Returns the current rotation *including* the active motion.
    Quaternion4f active_state() const { return m_incr * m_quat; }

    /**
     * \brief Interrupts the current Arcball motion by calling
     *        \ref Arcball::button with ``(0, 0)`` and ``false``.
     *
     * Use this method to "close" the state of the Arcball when a mouse release
     * event is not available.  You would use this method if you need to stop
     * the Arcball from updating its internal rotation, but the event stopping
     * the rotation does **not** come from a mouse release.  For example, you
     * have a callback that created a \ref nanogui::MessageDialog which will now
     * be in focus.
     */
    void interrupt() { button(Vector2i(0), false); }
protected:
    bool m_active;
    Vector2i m_last_pos;
    Vector2i m_size;
    Quaternion4f m_quat, m_incr;
    float m_speed_factor;
};

NAMESPACE_END(nanogui)
