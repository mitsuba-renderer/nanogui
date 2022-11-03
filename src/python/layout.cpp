#ifdef NANOGUI_PYTHON

#include "python.h"

DECLARE_LAYOUT(Layout);
DECLARE_LAYOUT(GroupLayout);
DECLARE_LAYOUT(BoxLayout);
DECLARE_LAYOUT(GridLayout);
DECLARE_LAYOUT(AdvancedGridLayout);

void register_layout(nb::module_ &m) {
    nb::class_<Layout, Object, PyLayout> layout(m, "Layout", D(Layout));
    layout
        .def(nb_method(Layout, preferred_size), D(Layout, preferred_size))
        .def(nb_method(Layout, perform_layout), D(Layout, perform_layout));

    nb::class_<BoxLayout, Layout, PyBoxLayout>(m, "BoxLayout", D(BoxLayout))
        .def(nb::init<Orientation, Alignment, int, int>(),
             "orientation"_a, "alignment"_a = Alignment::Middle,
             "margin"_a = 0, "spacing"_a = 0, D(BoxLayout, BoxLayout))
        .def(nb_method(BoxLayout, orientation), D(BoxLayout, orientation))
        .def(nb_method(BoxLayout, set_orientation), D(BoxLayout, set_orientation))
        .def(nb_method(BoxLayout, alignment), D(BoxLayout, alignment))
        .def(nb_method(BoxLayout, set_alignment), D(BoxLayout, set_alignment))
        .def(nb_method(BoxLayout, margin), D(BoxLayout, margin))
        .def(nb_method(BoxLayout, set_margin), D(BoxLayout, set_margin))
        .def(nb_method(BoxLayout, spacing), D(BoxLayout, spacing))
        .def(nb_method(BoxLayout, set_spacing), D(BoxLayout, set_spacing));

    nb::class_<GroupLayout, Layout, PyGroupLayout>(m, "GroupLayout", D(GroupLayout))
        .def(nb::init<int, int, int, int>(),
             "margin"_a = 15, "spacing"_a = 6,
             "group_spacing"_a = 14, "group_indent"_a = 20,
             D(GroupLayout, GroupLayout))
        .def(nb_method(GroupLayout, margin), D(GroupLayout, margin))
        .def(nb_method(GroupLayout, set_margin), D(GroupLayout, set_margin))
        .def(nb_method(GroupLayout, spacing), D(GroupLayout, spacing))
        .def(nb_method(GroupLayout, set_spacing), D(GroupLayout, set_spacing))
        .def(nb_method(GroupLayout, group_indent), D(GroupLayout, group_indent))
        .def(nb_method(GroupLayout, set_group_indent), D(GroupLayout, set_group_indent))
        .def(nb_method(GroupLayout, group_spacing), D(GroupLayout, group_spacing))
        .def(nb_method(GroupLayout, set_group_spacing), D(GroupLayout, set_group_spacing));

    nb::class_<GridLayout, Layout, PyGridLayout>(m, "GridLayout", D(GridLayout))
        .def(nb::init<Orientation, int, Alignment, int, int>(),
             "orientation"_a = Orientation::Horizontal,
             "resolution"_a = 2, "alignment"_a = Alignment::Middle,
             "margin"_a = 0, "spacing"_a = 0,
             D(GridLayout, GridLayout))
        .def(nb_method(GridLayout, orientation), D(GridLayout, orientation))
        .def(nb_method(GridLayout, set_orientation), D(GridLayout, set_orientation))
        .def(nb_method(GridLayout, resolution), D(GridLayout, resolution))
        .def(nb_method(GridLayout, set_resolution), D(GridLayout, set_resolution))
        .def(nb_method(GridLayout, margin), D(GridLayout, margin))
        .def(nb_method(GridLayout, set_margin), D(GridLayout, set_margin))
        .def(nb_method(GridLayout, spacing), D(GridLayout, spacing))
        .def("set_spacing", (void(GridLayout::*)(int)) &GridLayout::set_spacing, D(GridLayout, set_spacing))
        .def("set_spacing", (void(GridLayout::*)(int, int)) &GridLayout::set_spacing, D(GridLayout, set_spacing, 2))
        .def(nb_method(GridLayout, alignment), D(GridLayout, alignment))
        .def("set_col_alignment", (void(GridLayout::*)(Alignment)) &GridLayout::set_col_alignment, D(GridLayout, set_col_alignment))
        .def("set_row_alignment", (void(GridLayout::*)(Alignment)) &GridLayout::set_row_alignment, D(GridLayout, set_row_alignment))
        .def("set_col_alignment", (void(GridLayout::*)(const std::vector<Alignment>&)) &GridLayout::set_col_alignment/*, D(GridLayout, set_col_alignment, 2)*/)
        .def("set_row_alignment", (void(GridLayout::*)(const std::vector<Alignment>&)) &GridLayout::set_row_alignment/*, D(GridLayout, set_row_alignment, 2)*/);

    nb::class_<AdvancedGridLayout, Layout, PyAdvancedGridLayout> adv_grid_layout(
        m, "AdvancedGridLayout", D(AdvancedGridLayout));

    adv_grid_layout
        .def(nb::init<const std::vector<int> &, const std::vector<int> &>(),
             "widths"_a, "heights"_a,
             D(AdvancedGridLayout, AdvancedGridLayout))
        .def(nb_method(AdvancedGridLayout, row_count), D(AdvancedGridLayout, row_count))
        .def(nb_method(AdvancedGridLayout, col_count), D(AdvancedGridLayout, col_count))
        .def(nb_method(AdvancedGridLayout, margin), D(AdvancedGridLayout, margin))
        .def(nb_method(AdvancedGridLayout, set_margin), D(AdvancedGridLayout, set_margin))
        .def(nb_method(AdvancedGridLayout, append_row), "size"_a,
             "stretch"_a = 0, D(AdvancedGridLayout, append_row))
        .def(nb_method(AdvancedGridLayout, append_col), "size"_a,
             "stretch"_a = 0, D(AdvancedGridLayout, append_col))
        .def(nb_method(AdvancedGridLayout, set_row_stretch), D(AdvancedGridLayout, set_row_stretch))
        .def(nb_method(AdvancedGridLayout, set_col_stretch), D(AdvancedGridLayout, set_col_stretch))
        .def(nb_method(AdvancedGridLayout, set_anchor), D(AdvancedGridLayout, set_anchor))
        .def(nb_method(AdvancedGridLayout, anchor), D(AdvancedGridLayout, anchor));

    nb::class_<AdvancedGridLayout::Anchor>(adv_grid_layout, "Anchor")
        .def(nb::init<int, int, Alignment, Alignment>(),
             "x"_a, "y"_a,
             "horiz"_a = Alignment::Fill,
             "vert"_a = Alignment::Fill,
             D(AdvancedGridLayout, Anchor, Anchor, 2))
        .def(nb::init<int, int, int, int, Alignment, Alignment>(),
             "x"_a, "y"_a, "w"_a, "h"_a,
             "horiz"_a = Alignment::Fill,
             "vert"_a = Alignment::Fill,
             D(AdvancedGridLayout, Anchor, Anchor, 3));
}

#endif
