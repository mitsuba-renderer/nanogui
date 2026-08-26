/*
    src/camera.cpp -- Self-contained turntable camera controller

    NanoGUI was developed by Wenzel Jakob <wenzel.jakob@epfl.ch>.
    The widget drawing code is based on the NanoVG demo application
    by Mikko Mononen.

    All rights reserved. Use of this source code is governed by a
    BSD-style license that can be found in the LICENSE.txt file.
*/

#include <nanogui/camera.h>
#include <nanogui/opengl.h>
#include <algorithm>
#include <cmath>
#include <vector>

NAMESPACE_BEGIN(nanogui)

static const float Pi = 3.14159265358979f;

void CameraState::basis(Vector3f &forward_, Vector3f &right_, Vector3f &up_) const {
    forward_ = forward();
    right_ = normalize(cross(forward_, up));
    up_ = cross(right_, forward_);
}

/* Return a camera origin from which the points are centered on the image and
   span the field of view divided by margin. axes holds the (forward, right,
   up) basis, shift the lateral tangents of the image center, and min_depth a
   positive lower bound on every point's depth. */
static Vector3f fit_camera(const Vector3f *points, size_t count,
                           const Vector3f &center, const Vector3f axes[3],
                           const Vector2f &tan_half_fov, const Vector2f &shift,
                           float margin, float min_depth) {
    std::vector<float> depth(count), lat(count);
    for (size_t i = 0; i < count; ++i)
        depth[i] = dot(points[i] - center, axes[0]);
    float th[2] = { tan_half_fov.x() / margin, tan_half_fov.y() / margin };

    // The two frustum planes bounding an axis have fixed orientations, so each
    // touches a known extreme corner. Both touch at once when the eye is at
    // depth (hi - lo) / (2 th), and the larger axis decides.
    float dist = min_depth;
    for (size_t i = 0; i < count; ++i)
        dist = std::max(dist, min_depth - depth[i]);
    for (int a = 0; a < 2; ++a) {
        float lo = INFINITY, hi = -INFINITY;
        for (size_t i = 0; i < count; ++i) {
            float l = dot(points[i] - center, axes[a + 1]);
            lo = std::min(lo, l - (shift[a] - th[a]) * depth[i]);
            hi = std::max(hi, l - (shift[a] + th[a]) * depth[i]);
        }
        dist = std::max(dist, (hi - lo) / (2 * th[a]));
    }

    // Lateral eye offset per axis at which the extreme corner tangents are
    // symmetric around the image center. Their sum decreases monotonically
    // with the offset, and the points stay inside the frustum for offsets in
    // [lo, hi], so bisect there (the tight axis has lo == hi).
    Vector3f origin = center - axes[0] * dist;
    for (int a = 0; a < 2; ++a) {
        for (size_t i = 0; i < count; ++i)
            lat[i] = dot(points[i] - center, axes[a + 1]);
        float lo = -INFINITY, hi = INFINITY;
        for (size_t i = 0; i < count; ++i) {
            float w = depth[i] + dist;
            lo = std::max(lo, lat[i] - (shift[a] + th[a]) * w);
            hi = std::min(hi, lat[i] - (shift[a] - th[a]) * w);
        }
        for (int it = 0; it < 40; ++it) {
            float mid = (lo + hi) / 2, tmin = INFINITY, tmax = -INFINITY;
            for (size_t i = 0; i < count; ++i) {
                float t = (lat[i] - mid) / (depth[i] + dist);
                tmin = std::min(tmin, t);
                tmax = std::max(tmax, t);
            }
            if (tmax + tmin > 2 * shift[a])
                lo = mid;
            else
                hi = mid;
        }
        origin += axes[a + 1] * ((lo + hi) / 2);
    }
    return origin;
}

static bool is_finite(const Vector3f &v) {
    return std::isfinite(v.x()) && std::isfinite(v.y()) && std::isfinite(v.z());
}

CameraController::CameraController(const CameraState &state, const Vector3f &world_up) {
    set_world_up(world_up);
    set_state(state);
}

void CameraController::set_world_up(const Vector3f &world_up, bool snap) {
    float n = norm(world_up);
    if (!is_finite(world_up) || !(n > 0.f))
        return;
    Vector3f up = world_up / n;
    if (snap) {
        int k = 0;
        for (int i = 1; i < 3; ++i)
            if (std::abs(up[i]) > std::abs(up[k]))
                k = i;
        up = Vector3f(0.f);
        up[k] = world_up[k] > 0.f ? 1.f : -1.f;
    }
    CameraState state = to_state(m_turntable);
    m_world_up = up;
    // Horizontal axes u, v with v = world_up x u, seeded by the axis least aligned with world_up
    float ax = std::abs(m_world_up.x()), ay = std::abs(m_world_up.y()),
          az = std::abs(m_world_up.z());
    int k = ax < ay ? (ax < az ? 0 : 2) : (ay < az ? 1 : 2);
    Vector3f seed(0.f);
    seed[k] = 1.f;
    m_axis[0] = normalize(cross(m_world_up, seed));
    m_axis[1] = cross(m_world_up, m_axis[0]);
    set_state(state);
}

void CameraController::basis(float azimuth, float elevation, Vector3f &forward,
                             Vector3f &right, Vector3f &up) const {
    float ce = std::cos(elevation), se = std::sin(elevation);
    Vector3f h = m_axis[0] * std::cos(azimuth) + m_axis[1] * std::sin(azimuth);
    forward = h * ce + m_world_up * se;
    right = cross(h, m_world_up);
    up = m_world_up * ce - h * se;
}

CameraState CameraController::to_state(const Turntable &t) const {
    Vector3f forward, right, up;
    basis(t.azimuth, t.elevation, forward, right, up);
    return CameraState(t.pivot - forward * t.distance, t.pivot,
                       up * std::cos(t.roll) + right * std::sin(t.roll));
}

bool CameraController::from_state(const CameraState &c, Turntable &t) const {
    Vector3f offset = c.target - c.origin;
    float d = norm(offset);
    if (!is_finite(c.origin) || !is_finite(offset) || !is_finite(c.up) || !(d > 0.f))
        return false;
    Vector3f dir = offset / d;
    float se = std::min(std::max(dot(dir, m_world_up), -1.f), 1.f);
    Vector3f h = dir - m_world_up * se;
    // Keep the previous azimuth when looking straight along the pole, where it is undetermined
    float azimuth = norm(h) > 1e-6f
                  ? std::atan2(dot(h, m_axis[1]), dot(h, m_axis[0]))
                  : m_turntable.azimuth,
          elevation = std::asin(se);
    Vector3f forward, right, up;
    basis(azimuth, elevation, forward, right, up);
    float y = dot(c.up, right), x = dot(c.up, up),
          roll = std::abs(x) + std::abs(y) > 1e-6f ? std::atan2(y, x) : 0.f;
    // Represent an upside-down camera as an elevation past the pole with a small roll
    if (std::abs(roll) > Pi / 2) {
        azimuth += Pi;
        elevation = Pi - elevation;
        roll += roll > 0.f ? -Pi : Pi;
    }
    t = Turntable { c.target, d, azimuth, elevation, roll };
    return true;
}

void CameraController::commit(const Turntable &t) {
    m_turntable = t;
    // Keep the angles small so that long sessions do not lose precision
    m_turntable.azimuth = std::remainder(t.azimuth, 2 * Pi);
    m_turntable.elevation = std::remainder(t.elevation, 2 * Pi);
    if (m_callback)
        m_callback(to_state(m_turntable));
}

void CameraController::set_state(const CameraState &state) {
    Turntable t;
    if (!from_state(state, t))
        return;
    m_drag_button = -1;
    m_animating = false;
    m_fly = false;
    commit(t);
}

bool CameraController::mouse_button_event(const Vector2i &p, int button, bool down,
                                          int modifiers) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && m_click_callback) {
        if (down) {
            m_click_armed = true;
            m_click_double = (modifiers & MOD_DOUBLE_CLICK) != 0;
            m_click_pos = Vector2f(p);
        } else if (m_click_armed) {
            m_click_armed = false;
            m_click_callback(p, m_click_double);
        }
    }
    if (down) {
        if (m_drag_button >= 0 || button > GLFW_MOUSE_BUTTON_MIDDLE)
            return false;
        m_drag_button = button;
        m_drag_start = m_turntable;
        m_drag_offset = Vector2f(0.f);
        m_animating = false;
        return true;
    } else if (button == m_drag_button) {
        m_drag_button = -1;
        return true;
    }
    return false;
}

bool CameraController::mouse_motion_event(const Vector2f &p, const Vector2f &rel,
                                          int button, int) {
    // Moving away from the press position turns the click into a drag
    if (m_click_armed) {
        Vector2f d = p - m_click_pos;
        if (std::max(std::abs(d.x()), std::abs(d.y())) > drag_threshold)
            m_click_armed = false;
    }
    // A release lost while the window was unfocused must not leave the drag stuck
    if (m_drag_button >= 0 && !(button & (1 << m_drag_button)))
        m_drag_button = -1;
    if (m_drag_button < 0)
        return false;
    m_drag_offset += rel;
    Turntable t = m_drag_start;
    float dx = m_drag_offset.x(), dy = m_drag_offset.y(),
          scale = orbit_speed * (Pi / 180);
    // Upside down, the yaw direction flips so that the scene keeps following the mouse
    float yaw_sign = std::cos(t.elevation) < 0.f ? -1.f : 1.f;
    if (m_fly) {
        // First person: turn the view about the eye, which stays in place
        float limit = Pi / 2 - 1e-3f;
        t.azimuth -= scale * dx * yaw_sign;
        t.elevation = std::min(std::max(t.elevation - scale * dy,
                                        std::min(t.elevation, -limit)),
                               std::max(t.elevation, limit));
        Vector3f origin = to_state(m_drag_start).origin;
        t.pivot = origin + to_state(t).forward() * t.distance;
    } else if (m_drag_button == GLFW_MOUSE_BUTTON_LEFT) {
        t.azimuth -= scale * dx * yaw_sign;
        t.elevation -= scale * dy;
    } else {
        Vector3f forward, right, up;
        to_state(t).basis(forward, right, up);
        t.pivot += (up * dy - right * dx) * (t.distance * pan_speed);
    }
    commit(t);
    return true;
}

bool CameraController::scroll_event(const Vector2i &, const Vector2f &rel) {
    float step = std::min(std::max(rel.y(), -4.f), 4.f);
    if (step == 0.f)
        return false;
    float factor = std::pow(std::max(zoom_step, 1e-6f), step),
          scale = std::max(scene_scale, 1e-30f);
    // Applied to the drag snapshot as well so that a drag in progress carries on
    auto zoom = [&](Turntable &t) {
        float d = std::min(std::max(t.distance * factor, 1e-6f * scale),
                           1e6f * scale);
        // First person zoom moves the pivot and leaves the eye in place,
        // which only changes the fly speed
        if (m_fly) {
            Vector3f origin = to_state(t).origin;
            t.pivot = origin + (t.pivot - origin) * (d / t.distance);
        }
        t.distance = d;
    };
    Turntable t = m_turntable;
    zoom(t);
    zoom(m_drag_start);
    m_animating = false;
    commit(t);
    return true;
}

bool CameraController::keyboard_event(int key, int, int action, int modifiers) {
    int bit;
    switch (key) {
        case GLFW_KEY_W: case GLFW_KEY_UP:    bit = 1; break;
        case GLFW_KEY_S: case GLFW_KEY_DOWN:  bit = 2; break;
        case GLFW_KEY_D: case GLFW_KEY_RIGHT: bit = 4; break;
        case GLFW_KEY_A: case GLFW_KEY_LEFT:  bit = 8; break;
        default: return false;
    }
    if (action == GLFW_PRESS) {
        if (modifiers & (GLFW_MOD_CONTROL | GLFW_MOD_ALT | GLFW_MOD_SUPER))
            return false;
        m_keys |= bit;
        m_fly = true;
    } else if (action == GLFW_RELEASE) {
        m_keys &= ~bit;
    }
    return true;
}

void CameraController::focus_event(bool focused) {
    // Key releases are not delivered while another window has focus
    if (!focused)
        m_keys = 0;
}

bool CameraController::update() {
    double now = glfwGetTime();
    float dt = m_last_update < 0 ? 0.f
             : std::min((float) (now - m_last_update), 0.1f);
    m_last_update = now;
    bool changed = false;

    int fwd = !!(m_keys & 1) - !!(m_keys & 2),
        rgt = !!(m_keys & 4) - !!(m_keys & 8);
    if (m_fly && (fwd != 0 || rgt != 0) && dt > 0.f) {
        float ramp = std::max(fly_ramp, 1e-6f);
        m_held = std::min(m_held + dt, ramp);
        float step = m_turntable.distance * fly_speed * dt * m_held / ramp;
        Vector3f forward, right, up;
        to_state(m_turntable).basis(forward, right, up);
        Vector3f offset = forward * (fwd * step) + right * (rgt * step);
        m_animating = false;
        // The drag snapshot moves along so that a drag in progress carries on
        m_drag_start.pivot += offset;
        Turntable t = m_turntable;
        t.pivot += offset;
        commit(t);
        changed = true;
    } else {
        m_held = 0.f;
    }

    if (m_animating) {
        float elapsed = (float) (now - m_start),
              u = elapsed < m_duration ? elapsed / m_duration : 1.f,
              s = u * u * (3 - 2 * u),
              z = m_z0 * std::pow(m_z1 / m_z0, s);
        Vector3f dir = m_dir0 + (m_dir1 - m_dir0) * s;
        Turntable t = m_turntable;
        t.pivot = m_center + (to_state(m_turntable).forward() - dir) * z;
        t.distance = z;
        commit(t);
        m_animating = u < 1;
        changed = true;
    }
    return changed;
}

bool CameraController::frame(const Vector3f &min, const Vector3f &max,
                             const Matrix4f &projection, float margin,
                             float duration) {
    // Column-major: m[col][row]. A perspective matrix maps z to w through
    // m[2][3], while an orthographic one has w = 1 throughout.
    const auto &m = projection.m;
    if (m[2][3] == 0.f)
        return false;
    Vector2f tan_half_fov(1.f / m[0][0], 1.f / m[1][1]),
             shift(m[2][0] / m[0][0], m[2][1] / m[1][1]);

    CameraState cam = to_state(m_turntable);
    Vector3f axes[3];
    cam.basis(axes[0], axes[1], axes[2]);
    Vector3f corners[8];
    for (int i = 0; i < 8; ++i)
        corners[i] = Vector3f(i & 1 ? max.x() : min.x(),
                              i & 2 ? max.y() : min.y(),
                              i & 4 ? max.z() : min.z());
    Vector3f center = (min + max) * 0.5f;
    Vector3f origin = fit_camera(corners, 8, center, axes, tan_half_fov,
                                 shift, margin,
                                 1e-3f * std::max(scene_scale, 1e-30f));
    float z1 = dot(center - origin, axes[0]);
    // Depth of the center, which can be nonpositive when the camera sits
    // inside a large shape
    float z0 = std::max(dot(center - cam.origin, axes[0]), 1e-3f * z1);

    m_drag_button = -1;
    m_fly = false;
    m_start = glfwGetTime();
    m_duration = duration;
    m_center = center;
    m_dir0 = (center - cam.origin) / z0;
    m_dir1 = (center - origin) / z1;
    m_z0 = z0;
    m_z1 = z1;
    m_animating = true;
    return true;
}

NAMESPACE_END(nanogui)
