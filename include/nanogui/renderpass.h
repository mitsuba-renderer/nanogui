/*
    NanoGUI was developed by Wenzel Jakob <wenzel.jakob@epfl.ch>.
    The widget drawing code is based on the NanoVG demo application
    by Mikko Mononen.

    All rights reserved. Use of this source code is governed by a
    BSD-style license that can be found in the LICENSE.txt file.
*/

/**
 * \file nanogui/renderpass.h
 *
 * \brief Defines abstractions for rendering passes that work with OpenGL,
 * OpenGL ES, and Metal.
 */

#pragma once

#include <nanogui/object.h>
#include <unordered_map>

NAMESPACE_BEGIN(nanogui)

class NANOGUI_EXPORT RenderPass : public Object {
public:
    /// Depth test
    enum class DepthTest {
        Never,
        Less,
        Equal,
        LessEqual,
        Greater,
        NotEqual,
        GreaterEqual,
        Always
    };

    /// Culling mode
    enum class CullMode {
        Disabled,
        Front,
        Back
    };

    /**
     * \brief Create a new render pass for rendering to a specific
     * set of targets
     *
     * \param color_target
     *     One or more target objects to which color information will be
     *     rendered. Must either be a \ref Screen or a \ref Texture instance.
     *
     * \param depth_target
     *     Target object to which depth information will be rendered.
     *     Must either be \c NULL or a \ref Texture instance.
     *
     * \param stencil_target
     *     Target object to which stencil information will be rendered.
     *     Must either be \c NULL or a \ref Texture instance.
     *     Can be identical to 'depth_target' in case the texture has
     *     the pixel format \ref Texture::PixelFormat::DepthStencil.
     *
     * \param clear
     *     Should \ref enter() begin by clearing all buffers?
     *
     * \param clear_color
     *     Clear color for each color target (defaults to black
     *     if none are specified)
     *
     * \param clear_depth
     *     Clear value for the depth target (if applicable)
     *
     * \param clear_stencil
     *     Clear value for the stencil target (if applicable)
     */
    RenderPass(std::vector<Object *> color_targets,
               Object *depth_target = nullptr,
               Object *stencil_target = nullptr,
               bool clear = true,
               std::vector<Color> clear_color = {},
               float clear_depth = 1.f,
               uint8_t clear_stencil = 0);

    /**
     * \brief Begin the render pass
     *
     * The specified drawing state (e.g. depth tests, culling mode, blending
     * mode) are automatically set up at this point. Later changes between
     * \ref begin() and \ref end() are possible but cause additional
     * OpenGL/GLES/Metal API calls.
     *
     * The Python bindings also include extra \c __enter__ and \c __exit__
     * aliases so that the render pass can be activated via Pythons 'with'
     * statement.
     */
    void begin();

    /// Finish the render pass
    void end();

    /// Specify the depth test and depth write mask of this render pass
    void set_depth_test(DepthTest depth_test, bool depth_write);

    /// Return the depth test and depth write mask of this render pass
    std::pair<DepthTest, bool> depth_test() const { return { m_depth_test, m_depth_write }; }

    /// Set the pixel offset and size of the viewport region
    void set_viewport(const Vector2i &offset, const Vector2i &size);

    /// Return the pixel offset and size of the viewport region
    std::pair<Vector2i, Vector2i> viewport() {
        return { m_viewport_offset, m_viewport_size };
    }

    /// Specify the culling mode associated with the render pass
    void set_cull_mode(CullMode mode);

    /// Return the culling mode associated with the render pass
    CullMode cull_mode() const { return m_cull_mode; }

    /**
     * \brief Return the set of all render targets (including depth + stencil)
     * associated with this render pass
     */
    std::vector<ref<Object>> &targets() { return m_targets; }

    /// Resize all texture targets attached to the render pass
    void resize(const Vector2i &size);

#if defined(NANOGUI_USE_OPENGL) || defined(NANOGUI_USE_GLES2)
    uint32_t framebuffer_handle() const { return m_framebuffer_handle; }
#elif defined(NANOGUI_USE_METAL)
    void *command_encoder() const { return m_command_encoder; }
    void *command_buffer() const { return m_command_buffer; }
#endif

protected:
    virtual ~RenderPass();

protected:
    std::vector<ref<Object>> m_targets;
    bool m_clear;
    std::vector<Color> m_clear_color;
    float m_clear_depth;
    uint8_t m_clear_stencil;
    Vector2i m_viewport_offset;
    Vector2i m_viewport_size;
    DepthTest m_depth_test;
    bool m_depth_write;
    CullMode m_cull_mode;
    bool m_active;
#if defined(NANOGUI_USE_OPENGL) || defined(NANOGUI_USE_GLES2)
    uint32_t m_framebuffer_handle;
    Vector2i m_framebuffer_size;
#elif defined(NANOGUI_USE_METAL)
    void *m_command_buffer;
    void *m_command_encoder;
    void *m_pass_descriptor;
#endif
};

NAMESPACE_END(nanogui)
