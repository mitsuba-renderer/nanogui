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
#include <nanobind/nanobind.h>
#include <nanobind/trampoline.h>

extern "C" {
    /* Dummy handle types -- will never be instantiated */
    typedef struct NVGcontext { int unused; } NVGcontext;
    typedef struct GLFWwindow { int unused; } GLFWwindow;
};

/// Provides a ``NB_OVERRIDE`` for any relevant Widget items that need to be bound.
#define NANOGUI_WIDGET_OVERLOADS(Parent) \
    NB_TRAMPOLINE(Parent, 20) \
    bool mouse_button_event(const ::nanogui::Vector2i &p, int button, bool down, int modifiers) override { \
        NB_OVERRIDE(bool, Parent, mouse_button_event, p, button, down, modifiers); \
    } \
    bool mouse_motion_event(const ::nanogui::Vector2i &p, const ::nanogui::Vector2i &rel, int button, int modifiers) override { \
        NB_OVERRIDE(bool, Parent, mouse_motion_event, p, rel, button, modifiers); \
    } \
    bool mouse_drag_event(const ::nanogui::Vector2i &p, const ::nanogui::Vector2i &rel, int button, int modifiers) override { \
        NB_OVERRIDE(bool, Parent, mouse_drag_event, p, rel, button, modifiers); \
    } \
    bool mouse_enter_event(const ::nanogui::Vector2i &p, bool enter) override { \
        NB_OVERRIDE(bool, Parent, mouse_enter_event, p, enter); \
    } \
    bool scroll_event(const ::nanogui::Vector2i &p, const ::nanogui::Vector2f &rel) override { \
        NB_OVERRIDE(bool, Parent, scroll_event, p, rel); \
    } \
    bool focus_event(bool focused) override { \
        NB_OVERRIDE(bool, Parent, focus_event, focused); \
    } \
    bool keyboard_event(int key, int scancode, int action, int modifiers) override { \
        NB_OVERRIDE(bool, Parent, keyboard_event, key, scancode, action, modifiers); \
    } \
    bool keyboard_character_event(unsigned int codepoint) override { \
        NB_OVERRIDE(bool, Parent, keyboard_character_event, codepoint); \
    } \
    ::nanogui::Vector2i preferred_size(NVGcontext *ctx) const override { \
        NB_OVERRIDE(::nanogui::Vector2i, Parent, preferred_size, ctx); \
    } \
    void perform_layout(NVGcontext *ctx) override { \
        NB_OVERRIDE(void, Parent, perform_layout, ctx); \
    } \
    void draw(NVGcontext *ctx) override { \
        NB_OVERRIDE(void, Parent, draw, ctx); \
    }

/// Provides a ``NB_OVERRIDE`` for any relevant Layout items that need to be bound.
#define NANOGUI_LAYOUT_OVERLOADS(Parent) \
    NB_TRAMPOLINE(Parent, 2) \
    ::nanogui::Vector2i preferred_size(NVGcontext *ctx, const ::nanogui::Widget *widget) const override { \
        NB_OVERRIDE(::nanogui::Vector2i, Parent, preferred_size, ctx, widget); \
    } \
    void perform_layout(NVGcontext *ctx, ::nanogui::Widget *widget) const override { \
        NB_OVERRIDE(void, Parent, perform_layout, ctx, widget); \
    }

/// Provides a ``NB_OVERRIDE`` for any relevant Screen items that need to be bound.
#define NANOGUI_SCREEN_OVERLOADS(Parent) \
    virtual void draw_all() override { \
        NB_OVERRIDE(void, Parent, draw_all); \
    } \
    virtual void draw_contents() override { \
        NB_OVERRIDE(void, Parent, draw_contents); \
    } \
    virtual bool drop_event(const std::vector<std::string> &filenames) override { \
        NB_OVERRIDE(bool, Parent, drop_event, filenames); \
    } \
    virtual bool resize_event(const ::nanogui::Vector2i &size) override { \
        NB_OVERRIDE(bool, Parent, resize_event, size); \
    }

NAMESPACE_BEGIN(nanobind)
NAMESPACE_BEGIN(detail)

template <typename T> struct type_caster<nanogui::ref<T>> {
    using Value = nanogui::ref<T>;
    using Caster = make_caster<T>;
    static constexpr auto Name = Caster::Name;
    static constexpr bool IsClass = true;

    template <typename T_> using Cast = movable_cast_t<T_>;

    Value value;

    bool from_python(handle src, uint8_t flags,
                     cleanup_list *cleanup) noexcept {
        Caster caster;
        if (!caster.from_python(src, flags, cleanup))
            return false;

        value = Value(caster.operator T *());

        return true;
    }

    static handle from_cpp(const Value &value, rv_policy policy,
                           cleanup_list *cleanup) noexcept {
        return Caster::from_cpp(value.get(), policy, cleanup);
    }

    explicit operator Value *() { return &value; }
    explicit operator Value &() { return value; }
    explicit operator Value &&() && { return (Value &&) value; }
};

NAMESPACE_END(detail)
NAMESPACE_END(nanobind)
