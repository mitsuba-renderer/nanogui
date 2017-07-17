/*
    nanogui/progressbar.h -- Standard widget for visualizing progress

    NanoGUI was developed by Wenzel Jakob <wenzel.jakob@epfl.ch>.
    The widget drawing code is based on the NanoVG demo application
    by Mikko Mononen.

    All rights reserved. Use of this source code is governed by a
    BSD-style license that can be found in the LICENSE.txt file.
*/
/** \file */

#pragma once

#include <nanogui/widget.h>

NAMESPACE_BEGIN(nanogui)

/**
 * \class ProgressBar progressbar.h nanogui/progressbar.h
 *
 * \brief Standard widget for visualizing progress.
 */
class NANOGUI_EXPORT ProgressBar : public Widget {
public:
    ProgressBar(Widget *parent);

    float value() { return m_value; }
    void set_value(float value) { m_value = value; }

    virtual Vector2i preferred_size(NVGcontext *ctx) const override;
    virtual void draw(NVGcontext* ctx) override;
protected:
    float m_value;
};

NAMESPACE_END(nanogui)
