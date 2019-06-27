/*
    nanogui/popup.h -- Simple popup widget which is attached to another given
    window (can be nested)

    NanoGUI was developed by Wenzel Jakob <wenzel.jakob@epfl.ch>.
    The widget drawing code is based on the NanoVG demo application
    by Mikko Mononen.

    All rights reserved. Use of this source code is governed by a
    BSD-style license that can be found in the LICENSE.txt file.
*/
/** \file */

#pragma once

#include <nanogui/window.h>

NAMESPACE_BEGIN(nanogui)

/**
 * \class Popup popup.h nanogui/popup.h
 *
 * \brief Popup window for combo boxes, popup buttons, nested dialogs etc.
 *
 * Usually the Popup instance is constructed by another widget (e.g. \ref PopupButton)
 * and does not need to be created by hand.
 */
class NANOGUI_EXPORT Popup : public Window {
public:
    enum Side { Left = 0, Right };

    /// Create a new popup parented to a screen (first argument) and a parent window (if applicable)
    Popup(Widget *parent, Window *parent_window = nullptr);

    /// Return the anchor position in the parent window; the placement of the popup is relative to it
    void set_anchor_pos(const Vector2i &anchor_pos) { m_anchor_pos = anchor_pos; }
    /// Set the anchor position in the parent window; the placement of the popup is relative to it
    const Vector2i &anchor_pos() const { return m_anchor_pos; }

    /// Set the anchor height; this determines the vertical shift relative to the anchor position
    void set_anchor_offset(int anchor_offset) { m_anchor_offset = anchor_offset; }
    /// Return the anchor height; this determines the vertical shift relative to the anchor position
    int anchor_offset() const { return m_anchor_offset; }

    /// Set the anchor width
    void set_anchor_size(int anchor_size) { m_anchor_size = anchor_size; }
    /// Return the anchor width
    int anchor_size() const { return m_anchor_size; }

    /// Set the side of the parent window at which popup will appear
    void set_side(Side popup_side) { m_side = popup_side; }
    /// Return the side of the parent window at which popup will appear
    Side side() const { return m_side; }

    /// Return the parent window of the popup
    Window *parent_window() { return m_parent_window; }
    /// Return the parent window of the popup
    const Window *parent_window() const { return m_parent_window; }

    /// Invoke the associated layout generator to properly place child widgets, if any
    virtual void perform_layout(NVGcontext *ctx) override;

    /// Draw the popup window
    virtual void draw(NVGcontext* ctx) override;
protected:
    /// Internal helper function to maintain nested window position values
    virtual void refresh_relative_placement() override;

protected:
    Window *m_parent_window;
    Vector2i m_anchor_pos;
    int m_anchor_offset, m_anchor_size;
    Side m_side;
};

NAMESPACE_END(nanogui)
