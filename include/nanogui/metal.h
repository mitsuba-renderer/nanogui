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

#include <nanogui/common.h>

#if defined(NANOGUI_USE_METAL)
NAMESPACE_BEGIN(nanogui)

/// Initialize the Metal backend
void metal_init();

/// Shut down the Metal backend
void metal_shutdown();

/// Return the currently active MTLDevice
void *metal_device();

/// Associate a metal layer with a NSWindow created by GLEW
void metal_window_init(void *nswin, bool request_wide_gamut);

/// Set size of the drawable underlying an NSWindow
void metal_window_set_size(void *nswin, const Vector2i &size);

/// Return the CAMetalLayer associated with a given NSWindow
void *metal_window_layer(void *nswin);

NAMESPACE_END(nanogui)
#endif
