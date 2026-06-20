#include <nanogui/fence.h>
#import <Metal/Metal.h>

NAMESPACE_BEGIN(nanogui)

Fence::Fence() { }

Fence::~Fence() {
    if (m_command_buffer)
        (void) (__bridge_transfer id<MTLCommandBuffer>) m_command_buffer;
}

void Fence::signal(void *command_buffer) {
    if (m_command_buffer)
        (void) (__bridge_transfer id<MTLCommandBuffer>) m_command_buffer;
    id<MTLCommandBuffer> cb = (__bridge id<MTLCommandBuffer>) command_buffer;
    m_command_buffer = (__bridge_retained void *) cb;
}

bool Fence::completed() const {
    if (!m_command_buffer)
        return true;
    id<MTLCommandBuffer> cb = (__bridge id<MTLCommandBuffer>) m_command_buffer;
    // 'Error' (> 'Completed') also means the GPU is done with this buffer.
    return cb.status >= MTLCommandBufferStatusCompleted;
}

void Fence::wait() {
    if (!m_command_buffer)
        return;
    id<MTLCommandBuffer> cb = (__bridge id<MTLCommandBuffer>) m_command_buffer;
    [cb waitUntilCompleted];
}

NAMESPACE_END(nanogui)
