/*
    nanogui/glutil.h -- Convenience classes for accessing OpenGL >= 3.x

    NanoGUI was developed by Wenzel Jakob <wenzel.jakob@epfl.ch>.
    The widget drawing code is based on the NanoVG demo application
    by Mikko Mononen.

    All rights reserved. Use of this source code is governed by a
    BSD-style license that can be found in the LICENSE.txt file.
*/
/** \file */

#pragma once

#include <nanogui/opengl.h>
#include <enoki/transform.h>
#include <enoki/quaternion.h>
#include <map>

/// Ensures that ``GL_HALF_FLOAT`` and ``GL_DOUBLE`` are defined properly for all platforms.
#if !defined(GL_HALF_FLOAT) || defined(DOXYGEN_DOCUMENTATION_BUILD)
#  define GL_HALF_FLOAT 0x140B
#endif

#if !defined(GL_DOUBLE) || defined(DOXYGEN_DOCUMENTATION_BUILD)
#  define GL_DOUBLE     0x140A
#endif

NAMESPACE_BEGIN(nanogui)

using Matrix4f = enoki::Matrix<float, 4>;
using Quaternion4f = enoki::Quaternion<float>;

// bypass template specializations
#ifndef DOXYGEN_SHOULD_SKIP_THIS

NAMESPACE_BEGIN(detail)
template <typename T> struct gl_type;
template <> struct gl_type<uint32_t> { static constexpr GLuint value = GL_UNSIGNED_INT; };
template <> struct gl_type<int32_t> { static constexpr GLuint value = GL_INT; };
template <> struct gl_type<uint16_t> { static constexpr GLuint value = GL_UNSIGNED_SHORT; };
template <> struct gl_type<int16_t> { static constexpr GLuint value = GL_SHORT; };
template <> struct gl_type<uint8_t> { static constexpr GLuint value = GL_UNSIGNED_BYTE; };
template <> struct gl_type<int8_t> { static constexpr GLuint value = GL_BYTE; };
template <> struct gl_type<float> { static constexpr GLuint value = GL_FLOAT; };
template <> struct gl_type<double> { static constexpr GLuint value = GL_DOUBLE; };
template <> struct gl_type<enoki::half> { static constexpr GLuint value = GL_HALF_FLOAT; };
NAMESPACE_END(detail)

#endif // DOXYGEN_SHOULD_SKIP_THIS

using enoki::array_size_v;
using enoki::array_depth_v;
using enoki::scalar_t;

//  ----------------------------------------------------

/**
 * \class GLShader glutil.h nanogui/glutil.h
 *
 * Helper class for compiling and linking OpenGL shaders and uploading
 * associated vertex and index buffers.
 */
class NANOGUI_EXPORT GLShader {
public:
    /**
     * \struct Buffer glutil.h nanogui/glutil.h
     *
     * A wrapper struct for maintaining various aspects of items being managed
     * by OpenGL.  Buffers are created when \ref GLShader::upload_attrib is
     * called.
     */
    struct Buffer {
        GLuint id;        ///< The OpenGL buffer handle.
        GLint attrib_id;  ///< The associated attribute handle (if VAO not used)
        GLuint gl_type;   ///< The OpenGL type identifier of this buffer.
        size_t dim;       ///< The dimension of this buffer (typically the row width).
        size_t comp_size; ///< The size (in bytes) of an individual value in this buffer.
        size_t size;      ///< Size of the entire buffer in bytes
        int version;      ///< Version tag associated with the data contained in this buffer.
        bool integral;    ///< Distinguishes between integral and floating point buffers
        bool owned;       ///< Was this buffer allocated by the curent GLShader?
    };


    /// Create an unitialized OpenGL shader
    GLShader() = default;

    /**
     * \brief Initialize the shader using the specified source strings.
     *
     * \param name
     *     The name this shader will be registered as.
     *
     * \param vertex_str
     *     The source of the vertex shader as a string.
     *
     * \param fragment_str
     *     The source of the fragment shader as a string.
     *
     * \param geometry_str
     *     The source of the geometry shader as a string.  The default value is
     *     the empty string, which indicates no geometry shader will be used.
     */
    bool init(const std::string &name,
              const std::string &vertex_str,
              const std::string &fragment_str,
              const std::string &geometry_str = "");

    /**
     * \brief Initialize the shader using the specified files on disk.
     *
     * \param name
     *     The name this shader will be registered as.
     *
     * \param vertex_fname
     *     The path to the file containing the source of the fragment shader.
     *
     * \param fragment_fname
     *     The path to the file containing the source of the vertex shader.
     *
     * \param geometry_fname
     *     The path to the file containing the source of the geometry shader.
     *     The default value is the empty string, which indicates no geometry
     *     shader will be used.
     */
    bool init_from_files(const std::string &name,
                        const std::string &vertex_fname,
                        const std::string &fragment_fname,
                        const std::string &geometry_fname = "");

    /// Return the name of the shader
    const std::string &name() const { return m_name; }

    /**
     * Set a preprocessor definition.  Custom preprocessor definitions must be
     * added **before** initializing the shader (e.g., via \ref init_from_files).
     * See also: \ref m_definitions.
     */
    void define(const std::string &key, const std::string &value) {
        m_definitions[key] = value;
    }

    /**
     * Select this shader for subsequent draw calls.  Simply executes ``glUseProgram``
     * with \ref m_program_shader, and ``glBindVertexArray`` with \ref m_vertex_array_object.
     */
    void bind();

    /// Release underlying OpenGL objects
    void free();

    /// Return the handle of a named shader attribute (-1 if it does not exist)
    GLint attrib(const std::string &name, bool warn = true) const;

    /// Return the handle of a uniform attribute (-1 if it does not exist)
    GLint uniform(const std::string &name, bool warn = true) const;

    /// Upload a memory region as a vertex buffer object (refreshing it as needed)
    template <typename T>
    void upload_attrib(const std::string &name, T *data, size_t dim,
                       size_t count, int version = -1) {
        upload_attrib(name, dim, count, sizeof(T), detail::gl_type<T>::value,
                      std::is_integral_v<T>, data, version);
    }

    /// Upload an index buffer
    template <typename T>
    void upload_indices(T *data, size_t dim, size_t count, int version = -1) {
        upload_attrib("indices", data, dim, count, version);
    }

    /// Low-level version of upload_attrib
    void upload_attrib(const std::string &name, size_t dim, size_t count,
                       size_t comp_size, GLuint gl_type, bool integral,
                       const void *data, int version = -1);

    /// Return the size of the a given vertex buffer
    size_t attrib_size(const std::string &name) const;

    /// Download a vertex buffer object to CPU memory
    void download_attrib(const std::string &name, void *target);

    /// Invalidate the version numbers associated with attribute data
    void invalidate_attribs();

    /// Completely free an existing attribute buffer
    void free_attrib(const std::string &name);

    /// Check if an attribute was registered a given name
    bool has_attrib(const std::string &name) const {
        auto it = m_buffer_objects.find(name);
        if (it == m_buffer_objects.end())
            return false;
        return true;
    }

    /// Create a symbolic link to an attribute of another GLShader. This avoids duplicating unnecessary data
    void share_attrib(const GLShader &other_shader, const std::string &name,
                      const std::string &as = "");

    /// Return the version number of a given attribute
    int attrib_version(const std::string &name) const {
        auto it = m_buffer_objects.find(name);
        if (it == m_buffer_objects.end())
            return -1;
        return it->second.version;
    }

    /// Reset the version number of a given attribute
    void reset_attrib_version(const std::string &name) {
        auto it = m_buffer_objects.find(name);
        if (it != m_buffer_objects.end())
            it->second.version = -1;
    }

    /// Draw a sequence of primitives
    void draw_array(int type, uint32_t offset, uint32_t count);

    /// Draw a sequence of primitives using a previously uploaded index buffer
    void draw_indexed(int type, uint32_t offset, uint32_t count);

    /// Initialize a uniform parameter with a scalar value
    template <typename T, std::enable_if_t<std::is_arithmetic_v<T>, int> = 0>
    void set_uniform(const std::string &name, const T &v, bool warn = true) {
        GLint id = uniform(name, warn);
        if (std::is_integral<T>::value)
            glUniform1i(id, (int) v);
        else
            glUniform1f(id, (float) v);
    }

    /// Initialize a uniform parameter with an Enoki array
    template <typename T,
              std::enable_if_t<array_depth_v<T> == 1, int> = 0>
    void set_uniform(const std::string &name, const T &v, bool warn = true) {
        GLint id = uniform(name, warn);

        if constexpr (array_size_v<T> == 1) {
            if constexpr (std::is_integral<T>::value)
                glUniform1i(id, (int) v);
            else
                glUniform1f(id, (float) v);
        } else if constexpr (array_size_v<T> == 2) {
            if constexpr (std::is_integral_v<scalar_t<T>>)
                glUniform2i(id, (int) v.x(), (int) v.y());
            else
                glUniform2f(id, (float) v.x(), (float) v.y());
        } else if constexpr (array_size_v<T> == 3) {
            if constexpr (std::is_integral_v<scalar_t<T>>)
                glUniform3i(id, (int) v.x(), (int) v.y(), (int) v.z());
            else
                glUniform3f(id, (float) v.x(), (float) v.y(), (float) v.z());
        } else if constexpr (array_size_v<T> == 4) {
            if constexpr (std::is_integral_v<scalar_t<T>>)
                glUniform4i(id, (int) v.x(), (int) v.y(), (int) v.z(), (int) v.w());
            else
                glUniform4f(id, (float) v.x(), (float) v.y(), (float) v.z(), (float) v.w());
        } else {
            static_assert(enoki::detail::false_v<T>, "set_uniform(): Unexpected type!");
        }
    }

    /// Initialize a uniform parameter with a 4x4 matrix (float)
    template <typename T,
              std::enable_if_t<array_size_v<T> == 4 &&
                               array_depth_v<T> == 2, int> = 0>
    void set_uniform(const std::string &name, const T &v, bool warn = true) {
        GLint id = uniform(name, warn);
        glUniformMatrix4fv(id, 1, GL_FALSE, (const GLfloat *) v.data());
    }

    /// Return the size of all registered buffers in bytes
    size_t buffer_size() const {
        size_t size = 0;
        for (auto const &buf : m_buffer_objects)
            size += buf.second.size;
        return size;
    }

    /**
     * \brief (Advanced) Returns a reference to the specified \ref GLShader::Buffer.
     *
     * \rst
     * .. danger::
     *
     *    Extreme caution must be exercised when using this method.  The user is
     *    discouraged from explicitly storing the reference returned, as it can
     *    change, become deprecated, or no longer reside in
     *    :member:`m_buffer_objects <nanogui::GLShader::m_buffer_objects>`.
     *
     *    There are generally very few use cases that justify using this method
     *    directly.  For example, if you need the version of a buffer, call
     *    :func:`attrib_version <nanogui::GLShader::attrib_version>`.  If you want
     *    to share data between :class:`GLShader <nanogui::GLShader>` objects,
     *    call :func:`share_attrib <nanogui::GLShader::share_attrib>`.
     *
     *    One example use case for this method is sharing data between different
     *    GPU pipelines such as CUDA or OpenCL.  When sharing data, you
     *    typically need to map pointers between the API's.  The returned
     *    buffer's :member:`Buffer::id <nanogui::GLShader::Buffer::id>` is the
     *    ``GLuint`` you will want to map to the other API.
     *
     *    In short, only use this method if you absolutely need to.
     * \endrst
     *
     * \param name
     *     The name of the desired attribute.
     *
     * \return
     *     A reference to the current buffer associated with ``name``.  Should
     *     not be explicitly stored.
     *
     * \throws std::runtime_error
     *     If ``name`` is not found.
     */
    const Buffer &attrib_buffer(const std::string &name);

protected:
    /// The registered name of this GLShader.
    std::string m_name;

    /// The vertex shader handle of this GLShader (as returned by ``glCreateShader``).
    GLuint m_vertex_shader = 0;

    /// The fragment shader handle of this GLShader (as returned by ``glCreateShader``).
    GLuint m_fragment_shader = 0;

#if defined(NANOGUI_USE_OPENGL)
    /// The geometry shader handle (if requested) of this GLShader (as returned by ``glCreateShader``).
    GLuint m_geometry_shader = 0;
#endif

    /// The OpenGL program handle (as returned by ``glCreateProgram``).
    GLuint m_program_shader = 0;

#if defined(NANOGUI_USE_OPENGL)
    /// The vertex array associated with this GLShader (as returned by ``glGenVertexArrays``).
    GLuint m_vertex_array_object = 0;
#endif

    /**
     * The map of string names to buffer objects representing the various
     * attributes that have been uploaded using \ref upload_attrib.
     */
    std::map<std::string, Buffer> m_buffer_objects;

    /**
     * \rst
     * The map of preprocessor names to values (if any have been created).  If
     * a definition was added seeking to create ``#define WIDTH 256``, the key
     * would be ``"WIDTH"`` and the value would be ``"256"``.  These definitions
     * will be included automatically in the string that gets compiled for the
     * vertex, geometry, and fragment shader code.
     * \endrst
     */
    std::map<std::string, std::string> m_definitions;
};

//  ----------------------------------------------------

/**
 * \class GLFramebuffer glutil.h nanogui/glutil.h
 *
 * \brief Helper class for creating framebuffer objects.
 */
class NANOGUI_EXPORT GLFramebuffer {
public:
    /// Default constructor: unusable until you call the ``init()`` method
    GLFramebuffer() : m_framebuffer(0), m_depth(0), m_color(0), m_samples(0) { }

    /// Create a new framebuffer with the specified size and number of MSAA samples
    void init(const Vector2i &size, int n_samples);

    /// Release all associated resources
    void free();

    /// Bind the framebuffer object
    void bind();

    /// Release/unbind the framebuffer object
    void release();

    /// Blit the framebuffer object onto the screen
    void blit();

    /// Return whether or not the framebuffer object has been initialized
    bool ready() { return m_framebuffer != 0; }

    /// Return the number of MSAA samples
    int samples() const { return m_samples; }

    /// Quick and dirty method to write a TGA (32bpp RGBA) file of the framebuffer contents for debugging
    void download_tga(const std::string &filename);
protected:
    GLuint m_framebuffer, m_depth, m_color;
    Vector2i m_size;
    int m_samples;
};

//  ----------------------------------------------------

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
