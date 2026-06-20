/*
    NanoGUI was developed by Wenzel Jakob <wenzel.jakob@epfl.ch>.
    The widget drawing code is based on the NanoVG demo application
    by Mikko Mononen.

    All rights reserved. Use of this source code is governed by a
    BSD-style license that can be found in the LICENSE.txt file.
*/

#pragma once

#include <nanogui/object.h>

NAMESPACE_BEGIN(nanogui)

/**
 * \brief A passive, render-pass-scoped GPU-completion token.
 *
 * Inserted into an active \ref RenderPass via \ref RenderPass::insert_fence(),
 * it signals once that pass's GPU work (notably its texture samples) has
 * retired. It is the read side of a producer/consumer texture handoff: a
 * producer may overwrite a texture only after the fence guarding the consumer's
 * last read of it completes.
 *
 * A fence tracks only its most recent signal (a ``GLsync`` on OpenGL, the
 * committed ``id<MTLCommandBuffer>`` on Metal), so re-inserting it on a later
 * pass simply supersedes the previous token. All methods must be called on the
 * render (GL/Metal) thread.
 *
 * OpenGL ES 2 lacks required functionality and implements the fence as a no-op:
 * \ref completed() is always true and \ref wait() returns immediately.
 */
class NANOGUI_EXPORT Fence : public Object {
public:
    Fence();

    /// Non-blocking poll: has the most recent signal completed?
    bool completed() const;

    /// Host-side blocking wait until the most recent signal completes
    void wait();

    virtual ~Fence();

protected:
    friend class RenderPass;

    /// Called by RenderPass::insert_fence(): record a GLsync, or retain
    /// \c command_buffer (an id<MTLCommandBuffer>) for completion tracking.
    void signal(void *command_buffer);

protected:
#if defined(NANOGUI_USE_METAL)
    void *m_command_buffer = nullptr; // most recent committed buffer (retained)
#elif defined(NANOGUI_USE_OPENGL) || defined(NANOGUI_USE_GLES)
    void *m_sync = nullptr; // most recent GLsync
#endif
};

NAMESPACE_END(nanogui)
