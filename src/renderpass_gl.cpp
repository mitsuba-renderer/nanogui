#include <nanogui/renderpass.h>
#include <nanogui/screen.h>
#include <nanogui/opengl.h>
#include <nanogui/texture.h>
#include "opengl_check.h"

NAMESPACE_BEGIN(nanogui)

RenderPass::RenderPass(std::vector<Object *> color_targets,
                       Object *depth_target,
                       Object *stencil_target,
                       bool clear,
                       std::vector<Color> clear_color,
                       float clear_depth,
                       uint8_t clear_stencil,
                       Object *blit_target)
    : m_targets(color_targets.size() + 2), m_clear(clear),
      m_clear_color(clear_color), m_clear_depth(clear_depth),
      m_clear_stencil(clear_stencil), m_viewport_offset(0),
      m_viewport_size(0), m_depth_test(DepthTest::Less),
      m_depth_write(true), m_cull_mode(CullMode::Back),
      m_blit_target(blit_target), m_active(false),
      m_framebuffer_handle(0) {

    m_targets[0] = depth_target;
    m_targets[1] = stencil_target;
    for (size_t i = 0; i < color_targets.size(); ++i)
        m_targets[i + 2] = color_targets[i];

    if (!clear_color.empty() && clear_color.size() != color_targets.size())
        throw std::runtime_error("RenderPass::RenderPass(): 'clear_color' must either "
                                 "be empty or match 'color_targets' in size.");

    if (!m_targets[0].get()) {
        m_depth_write = false;
        m_depth_test = DepthTest::Always;
    }

    CHK(glGenFramebuffers(1, &m_framebuffer_handle));
    CHK(glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer_handle));

    std::vector<GLenum> draw_buffers;

    bool has_texture = false,
         has_screen  = false;

    for (size_t i = 0; i < m_targets.size(); ++i) {
        GLenum attachment_id;
        if (i == 0)
            attachment_id = GL_DEPTH_ATTACHMENT;
        else if (i == 1)
            attachment_id = GL_STENCIL_ATTACHMENT;
        else
            attachment_id = GL_COLOR_ATTACHMENT0 + i - 2;

        Screen *screen = dynamic_cast<Screen *>(m_targets[i].get());
        Texture *texture = dynamic_cast<Texture *>(m_targets[i].get());
        if (screen) {
            m_viewport_size = max(m_viewport_size, screen->framebuffer_size());
            if (i >= 2)
                draw_buffers.push_back(GL_BACK_LEFT);
            has_screen = true;
        } else if (texture) {
            if (texture->flags() & Texture::TextureFlags::ShaderRead) {
                CHK(glFramebufferTexture(GL_FRAMEBUFFER, attachment_id, texture->texture_handle(), 0));
            } else {
                CHK(glFramebufferRenderbuffer(GL_FRAMEBUFFER, attachment_id, GL_RENDERBUFFER,
                                              texture->renderbuffer_handle()));
            }
            if (i >= 2)
                draw_buffers.push_back(attachment_id);
            m_viewport_size = max(m_viewport_size, texture->size());
            has_texture = true;
        }
    }

    if (has_screen && !has_texture) {
        CHK(glDeleteFramebuffers(1, &m_framebuffer_handle));
        m_framebuffer_handle = 0;
    } else {
        CHK(glDrawBuffers((GLsizei) draw_buffers.size(), draw_buffers.data()));

        GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
        if (status != GL_FRAMEBUFFER_COMPLETE) {
            const char *reason = "unknown";
            switch (status) {
                case GL_FRAMEBUFFER_UNDEFINED:
                    reason = "undefined";
                    break;

                case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
                    reason = "incomplete attachment";
                    break;

                case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
                    reason = "incomplete, missing attachment";
                    break;

                case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
                    reason = "incomplete draw buffer";
                    break;

                case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
                    reason = "incomplete read buffer";
                    break;

                case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE:
                    reason = "incomplete multisample";
                    break;

                case GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS:
                    reason = "incomplete layer targets";
                    break;

                case GL_FRAMEBUFFER_UNSUPPORTED:
                    reason = "unsupported";
                    break;
            }
            throw std::runtime_error(
                "RenderPass::RenderPass(): framebuffer is marked as incomplete: " +
                std::string(reason));
        }
    }

    CHK(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}

RenderPass::~RenderPass() {
    CHK(glDeleteFramebuffers(1, &m_framebuffer_handle));
}

void RenderPass::begin() {
#if !defined(NDEBUG)
    if (m_active)
        throw std::runtime_error("RenderPass::begin(): render pass is already active!");
#endif
    m_active = true;

    m_framebuffer_size = 0;
    for (size_t i = 0; i < m_targets.size(); ++i) {
        Screen *screen = dynamic_cast<Screen *>(m_targets[i].get());
        Texture *texture = dynamic_cast<Texture *>(m_targets[i].get());

        if (screen)
            m_framebuffer_size = max(m_framebuffer_size, screen->framebuffer_size());
        else if (texture)
            m_framebuffer_size = max(m_framebuffer_size, texture->size());
    }

    CHK(glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer_handle));
    set_viewport(m_viewport_offset, m_viewport_size);

    if (!m_clear)
        return;
    for (size_t i = 0; i < m_targets.size(); ++i) {
        if (i == 0 && m_targets[0]) {
            if (m_targets[0] == m_targets[1])
                CHK(glClearBufferfi(GL_DEPTH_STENCIL, 0, m_clear_depth, m_clear_stencil));
            else {
                CHK(glClearBufferfv(GL_DEPTH, 0, &m_clear_depth));
            }
        }

        if (i >= 2) {
            float zero[4] = { 0.0f, 0.0f, 0.0f, 1.0f },
                 *ptr = zero;
            if (!m_clear_color.empty())
                ptr = m_clear_color[i].data();
            CHK(glClearBufferfv(GL_COLOR, i - 2, ptr));
        }
    }

    set_depth_test(m_depth_test, m_depth_write);
    set_cull_mode(m_cull_mode);
}

void RenderPass::end() {
#if !defined(NDEBUG)
    if (!m_active)
        throw std::runtime_error("RenderPass::end(): render pass is not active!");
#endif

    if (m_blit_target) {
        Screen *screen   = dynamic_cast<Screen *>(m_blit_target.get());
        RenderPass *rp = dynamic_cast<RenderPass *>(m_blit_target.get());

        GLuint target_id;
        GLenum what = 0;

        if (screen) {
            target_id = 0;
            what = GL_COLOR_BUFFER_BIT;
            if (screen->has_depth_buffer())
                what |= GL_STENCIL_BUFFER_BIT;
            if (screen->has_stencil_buffer())
                what |= GL_STENCIL_BUFFER_BIT;
        } else if (rp) {
            target_id = rp->framebuffer_handle();
            if (rp->targets().size() > 0 && rp->targets()[0])
                what |= GL_DEPTH_BUFFER_BIT;
            if (rp->targets().size() > 1 && rp->targets()[1])
                what |= GL_STENCIL_BUFFER_BIT;
            if (rp->targets().size() > 2 && rp->targets()[2])
                what |= GL_COLOR_BUFFER_BIT;
        } else {
            throw std::runtime_error(
                "RenderPass::end(): 'blit_target' must either be a RenderPass or a Screen instance.");
        }

        CHK(glBindFramebuffer(GL_READ_FRAMEBUFFER, m_framebuffer_handle));
        CHK(glBindFramebuffer(GL_DRAW_FRAMEBUFFER, target_id));

        if (target_id == 0)
            CHK(glDrawBuffer(GL_BACK));

        CHK(glBlitFramebuffer(0, 0, (GLsizei) m_framebuffer_size.x(), (GLsizei) m_framebuffer_size.y(),
                              0, 0, (GLsizei) m_framebuffer_size.x(), (GLsizei) m_framebuffer_size.y(),
                              what, GL_NEAREST));
    }
    CHK(glBindFramebuffer(GL_FRAMEBUFFER, 0));
    m_active = false;
}

void RenderPass::resize(const Vector2i &size) {
    for (size_t i = 0; i < m_targets.size(); ++i) {
        Texture *texture = dynamic_cast<Texture *>(m_targets[i].get());
        if (texture)
            texture->resize(size);
    }
}

void RenderPass::set_viewport(const Vector2i &offset, const Vector2i &size) {
    m_viewport_offset = offset;
    m_viewport_size = size;

    if (m_active) {
        int ypos = m_framebuffer_size.y() - m_viewport_size.y() - m_viewport_offset.y();
        CHK(glViewport(m_viewport_offset.x(), ypos,
                       m_viewport_size.x(), m_viewport_size.y()));
    }
}

void RenderPass::set_depth_test(DepthTest depth_test, bool depth_write) {
    m_depth_test = depth_test;
    m_depth_write = depth_write;

    if (m_active) {
        if (m_targets[0] && depth_test != DepthTest::Always) {
            GLenum func;
            switch (depth_test) {
                case DepthTest::Never:        func = GL_NEVER;    break;
                case DepthTest::Less:         func = GL_LESS;     break;
                case DepthTest::Equal:        func = GL_EQUAL;    break;
                case DepthTest::LessEqual:    func = GL_LEQUAL;   break;
                case DepthTest::Greater:      func = GL_GREATER;  break;
                case DepthTest::NotEqual:     func = GL_NOTEQUAL; break;
                case DepthTest::GreaterEqual: func = GL_GEQUAL;   break;
                default:
                    throw std::runtime_error("Shader::set_depth_test(): invalid depth test mode!");
            }
            CHK(glEnable(GL_DEPTH_TEST));
            CHK(glDepthFunc(func));
        } else {
            CHK(glDisable(GL_DEPTH_TEST));
        }
        CHK(glDepthMask(depth_write ? GL_TRUE : GL_FALSE));
    }
}

void RenderPass::set_cull_mode(CullMode cull_mode) {
    m_cull_mode = cull_mode;

    if (m_active) {
        if (cull_mode == CullMode::Disabled) {
            CHK(glDisable(GL_CULL_FACE));
        } else {
            CHK(glEnable(GL_CULL_FACE));
            if (cull_mode == CullMode::Front)
                CHK(glCullFace(GL_FRONT));
            else if (cull_mode == CullMode::Back)
                CHK(glCullFace(GL_BACK));
            else
                throw std::runtime_error("Shader::set_cull_mode(): invalid cull mode!");
        }
    }
}

NAMESPACE_END(nanogui)
