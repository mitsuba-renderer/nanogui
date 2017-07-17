/*
    nanogui/stackedwidget.h -- Widget used to stack widgets on top
    of each other. Only the active widget is visible.

    The stacked widget was contributed by Stefan Ivanov.

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
 * \class StackedWidget stackedwidget.h nanogui/stackedwidget.h
 *
 * \brief A stack widget.
 */
class NANOGUI_EXPORT StackedWidget : public Widget {
public:
    StackedWidget(Widget* parent);

    void set_selected_index(int index);
    int selected_index() const;

    virtual void perform_layout(NVGcontext* ctx) override;
    virtual Vector2i preferred_size(NVGcontext* ctx) const override;
    virtual void add_child(int index, Widget* widget) override;

private:
    int m_selected_index = -1;
};

NAMESPACE_END(nanogui)
