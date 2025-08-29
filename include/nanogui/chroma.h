/*
    NanoGUI was developed by Wenzel Jakob <wenzel.jakob@epfl.ch>.
    The widget drawing code is based on the NanoVG demo application
    by Mikko Mononen.

    All rights reserved. Use of this source code is governed by a
    BSD-style license that can be found in the LICENSE.txt file.
*/
/**
 * \file nanogui/chroma.h
 *
 * \brief Color space conversion utilities.
 */

#pragma once

#include <nanogui/widget.h>

#include <array>
#include <string_view>

NAMESPACE_BEGIN(nanogui)

extern NANOGUI_EXPORT Matrix3f chroma_to_rec709_matrix(const std::array<Vector2f, 4>& chroma);

extern NANOGUI_EXPORT std::array<nanogui::Vector2f, 4> chroma_from_wp_primaries(int wp_primaries);
extern NANOGUI_EXPORT std::string_view wp_primaries_to_string(int wp_primaties);

// Partial implementation of https://www.itu.int/rec/T-REC-H.273-202407-I/en
NAMESPACE_BEGIN(ituth273)

enum class ColorPrimaries : uint8_t {
    BT709 = 1,
    Unspecified = 2,
    BT470M = 4,
    BT470BG = 5,
    SMPTE170M = 6,
    SMPTE240M = 7,
    Film = 8,
    BT2020 = 9, // Same as BT2100
    SMPTE428 = 10,
    SMPTE431 = 11,
    SMPTE432 = 12,
    Weird = 22, // The spec says "No corresponding industry specification identified"
};

extern NANOGUI_EXPORT std::string_view to_string(const ColorPrimaries primaries);
extern NANOGUI_EXPORT std::array<nanogui::Vector2f, 4> chroma(const ColorPrimaries primaries);

extern NANOGUI_EXPORT ColorPrimaries from_wp_primaries(int wp_primaries);

extern NANOGUI_EXPORT ColorPrimaries from_screen(const Screen *screen);

NAMESPACE_END(ituth273)

NAMESPACE_END(nanogui)
