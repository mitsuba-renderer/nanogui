/*
    NanoGUI was developed by Wenzel Jakob <wenzel.jakob@epfl.ch>.
    The widget drawing code is based on the NanoVG demo application
    by Mikko Mononen.

    All rights reserved. Use of this source code is governed by a
    BSD-style license that can be found in the LICENSE.txt file.
*/

/**
 * \file nanogui/shader.h
 *
 * \brief Defines abstractions for shaders that work with OpenGL,
 * OpenGL ES, and Metal.
 */

#pragma once

#include <nanogui/object.h>
#include <unordered_map>

NAMESPACE_BEGIN(nanogui)

class NANOGUI_EXPORT Shader : public Object {
public:
    /// The type of geometry that should be rendered
    enum class PrimitiveType {
        Point,
        Line,
        LineStrip,
        Triangle,
        TriangleStrip
    };

    /// Alpha blending mode
    enum class BlendMode {
        None,
        AlphaBlend // alpha * new_color + (1 - alpha) * old_color
    };


    /**
     * \brief Initialize the shader using the specified source strings.
     *
     * \param render_pass
     *     RenderPass object encoding targets to which color, depth,
     *     and stencil information will be rendered.
     *
     * \param name
     *     A name identifying this shader
     *
     * \param vertex_shader
     *     The source of the vertex shader as a string.
     *
     * \param fragment_shader
     *     The source of the fragment shader as a string.
     */
    Shader(RenderPass *render_pass,
           const std::string &name,
           const std::string &vertex_shader,
           const std::string &fragment_shader,
           BlendMode blend_mode = BlendMode::None);

    /// Return the render pass associated with this shader
    RenderPass *render_pass() { return m_render_pass; }

    /// Return the name of this shader
    const std::string &name() const { return m_name; }

    /// Return the blending mode of this shader
    BlendMode blend_mode() const { return m_blend_mode; }

    /**
     * \brief Upload a buffer (e.g. vertex positions) that will be associated
     * with a named shader parameter.
     *
     * Note that this function should be used both for 'varying' and 'uniform'
     * data---the implementation takes care of routing the data to the right
     * endpoint. Matrices should be specified in column-major order.
     *
     * The buffer will be replaced if it is already present.
     */
    void set_buffer(const std::string &name,
                    enoki::EnokiType type,
                    size_t ndim,
                    std::array<size_t, 3> shape,
                    const void *data);

    /**
     * \brief Upload a uniform variable (e.g. a vector or matrix) that will be
     * associated with a named shader parameter.
     */
    template <typename Array> void set_uniform(const std::string &name,
                                               const Array &value) {
        std::array<size_t, 3> shape = { 1, 1, 1 };
        size_t ndim;
        const void *data;

        if constexpr (enoki::array_depth_v<Array> == 0) {
            data = &value;
            ndim = 0;
        } else if constexpr (enoki::array_depth_v<Array> == 1) {
            data = value.data();
            shape[0] = value.size();
            ndim = 1;
        } else if constexpr (enoki::array_depth_v<Array> == 2) {
            data = value.data();
            shape[0] = value.size();
            shape[1] = value[0].size();
            ndim = 2;
        } else if constexpr (enoki::array_depth_v<Array> == 3) {
            data = value.data();
            shape[0] = value.size();
            shape[1] = value[0].size();
            shape[2] = value[0][0].size();
            ndim = 3;
        } else {
            throw std::runtime_error("Shader::set_uniform(): invalid input array dimension!");
        }
        set_buffer(name, enoki::enoki_type_v<enoki::scalar_t<Array>>, ndim, shape, data);
    }

    /**
     * \brief Associate a texture with a named shader parameter
     *
     * The association will be replaced if it is already present.
     */
    void set_texture(const std::string &name, Texture *texture);

    /**
     * \brief Begin drawing using this shader
     *
     * Note that any updates to 'uniform' and 'varying' shader parameters
     * *must* occur prior to this method call.
     *
     * The Python bindings also include extra \c __enter__ and \c __exit__
     * aliases so that the shader can be activated via Pythons 'with'
     * statement.
     */
    void begin();

    /// End drawing using this shader
    void end();

    /**
     * \brief Render geometry arrays, either directly or
     * using an index array.
     *
     * \param primitive_type
     *     What type of geometry should be rendered?
     *
     * \param offset
     *     First index to render. Must be a multiple of 2 or 3 for lines and
     *     triangles, respectively (unless specified using strips).
     *
     * \param offset
     *     Number of indices to render. Must be a multiple of 2 or 3 for lines
     *     and triangles, respectively (unless specified using strips).
     *
     * \param indexed
     *     Render indexed geometry? In this case, an
     *     \c uint32_t valued buffer with name \c indices
     *     must have been uploaded using \ref set().
     */
    void draw_array(PrimitiveType primitive_type,
                    size_t offset, size_t count,
                    bool indexed = false);

#if defined(NANOGUI_USE_OPENGL) || defined(NANOGUI_USE_GLES)
    uint32_t shader_handle() const { return m_shader_handle; }
#elif defined(NANOGUI_USE_METAL)
    void *pipeline_state() const { return m_pipeline_state; }
#endif

#if defined(NANOGUI_USE_OPENGL)
    uint32_t vertex_array_handle() const { return m_vertex_array_handle; }
#endif

protected:
    enum BufferType {
        Unknown = 0,
        VertexBuffer,
        VertexTexture,
        VertexSampler,
        FragmentBuffer,
        FragmentTexture,
        FragmentSampler,
        UniformBuffer,
        IndexBuffer,
    };

    struct Buffer {
        void *buffer = nullptr;
        BufferType type = Unknown;
        enoki::EnokiType dtype = enoki::EnokiType::Invalid;
        int index = 0;
        size_t ndim = 0;
        std::array<size_t, 3> shape { 0, 0, 0 };
        size_t size = 0;
        bool dirty = false;

        std::string to_string() const;
    };

    /// Release all resources
    virtual ~Shader();

protected:
    RenderPass* m_render_pass;
    std::string m_name;
    std::unordered_map<std::string, Buffer> m_buffers;
    BlendMode m_blend_mode;

    #if defined(NANOGUI_USE_OPENGL) || defined(NANOGUI_USE_GLES)
        uint32_t m_shader_handle = 0;
    #  if defined(NANOGUI_USE_OPENGL)
        uint32_t m_vertex_array_handle = 0;
        bool m_uses_point_size = false;
    #  endif
    #elif defined(NANOGUI_USE_METAL)
        void *m_pipeline_state;
    #endif
};

/// Return the size in bytes associated with a specific Enoki type
extern NANOGUI_EXPORT size_t enoki_type_size(enoki::EnokiType type);

/// Return the name (e.g. "uint8") associated with a specific Enoki type
extern NANOGUI_EXPORT const char *enoki_type_name(enoki::EnokiType type);

/// Access binary data stored in nanogui_resources.cpp
#define NANOGUI_RESOURCE_STRING(name) std::string(name, name + name##_size)

/// Access a shader stored in nanogui_resources.cpp
#if defined(NANOGUI_USE_OPENGL)
#  define NANOGUI_SHADER(name) NANOGUI_RESOURCE_STRING(name##_gl)
#elif defined(NANOGUI_USE_GLES)
#  define NANOGUI_SHADER(name) NANOGUI_RESOURCE_STRING(name##_gles)
#elif defined(NANOGUI_USE_METAL)
#  define NANOGUI_SHADER(name) NANOGUI_RESOURCE_STRING(name##_metallib)
#endif


NAMESPACE_END(nanogui)
