/*
    nanogui/python.h -- Macros to facilitate Python bindings of custom widgets

    NanoGUI was developed by Wenzel Jakob <wenzel.jakob@epfl.ch>.
    The widget drawing code is based on the NanoVG demo application
    by Mikko Mononen.

    All rights reserved. Use of this source code is governed by a
    BSD-style license that can be found in the LICENSE.txt file.
*/
/** \file */

#pragma once


#if defined(_MSC_VER)
#  pragma warning (disable:5033) // 'register' is no longer a supported storage class
#endif

#include <nanogui/common.h>
#include <pybind11/pybind11.h>

/// Provides a ``PYBIND11_OVERLOAD`` for any relevant Widget items that need to be bound.
#define NANOGUI_WIDGET_OVERLOADS(Parent) \
    bool mouse_button_event(const ::nanogui::Vector2i &p, int button, bool down, int modifiers) override { \
        PYBIND11_OVERLOAD(bool, Parent, mouse_button_event, p, button, down, modifiers); \
    } \
    bool mouse_motion_event(const ::nanogui::Vector2i &p, const ::nanogui::Vector2i &rel, int button, int modifiers) override { \
        PYBIND11_OVERLOAD(bool, Parent, mouse_motion_event, p, rel, button, modifiers); \
    } \
    bool mouse_drag_event(const ::nanogui::Vector2i &p, const ::nanogui::Vector2i &rel, int button, int modifiers) override { \
        PYBIND11_OVERLOAD(bool, Parent, mouse_drag_event, p, rel, button, modifiers); \
    } \
    bool mouse_enter_event(const ::nanogui::Vector2i &p, bool enter) override { \
        PYBIND11_OVERLOAD(bool, Parent, mouse_enter_event, p, enter); \
    } \
    bool scroll_event(const ::nanogui::Vector2i &p, const ::nanogui::Vector2f &rel) override { \
        PYBIND11_OVERLOAD(bool, Parent, scroll_event, p, rel); \
    } \
    bool focus_event(bool focused) override { \
        PYBIND11_OVERLOAD(bool, Parent, focus_event, focused); \
    } \
    bool keyboard_event(int key, int scancode, int action, int modifiers) override { \
        PYBIND11_OVERLOAD(bool, Parent, keyboard_event, key, scancode, action, modifiers); \
    } \
    bool keyboard_character_event(unsigned int codepoint) override { \
        PYBIND11_OVERLOAD(bool, Parent, keyboard_character_event, codepoint); \
    } \
    ::nanogui::Vector2i preferred_size(NVGcontext *ctx) const override { \
        PYBIND11_OVERLOAD(::nanogui::Vector2i, Parent, preferred_size, ctx); \
    } \
    void perform_layout(NVGcontext *ctx) override { \
        PYBIND11_OVERLOAD(void, Parent, perform_layout, ctx); \
    } \
    void draw(NVGcontext *ctx) override { \
        PYBIND11_OVERLOAD(void, Parent, draw, ctx); \
    }

/// Provides a ``PYBIND11_OVERLOAD`` for any relevant Layout items that need to be bound.
#define NANOGUI_LAYOUT_OVERLOADS(Parent) \
    ::nanogui::Vector2i preferred_size(NVGcontext *ctx, const ::nanogui::Widget *widget) const override { \
        PYBIND11_OVERLOAD(::nanogui::Vector2i, Parent, preferred_size, ctx, widget); \
    } \
    void perform_layout(NVGcontext *ctx, ::nanogui::Widget *widget) const override { \
        PYBIND11_OVERLOAD(void, Parent, perform_layout, ctx, widget); \
    }

/// Provides a ``PYBIND11_OVERLOAD`` for any relevant Screen items that need to be bound.
#define NANOGUI_SCREEN_OVERLOADS(Parent) \
    virtual void draw_all() override { \
        PYBIND11_OVERLOAD(void, Parent, draw_all); \
    } \
    virtual void draw_contents() override { \
        PYBIND11_OVERLOAD(void, Parent, draw_contents); \
    } \
    virtual bool drop_event(const std::vector<std::string> &filenames) override { \
        PYBIND11_OVERLOAD(bool, Parent, drop_event, filenames); \
    } \
    virtual bool resize_event(const ::nanogui::Vector2i &size) override { \
        PYBIND11_OVERLOAD(bool, Parent, resize_event, size); \
    }

extern "C" {
    /* Dummy handle types -- will never be instantiated */
    typedef struct NVGcontext { int unused; } NVGcontext;
    typedef struct GLFWwindow { int unused; } GLFWwindow;
};
