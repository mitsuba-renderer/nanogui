/*
    nanogui/tabheader.h -- Widget used to control tabs.

    The tab header widget was contributed by Stefan Ivanov.

    NanoGUI was developed by Wenzel Jakob <wenzel.jakob@epfl.ch>.
    The widget drawing code is based on the NanoVG demo application
    by Mikko Mononen.

    All rights reserved. Use of this source code is governed by a
    BSD-style license that can be found in the LICENSE.txt file.
*/
/** \file */

#pragma once

#include <nanogui/widget.h>
#include <vector>
#include <string>
#include <functional>
#include <utility>
#include <iterator>

NAMESPACE_BEGIN(nanogui)

/**
 * \class TabHeader tabheader.h nanogui/tabheader.h
 *
 * \brief A Tab navigable widget.
 */
class NANOGUI_EXPORT TabHeader : public Widget {
public:
    TabHeader(Widget *parent, const std::string &font = "sans-bold");

    void set_font(const std::string& font) { m_font = font; }
    const std::string& font() const { return m_font; }
    bool overflowing() const { return m_overflowing; }

    /**
     * Sets the callable objects which is invoked when a tab button is pressed.
     * The argument provided to the callback is the index of the tab.
     */
    void set_callback(const std::function<void(int)>& callback) { m_callback = callback; };
    const std::function<void(int)>& callback() const { return m_callback; }

    void set_active_tab(int tab_index);
    int active_tab() const;
    bool is_tab_visible(int index) const;
    int tab_count() const { return (int) m_tab_buttons.size();  }

    /// Inserts a tab at the end of the tabs collection.
    void add_tab(const std::string& label);

    /// Inserts a tab into the tabs collection at the specified index.
    void add_tab(int index, const std::string& label);

    /**
     * Removes the tab with the specified label and returns the index of the label.
     * Returns -1 if there was no such tab
     */
    int remove_tab(const std::string& label);

    /// Removes the tab with the specified index.
    void remove_tab(int index);

    /// Retrieves the label of the tab at a specific index.
    const std::string& tab_label_at(int index) const;

    /**
     * Retrieves the index of a specific tab label.
     * Returns the number of tabs (tabs_count) if there is no such tab.
     */
    int tab_index(const std::string& label);

    /**
     * Recalculate the visible range of tabs so that the tab with the specified
     * index is visible. The tab with the specified index will either be the
     * first or last visible one depending on the position relative to the
     * old visible range.
     */
    void ensure_tab_visible(int index);

    /**
     * Returns a pair of Vectors describing the top left (pair.first) and the
     * bottom right (pair.second) positions of the rectangle containing the visible tab buttons.
     */
    std::pair<Vector2i, Vector2i> visible_button_area() const;

    /**
     * Returns a pair of Vectors describing the top left (pair.first) and the
     * bottom right (pair.second) positions of the rectangle containing the active tab button.
     * Returns two zero vectors if the active button is not visible.
     */
    std::pair<Vector2i, Vector2i> active_button_area() const;

    virtual void perform_layout(NVGcontext* ctx) override;
    virtual Vector2i preferred_size(NVGcontext* ctx) const override;
    virtual bool mouse_button_event(const Vector2i &p, int button, bool down, int modifiers) override;

    virtual void draw(NVGcontext* ctx) override;

private:
    /**
     * \class TabButton tabheader.h
     *
     * \brief Implementation class of the actual tab buttons.
     */
    class TabButton {
    public:
        constexpr static const char* dots = "...";

        TabButton(TabHeader& header, const std::string& label);

        void set_label(const std::string& label) { m_label = label; }
        const std::string& label() const { return m_label; }
        void set_size(const Vector2i& size) { m_size = size; }
        const Vector2i& size() const { return m_size; }

        Vector2i preferred_size(NVGcontext* ctx) const;
        void calculate_visible_string(NVGcontext* ctx);
        void draw_at_position(NVGcontext* ctx, const Vector2i& position, bool active);
        void draw_active_border_at(NVGcontext * ctx, const Vector2i& position, float offset, const Color& color);
        void draw_inactive_border_at(NVGcontext * ctx, const Vector2i& position, float offset, const Color& color);

    private:
        TabHeader* m_header;
        std::string m_label;
        Vector2i m_size;

        /**
         * \struct StringView tabheader.h nanogui/tabheader.h
         *
         * \brief Helper struct to represent the TabButton.
         */
        struct StringView {
            const char* first = nullptr;
            const char* last = nullptr;
        };
        StringView m_visible_text;
        int m_visible_width = 0;
    };

    using TabIterator = std::vector<TabButton>::iterator;
    using ConstTabIterator = std::vector<TabButton>::const_iterator;

    /// The location in which the Widget will be facing.
    enum class ClickLocation {
        LeftControls, RightControls, TabButtons
    };

    TabIterator visible_begin() { return std::next(m_tab_buttons.begin(), m_visible_start); }
    TabIterator visible_end() { return std::next(m_tab_buttons.begin(), m_visible_end); }
    TabIterator active_iterator() { return std::next(m_tab_buttons.begin(), m_active_tab); }
    TabIterator tab_iterator(int index) { return std::next(m_tab_buttons.begin(), index); }

    ConstTabIterator visible_begin() const { return std::next(m_tab_buttons.begin(), m_visible_start); }
    ConstTabIterator visible_end() const { return std::next(m_tab_buttons.begin(), m_visible_end); }
    ConstTabIterator active_iterator() const { return std::next(m_tab_buttons.begin(), m_active_tab); }
    ConstTabIterator tab_iterator(int index) const { return std::next(m_tab_buttons.begin(), index); }

    /// Given the beginning of the visible tabs, calculate the end.
    void calculate_visible_end();

    void draw_controls(NVGcontext* ctx);
    ClickLocation locate_click(const Vector2i& p);
    void on_arrow_left();
    void on_arrow_right();

    std::function<void(int)> m_callback;
    std::vector<TabButton> m_tab_buttons;
    int m_visible_start = 0;
    int m_visible_end = 0;
    int m_active_tab = 0;
    bool m_overflowing = false;

    std::string m_font;
};

NAMESPACE_END(nanogui)
