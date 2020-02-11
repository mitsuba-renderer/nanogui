/*
    NanoGUI was developed by Wenzel Jakob <wenzel.jakob@epfl.ch>.
    The widget drawing code is based on the NanoVG demo application
    by Mikko Mononen.

    All rights reserved. Use of this source code is governed by a
    BSD-style license that can be found in the LICENSE.txt file.
*/
/**
 * \file nanogui/common.h
 *
 * \brief Common definitions used by NanoGUI.
 */

#pragma once

#include <nanogui/vector.h>

#if defined(NANOGUI_USE_METAL)
NAMESPACE_BEGIN(nanogui)

/// Initialize the Metal backend
extern NANOGUI_EXPORT void metal_init();

/// Shut down the Metal backend
extern NANOGUI_EXPORT void metal_shutdown();

/// Return a pointer to the underlying Metal device (id<MTLDevice>)
extern NANOGUI_EXPORT void *metal_device();

/// Return a pointer to the underlying Metal command queue (id<MTLCommandQueue>)
extern NANOGUI_EXPORT void *metal_command_queue();

/// Return a pointer to the underlying Metal command queue (CAMetalLayer *)
extern NANOGUI_EXPORT void *metal_layer(void *nswin);

/// Associate a metal layer with a NSWindow created by GLEW
extern NANOGUI_EXPORT void metal_window_init(void *nswin, bool float_buffer);

/// Set size of the drawable underlying an NSWindow
extern NANOGUI_EXPORT void metal_window_set_size(void *nswin, const Vector2i &size);

/// Return the CAMetalLayer associated with a given NSWindow
extern NANOGUI_EXPORT void *metal_window_layer(void *nswin);

/// Acquire the next id<MTLDrawable> from the Metal layer
extern NANOGUI_EXPORT void* metal_window_next_drawable(void *nswin);

/// Return the id<MTLTexture> associated with an id<MTLDrawable>
extern NANOGUI_EXPORT void *metal_drawable_texture(void *drawable);

/// Release a drawable back to the pool
extern NANOGUI_EXPORT void metal_present_and_release_drawable(void *drawable);

NAMESPACE_END(nanogui)
#endif
