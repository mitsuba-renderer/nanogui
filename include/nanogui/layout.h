/*
    nanogui/layout.h -- A collection of useful layout managers

    The grid layout was contributed by Christian Schueller.

    NanoGUI was developed by Wenzel Jakob <wenzel.jakob@epfl.ch>.
    The widget drawing code is based on the NanoVG demo application
    by Mikko Mononen.

    All rights reserved. Use of this source code is governed by a
    BSD-style license that can be found in the LICENSE.txt file.
*/
/** \file */

#pragma once

#include <nanogui/compat.h>
#include <nanogui/object.h>
#include <unordered_map>

NAMESPACE_BEGIN(nanogui)

/// The different kinds of alignments a layout can perform.
enum class Alignment : uint8_t {
    Minimum = 0,
    Middle,
    Maximum,
    Fill
};

/// The direction of data flow for a layout.
enum class Orientation {
    Horizontal = 0,
    Vertical
};

/**
 * \class Layout layout.h nanogui/layout.h
 *
 * \brief Basic interface of a layout engine.
 */
class NANOGUI_EXPORT Layout : public Object {
public:
    virtual void perform_layout(NVGcontext *ctx, Widget *widget) const = 0;
    virtual Vector2i preferred_size(NVGcontext *ctx, const Widget *widget) const = 0;
protected:
    virtual ~Layout() { }
};

/**
 * \class BoxLayout layout.h nanogui/layout.h
 *
 * \brief Simple horizontal/vertical box layout
 *
 * This widget stacks up a bunch of widgets horizontally or vertically. It adds
 * margins around the entire container and a custom spacing between adjacent
 * widgets.
 */
class NANOGUI_EXPORT BoxLayout : public Layout {
public:
    /**
     * \brief Construct a box layout which packs widgets in the given \c Orientation
     *
     * \param alignment
     *     Widget alignment perpendicular to the chosen orientation
     *
     * \param margin
     *     Margin around the layout container
     *
     * \param spacing
     *     Extra spacing placed between widgets
     */
    BoxLayout(Orientation orientation, Alignment alignment = Alignment::Middle,
              int margin = 0, int spacing = 0);

    Orientation orientation() const { return m_orientation; }
    void set_orientation(Orientation orientation) { m_orientation = orientation; }

    Alignment alignment() const { return m_alignment; }
    void set_alignment(Alignment alignment) { m_alignment = alignment; }

    int margin() const { return m_margin; }
    void set_margin(int margin) { m_margin = margin; }

    int spacing() const { return m_spacing; }
    void set_spacing(int spacing) { m_spacing = spacing; }

    /* Implementation of the layout interface */
    virtual Vector2i preferred_size(NVGcontext *ctx, const Widget *widget) const override;
    virtual void perform_layout(NVGcontext *ctx, Widget *widget) const override;

protected:
    Orientation m_orientation;
    Alignment m_alignment;
    int m_margin;
    int m_spacing;
};

/**
 * \class GroupLayout layout.h nanogui/layout.h
 *
 * \brief Special layout for widgets grouped by labels.
 *
 * This widget resembles a box layout in that it arranges a set of widgets
 * vertically. All widgets are indented on the horizontal axis except for
 * \ref Label widgets, which are not indented.
 *
 * This creates a pleasing layout where a number of widgets are grouped
 * under some high-level heading.
 */
class NANOGUI_EXPORT GroupLayout : public Layout {
public:
    GroupLayout(int margin = 15, int spacing = 6, int group_spacing = 14,
                int group_indent = 20)
        : m_margin(margin), m_spacing(spacing), m_group_spacing(group_spacing),
          m_group_indent(group_indent) {}

    int margin() const { return m_margin; }
    void set_margin(int margin) { m_margin = margin; }

    int spacing() const { return m_spacing; }
    void set_spacing(int spacing) { m_spacing = spacing; }

    int group_indent() const { return m_group_indent; }
    void set_group_indent(int group_indent) { m_group_indent = group_indent; }

    int group_spacing() const { return m_group_spacing; }
    void set_group_spacing(int group_spacing) { m_group_spacing = group_spacing; }

    /* Implementation of the layout interface */
    virtual Vector2i preferred_size(NVGcontext *ctx, const Widget *widget) const override;
    virtual void perform_layout(NVGcontext *ctx, Widget *widget) const override;

protected:
    int m_margin;
    int m_spacing;
    int m_group_spacing;
    int m_group_indent;
};

/**
 * \class GridLayout layout.h nanogui/layout.h
 *
 * \brief Grid layout.
 *
 * Widgets are arranged in a grid that has a fixed grid resolution \c resolution
 * along one of the axes. The layout orientation indicates the fixed dimension;
 * widgets are also appended on this axis. The spacing between items can be
 * specified per axis. The horizontal/vertical alignment can be specified per
 * row and column.
 */
class NANOGUI_EXPORT GridLayout : public Layout {
public:
    /// Create a 2-column grid layout by default
    GridLayout(Orientation orientation = Orientation::Horizontal, int resolution = 2,
               Alignment alignment = Alignment::Middle,
               int margin = 0, int spacing = 0)
        : m_orientation(orientation), m_resolution(resolution), m_margin(margin) {
        m_default_alignment[0] = m_default_alignment[1] = alignment;
        m_spacing = Vector2i(spacing);
    }

    Orientation orientation() const { return m_orientation; }
    void set_orientation(Orientation orientation) {
        m_orientation = orientation;
    }

    int resolution() const { return m_resolution; }
    void set_resolution(int resolution) { m_resolution = resolution; }

    int spacing(int axis) const { return m_spacing[axis]; }
    void set_spacing(int axis, int spacing) { m_spacing[axis] = spacing; }
    void set_spacing(int spacing) { m_spacing[0] = m_spacing[1] = spacing; }

    int margin() const { return m_margin; }
    void set_margin(int margin) { m_margin = margin; }

    Alignment alignment(int axis, int item) const {
        if (item < (int) m_alignment[axis].size())
            return m_alignment[axis][item];
        else
            return m_default_alignment[axis];
    }

    void set_col_alignment(Alignment value) { m_default_alignment[0] = value; }
    void set_row_alignment(Alignment value) { m_default_alignment[1] = value; }
    void set_col_alignment(const std::vector<Alignment> &value) { m_alignment[0] = value; }
    void set_row_alignment(const std::vector<Alignment> &value) { m_alignment[1] = value; }

    /* Implementation of the layout interface */
    virtual Vector2i preferred_size(NVGcontext *ctx, const Widget *widget) const override;
    virtual void perform_layout(NVGcontext *ctx, Widget *widget) const override;

protected:
    // Compute the maximum row and column sizes
    void compute_layout(NVGcontext *ctx, const Widget *widget,
                       std::vector<int> *grid) const;

protected:
    Orientation m_orientation;
    Alignment m_default_alignment[2];
    std::vector<Alignment> m_alignment[2];
    int m_resolution;
    Vector2i m_spacing;
    int m_margin;
};

/**
 * \class AdvancedGridLayout layout.h nanogui/layout.h
 *
 * \brief Advanced Grid layout.
 *
 * The is a fancier grid layout with support for items that span multiple rows
 * or columns, and per-widget alignment flags. Each row and column additionally
 * stores a stretch factor that controls how additional space is redistributed.
 * The downside of this flexibility is that a layout anchor data structure must
 * be provided for each widget.
 *
 * An example:
 *
 * \rst
 * .. code-block:: cpp
 *
 *    using AdvancedGridLayout::Anchor;
 *    Label *label = new Label(window, "A label");
 *    // Add a centered label at grid position (1, 5), which spans two horizontal cells
 *    layout->set_anchor(label, Anchor(1, 5, 2, 1, Alignment::Middle, Alignment::Middle));
 *
 * \endrst
 *
 * The grid is initialized with user-specified column and row size vectors
 * (which can be expanded later on if desired). If a size value of zero is
 * specified for a column or row, the size is set to the maximum preferred size
 * of any widgets contained in the same row or column. Any remaining space is
 * redistributed according to the row and column stretch factors.
 *
 * The high level usage somewhat resembles the classic HIG layout:
 *
 * - https://web.archive.org/web/20070813221705/http://www.autel.cz/dmi/tutorial.html
 * - https://github.com/jaapgeurts/higlayout
 */
class NANOGUI_EXPORT AdvancedGridLayout : public Layout {
public:
    /**
     * \struct Anchor layout.h nanogui/layout.h
     *
     * \brief Helper struct to coordinate anchor points for the layout.
     */
    struct Anchor {
        uint8_t pos[2];
        uint8_t size[2];
        Alignment align[2];

        Anchor() { }

        Anchor(int x, int y, Alignment horiz = Alignment::Fill,
              Alignment vert = Alignment::Fill) {
            pos[0] = (uint8_t) x; pos[1] = (uint8_t) y;
            size[0] = size[1] = 1;
            align[0] = horiz; align[1] = vert;
        }

        Anchor(int x, int y, int w, int h,
              Alignment horiz = Alignment::Fill,
              Alignment vert = Alignment::Fill) {
            pos[0] = (uint8_t) x; pos[1] = (uint8_t) y;
            size[0] = (uint8_t) w; size[1] = (uint8_t) h;
            align[0] = horiz; align[1] = vert;
        }

        operator std::string() const {
            char buf[50];
            NANOGUI_SNPRINTF(buf, 50, "Format[pos=(%i, %i), size=(%i, %i), align=(%i, %i)]",
                pos[0], pos[1], size[0], size[1], (int) align[0], (int) align[1]);
            return buf;
        }
    };

    AdvancedGridLayout(const std::vector<int> &cols = {}, const std::vector<int> &rows = {}, int margin = 0);

    int margin() const { return m_margin; }
    void set_margin(int margin) { m_margin = margin; }

    /// Return the number of cols
    int col_count() const { return (int) m_cols.size(); }

    /// Return the number of rows
    int row_count() const { return (int) m_rows.size(); }

    /// Append a row of the given size (and stretch factor)
    void append_row(int size, float stretch = 0.f) { m_rows.push_back(size); m_row_stretch.push_back(stretch); };

    /// Append a column of the given size (and stretch factor)
    void append_col(int size, float stretch = 0.f) { m_cols.push_back(size); m_col_stretch.push_back(stretch); };

    /// Set the stretch factor of a given row
    void set_row_stretch(int index, float stretch) { m_row_stretch.at(index) = stretch; }

    /// Set the stretch factor of a given column
    void set_col_stretch(int index, float stretch) { m_col_stretch.at(index) = stretch; }

    /// Specify the anchor data structure for a given widget
    void set_anchor(const Widget *widget, const Anchor &anchor) { m_anchor[widget] = anchor; }

    /// Retrieve the anchor data structure for a given widget
    Anchor anchor(const Widget *widget) const {
        auto it = m_anchor.find(widget);
        if (it == m_anchor.end())
            throw std::runtime_error("Widget was not registered with the grid layout!");
        return it->second;
    }

    /* Implementation of the layout interface */
    virtual Vector2i preferred_size(NVGcontext *ctx, const Widget *widget) const override;
    virtual void perform_layout(NVGcontext *ctx, Widget *widget) const override;

protected:
    void compute_layout(NVGcontext *ctx, const Widget *widget,
                       std::vector<int> *grid) const;

protected:
    std::vector<int> m_cols, m_rows;
    std::vector<float> m_col_stretch, m_row_stretch;
    std::unordered_map<const Widget *, Anchor> m_anchor;
    int m_margin;
};

NAMESPACE_END(nanogui)
