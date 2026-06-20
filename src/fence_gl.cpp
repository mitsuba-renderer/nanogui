#include <nanogui/fence.h>
#include <nanogui/opengl.h>
#include "opengl_check.h"

NAMESPACE_BEGIN(nanogui)

#if defined(NANOGUI_USE_OPENGL) || (defined(NANOGUI_USE_GLES) && NANOGUI_GLES_VERSION >= 3)
#  define NANOGUI_GL_HAS_SYNC 1
#endif

Fence::Fence() { }

Fence::~Fence() {
#if defined(NANOGUI_GL_HAS_SYNC)
    if (m_sync)
        CHK(glDeleteSync((GLsync) m_sync));
#endif
}

void Fence::signal(void * /* command_buffer */) {
#if defined(NANOGUI_GL_HAS_SYNC)
    // GL is a single ordered stream, so this retires after the pass's draws.
    if (m_sync)
        CHK(glDeleteSync((GLsync) m_sync));
    m_sync = glFenceSync(GL_SYNC_GPU_COMMANDS_COMPLETE, 0);
#endif
}

bool Fence::completed() const {
#if defined(NANOGUI_GL_HAS_SYNC)
    if (!m_sync)
        return true;
    GLenum status = glClientWaitSync((GLsync) m_sync, 0, 0);
    return status == GL_ALREADY_SIGNALED || status == GL_CONDITION_SATISFIED;
#else
    return true;
#endif
}

void Fence::wait() {
#if defined(NANOGUI_GL_HAS_SYNC)
    if (!m_sync)
        return;
    while (true) {
        // GL_SYNC_FLUSH_COMMANDS_BIT flushes so we cannot deadlock on un-submitted work.
        GLenum status = glClientWaitSync((GLsync) m_sync, GL_SYNC_FLUSH_COMMANDS_BIT,
                                         1000000000ull /* 1s, then retry */);
        if (status == GL_ALREADY_SIGNALED || status == GL_CONDITION_SATISFIED ||
            status == GL_WAIT_FAILED)
            return;
    }
#endif
}

NAMESPACE_END(nanogui)
