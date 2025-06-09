/*
    NanoGUI was developed by Wenzel Jakob <wenzel.jakob@epfl.ch>.
    The widget drawing code is based on the NanoVG demo application
    by Mikko Mononen.

    All rights reserved. Use of this source code is governed by a
    BSD-style license that can be found in the LICENSE.txt file.
*/
/**
 * \file nanogui/borderlayout.h
 *
 * \brief Defines the \ref nanogui::BorderLayout algorithm.
 */
#pragma once

#include <nanogui/layout.h>

NAMESPACE_BEGIN(nanogui)

/**
 * \class BorderLayout borderlayout.h nanogui/borderlayout.h
 *
 * \brief Layout inspired by Java's BorderLayout
 *
 * Each widget will "carve" out a piece from the border of the parent.
 * The center widget(s) will get the remaining area.
 *
 * Each child widget will be processed in order. Unlike in the Java version,
 * it's allowed to have multiple widgets on the same side, they will be
 * placed next to each other.
 *
 * It's also allowed to have multiple center widgets, they will be overlapped though.
 *
 * The layout needs to know the direction of the children, this is kept in a local
 * hash map. After adding a child widget, the user also needs to call "set_side"
 * to associate one of the borders/sides to the given widget. If no side is associated,
 * it will be assumed to be center.
 */
class NANOGUI_EXPORT BorderLayout : public Layout
{
public:
    /// The possible border/side positions of the children
    enum Side {
        North,      ///< Place the child in the top of the available area
        South,      ///< Place the child in the bottom of the available area
        West,       ///< Place the child in the left of the available area
        East,       ///< Place the child in the right of the available area
        Center,     ///< Place the child in the available area
    };

    /**
     * \brief Creates an instance of the layout
     *
     * \param margin
     *     Specifies the empty gap reserved at the edge of the parent widget
     *
     * \param spacing
     *     Specifies the empty gap reserved in between the children
     */
    explicit BorderLayout(int margin = 0, int spacing = 0);

    /// Associates a side with a child widget
    void set_side(const Widget *widget, Side side);
    /// Returns the side associated to the child widget
    Side side(const Widget *widget) const;

    Vector2i preferred_size(NVGcontext *ctx, const Widget *widget) const override;
    void perform_layout(NVGcontext *ctx, Widget *widget) const override;

private:
    std::unordered_map<const Widget *, Side> m_side;
    int m_margin;
    int m_spacing;
};

NAMESPACE_END(nanogui)
