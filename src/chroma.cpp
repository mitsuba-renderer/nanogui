/*
    nanogui/chroma.cpp -- Color space conversion utilities.

    NanoGUI was developed by Wenzel Jakob <wenzel.jakob@epfl.ch>.
    The widget drawing code is based on the NanoVG demo application
    by Mikko Mononen.

    All rights reserved. Use of this source code is governed by a
    BSD-style license that can be found in the LICENSE.txt file.
*/

#include <nanogui/chroma.h>
#include <nanogui/vector.h>
#include <nanogui/screen.h>
#include <nanogui/opengl.h>

#include <array>
#include <stdexcept>

NAMESPACE_BEGIN(nanogui)

// This routine was copied from OpenEXR's ImfChromaticities.cpp in accordance
// with its BSD-3-Clause license. See the header of that file for details.
// https://github.com/AcademySoftwareFoundation/openexr/blob/main/src/lib/OpenEXR/ImfChromaticities.cpp
Matrix3f rgb_to_xyz(const std::array<Vector2f, 4>& chroma, float Y) {
    //
    // For an explanation of how the color conversion matrix is derived,
    // see Roy Hall, "Illumination and Color in Computer Generated Imagery",
    // Springer-Verlag, 1989, chapter 3, "Perceptual Response"; and
    // Charles A. Poynton, "A Technical Introduction to Digital Video",
    // John Wiley & Sons, 1996, chapter 7, "Color science for video".
    //

    //
    // X and Z values of RGB value (1, 1, 1), or "white"
    //

    const Vector2f& red = chroma[0];
    const Vector2f& green = chroma[1];
    const Vector2f& blue = chroma[2];
    const Vector2f& white = chroma[3];

    // prevent a division that rounds to zero
    if (std::abs (white.y()) <= 1.f &&
        std::abs (white.x() * Y) >= std::abs (white.y()) * std::numeric_limits<float>::max())
    {
        throw std::invalid_argument (
            "Bad chromaticities: white.y cannot be zero");
    }

    float X = white.x() * Y / white.y();
    float Z = (1 - white.x() - white.y()) * Y / white.y();

    //
    // Scale factors for matrix rows, compute numerators and common denominator
    //

    float d = red.x() * (blue.y() - green.y()) +
              blue.x() * (green.y() - red.y()) +
              green.x() * (red.y() - blue.y());

    float SrN =
        (X * (blue.y() - green.y()) -
         green.x() * (Y * (blue.y() - 1) + blue.y() * (X + Z)) +
         blue.x() * (Y * (green.y() - 1) + green.y() * (X + Z)));

    float SgN =
        (X * (red.y() - blue.y()) +
         red.x() * (Y * (blue.y() - 1) + blue.y() * (X + Z)) -
         blue.x() * (Y * (red.y() - 1) + red.y() * (X + Z)));

    float SbN =
        (X * (green.y() - red.y()) -
         red.x() * (Y * (green.y() - 1) + green.y() * (X + Z)) +
         green.x() * (Y * (red.y() - 1) + red.y() * (X + Z)));

    if (std::abs (d) < 1.f && (std::abs (SrN) >= std::abs (d) * std::numeric_limits<float>::max() ||
                               std::abs (SgN) >= std::abs (d) * std::numeric_limits<float>::max() ||
                               std::abs (SbN) >= std::abs (d) * std::numeric_limits<float>::max()))
    {
        // cannot generate matrix if all RGB primaries have the same y value
        // or if they all have the an x value of zero
        // in both cases, the primaries are colinear, which makes them unusable
        throw std::invalid_argument (
            "Bad chromaticities: RGBtoXYZ matrix is degenerate");
    }

    float Sr = SrN / d;
    float Sg = SgN / d;
    float Sb = SbN / d;

    //
    // Assemble the matrix
    //

    Matrix3f M;

    M.m[0][0] = Sr * red.x();
    M.m[0][1] = Sr * red.y();
    M.m[0][2] = Sr * (1 - red.x() - red.y());

    M.m[1][0] = Sg * green.x();
    M.m[1][1] = Sg * green.y();
    M.m[1][2] = Sg * (1 - green.x() - green.y());

    M.m[2][0] = Sb * blue.x();
    M.m[2][1] = Sb * blue.y();
    M.m[2][2] = Sb * (1 - blue.x() - blue.y());

    return M;
}

Matrix3f xyz_to_rgb(const std::array<Vector2f, 4>& chroma, float Y) {
    return inverse(rgb_to_xyz(chroma, Y));
}

Vector2f white_d65() { return {0.31271f, 0.32902f}; }
Vector2f white_center() { return {0.333333f, 0.333333f}; }
Vector2f white_c() { return {0.310f, 0.316f}; }
Vector2f white_dci() { return {0.314f, 0.351f}; }

std::array<Vector2f, 4> rec709_chroma() {
    return {
        {
         {0.6400f, 0.3300f},
         {0.3000f, 0.6000f},
         {0.1500f, 0.0600f},
         white_d65(),
         }
    };
}

std::array<Vector2f, 4> adobe_chroma() {
    return {
        {
         {0.6400f, 0.3300f},
         {0.2100f, 0.7100f},
         {0.1500f, 0.0600f},
         white_d65(),
         }
    };
}

std::array<Vector2f, 4> pro_photo_chroma() {
    return {
        {
         {0.734699f, 0.265301f},
         {0.159597f, 0.840403f},
         {0.036598f, 0.000105f},
         {0.345704f, 0.358540f},
         }
    };
}

std::array<Vector2f, 4> display_p3_chroma() {
    return {
        {
         {0.6800f, 0.3200f},
         {0.2650f, 0.6900f},
         {0.1500f, 0.0600f},
         white_d65(),
         }
    };
}

std::array<Vector2f, 4> dci_p3_chroma() {
    return {
        {
         {0.6800f, 0.3200f},
         {0.2650f, 0.6900f},
         {0.1500f, 0.0600f},
         white_dci(),
         }
    };
}

std::array<Vector2f, 4> bt2020_chroma() {
    return {
        {
         {0.7080f, 0.2920f},
         {0.1700f, 0.7970f},
         {0.1310f, 0.0460f},
         white_d65(),
         }
    };
}

std::array<Vector2f, 4> bt2100_chroma() {
    return bt2020_chroma(); // BT.2100 uses the same primaries as BT.2020
}

Matrix3f chroma_to_rec709_matrix(const std::array<Vector2f, 4>& chroma) {
    return xyz_to_rgb(rec709_chroma(), 1) * rgb_to_xyz(chroma, 1);
}

std::array<Vector2f, 4> chroma_from_wp_primaries(int wp_primaries) {
    if (wp_primaries == 10) {
        // Special case for Adobe RGB (1998) primaries, which is not in the H.273 spec
        return adobe_chroma();
    }

    return ituth273::chroma(ituth273::from_wp_primaries(wp_primaries));
}

std::string_view wp_primaries_to_string(int wp_primaries) {
    if (wp_primaries == 10) {
        // Special case for Adobe RGB (1998) primaries, which is not in the H.273 spec
        return "adobe_rgb";
    }

    return ituth273::to_string(ituth273::from_wp_primaries(wp_primaries));
}

// Partial implementation of https://www.itu.int/rec/T-REC-H.273-202407-I/en
NAMESPACE_BEGIN(ituth273)

std::string_view to_string(const ColorPrimaries primaries) {
    switch (primaries) {
        case ColorPrimaries::BT709: return "bt709";
        case ColorPrimaries::Unspecified: return "unspecified";
        case ColorPrimaries::BT470M: return "bt470m";
        case ColorPrimaries::BT470BG: return "bt470bg";
        case ColorPrimaries::SMPTE170M: return "smpte170m";
        case ColorPrimaries::SMPTE240M: return "smpte240m";
        case ColorPrimaries::Film: return "film";
        case ColorPrimaries::BT2020: return "bt2020";
        case ColorPrimaries::SMPTE428: return "smpte428";
        case ColorPrimaries::SMPTE431: return "smpte431";
        case ColorPrimaries::SMPTE432: return "smpte432";
        case ColorPrimaries::Weird: return "weird";
    }

    return "invalid";
}

std::array<Vector2f, 4> chroma(const ColorPrimaries primaries) {
    switch (primaries) {
        default: fprintf(stderr, "Unknown color primaries %d. Using Rec.709 chroma.", (int)primaries); return rec709_chroma();
        case ColorPrimaries::BT709: return rec709_chroma();
        case ColorPrimaries::Unspecified: fprintf(stderr, "Unspecified color primaries. Using Rec.709 chroma."); return rec709_chroma();
        case ColorPrimaries::BT470M:
            return {
                {
                 {0.6700f, 0.3300f},
                 {0.2100f, 0.7100f},
                 {0.1400f, 0.0800f},
                 white_c(),
                 }
            };
        case ColorPrimaries::BT470BG:
            return {
                {
                 {0.6400f, 0.3300f},
                 {0.2900f, 0.6000f},
                 {0.1500f, 0.0600f},
                 white_d65(),
                 }
            };
        case ColorPrimaries::SMPTE170M:
        case ColorPrimaries::SMPTE240M:
            return {
                {
                 {0.6300f, 0.3400f},
                 {0.3100f, 0.5950f},
                 {0.1550f, 0.0700f},
                 white_d65(),
                 }
            };
        case ColorPrimaries::Film:
            return {
                {
                 {0.6810f, 0.3190f}, // Wratten 25
                    {0.2430f, 0.6920f}, // Wratten 58
                    {0.1450f, 0.0490f}, // Wratten 47
                    white_c(),
                 }
            };
        case ColorPrimaries::BT2020: return bt2020_chroma();
        case ColorPrimaries::SMPTE428:
            return {
                {
                 {1.0f, 0.0f},
                 {0.0f, 1.0f},
                 {0.0f, 0.0f},
                 white_center(),
                 }
            };
        case ColorPrimaries::SMPTE431: return dci_p3_chroma();
        case ColorPrimaries::SMPTE432: return display_p3_chroma();
        case ColorPrimaries::Weird:
            return {
                {
                 {0.6300f, 0.3400f},
                 {0.2950f, 0.6050f},
                 {0.1550f, 0.0770f},
                 white_d65(),
                 }
            };
    }

    return rec709_chroma(); // Fallback to Rec.709 if unknown
}

ColorPrimaries from_wp_primaries(int wp_primaries) {
    switch (wp_primaries) {
        case 1: return ituth273::ColorPrimaries::BT709;
        case 2: return ituth273::ColorPrimaries::BT470M;
        case 3: return ituth273::ColorPrimaries::BT470BG;
        case 4: return ituth273::ColorPrimaries::SMPTE170M;
        case 5: return ituth273::ColorPrimaries::Film;
        case 6: return ituth273::ColorPrimaries::BT2020;
        case 7: return ituth273::ColorPrimaries::SMPTE428;
        case 8: return ituth273::ColorPrimaries::SMPTE431;
        case 9: return ituth273::ColorPrimaries::SMPTE432;
    }

    throw std::invalid_argument{"Unknown wp color primaries: " + std::to_string(wp_primaries)};
}

ColorPrimaries from_screen(const Screen *screen) {
    return from_wp_primaries(glfwGetWindowPrimaries(screen->glfw_window()));
}

NAMESPACE_END(ituth273)

NAMESPACE_END(nanogui)
