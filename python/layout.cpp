#ifdef NANOGUI_PYTHON

#include "python.h"

DECLARE_LAYOUT(Layout);
DECLARE_LAYOUT(GroupLayout);
DECLARE_LAYOUT(BoxLayout);
DECLARE_LAYOUT(GridLayout);
DECLARE_LAYOUT(AdvancedGridLayout);

void register_layout(py::module &m) {
    py::class_<Layout, ref<Layout>, PyLayout> layout(m, "Layout", D(Layout));
    layout
        .def("preferred_size", &Layout::preferred_size, D(Layout, preferred_size))
        .def("perform_layout", &Layout::perform_layout, D(Layout, perform_layout));

    py::class_<BoxLayout, Layout, ref<BoxLayout>, PyBoxLayout>(m, "BoxLayout", D(BoxLayout))
        .def(py::init<Orientation, Alignment, int, int>(),
             "orientation"_a, "alignment"_a = Alignment::Middle,
             "margin"_a = 0, "spacing"_a = 0, D(BoxLayout, BoxLayout))
        .def("orientation", &BoxLayout::orientation, D(BoxLayout, orientation))
        .def("set_orientation", &BoxLayout::set_orientation, D(BoxLayout, set_orientation))
        .def("alignment", &BoxLayout::alignment, D(BoxLayout, alignment))
        .def("set_alignment", &BoxLayout::set_alignment, D(BoxLayout, set_alignment))
        .def("margin", &BoxLayout::margin, D(BoxLayout, margin))
        .def("set_margin", &BoxLayout::set_margin, D(BoxLayout, set_margin))
        .def("spacing", &BoxLayout::spacing, D(BoxLayout, spacing))
        .def("set_spacing", &BoxLayout::set_spacing, D(BoxLayout, set_spacing));

    py::class_<GroupLayout, Layout, ref<GroupLayout>, PyGroupLayout>(m, "GroupLayout", D(GroupLayout))
        .def(py::init<int, int, int, int>(),
             "margin"_a = 15, "spacing"_a = 6,
             "group_spacing"_a = 14, "group_indent"_a = 20,
             D(GroupLayout, GroupLayout))
        .def("margin", &GroupLayout::margin, D(GroupLayout, margin))
        .def("set_margin", &GroupLayout::set_margin, D(GroupLayout, set_margin))
        .def("spacing", &GroupLayout::spacing, D(GroupLayout, spacing))
        .def("set_spacing", &GroupLayout::set_spacing, D(GroupLayout, set_spacing))
        .def("group_indent", &GroupLayout::group_indent, D(GroupLayout, group_indent))
        .def("set_group_indent", &GroupLayout::set_group_indent, D(GroupLayout, set_group_indent))
        .def("group_spacing", &GroupLayout::group_spacing, D(GroupLayout, group_spacing))
        .def("set_group_spacing", &GroupLayout::set_group_spacing, D(GroupLayout, set_group_spacing));

    py::class_<GridLayout, Layout, ref<GridLayout>, PyGridLayout>(m, "GridLayout", D(GridLayout))
        .def(py::init<Orientation, int, Alignment, int, int>(),
             "orientation"_a = Orientation::Horizontal,
             "resolution"_a = 2, "alignment"_a = Alignment::Middle,
             "margin"_a = 0, "spacing"_a = 0,
             D(GridLayout, GridLayout))
        .def("orientation", &GridLayout::orientation, D(GridLayout, orientation))
        .def("set_orientation", &GridLayout::set_orientation, D(GridLayout, set_orientation))
        .def("resolution", &GridLayout::resolution, D(GridLayout, resolution))
        .def("set_resolution", &GridLayout::set_resolution, D(GridLayout, set_resolution))
        .def("margin", &GridLayout::margin, D(GridLayout, margin))
        .def("set_margin", &GridLayout::set_margin, D(GridLayout, set_margin))
        .def("spacing", &GridLayout::spacing, D(GridLayout, spacing))
        .def("set_spacing", (void(GridLayout::*)(int)) &GridLayout::set_spacing, D(GridLayout, set_spacing))
        .def("set_spacing", (void(GridLayout::*)(int, int)) &GridLayout::set_spacing, D(GridLayout, set_spacing, 2))
        .def("alignment", &GridLayout::alignment, D(GridLayout, alignment))
        .def("set_col_alignment", (void(GridLayout::*)(Alignment)) &GridLayout::set_col_alignment, D(GridLayout, set_col_alignment))
        .def("set_row_alignment", (void(GridLayout::*)(Alignment)) &GridLayout::set_row_alignment, D(GridLayout, set_row_alignment))
        .def("set_col_alignment", (void(GridLayout::*)(const std::vector<Alignment>&)) &GridLayout::set_col_alignment/*, D(GridLayout, set_col_alignment, 2)*/)
        .def("set_row_alignment", (void(GridLayout::*)(const std::vector<Alignment>&)) &GridLayout::set_row_alignment/*, D(GridLayout, set_row_alignment, 2)*/);

    py::class_<AdvancedGridLayout, Layout, ref<AdvancedGridLayout>, PyAdvancedGridLayout> adv_grid_layout(
        m, "AdvancedGridLayout", D(AdvancedGridLayout));

    adv_grid_layout
        .def(py::init<const std::vector<int> &, const std::vector<int> &>(),
             "widths"_a, "heights"_a,
             D(AdvancedGridLayout, AdvancedGridLayout))
        .def("row_count", &AdvancedGridLayout::row_count, D(AdvancedGridLayout, row_count))
        .def("col_count", &AdvancedGridLayout::col_count, D(AdvancedGridLayout, col_count))
        .def("margin", &AdvancedGridLayout::margin, D(AdvancedGridLayout, margin))
        .def("set_margin", &AdvancedGridLayout::set_margin, D(AdvancedGridLayout, set_margin))
        .def("append_row", &AdvancedGridLayout::append_row, "size"_a,
             "stretch"_a = 0, D(AdvancedGridLayout, append_row))
        .def("append_col", &AdvancedGridLayout::append_col, "size"_a,
             "stretch"_a = 0, D(AdvancedGridLayout, append_col))
        .def("set_row_stretch", &AdvancedGridLayout::set_row_stretch, D(AdvancedGridLayout, set_row_stretch))
        .def("set_col_stretch", &AdvancedGridLayout::set_col_stretch, D(AdvancedGridLayout, set_col_stretch))
        .def("set_anchor", &AdvancedGridLayout::set_anchor, D(AdvancedGridLayout, set_anchor))
        .def("anchor", &AdvancedGridLayout::anchor, D(AdvancedGridLayout, anchor));

    py::class_<AdvancedGridLayout::Anchor>(adv_grid_layout, "Anchor")
        .def(py::init<int, int, Alignment, Alignment>(),
             "x"_a, "y"_a,
             "horiz"_a = Alignment::Fill,
             "vert"_a = Alignment::Fill,
             D(AdvancedGridLayout, Anchor, Anchor, 2))
        .def(py::init<int, int, int, int, Alignment, Alignment>(),
             "x"_a, "y"_a, "w"_a, "h"_a,
             "horiz"_a = Alignment::Fill,
             "vert"_a = Alignment::Fill,
             D(AdvancedGridLayout, Anchor, Anchor, 3));
}

#endif
