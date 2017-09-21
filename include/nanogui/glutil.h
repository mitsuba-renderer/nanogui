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

using enoki::array_size;
using enoki::array_depth;
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

    /// Set a preprocessor definition
    void define(const std::string &key, const std::string &value) {
        m_definitions[key] = value;
    }

    /// Select this shader for subsequent draw calls
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
                      std::is_integral<T>::value, data, version);
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
    template <typename T, std::enable_if_t<std::is_arithmetic<T>::value, int> = 0>
    void set_uniform(const std::string &name, const T &v, bool warn = true) {
        GLint id = uniform(name, warn);
        if (std::is_integral<T>::value)
            glUniform1i(id, (int) v);
        else
            glUniform1f(id, (float) v);
    }

    /// Initialize a uniform parameter with a 2D vector
    template <typename T,
              std::enable_if_t<enoki::array_size<T>::value == 2 &&
                               enoki::array_depth<T>::value == 1, int> = 0>
    void set_uniform(const std::string &name, const T &v, bool warn = true) {
        GLint id = uniform(name, warn);
        if (std::is_integral<scalar_t<T>>::value)
            glUniform2i(id, (int) v.x(), (int) v.y());
        else
            glUniform2f(id, (float) v.x(), (float) v.y());
    }

    /// Initialize a uniform parameter with a 3D vector
    template <typename T,
              std::enable_if_t<enoki::array_size<T>::value == 3 &&
                               enoki::array_depth<T>::value == 1, int> = 0>
    void set_uniform(const std::string &name, const T &v, bool warn = true) {
        GLint id = uniform(name, warn);
        if (std::is_integral<scalar_t<T>>::value)
            glUniform3i(id, (int) v.x(), (int) v.y(), (int) v.z());
        else
            glUniform3f(id, (float) v.x(), (float) v.y(), (float) v.z());
    }

    /// Initialize a uniform parameter with a 4D vector
    template <typename T,
              std::enable_if_t<enoki::array_size<T>::value == 4 &&
                               enoki::array_depth<T>::value == 1, int> = 0>
    void set_uniform(const std::string &name, const T &v, bool warn = true) {
        GLint id = uniform(name, warn);
        if (std::is_integral<scalar_t<T>>::value)
            glUniform4i(id, (int) v.x(), (int) v.y(), (int) v.z(), (int) v.w());
        else
            glUniform4f(id, (float) v.x(), (float) v.y(), (float) v.z(), (float) v.w());
    }

    /// Initialize a uniform parameter with a 4x4 matrix (float)
    template <typename T,
              std::enable_if_t<enoki::array_size<T>::value == 4 &&
                               enoki::array_depth<T>::value == 2, int> = 0>
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

public:

protected:
    /**
     * \struct Buffer glutil.h nanogui/glutil.h
     *
     * A wrapper struct for maintaining various aspects of items being managed
     * by OpenGL.
     */
    struct Buffer {
        GLuint id;
        GLint attrib_id;
        GLuint gl_type;
        size_t dim;
        size_t comp_size;
        size_t size;
        int version;
        bool integral;
    };

    std::string m_name;
    GLuint m_vertex_shader = 0;
    GLuint m_fragment_shader = 0;
#if defined(NANOGUI_USE_OPENGL)
    GLuint m_geometry_shader = 0;
#endif
    GLuint m_program_shader = 0;
#if defined(NANOGUI_USE_OPENGL)
    GLuint m_vertex_array_object = 0;
#endif
    std::map<std::string, Buffer> m_buffer_objects;
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
 */
struct Arcball {
    Arcball(float speed_factor = 2.0f)
        : m_active(false), m_last_pos(0), m_size(0),
          m_quat(enoki::identity<Quaternion4f>()),
          m_incr(enoki::identity<Quaternion4f>()),
          m_speed_factor(speed_factor) { }

    Arcball(const Quaternion4f &quat)
        : m_active(false), m_last_pos(0), m_size(0),
          m_quat(quat),
          m_incr(enoki::identity<Quaternion4f>()),
          m_speed_factor(2.0f) { }

    Quaternion4f &state() { return m_quat; }

    void set_state(const Quaternion4f &state) {
        m_active = false;
        m_last_pos = 0;
        m_quat = state;
        m_incr = enoki::identity<Quaternion4f>();
    }

    void set_size(Vector2i size) { m_size = size; }
    const Vector2i &size() const { return m_size; }
    void set_speed_factor(float speed_factor) { m_speed_factor = speed_factor; }
    float speed_factor() const { return m_speed_factor; }
    bool active() const { return m_active; }

    void button(Vector2i pos, bool pressed) {
        m_active = pressed;
        m_last_pos = pos;
        if (!m_active)
            m_quat = normalize(m_incr * m_quat);
        m_incr = enoki::identity<Quaternion4f>();
    }

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

    Matrix4f matrix() const {
        return enoki::quat_to_matrix<Matrix4f>(m_incr * m_quat);
    }

    ENOKI_ALIGNED_OPERATOR_NEW()
protected:
    bool m_active;
    Vector2i m_last_pos;
    Vector2i m_size;
    Quaternion4f m_quat, m_incr;
    float m_speed_factor;
};


NAMESPACE_END(nanogui)
