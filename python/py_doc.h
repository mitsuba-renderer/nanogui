/*
  This file contains docstrings for the Python bindings.
  Do not edit! These were automatically extracted by mkdoc.py
 */

#define __EXPAND(x)                                      x
#define __COUNT(_1, _2, _3, _4, _5, _6, _7, COUNT, ...)  COUNT
#define __VA_SIZE(...)                                   __EXPAND(__COUNT(__VA_ARGS__, 7, 6, 5, 4, 3, 2, 1))
#define __CAT1(a, b)                                     a ## b
#define __CAT2(a, b)                                     __CAT1(a, b)
#define __DOC1(n1)                                       __doc_##n1
#define __DOC2(n1, n2)                                   __doc_##n1##_##n2
#define __DOC3(n1, n2, n3)                               __doc_##n1##_##n2##_##n3
#define __DOC4(n1, n2, n3, n4)                           __doc_##n1##_##n2##_##n3##_##n4
#define __DOC5(n1, n2, n3, n4, n5)                       __doc_##n1##_##n2##_##n3##_##n4##_##n5
#define __DOC6(n1, n2, n3, n4, n5, n6)                   __doc_##n1##_##n2##_##n3##_##n4##_##n5##_##n6
#define __DOC7(n1, n2, n3, n4, n5, n6, n7)               __doc_##n1##_##n2##_##n3##_##n4##_##n5##_##n6##_##n7
#define DOC(...)                                         __EXPAND(__EXPAND(__CAT2(__DOC, __VA_SIZE(__VA_ARGS__)))(__VA_ARGS__))

#if defined(__GNUG__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif


static const char *__doc_nanogui_AdvancedGridLayout =
R"doc(Advanced Grid layout.

The is a fancier grid layout with support for items that span multiple
rows or columns, and per-widget alignment flags. Each row and column
additionally stores a stretch factor that controls how additional
space is redistributed. The downside of this flexibility is that a
layout anchor data structure must be provided for each widget.

An example:

```
using AdvancedGridLayout::Anchor;
Label *label = new Label(window, "A label");
// Add a centered label at grid position (1, 5), which spans two horizontal cells
layout->set_anchor(label, Anchor(1, 5, 2, 1, Alignment::Middle, Alignment::Middle));

```

The grid is initialized with user-specified column and row size
vectors (which can be expanded later on if desired). If a size value
of zero is specified for a column or row, the size is set to the
maximum preferred size of any widgets contained in the same row or
column. Any remaining space is redistributed according to the row and
column stretch factors.

The high level usage somewhat resembles the classic HIG layout:

- https://web.archive.org/web/20070813221705/http://www.autel.cz/dmi/t
utorial.html - https://github.com/jaapgeurts/higlayout)doc";

static const char *__doc_nanogui_AdvancedGridLayout_AdvancedGridLayout = R"doc()doc";

static const char *__doc_nanogui_AdvancedGridLayout_Anchor = R"doc(Helper struct to coordinate anchor points for the layout.)doc";

static const char *__doc_nanogui_AdvancedGridLayout_Anchor_Anchor = R"doc()doc";

static const char *__doc_nanogui_AdvancedGridLayout_Anchor_Anchor_2 = R"doc()doc";

static const char *__doc_nanogui_AdvancedGridLayout_Anchor_Anchor_3 = R"doc()doc";

static const char *__doc_nanogui_AdvancedGridLayout_Anchor_align = R"doc()doc";

static const char *__doc_nanogui_AdvancedGridLayout_Anchor_operator_basic_string = R"doc()doc";

static const char *__doc_nanogui_AdvancedGridLayout_Anchor_pos = R"doc()doc";

static const char *__doc_nanogui_AdvancedGridLayout_Anchor_size = R"doc()doc";

static const char *__doc_nanogui_AdvancedGridLayout_anchor = R"doc(Retrieve the anchor data structure for a given widget)doc";

static const char *__doc_nanogui_AdvancedGridLayout_append_col = R"doc(Append a column of the given size (and stretch factor))doc";

static const char *__doc_nanogui_AdvancedGridLayout_append_row = R"doc(Append a row of the given size (and stretch factor))doc";

static const char *__doc_nanogui_AdvancedGridLayout_col_count = R"doc(Return the number of cols)doc";

static const char *__doc_nanogui_AdvancedGridLayout_compute_layout = R"doc()doc";

static const char *__doc_nanogui_AdvancedGridLayout_m_anchor = R"doc()doc";

static const char *__doc_nanogui_AdvancedGridLayout_m_col_stretch = R"doc()doc";

static const char *__doc_nanogui_AdvancedGridLayout_m_cols = R"doc()doc";

static const char *__doc_nanogui_AdvancedGridLayout_m_margin = R"doc()doc";

static const char *__doc_nanogui_AdvancedGridLayout_m_row_stretch = R"doc()doc";

static const char *__doc_nanogui_AdvancedGridLayout_m_rows = R"doc()doc";

static const char *__doc_nanogui_AdvancedGridLayout_margin = R"doc()doc";

static const char *__doc_nanogui_AdvancedGridLayout_perform_layout = R"doc()doc";

static const char *__doc_nanogui_AdvancedGridLayout_preferred_size = R"doc()doc";

static const char *__doc_nanogui_AdvancedGridLayout_row_count = R"doc(Return the number of rows)doc";

static const char *__doc_nanogui_AdvancedGridLayout_set_anchor = R"doc(Specify the anchor data structure for a given widget)doc";

static const char *__doc_nanogui_AdvancedGridLayout_set_col_stretch = R"doc(Set the stretch factor of a given column)doc";

static const char *__doc_nanogui_AdvancedGridLayout_set_margin = R"doc()doc";

static const char *__doc_nanogui_AdvancedGridLayout_set_row_stretch = R"doc(Set the stretch factor of a given row)doc";

static const char *__doc_nanogui_Alignment = R"doc(The different kinds of alignments a layout can perform.)doc";

static const char *__doc_nanogui_Alignment_Fill = R"doc()doc";

static const char *__doc_nanogui_Alignment_Maximum = R"doc()doc";

static const char *__doc_nanogui_Alignment_Middle = R"doc()doc";

static const char *__doc_nanogui_Alignment_Minimum = R"doc()doc";

static const char *__doc_nanogui_Arcball = R"doc(Arcball helper class to interactively rotate objects on-screen.)doc";

static const char *__doc_nanogui_Arcball_Arcball = R"doc()doc";

static const char *__doc_nanogui_Arcball_Arcball_2 = R"doc()doc";

static const char *__doc_nanogui_Arcball_active = R"doc()doc";

static const char *__doc_nanogui_Arcball_button = R"doc()doc";

static const char *__doc_nanogui_Arcball_m_active = R"doc()doc";

static const char *__doc_nanogui_Arcball_m_incr = R"doc()doc";

static const char *__doc_nanogui_Arcball_m_last_pos = R"doc()doc";

static const char *__doc_nanogui_Arcball_m_quat = R"doc()doc";

static const char *__doc_nanogui_Arcball_m_size = R"doc()doc";

static const char *__doc_nanogui_Arcball_m_speed_factor = R"doc()doc";

static const char *__doc_nanogui_Arcball_matrix = R"doc()doc";

static const char *__doc_nanogui_Arcball_motion = R"doc()doc";

static const char *__doc_nanogui_Arcball_operator_delete = R"doc()doc";

static const char *__doc_nanogui_Arcball_operator_delete_2 = R"doc()doc";

static const char *__doc_nanogui_Arcball_operator_delete_3 = R"doc()doc";

static const char *__doc_nanogui_Arcball_operator_delete_4 = R"doc()doc";

static const char *__doc_nanogui_Arcball_operator_delete_5 = R"doc()doc";

static const char *__doc_nanogui_Arcball_operator_delete_6 = R"doc()doc";

static const char *__doc_nanogui_Arcball_operator_new = R"doc()doc";

static const char *__doc_nanogui_Arcball_operator_new_2 = R"doc()doc";

static const char *__doc_nanogui_Arcball_operator_new_3 = R"doc()doc";

static const char *__doc_nanogui_Arcball_operator_new_4 = R"doc()doc";

static const char *__doc_nanogui_Arcball_set_size = R"doc()doc";

static const char *__doc_nanogui_Arcball_set_speed_factor = R"doc()doc";

static const char *__doc_nanogui_Arcball_set_state = R"doc()doc";

static const char *__doc_nanogui_Arcball_size = R"doc()doc";

static const char *__doc_nanogui_Arcball_speed_factor = R"doc()doc";

static const char *__doc_nanogui_Arcball_state = R"doc()doc";

static const char *__doc_nanogui_BoxLayout =
R"doc(Simple horizontal/vertical box layout

This widget stacks up a bunch of widgets horizontally or vertically.
It adds margins around the entire container and a custom spacing
between adjacent widgets.)doc";

static const char *__doc_nanogui_BoxLayout_BoxLayout =
R"doc(Construct a box layout which packs widgets in the given
``Orientation``

Parameter ``alignment``:
    Widget alignment perpendicular to the chosen orientation

Parameter ``margin``:
    Margin around the layout container

Parameter ``spacing``:
    Extra spacing placed between widgets)doc";

static const char *__doc_nanogui_BoxLayout_alignment = R"doc()doc";

static const char *__doc_nanogui_BoxLayout_m_alignment = R"doc()doc";

static const char *__doc_nanogui_BoxLayout_m_margin = R"doc()doc";

static const char *__doc_nanogui_BoxLayout_m_orientation = R"doc()doc";

static const char *__doc_nanogui_BoxLayout_m_spacing = R"doc()doc";

static const char *__doc_nanogui_BoxLayout_margin = R"doc()doc";

static const char *__doc_nanogui_BoxLayout_orientation = R"doc()doc";

static const char *__doc_nanogui_BoxLayout_perform_layout = R"doc()doc";

static const char *__doc_nanogui_BoxLayout_preferred_size = R"doc()doc";

static const char *__doc_nanogui_BoxLayout_set_alignment = R"doc()doc";

static const char *__doc_nanogui_BoxLayout_set_margin = R"doc()doc";

static const char *__doc_nanogui_BoxLayout_set_orientation = R"doc()doc";

static const char *__doc_nanogui_BoxLayout_set_spacing = R"doc()doc";

static const char *__doc_nanogui_BoxLayout_spacing = R"doc()doc";

static const char *__doc_nanogui_Button = R"doc([Normal/Toggle/Radio/Popup] Button widget.)doc";

static const char *__doc_nanogui_Button_Button = R"doc()doc";

static const char *__doc_nanogui_Button_Flags = R"doc(Flags to specify the button behavior (can be combined with binary OR))doc";

static const char *__doc_nanogui_Button_Flags_NormalButton = R"doc()doc";

static const char *__doc_nanogui_Button_Flags_PopupButton = R"doc()doc";

static const char *__doc_nanogui_Button_Flags_RadioButton = R"doc()doc";

static const char *__doc_nanogui_Button_Flags_ToggleButton = R"doc()doc";

static const char *__doc_nanogui_Button_IconPosition = R"doc(The available icon positions.)doc";

static const char *__doc_nanogui_Button_IconPosition_Left = R"doc()doc";

static const char *__doc_nanogui_Button_IconPosition_LeftCentered = R"doc()doc";

static const char *__doc_nanogui_Button_IconPosition_Right = R"doc()doc";

static const char *__doc_nanogui_Button_IconPosition_RightCentered = R"doc()doc";

static const char *__doc_nanogui_Button_background_color = R"doc()doc";

static const char *__doc_nanogui_Button_button_group = R"doc()doc";

static const char *__doc_nanogui_Button_callback = R"doc(Set the push callback (for any type of button))doc";

static const char *__doc_nanogui_Button_caption = R"doc()doc";

static const char *__doc_nanogui_Button_change_callback = R"doc(Set the change callback (for toggle buttons))doc";

static const char *__doc_nanogui_Button_draw = R"doc()doc";

static const char *__doc_nanogui_Button_flags = R"doc()doc";

static const char *__doc_nanogui_Button_icon = R"doc()doc";

static const char *__doc_nanogui_Button_icon_position = R"doc()doc";

static const char *__doc_nanogui_Button_m_background_color = R"doc()doc";

static const char *__doc_nanogui_Button_m_button_group = R"doc()doc";

static const char *__doc_nanogui_Button_m_callback = R"doc()doc";

static const char *__doc_nanogui_Button_m_caption = R"doc()doc";

static const char *__doc_nanogui_Button_m_change_callback = R"doc()doc";

static const char *__doc_nanogui_Button_m_flags = R"doc()doc";

static const char *__doc_nanogui_Button_m_icon = R"doc()doc";

static const char *__doc_nanogui_Button_m_icon_position = R"doc()doc";

static const char *__doc_nanogui_Button_m_pushed = R"doc()doc";

static const char *__doc_nanogui_Button_m_text_color = R"doc()doc";

static const char *__doc_nanogui_Button_mouse_button_event = R"doc()doc";

static const char *__doc_nanogui_Button_preferred_size = R"doc()doc";

static const char *__doc_nanogui_Button_pushed = R"doc()doc";

static const char *__doc_nanogui_Button_set_background_color = R"doc()doc";

static const char *__doc_nanogui_Button_set_button_group = R"doc(Set the button group (for radio buttons))doc";

static const char *__doc_nanogui_Button_set_callback = R"doc()doc";

static const char *__doc_nanogui_Button_set_caption = R"doc()doc";

static const char *__doc_nanogui_Button_set_change_callback = R"doc()doc";

static const char *__doc_nanogui_Button_set_flags = R"doc()doc";

static const char *__doc_nanogui_Button_set_icon = R"doc()doc";

static const char *__doc_nanogui_Button_set_icon_position = R"doc()doc";

static const char *__doc_nanogui_Button_set_pushed = R"doc()doc";

static const char *__doc_nanogui_Button_set_text_color = R"doc()doc";

static const char *__doc_nanogui_Button_text_color = R"doc()doc";

static const char *__doc_nanogui_CheckBox = R"doc(Two-state check box widget.)doc";

static const char *__doc_nanogui_CheckBox_CheckBox = R"doc()doc";

static const char *__doc_nanogui_CheckBox_callback = R"doc()doc";

static const char *__doc_nanogui_CheckBox_caption = R"doc()doc";

static const char *__doc_nanogui_CheckBox_checked = R"doc()doc";

static const char *__doc_nanogui_CheckBox_draw = R"doc()doc";

static const char *__doc_nanogui_CheckBox_m_callback = R"doc()doc";

static const char *__doc_nanogui_CheckBox_m_caption = R"doc()doc";

static const char *__doc_nanogui_CheckBox_m_checked = R"doc()doc";

static const char *__doc_nanogui_CheckBox_m_pushed = R"doc()doc";

static const char *__doc_nanogui_CheckBox_mouse_button_event = R"doc()doc";

static const char *__doc_nanogui_CheckBox_preferred_size = R"doc()doc";

static const char *__doc_nanogui_CheckBox_pushed = R"doc()doc";

static const char *__doc_nanogui_CheckBox_set_callback = R"doc()doc";

static const char *__doc_nanogui_CheckBox_set_caption = R"doc()doc";

static const char *__doc_nanogui_CheckBox_set_checked = R"doc()doc";

static const char *__doc_nanogui_CheckBox_set_pushed = R"doc()doc";

static const char *__doc_nanogui_Color =
R"doc(Stores an RGBA floating point color value.

This class simply wraps around an ``Vector4f``, providing some
convenient methods and terminology for thinking of it as a color. The
data operates in the same way as ``Vector4f``, and the following
values are identical:

```
+---------+-------------+-----------------------+-------------+
| Channel | Array Index | enoki Vector4f Value  | Color Value |
+=========+=============+=======================+=============+
| Red     | ``0``       | x()                   | r()         |
+---------+-------------+-----------------------+-------------+
| Green   | ``1``       | y()                   | g()         |
+---------+-------------+-----------------------+-------------+
| Blue    | ``2``       | z()                   | b()         |
+---------+-------------+-----------------------+-------------+
| Alpha   | ``3``       | w()                   | w()         |
+---------+-------------+-----------------------+-------------+
Note:
The method for the alpha component is **always** ``w()``.

```)doc";

static const char *__doc_nanogui_ColorPicker = R"doc(Push button with a popup to tweak a color value.)doc";

static const char *__doc_nanogui_ColorPicker_ColorPicker = R"doc()doc";

static const char *__doc_nanogui_ColorPicker_callback = R"doc(Set the change callback)doc";

static const char *__doc_nanogui_ColorPicker_color = R"doc(Get the current color)doc";

static const char *__doc_nanogui_ColorPicker_m_callback = R"doc()doc";

static const char *__doc_nanogui_ColorPicker_m_color_wheel = R"doc()doc";

static const char *__doc_nanogui_ColorPicker_m_pick_button = R"doc()doc";

static const char *__doc_nanogui_ColorPicker_set_callback = R"doc()doc";

static const char *__doc_nanogui_ColorPicker_set_color = R"doc(Set the current color)doc";

static const char *__doc_nanogui_ColorWheel = R"doc(Fancy analog widget to select a color value.)doc";

static const char *__doc_nanogui_ColorWheel_ColorWheel = R"doc()doc";

static const char *__doc_nanogui_ColorWheel_Region = R"doc()doc";

static const char *__doc_nanogui_ColorWheel_Region_Both = R"doc()doc";

static const char *__doc_nanogui_ColorWheel_Region_InnerTriangle = R"doc()doc";

static const char *__doc_nanogui_ColorWheel_Region_None = R"doc()doc";

static const char *__doc_nanogui_ColorWheel_Region_OuterCircle = R"doc()doc";

static const char *__doc_nanogui_ColorWheel_adjust_position = R"doc()doc";

static const char *__doc_nanogui_ColorWheel_callback = R"doc(Set the change callback)doc";

static const char *__doc_nanogui_ColorWheel_color = R"doc(Get the current color)doc";

static const char *__doc_nanogui_ColorWheel_draw = R"doc()doc";

static const char *__doc_nanogui_ColorWheel_hue2rgb = R"doc()doc";

static const char *__doc_nanogui_ColorWheel_m_black = R"doc()doc";

static const char *__doc_nanogui_ColorWheel_m_callback = R"doc()doc";

static const char *__doc_nanogui_ColorWheel_m_drag_region = R"doc()doc";

static const char *__doc_nanogui_ColorWheel_m_hue = R"doc()doc";

static const char *__doc_nanogui_ColorWheel_m_white = R"doc()doc";

static const char *__doc_nanogui_ColorWheel_mouse_button_event = R"doc()doc";

static const char *__doc_nanogui_ColorWheel_mouse_drag_event = R"doc()doc";

static const char *__doc_nanogui_ColorWheel_preferred_size = R"doc()doc";

static const char *__doc_nanogui_ColorWheel_set_callback = R"doc()doc";

static const char *__doc_nanogui_ColorWheel_set_color = R"doc(Set the current color)doc";

static const char *__doc_nanogui_Color_Color = R"doc(Default constructor: represents black (``r, g, b, a = 0``))doc";

static const char *__doc_nanogui_Color_Color_10 =
R"doc(Explicit constructor: creates the Color ``(r, g, b, a) / 255.0``.
Values are casted to floats before division.

Parameter ``r``:
    The red component of the color, will be divided by ``255.0``.

Parameter ``g``:
    The green component of the color, will be divided by ``255.0``.

Parameter ``b``:
    The blue component of the color, will be divided by ``255.0``.

Parameter ``a``:
    The alpha component of the color, will be divided by ``255.0``.)doc";

static const char *__doc_nanogui_Color_Color_2 =
R"doc(Copies (x, y, z) from the input vector, and uses the value specified
by the ``alpha`` parameter for this Color object's alpha component.

Parameter ``color``:
    The three dimensional float vector being copied.

Parameter ``alpha``:
    The value to set this object's alpha component to.)doc";

static const char *__doc_nanogui_Color_Color_3 =
R"doc(Copies (x, y, z) from the input vector, casted as floats first and
then divided by ``255.0``, and uses the value specified by the
``alpha`` parameter, casted to a float and divided by ``255.0`` as
well, for this Color object's alpha component.

Parameter ``color``:
    The three dimensional integer vector being copied, will be divided
    by ``255.0``.

Parameter ``alpha``:
    The value to set this object's alpha component to, will be divided
    by ``255.0``.)doc";

static const char *__doc_nanogui_Color_Color_4 =
R"doc(Copies (x, y, z) from the input vector, and sets the alpha of this
color to be ``1.0``.

Parameter ``color``:
    The three dimensional float vector being copied.)doc";

static const char *__doc_nanogui_Color_Color_5 =
R"doc(Copies (x, y, z) from the input vector, casting to floats and dividing
by ``255.0``. The alpha of this color will be set to ``1.0``.

Parameter ``color``:
    The three dimensional integer vector being copied, will be divided
    by ``255.0``.)doc";

static const char *__doc_nanogui_Color_Color_6 =
R"doc(Copies (x, y, z, w) from the input vector, casting to floats and
dividing by ``255.0``.

Parameter ``color``:
    The three dimensional integer vector being copied, will be divided
    by ``255.0``.)doc";

static const char *__doc_nanogui_Color_Color_7 =
R"doc(Creates the Color ``(intensity, intensity, intensity, alpha)``.

Parameter ``intensity``:
    The value to be used for red, green, and blue.

Parameter ``alpha``:
    The alpha component of the color.)doc";

static const char *__doc_nanogui_Color_Color_8 =
R"doc(Creates the Color ``(intensity, intensity, intensity, alpha) /
255.0``. Values are casted to floats before division.

Parameter ``intensity``:
    The value to be used for red, green, and blue, will be divided by
    ``255.0``.

Parameter ``alpha``:
    The alpha component of the color, will be divided by ``255.0``.)doc";

static const char *__doc_nanogui_Color_Color_9 =
R"doc(Explicit constructor: creates the Color ``(r, g, b, a)``.

Parameter ``r``:
    The red component of the color.

Parameter ``g``:
    The green component of the color.

Parameter ``b``:
    The blue component of the color.

Parameter ``a``:
    The alpha component of the color.)doc";

static const char *__doc_nanogui_Color_b = R"doc(Return a reference to the blue channel)doc";

static const char *__doc_nanogui_Color_b_2 = R"doc(Return a reference to the blue channel (const version))doc";

static const char *__doc_nanogui_Color_contrasting_color =
R"doc(Computes the luminance as ``l = 0.299r + 0.587g + 0.144b + 0.0a``. If
the luminance is less than 0.5, white is returned. If the luminance is
greater than or equal to 0.5, black is returned. Both returns will
have an alpha component of 1.0.)doc";

static const char *__doc_nanogui_Color_g = R"doc(Return a reference to the green channel)doc";

static const char *__doc_nanogui_Color_g_2 = R"doc(Return a reference to the green channel (const version))doc";

static const char *__doc_nanogui_Color_operator_const_NVGcolor = R"doc(Allows for conversion between this Color and NanoVG's representation.)doc";

static const char *__doc_nanogui_Color_r = R"doc(Return a reference to the red channel)doc";

static const char *__doc_nanogui_Color_r_2 = R"doc(Return a reference to the red channel (const version))doc";

static const char *__doc_nanogui_ComboBox = R"doc(Simple combo box widget based on a popup button.)doc";

static const char *__doc_nanogui_ComboBox_ComboBox = R"doc(Create an empty combo box)doc";

static const char *__doc_nanogui_ComboBox_ComboBox_2 = R"doc(Create a new combo box with the given items)doc";

static const char *__doc_nanogui_ComboBox_ComboBox_3 =
R"doc(Create a new combo box with the given items, providing both short and
long descriptive labels for each item)doc";

static const char *__doc_nanogui_ComboBox_callback = R"doc()doc";

static const char *__doc_nanogui_ComboBox_items = R"doc()doc";

static const char *__doc_nanogui_ComboBox_items_short = R"doc()doc";

static const char *__doc_nanogui_ComboBox_m_callback = R"doc()doc";

static const char *__doc_nanogui_ComboBox_m_items = R"doc()doc";

static const char *__doc_nanogui_ComboBox_m_items_short = R"doc()doc";

static const char *__doc_nanogui_ComboBox_m_selected_index = R"doc()doc";

static const char *__doc_nanogui_ComboBox_scroll_event = R"doc()doc";

static const char *__doc_nanogui_ComboBox_selected_index = R"doc()doc";

static const char *__doc_nanogui_ComboBox_set_callback = R"doc()doc";

static const char *__doc_nanogui_ComboBox_set_items = R"doc()doc";

static const char *__doc_nanogui_ComboBox_set_items_2 = R"doc()doc";

static const char *__doc_nanogui_ComboBox_set_selected_index = R"doc()doc";

static const char *__doc_nanogui_Cursor = R"doc(Cursor shapes available to use in GLFW.)doc";

static const char *__doc_nanogui_Cursor_Arrow = R"doc()doc";

static const char *__doc_nanogui_Cursor_Crosshair = R"doc()doc";

static const char *__doc_nanogui_Cursor_CursorCount =
R"doc(Not a cursor --- should always be last: enables a loop over the cursor
types.)doc";

static const char *__doc_nanogui_Cursor_HResize = R"doc()doc";

static const char *__doc_nanogui_Cursor_Hand = R"doc()doc";

static const char *__doc_nanogui_Cursor_IBeam = R"doc()doc";

static const char *__doc_nanogui_Cursor_VResize = R"doc()doc";

static const char *__doc_nanogui_FloatBox =
R"doc(A specialization of TextBox representing floating point values.

Template parameters should be float types, e.g. ``float``, ``double``,
``float64_t``, etc.)doc";

static const char *__doc_nanogui_FloatBox_FloatBox = R"doc()doc";

static const char *__doc_nanogui_FloatBox_m_max_value = R"doc()doc";

static const char *__doc_nanogui_FloatBox_m_min_value = R"doc()doc";

static const char *__doc_nanogui_FloatBox_m_mouse_down_value = R"doc()doc";

static const char *__doc_nanogui_FloatBox_m_number_format = R"doc()doc";

static const char *__doc_nanogui_FloatBox_m_value_increment = R"doc()doc";

static const char *__doc_nanogui_FloatBox_mouse_button_event = R"doc()doc";

static const char *__doc_nanogui_FloatBox_mouse_drag_event = R"doc()doc";

static const char *__doc_nanogui_FloatBox_number_format = R"doc()doc";

static const char *__doc_nanogui_FloatBox_number_format_2 = R"doc()doc";

static const char *__doc_nanogui_FloatBox_scroll_event = R"doc()doc";

static const char *__doc_nanogui_FloatBox_set_callback = R"doc()doc";

static const char *__doc_nanogui_FloatBox_set_max_value = R"doc()doc";

static const char *__doc_nanogui_FloatBox_set_min_max_values = R"doc()doc";

static const char *__doc_nanogui_FloatBox_set_min_value = R"doc()doc";

static const char *__doc_nanogui_FloatBox_set_value = R"doc()doc";

static const char *__doc_nanogui_FloatBox_set_value_increment = R"doc()doc";

static const char *__doc_nanogui_FloatBox_value = R"doc()doc";

static const char *__doc_nanogui_FormHelper =
R"doc(Convenience class to create simple AntTweakBar-style layouts that
expose variables of various types using NanoGUI widgets

**Example**:

```
// [ ... initialize NanoGUI, construct screen ... ]
FormHelper* h = new FormHelper(screen);
// Add a new windows widget
h->add_window(Vector2i(10,10),"Menu");
// Start a new group
h->add_group("Group 1");
// Expose an integer variable by reference
h->add_variable("integer variable", a_int);
// Expose a float variable via setter/getter functions
h->add_variable(
[&](float value) { a_float = value; },
[&]() { return *a_float; },
"float variable");
// add a new button
h->add_button("Button", [&]() { std::cout << "Button pressed" << std::endl; });

```)doc";

static const char *__doc_nanogui_FormHelper_FormHelper = R"doc(Create a helper class to construct NanoGUI widgets on the given screen)doc";

static const char *__doc_nanogui_FormHelper_add_button = R"doc(Add a button with a custom callback)doc";

static const char *__doc_nanogui_FormHelper_add_group = R"doc(Add a new group that may contain several sub-widgets)doc";

static const char *__doc_nanogui_FormHelper_add_variable = R"doc(Add a new data widget controlled using custom getter/setter functions)doc";

static const char *__doc_nanogui_FormHelper_add_variable_2 = R"doc(Add a new data widget that exposes a raw variable in memory)doc";

static const char *__doc_nanogui_FormHelper_add_widget = R"doc(Add an arbitrary (optionally labeled) widget to the layout)doc";

static const char *__doc_nanogui_FormHelper_add_window = R"doc(Add a new top-level window)doc";

static const char *__doc_nanogui_FormHelper_fixed_size = R"doc()doc";

static const char *__doc_nanogui_FormHelper_group_font_name = R"doc()doc";

static const char *__doc_nanogui_FormHelper_group_font_size = R"doc()doc";

static const char *__doc_nanogui_FormHelper_label_font_name = R"doc()doc";

static const char *__doc_nanogui_FormHelper_label_font_size = R"doc()doc";

static const char *__doc_nanogui_FormHelper_m_fixed_size = R"doc()doc";

static const char *__doc_nanogui_FormHelper_m_group_font_name = R"doc()doc";

static const char *__doc_nanogui_FormHelper_m_group_font_size = R"doc()doc";

static const char *__doc_nanogui_FormHelper_m_label_font_name = R"doc()doc";

static const char *__doc_nanogui_FormHelper_m_label_font_size = R"doc()doc";

static const char *__doc_nanogui_FormHelper_m_layout = R"doc()doc";

static const char *__doc_nanogui_FormHelper_m_post_group_spacing = R"doc()doc";

static const char *__doc_nanogui_FormHelper_m_pre_group_spacing = R"doc()doc";

static const char *__doc_nanogui_FormHelper_m_refresh_callbacks = R"doc()doc";

static const char *__doc_nanogui_FormHelper_m_screen = R"doc()doc";

static const char *__doc_nanogui_FormHelper_m_variable_spacing = R"doc()doc";

static const char *__doc_nanogui_FormHelper_m_widget_font_size = R"doc()doc";

static const char *__doc_nanogui_FormHelper_m_window = R"doc()doc";

static const char *__doc_nanogui_FormHelper_refresh = R"doc(Cause all widgets to re-synchronize with the underlying variable state)doc";

static const char *__doc_nanogui_FormHelper_set_fixed_size = R"doc(Specify a fixed size for newly added widgets)doc";

static const char *__doc_nanogui_FormHelper_set_group_font_name = R"doc()doc";

static const char *__doc_nanogui_FormHelper_set_group_font_size = R"doc()doc";

static const char *__doc_nanogui_FormHelper_set_label_font_name = R"doc()doc";

static const char *__doc_nanogui_FormHelper_set_label_font_size = R"doc()doc";

static const char *__doc_nanogui_FormHelper_set_widget_font_size = R"doc()doc";

static const char *__doc_nanogui_FormHelper_set_window = R"doc()doc";

static const char *__doc_nanogui_FormHelper_widget_font_size = R"doc()doc";

static const char *__doc_nanogui_FormHelper_window = R"doc(Access the currently active Window instance)doc";

static const char *__doc_nanogui_GLCanvas =
R"doc(Canvas widget for rendering OpenGL content

Canvas widget that can be used to display arbitrary OpenGL content.
This is useful to display and manipulate 3D objects as part of an
interactive application. The implementation uses scissoring to ensure
that rendered objects don't spill into neighboring widgets.

Usage: override `draw_gl` in subclasses to provide custom drawing
code.)doc";

static const char *__doc_nanogui_GLCanvas_GLCanvas = R"doc()doc";

static const char *__doc_nanogui_GLCanvas_background_color = R"doc(Return the background color)doc";

static const char *__doc_nanogui_GLCanvas_draw = R"doc(Draw the canvas)doc";

static const char *__doc_nanogui_GLCanvas_draw_border = R"doc(Return whether the widget border gets drawn or not)doc";

static const char *__doc_nanogui_GLCanvas_draw_gl = R"doc(Draw the GL scene. Override this method to draw the actual GL content.)doc";

static const char *__doc_nanogui_GLCanvas_draw_widget_border = R"doc(Internal helper function for drawing the widget border)doc";

static const char *__doc_nanogui_GLCanvas_m_background_color = R"doc()doc";

static const char *__doc_nanogui_GLCanvas_m_draw_border = R"doc()doc";

static const char *__doc_nanogui_GLCanvas_set_background_color = R"doc(Set the background color)doc";

static const char *__doc_nanogui_GLCanvas_set_draw_border = R"doc(Set whether to draw the widget border or not)doc";

static const char *__doc_nanogui_GLFramebuffer = R"doc(Helper class for creating framebuffer objects.)doc";

static const char *__doc_nanogui_GLFramebuffer_GLFramebuffer = R"doc(Default constructor: unusable until you call the ``init()`` method)doc";

static const char *__doc_nanogui_GLFramebuffer_bind = R"doc(Bind the framebuffer object)doc";

static const char *__doc_nanogui_GLFramebuffer_blit = R"doc(Blit the framebuffer object onto the screen)doc";

static const char *__doc_nanogui_GLFramebuffer_download_tga =
R"doc(Quick and dirty method to write a TGA (32bpp RGBA) file of the
framebuffer contents for debugging)doc";

static const char *__doc_nanogui_GLFramebuffer_free = R"doc(Release all associated resources)doc";

static const char *__doc_nanogui_GLFramebuffer_init =
R"doc(Create a new framebuffer with the specified size and number of MSAA
samples)doc";

static const char *__doc_nanogui_GLFramebuffer_m_color = R"doc()doc";

static const char *__doc_nanogui_GLFramebuffer_m_depth = R"doc()doc";

static const char *__doc_nanogui_GLFramebuffer_m_framebuffer = R"doc()doc";

static const char *__doc_nanogui_GLFramebuffer_m_samples = R"doc()doc";

static const char *__doc_nanogui_GLFramebuffer_m_size = R"doc()doc";

static const char *__doc_nanogui_GLFramebuffer_ready = R"doc(Return whether or not the framebuffer object has been initialized)doc";

static const char *__doc_nanogui_GLFramebuffer_release = R"doc(Release/unbind the framebuffer object)doc";

static const char *__doc_nanogui_GLFramebuffer_samples = R"doc(Return the number of MSAA samples)doc";

static const char *__doc_nanogui_GLShader =
R"doc(Helper class for compiling and linking OpenGL shaders and uploading
associated vertex and index buffers.)doc";

static const char *__doc_nanogui_GLShader_Buffer =
R"doc(A wrapper struct for maintaining various aspects of items being
managed by OpenGL.)doc";

static const char *__doc_nanogui_GLShader_Buffer_comp_size = R"doc()doc";

static const char *__doc_nanogui_GLShader_Buffer_dim = R"doc()doc";

static const char *__doc_nanogui_GLShader_Buffer_gl_type = R"doc()doc";

static const char *__doc_nanogui_GLShader_Buffer_id = R"doc()doc";

static const char *__doc_nanogui_GLShader_Buffer_size = R"doc()doc";

static const char *__doc_nanogui_GLShader_Buffer_version = R"doc()doc";

static const char *__doc_nanogui_GLShader_GLShader = R"doc(Create an unitialized OpenGL shader)doc";

static const char *__doc_nanogui_GLShader_attrib =
R"doc(Return the handle of a named shader attribute (-1 if it does not
exist))doc";

static const char *__doc_nanogui_GLShader_attrib_size = R"doc(Return the size of the a given vertex buffer)doc";

static const char *__doc_nanogui_GLShader_attrib_version = R"doc(Return the version number of a given attribute)doc";

static const char *__doc_nanogui_GLShader_bind = R"doc(Select this shader for subsequent draw calls)doc";

static const char *__doc_nanogui_GLShader_buffer_size = R"doc(Return the size of all registered buffers in bytes)doc";

static const char *__doc_nanogui_GLShader_define = R"doc(Set a preprocessor definition)doc";

static const char *__doc_nanogui_GLShader_download_attrib = R"doc(Download a vertex buffer object to CPU memory)doc";

static const char *__doc_nanogui_GLShader_draw_array = R"doc(Draw a sequence of primitives)doc";

static const char *__doc_nanogui_GLShader_draw_indexed = R"doc(Draw a sequence of primitives using a previously uploaded index buffer)doc";

static const char *__doc_nanogui_GLShader_free = R"doc(Release underlying OpenGL objects)doc";

static const char *__doc_nanogui_GLShader_free_attrib = R"doc(Completely free an existing attribute buffer)doc";

static const char *__doc_nanogui_GLShader_has_attrib = R"doc(Check if an attribute was registered a given name)doc";

static const char *__doc_nanogui_GLShader_init =
R"doc(Initialize the shader using the specified source strings.

Parameter ``name``:
    The name this shader will be registered as.

Parameter ``vertex_str``:
    The source of the vertex shader as a string.

Parameter ``fragment_str``:
    The source of the fragment shader as a string.

Parameter ``geometry_str``:
    The source of the geometry shader as a string. The default value
    is the empty string, which indicates no geometry shader will be
    used.)doc";

static const char *__doc_nanogui_GLShader_init_from_files =
R"doc(Initialize the shader using the specified files on disk.

Parameter ``name``:
    The name this shader will be registered as.

Parameter ``vertex_fname``:
    The path to the file containing the source of the fragment shader.

Parameter ``fragment_fname``:
    The path to the file containing the source of the vertex shader.

Parameter ``geometry_fname``:
    The path to the file containing the source of the geometry shader.
    The default value is the empty string, which indicates no geometry
    shader will be used.)doc";

static const char *__doc_nanogui_GLShader_invalidate_attribs = R"doc(Invalidate the version numbers associated with attribute data)doc";

static const char *__doc_nanogui_GLShader_m_buffer_objects = R"doc()doc";

static const char *__doc_nanogui_GLShader_m_definitions = R"doc()doc";

static const char *__doc_nanogui_GLShader_m_fragment_shader = R"doc()doc";

static const char *__doc_nanogui_GLShader_m_geometry_shader = R"doc()doc";

static const char *__doc_nanogui_GLShader_m_name = R"doc()doc";

static const char *__doc_nanogui_GLShader_m_program_shader = R"doc()doc";

static const char *__doc_nanogui_GLShader_m_vertex_array_object = R"doc()doc";

static const char *__doc_nanogui_GLShader_m_vertex_shader = R"doc()doc";

static const char *__doc_nanogui_GLShader_name = R"doc(Return the name of the shader)doc";

static const char *__doc_nanogui_GLShader_reset_attrib_version = R"doc(Reset the version number of a given attribute)doc";

static const char *__doc_nanogui_GLShader_set_uniform = R"doc(Initialize a uniform parameter with a scalar value)doc";

static const char *__doc_nanogui_GLShader_set_uniform_2 = R"doc(Initialize a uniform parameter with a 2D vector)doc";

static const char *__doc_nanogui_GLShader_set_uniform_3 = R"doc(Initialize a uniform parameter with a 3D vector)doc";

static const char *__doc_nanogui_GLShader_set_uniform_4 = R"doc(Initialize a uniform parameter with a 4D vector)doc";

static const char *__doc_nanogui_GLShader_set_uniform_5 = R"doc(Initialize a uniform parameter with a 4x4 matrix (float))doc";

static const char *__doc_nanogui_GLShader_share_attrib =
R"doc(Create a symbolic link to an attribute of another GLShader. This
avoids duplicating unnecessary data)doc";

static const char *__doc_nanogui_GLShader_uniform = R"doc(Return the handle of a uniform attribute (-1 if it does not exist))doc";

static const char *__doc_nanogui_GLShader_upload_attrib =
R"doc(Upload a memory region as a vertex buffer object (refreshing it as
needed))doc";

static const char *__doc_nanogui_GLShader_upload_attrib_2 = R"doc(Low-level version of upload_attrib)doc";

static const char *__doc_nanogui_GLShader_upload_indices = R"doc(Upload an index buffer)doc";

static const char *__doc_nanogui_Graph = R"doc(Simple graph widget for showing a function plot.)doc";

static const char *__doc_nanogui_Graph_Graph = R"doc()doc";

static const char *__doc_nanogui_Graph_background_color = R"doc()doc";

static const char *__doc_nanogui_Graph_caption = R"doc()doc";

static const char *__doc_nanogui_Graph_draw = R"doc()doc";

static const char *__doc_nanogui_Graph_footer = R"doc()doc";

static const char *__doc_nanogui_Graph_foreground_color = R"doc()doc";

static const char *__doc_nanogui_Graph_header = R"doc()doc";

static const char *__doc_nanogui_Graph_m_background_color = R"doc()doc";

static const char *__doc_nanogui_Graph_m_caption = R"doc()doc";

static const char *__doc_nanogui_Graph_m_footer = R"doc()doc";

static const char *__doc_nanogui_Graph_m_foreground_color = R"doc()doc";

static const char *__doc_nanogui_Graph_m_header = R"doc()doc";

static const char *__doc_nanogui_Graph_m_text_color = R"doc()doc";

static const char *__doc_nanogui_Graph_m_values = R"doc()doc";

static const char *__doc_nanogui_Graph_preferred_size = R"doc()doc";

static const char *__doc_nanogui_Graph_set_background_color = R"doc()doc";

static const char *__doc_nanogui_Graph_set_caption = R"doc()doc";

static const char *__doc_nanogui_Graph_set_footer = R"doc()doc";

static const char *__doc_nanogui_Graph_set_foreground_color = R"doc()doc";

static const char *__doc_nanogui_Graph_set_header = R"doc()doc";

static const char *__doc_nanogui_Graph_set_text_color = R"doc()doc";

static const char *__doc_nanogui_Graph_set_values = R"doc()doc";

static const char *__doc_nanogui_Graph_text_color = R"doc()doc";

static const char *__doc_nanogui_Graph_values = R"doc()doc";

static const char *__doc_nanogui_Graph_values_2 = R"doc()doc";

static const char *__doc_nanogui_GridLayout =
R"doc(Grid layout.

Widgets are arranged in a grid that has a fixed grid resolution
``resolution`` along one of the axes. The layout orientation indicates
the fixed dimension; widgets are also appended on this axis. The
spacing between items can be specified per axis. The
horizontal/vertical alignment can be specified per row and column.)doc";

static const char *__doc_nanogui_GridLayout_GridLayout = R"doc(Create a 2-column grid layout by default)doc";

static const char *__doc_nanogui_GridLayout_alignment = R"doc()doc";

static const char *__doc_nanogui_GridLayout_compute_layout = R"doc()doc";

static const char *__doc_nanogui_GridLayout_m_alignment = R"doc()doc";

static const char *__doc_nanogui_GridLayout_m_default_alignment = R"doc()doc";

static const char *__doc_nanogui_GridLayout_m_margin = R"doc()doc";

static const char *__doc_nanogui_GridLayout_m_orientation = R"doc()doc";

static const char *__doc_nanogui_GridLayout_m_resolution = R"doc()doc";

static const char *__doc_nanogui_GridLayout_m_spacing = R"doc()doc";

static const char *__doc_nanogui_GridLayout_margin = R"doc()doc";

static const char *__doc_nanogui_GridLayout_orientation = R"doc()doc";

static const char *__doc_nanogui_GridLayout_perform_layout = R"doc()doc";

static const char *__doc_nanogui_GridLayout_preferred_size = R"doc()doc";

static const char *__doc_nanogui_GridLayout_resolution = R"doc()doc";

static const char *__doc_nanogui_GridLayout_set_col_alignment = R"doc()doc";

static const char *__doc_nanogui_GridLayout_set_col_alignment_2 = R"doc()doc";

static const char *__doc_nanogui_GridLayout_set_margin = R"doc()doc";

static const char *__doc_nanogui_GridLayout_set_orientation = R"doc()doc";

static const char *__doc_nanogui_GridLayout_set_resolution = R"doc()doc";

static const char *__doc_nanogui_GridLayout_set_row_alignment = R"doc()doc";

static const char *__doc_nanogui_GridLayout_set_row_alignment_2 = R"doc()doc";

static const char *__doc_nanogui_GridLayout_set_spacing = R"doc()doc";

static const char *__doc_nanogui_GridLayout_set_spacing_2 = R"doc()doc";

static const char *__doc_nanogui_GridLayout_spacing = R"doc()doc";

static const char *__doc_nanogui_GroupLayout =
R"doc(Special layout for widgets grouped by labels.

This widget resembles a box layout in that it arranges a set of
widgets vertically. All widgets are indented on the horizontal axis
except for Label widgets, which are not indented.

This creates a pleasing layout where a number of widgets are grouped
under some high-level heading.)doc";

static const char *__doc_nanogui_GroupLayout_GroupLayout = R"doc()doc";

static const char *__doc_nanogui_GroupLayout_group_indent = R"doc()doc";

static const char *__doc_nanogui_GroupLayout_group_spacing = R"doc()doc";

static const char *__doc_nanogui_GroupLayout_m_group_indent = R"doc()doc";

static const char *__doc_nanogui_GroupLayout_m_group_spacing = R"doc()doc";

static const char *__doc_nanogui_GroupLayout_m_margin = R"doc()doc";

static const char *__doc_nanogui_GroupLayout_m_spacing = R"doc()doc";

static const char *__doc_nanogui_GroupLayout_margin = R"doc()doc";

static const char *__doc_nanogui_GroupLayout_perform_layout = R"doc()doc";

static const char *__doc_nanogui_GroupLayout_preferred_size = R"doc()doc";

static const char *__doc_nanogui_GroupLayout_set_group_indent = R"doc()doc";

static const char *__doc_nanogui_GroupLayout_set_group_spacing = R"doc()doc";

static const char *__doc_nanogui_GroupLayout_set_margin = R"doc()doc";

static const char *__doc_nanogui_GroupLayout_set_spacing = R"doc()doc";

static const char *__doc_nanogui_GroupLayout_spacing = R"doc()doc";

static const char *__doc_nanogui_ImagePanel = R"doc(Image panel widget which shows a number of square-shaped icons.)doc";

static const char *__doc_nanogui_ImagePanel_ImagePanel = R"doc()doc";

static const char *__doc_nanogui_ImagePanel_callback = R"doc()doc";

static const char *__doc_nanogui_ImagePanel_draw = R"doc()doc";

static const char *__doc_nanogui_ImagePanel_grid_size = R"doc()doc";

static const char *__doc_nanogui_ImagePanel_images = R"doc()doc";

static const char *__doc_nanogui_ImagePanel_index_for_position = R"doc()doc";

static const char *__doc_nanogui_ImagePanel_m_callback = R"doc()doc";

static const char *__doc_nanogui_ImagePanel_m_images = R"doc()doc";

static const char *__doc_nanogui_ImagePanel_m_margin = R"doc()doc";

static const char *__doc_nanogui_ImagePanel_m_mouse_index = R"doc()doc";

static const char *__doc_nanogui_ImagePanel_m_spacing = R"doc()doc";

static const char *__doc_nanogui_ImagePanel_m_thumb_size = R"doc()doc";

static const char *__doc_nanogui_ImagePanel_mouse_button_event = R"doc()doc";

static const char *__doc_nanogui_ImagePanel_mouse_motion_event = R"doc()doc";

static const char *__doc_nanogui_ImagePanel_preferred_size = R"doc()doc";

static const char *__doc_nanogui_ImagePanel_set_callback = R"doc()doc";

static const char *__doc_nanogui_ImagePanel_set_images = R"doc()doc";

static const char *__doc_nanogui_ImageView = R"doc(Widget used to display images.)doc";

static const char *__doc_nanogui_ImageView_ImageView = R"doc()doc";

static const char *__doc_nanogui_ImageView_bind_image = R"doc()doc";

static const char *__doc_nanogui_ImageView_center = R"doc(Centers the image without affecting the scaling factor.)doc";

static const char *__doc_nanogui_ImageView_clamped_image_coordinate_at =
R"doc(Calculates the image coordinates of the given pixel position on the
widget. If the position provided corresponds to a coordinate outside
the range of the image, the coordinates are clamped to edges of the
image.)doc";

static const char *__doc_nanogui_ImageView_draw = R"doc()doc";

static const char *__doc_nanogui_ImageView_draw_helpers = R"doc()doc";

static const char *__doc_nanogui_ImageView_draw_image_border = R"doc()doc";

static const char *__doc_nanogui_ImageView_draw_pixel_grid = R"doc()doc";

static const char *__doc_nanogui_ImageView_draw_pixel_info = R"doc()doc";

static const char *__doc_nanogui_ImageView_draw_widget_border = R"doc()doc";

static const char *__doc_nanogui_ImageView_fit = R"doc(Centers and scales the image so that it fits inside the widgets.)doc";

static const char *__doc_nanogui_ImageView_fixed_offset = R"doc()doc";

static const char *__doc_nanogui_ImageView_fixed_scale = R"doc()doc";

static const char *__doc_nanogui_ImageView_font_scale_factor = R"doc()doc";

static const char *__doc_nanogui_ImageView_grid_threshold = R"doc()doc";

static const char *__doc_nanogui_ImageView_grid_visible = R"doc(Function indicating whether the grid is currently visible.)doc";

static const char *__doc_nanogui_ImageView_helpers_visible = R"doc(Function indicating whether any of the overlays are visible.)doc";

static const char *__doc_nanogui_ImageView_image_coordinate_at =
R"doc(Calculates the image coordinates of the given pixel position on the
widget.)doc";

static const char *__doc_nanogui_ImageView_image_shader = R"doc()doc";

static const char *__doc_nanogui_ImageView_image_size = R"doc()doc";

static const char *__doc_nanogui_ImageView_image_size_f = R"doc()doc";

static const char *__doc_nanogui_ImageView_keyboard_character_event = R"doc()doc";

static const char *__doc_nanogui_ImageView_keyboard_event = R"doc()doc";

static const char *__doc_nanogui_ImageView_m_fixed_offset = R"doc()doc";

static const char *__doc_nanogui_ImageView_m_fixed_scale = R"doc()doc";

static const char *__doc_nanogui_ImageView_m_font_scale_factor = R"doc()doc";

static const char *__doc_nanogui_ImageView_m_grid_threshold = R"doc()doc";

static const char *__doc_nanogui_ImageView_m_image_id = R"doc()doc";

static const char *__doc_nanogui_ImageView_m_image_size = R"doc()doc";

static const char *__doc_nanogui_ImageView_m_offset = R"doc()doc";

static const char *__doc_nanogui_ImageView_m_pixel_info_callback = R"doc()doc";

static const char *__doc_nanogui_ImageView_m_pixel_info_threshold = R"doc()doc";

static const char *__doc_nanogui_ImageView_m_scale = R"doc()doc";

static const char *__doc_nanogui_ImageView_m_shader = R"doc()doc";

static const char *__doc_nanogui_ImageView_m_zoom_sensitivity = R"doc()doc";

static const char *__doc_nanogui_ImageView_mouse_drag_event = R"doc()doc";

static const char *__doc_nanogui_ImageView_move_offset = R"doc(Moves the offset by the specified amount. Does bound checking.)doc";

static const char *__doc_nanogui_ImageView_offset = R"doc()doc";

static const char *__doc_nanogui_ImageView_perform_layout = R"doc()doc";

static const char *__doc_nanogui_ImageView_pixel_info_callback = R"doc()doc";

static const char *__doc_nanogui_ImageView_pixel_info_threshold = R"doc()doc";

static const char *__doc_nanogui_ImageView_pixel_info_visible =
R"doc(Function indicating whether the pixel information is currently
visible.)doc";

static const char *__doc_nanogui_ImageView_position_f = R"doc()doc";

static const char *__doc_nanogui_ImageView_position_for_coordinate =
R"doc(Calculates the position inside the widget for the given image
coordinate.)doc";

static const char *__doc_nanogui_ImageView_preferred_size = R"doc()doc";

static const char *__doc_nanogui_ImageView_scale = R"doc()doc";

static const char *__doc_nanogui_ImageView_scaled_image_size = R"doc()doc";

static const char *__doc_nanogui_ImageView_scaled_image_size_f = R"doc()doc";

static const char *__doc_nanogui_ImageView_scroll_event = R"doc()doc";

static const char *__doc_nanogui_ImageView_set_fixed_offset = R"doc()doc";

static const char *__doc_nanogui_ImageView_set_fixed_scale = R"doc()doc";

static const char *__doc_nanogui_ImageView_set_font_scale_factor = R"doc()doc";

static const char *__doc_nanogui_ImageView_set_grid_threshold = R"doc()doc";

static const char *__doc_nanogui_ImageView_set_image_coordinate_at =
R"doc(Modifies the internal state of the image viewer widget so that the
pixel at the provided position on the widget has the specified image
coordinate. Also clamps the values of offset to the sides of the
widget.)doc";

static const char *__doc_nanogui_ImageView_set_offset = R"doc()doc";

static const char *__doc_nanogui_ImageView_set_pixel_info_callback = R"doc()doc";

static const char *__doc_nanogui_ImageView_set_pixel_info_threshold = R"doc()doc";

static const char *__doc_nanogui_ImageView_set_scale = R"doc()doc";

static const char *__doc_nanogui_ImageView_set_scale_centered = R"doc(Set the scale while keeping the image centered)doc";

static const char *__doc_nanogui_ImageView_set_zoom_sensitivity = R"doc()doc";

static const char *__doc_nanogui_ImageView_size_f = R"doc()doc";

static const char *__doc_nanogui_ImageView_update_image_parameters = R"doc()doc";

static const char *__doc_nanogui_ImageView_write_pixel_info = R"doc()doc";

static const char *__doc_nanogui_ImageView_zoom =
R"doc(Changes the scale factor by the provided amount modified by the zoom
sensitivity member variable. The scaling occurs such that the image
coordinate under the focused position remains in the same position
before and after the scaling.)doc";

static const char *__doc_nanogui_ImageView_zoom_sensitivity = R"doc()doc";

static const char *__doc_nanogui_IntBox =
R"doc(A specialization of TextBox for representing integral values.

Template parameters should be integral types, e.g. ``int``, ``long``,
``uint32_t``, etc.)doc";

static const char *__doc_nanogui_IntBox_IntBox = R"doc()doc";

static const char *__doc_nanogui_IntBox_m_max_value = R"doc()doc";

static const char *__doc_nanogui_IntBox_m_min_value = R"doc()doc";

static const char *__doc_nanogui_IntBox_m_mouse_down_value = R"doc()doc";

static const char *__doc_nanogui_IntBox_m_value_increment = R"doc()doc";

static const char *__doc_nanogui_IntBox_mouse_button_event = R"doc()doc";

static const char *__doc_nanogui_IntBox_mouse_drag_event = R"doc()doc";

static const char *__doc_nanogui_IntBox_scroll_event = R"doc()doc";

static const char *__doc_nanogui_IntBox_set_callback = R"doc()doc";

static const char *__doc_nanogui_IntBox_set_max_value = R"doc()doc";

static const char *__doc_nanogui_IntBox_set_min_max_values = R"doc()doc";

static const char *__doc_nanogui_IntBox_set_min_value = R"doc()doc";

static const char *__doc_nanogui_IntBox_set_value = R"doc()doc";

static const char *__doc_nanogui_IntBox_set_value_increment = R"doc()doc";

static const char *__doc_nanogui_IntBox_value = R"doc()doc";

static const char *__doc_nanogui_Label =
R"doc(Text label widget.

The font and color can be customized. When Widget::set_fixed_width()
is used, the text is wrapped when it surpasses the specified width.)doc";

static const char *__doc_nanogui_Label_Label = R"doc()doc";

static const char *__doc_nanogui_Label_caption = R"doc(Get the label's text caption)doc";

static const char *__doc_nanogui_Label_color = R"doc(Get the label color)doc";

static const char *__doc_nanogui_Label_draw = R"doc(Draw the label)doc";

static const char *__doc_nanogui_Label_font = R"doc(Get the currently active font)doc";

static const char *__doc_nanogui_Label_m_caption = R"doc()doc";

static const char *__doc_nanogui_Label_m_color = R"doc()doc";

static const char *__doc_nanogui_Label_m_font = R"doc()doc";

static const char *__doc_nanogui_Label_preferred_size = R"doc(Compute the size needed to fully display the label)doc";

static const char *__doc_nanogui_Label_set_caption = R"doc(Set the label's text caption)doc";

static const char *__doc_nanogui_Label_set_color = R"doc(Set the label color)doc";

static const char *__doc_nanogui_Label_set_font =
R"doc(Set the currently active font (2 are available by default: 'sans' and
'sans-bold'))doc";

static const char *__doc_nanogui_Label_set_theme = R"doc(Set the Theme used to draw this widget)doc";

static const char *__doc_nanogui_Layout = R"doc(Basic interface of a layout engine.)doc";

static const char *__doc_nanogui_Layout_perform_layout = R"doc()doc";

static const char *__doc_nanogui_Layout_preferred_size = R"doc()doc";

static const char *__doc_nanogui_MessageDialog = R"doc(Simple "OK" or "Yes/No"-style modal dialogs.)doc";

static const char *__doc_nanogui_MessageDialog_MessageDialog = R"doc()doc";

static const char *__doc_nanogui_MessageDialog_Type = R"doc(Classification of the type of message this MessageDialog represents.)doc";

static const char *__doc_nanogui_MessageDialog_Type_Information = R"doc()doc";

static const char *__doc_nanogui_MessageDialog_Type_Question = R"doc()doc";

static const char *__doc_nanogui_MessageDialog_Type_Warning = R"doc()doc";

static const char *__doc_nanogui_MessageDialog_callback = R"doc()doc";

static const char *__doc_nanogui_MessageDialog_m_callback = R"doc()doc";

static const char *__doc_nanogui_MessageDialog_m_message_label = R"doc()doc";

static const char *__doc_nanogui_MessageDialog_message_label = R"doc()doc";

static const char *__doc_nanogui_MessageDialog_message_label_2 = R"doc()doc";

static const char *__doc_nanogui_MessageDialog_set_callback = R"doc()doc";

static const char *__doc_nanogui_Object = R"doc(Reference counted object base class.)doc";

static const char *__doc_nanogui_Object_Object = R"doc(Default constructor)doc";

static const char *__doc_nanogui_Object_Object_2 = R"doc(Copy constructor)doc";

static const char *__doc_nanogui_Object_dec_ref =
R"doc(Decrease the reference count of the object and possibly deallocate it.

The object will automatically be deallocated once the reference count
reaches zero.)doc";

static const char *__doc_nanogui_Object_get_ref_count = R"doc(Return the current reference count)doc";

static const char *__doc_nanogui_Object_inc_ref = R"doc(Increase the object's reference count by one)doc";

static const char *__doc_nanogui_Object_m_ref_count = R"doc()doc";

static const char *__doc_nanogui_Object_operator_delete = R"doc()doc";

static const char *__doc_nanogui_Object_operator_delete_2 = R"doc()doc";

static const char *__doc_nanogui_Object_operator_delete_3 = R"doc()doc";

static const char *__doc_nanogui_Object_operator_delete_4 = R"doc()doc";

static const char *__doc_nanogui_Object_operator_delete_5 = R"doc()doc";

static const char *__doc_nanogui_Object_operator_delete_6 = R"doc()doc";

static const char *__doc_nanogui_Object_operator_new = R"doc()doc";

static const char *__doc_nanogui_Object_operator_new_2 = R"doc()doc";

static const char *__doc_nanogui_Object_operator_new_3 = R"doc()doc";

static const char *__doc_nanogui_Object_operator_new_4 = R"doc()doc";

static const char *__doc_nanogui_Orientation = R"doc(The direction of data flow for a layout.)doc";

static const char *__doc_nanogui_Orientation_Horizontal = R"doc()doc";

static const char *__doc_nanogui_Orientation_Vertical = R"doc()doc";

static const char *__doc_nanogui_Popup =
R"doc(Popup window for combo boxes, popup buttons, nested dialogs etc.

Usually the Popup instance is constructed by another widget (e.g.
PopupButton) and does not need to be created by hand.)doc";

static const char *__doc_nanogui_PopupButton = R"doc(Button which launches a popup widget.)doc";

static const char *__doc_nanogui_PopupButton_PopupButton = R"doc()doc";

static const char *__doc_nanogui_PopupButton_chevron_icon = R"doc()doc";

static const char *__doc_nanogui_PopupButton_draw = R"doc()doc";

static const char *__doc_nanogui_PopupButton_m_chevron_icon = R"doc()doc";

static const char *__doc_nanogui_PopupButton_m_popup = R"doc()doc";

static const char *__doc_nanogui_PopupButton_perform_layout = R"doc()doc";

static const char *__doc_nanogui_PopupButton_popup = R"doc()doc";

static const char *__doc_nanogui_PopupButton_popup_2 = R"doc()doc";

static const char *__doc_nanogui_PopupButton_preferred_size = R"doc()doc";

static const char *__doc_nanogui_PopupButton_set_chevron_icon = R"doc()doc";

static const char *__doc_nanogui_PopupButton_set_side = R"doc()doc";

static const char *__doc_nanogui_PopupButton_side = R"doc()doc";

static const char *__doc_nanogui_Popup_Popup =
R"doc(Create a new popup parented to a screen (first argument) and a parent
window)doc";

static const char *__doc_nanogui_Popup_Side = R"doc()doc";

static const char *__doc_nanogui_Popup_Side_Left = R"doc()doc";

static const char *__doc_nanogui_Popup_Side_Right = R"doc()doc";

static const char *__doc_nanogui_Popup_anchor_height =
R"doc(Return the anchor height; this determines the vertical shift relative
to the anchor position)doc";

static const char *__doc_nanogui_Popup_anchor_pos =
R"doc(Set the anchor position in the parent window; the placement of the
popup is relative to it)doc";

static const char *__doc_nanogui_Popup_draw = R"doc(Draw the popup window)doc";

static const char *__doc_nanogui_Popup_m_anchor_height = R"doc()doc";

static const char *__doc_nanogui_Popup_m_anchor_pos = R"doc()doc";

static const char *__doc_nanogui_Popup_m_parent_window = R"doc()doc";

static const char *__doc_nanogui_Popup_m_side = R"doc()doc";

static const char *__doc_nanogui_Popup_parent_window = R"doc(Return the parent window of the popup)doc";

static const char *__doc_nanogui_Popup_parent_window_2 = R"doc(Return the parent window of the popup)doc";

static const char *__doc_nanogui_Popup_perform_layout =
R"doc(Invoke the associated layout generator to properly place child
widgets, if any)doc";

static const char *__doc_nanogui_Popup_refresh_relative_placement = R"doc(Internal helper function to maintain nested window position values)doc";

static const char *__doc_nanogui_Popup_set_anchor_height =
R"doc(Set the anchor height; this determines the vertical shift relative to
the anchor position)doc";

static const char *__doc_nanogui_Popup_set_anchor_pos =
R"doc(Return the anchor position in the parent window; the placement of the
popup is relative to it)doc";

static const char *__doc_nanogui_Popup_set_side = R"doc(Set the side of the parent window at which popup will appear)doc";

static const char *__doc_nanogui_Popup_side = R"doc(Return the side of the parent window at which popup will appear)doc";

static const char *__doc_nanogui_ProgressBar = R"doc(Standard widget for visualizing progress.)doc";

static const char *__doc_nanogui_ProgressBar_ProgressBar = R"doc()doc";

static const char *__doc_nanogui_ProgressBar_draw = R"doc()doc";

static const char *__doc_nanogui_ProgressBar_m_value = R"doc()doc";

static const char *__doc_nanogui_ProgressBar_preferred_size = R"doc()doc";

static const char *__doc_nanogui_ProgressBar_set_value = R"doc()doc";

static const char *__doc_nanogui_ProgressBar_value = R"doc()doc";

static const char *__doc_nanogui_Screen =
R"doc(Represents a display surface (i.e. a full-screen or windowed GLFW
window) and forms the root element of a hierarchy of nanogui widgets.)doc";

static const char *__doc_nanogui_Screen_Screen =
R"doc(Create a new Screen instance

Parameter ``size``:
    Size in pixels at 96 dpi (on high-DPI screens, the actual
    resolution in terms of hardware pixels may be larger by an integer
    factor)

Parameter ``caption``:
    Window title (in UTF-8 encoding)

Parameter ``resizable``:
    If creating a window, should it be resizable?

Parameter ``fullscreen``:
    Specifies whether to create a windowed or full-screen view

Parameter ``color_bits``:
    Number of bits per pixel dedicated to the R/G/B color components

Parameter ``alpha_bits``:
    Number of bits per pixel dedicated to the alpha channel

Parameter ``depth_bits``:
    Number of bits per pixel dedicated to the Z-buffer

Parameter ``stencil_bits``:
    Number of bits per pixel dedicated to the stencil buffer
    (recommended to set this to 8. NanoVG can draw higher-quality
    strokes using a stencil buffer)

Parameter ``n_samples``:
    Number of MSAA samples (set to 0 to disable)

Parameter ``gl_major``:
    The requested OpenGL Major version number. Default is 3, if
    changed the value must correspond to a forward compatible core
    profile (for portability reasons). For example, set this to 4 and
    gl_minor to 1 for a forward compatible core OpenGL 4.1 profile.
    Requesting an invalid profile will result in no context (and
    therefore no GUI) being created.

Parameter ``gl_minor``:
    The requested OpenGL Minor version number. Default is 3, if
    changed the value must correspond to a forward compatible core
    profile (for portability reasons). For example, set this to 1 and
    gl_major to 4 for a forward compatible core OpenGL 4.1 profile.
    Requesting an invalid profile will result in no context (and
    therefore no GUI) being created.)doc";

static const char *__doc_nanogui_Screen_Screen_2 =
R"doc(Default constructor

Performs no initialization at all. Use this if the application is
responsible for setting up GLFW, OpenGL, etc.

In this case, override Screen and call initalize() with a pointer to
an existing ``GLFWwindow`` instance

You will also be responsible in this case to deliver GLFW callbacks to
the appropriate callback event handlers below)doc";

static const char *__doc_nanogui_Screen_background = R"doc(Return the screen's background color)doc";

static const char *__doc_nanogui_Screen_caption = R"doc(Get the window title bar caption)doc";

static const char *__doc_nanogui_Screen_center_window = R"doc()doc";

static const char *__doc_nanogui_Screen_char_callback_event = R"doc()doc";

static const char *__doc_nanogui_Screen_cursor_pos_callback_event = R"doc()doc";

static const char *__doc_nanogui_Screen_dispose_window = R"doc()doc";

static const char *__doc_nanogui_Screen_draw_all = R"doc(Draw the Screen contents)doc";

static const char *__doc_nanogui_Screen_draw_contents = R"doc(Draw the window contents --- put your OpenGL draw calls here)doc";

static const char *__doc_nanogui_Screen_draw_widgets = R"doc()doc";

static const char *__doc_nanogui_Screen_drop_callback_event = R"doc()doc";

static const char *__doc_nanogui_Screen_drop_event = R"doc(Handle a file drop event)doc";

static const char *__doc_nanogui_Screen_glfw_window = R"doc(Return a pointer to the underlying GLFW window data structure)doc";

static const char *__doc_nanogui_Screen_initialize = R"doc(Initialize the Screen)doc";

static const char *__doc_nanogui_Screen_key_callback_event = R"doc()doc";

static const char *__doc_nanogui_Screen_keyboard_character_event = R"doc(Text input event handler: codepoint is native endian UTF-32 format)doc";

static const char *__doc_nanogui_Screen_keyboard_event = R"doc(Default keyboard event handler)doc";

static const char *__doc_nanogui_Screen_m_background = R"doc()doc";

static const char *__doc_nanogui_Screen_m_caption = R"doc()doc";

static const char *__doc_nanogui_Screen_m_cursor = R"doc()doc";

static const char *__doc_nanogui_Screen_m_cursors = R"doc()doc";

static const char *__doc_nanogui_Screen_m_drag_active = R"doc()doc";

static const char *__doc_nanogui_Screen_m_drag_widget = R"doc()doc";

static const char *__doc_nanogui_Screen_m_fbsize = R"doc()doc";

static const char *__doc_nanogui_Screen_m_focus_path = R"doc()doc";

static const char *__doc_nanogui_Screen_m_fullscreen = R"doc()doc";

static const char *__doc_nanogui_Screen_m_glfw_window = R"doc()doc";

static const char *__doc_nanogui_Screen_m_last_interaction = R"doc()doc";

static const char *__doc_nanogui_Screen_m_modifiers = R"doc()doc";

static const char *__doc_nanogui_Screen_m_mouse_pos = R"doc()doc";

static const char *__doc_nanogui_Screen_m_mouse_state = R"doc()doc";

static const char *__doc_nanogui_Screen_m_nvg_context = R"doc()doc";

static const char *__doc_nanogui_Screen_m_pixel_ratio = R"doc()doc";

static const char *__doc_nanogui_Screen_m_process_events = R"doc()doc";

static const char *__doc_nanogui_Screen_m_resize_callback = R"doc()doc";

static const char *__doc_nanogui_Screen_m_shutdown_glfw = R"doc()doc";

static const char *__doc_nanogui_Screen_mouse_button_callback_event = R"doc()doc";

static const char *__doc_nanogui_Screen_mouse_pos = R"doc(Return the last observed mouse position value)doc";

static const char *__doc_nanogui_Screen_move_window_to_front = R"doc()doc";

static const char *__doc_nanogui_Screen_nvg_context = R"doc(Return a pointer to the underlying NanoVG draw context)doc";

static const char *__doc_nanogui_Screen_perform_layout = R"doc(Compute the layout of all widgets)doc";

static const char *__doc_nanogui_Screen_pixel_ratio =
R"doc(Return the ratio between pixel and device coordinates (e.g. >= 2 on
Mac Retina displays))doc";

static const char *__doc_nanogui_Screen_resize_callback = R"doc(Set the resize callback)doc";

static const char *__doc_nanogui_Screen_resize_callback_event = R"doc()doc";

static const char *__doc_nanogui_Screen_resize_event = R"doc(Window resize event handler)doc";

static const char *__doc_nanogui_Screen_scroll_callback_event = R"doc()doc";

static const char *__doc_nanogui_Screen_set_background = R"doc(Set the screen's background color)doc";

static const char *__doc_nanogui_Screen_set_caption = R"doc(Set the window title bar caption)doc";

static const char *__doc_nanogui_Screen_set_resize_callback = R"doc()doc";

static const char *__doc_nanogui_Screen_set_shutdown_glfw = R"doc(Shut down GLFW when the window is closed?)doc";

static const char *__doc_nanogui_Screen_set_size = R"doc(Set window size)doc";

static const char *__doc_nanogui_Screen_set_visible = R"doc(Set the top-level window visibility (no effect on full-screen windows))doc";

static const char *__doc_nanogui_Screen_shutdown_glfw = R"doc()doc";

static const char *__doc_nanogui_Screen_update_focus = R"doc()doc";

static const char *__doc_nanogui_Slider = R"doc(Fractional slider widget with mouse control.)doc";

static const char *__doc_nanogui_Slider_Slider = R"doc()doc";

static const char *__doc_nanogui_Slider_callback = R"doc()doc";

static const char *__doc_nanogui_Slider_draw = R"doc()doc";

static const char *__doc_nanogui_Slider_final_callback = R"doc()doc";

static const char *__doc_nanogui_Slider_highlight_color = R"doc()doc";

static const char *__doc_nanogui_Slider_highlighted_range = R"doc()doc";

static const char *__doc_nanogui_Slider_m_callback = R"doc()doc";

static const char *__doc_nanogui_Slider_m_final_callback = R"doc()doc";

static const char *__doc_nanogui_Slider_m_highlight_color = R"doc()doc";

static const char *__doc_nanogui_Slider_m_highlighted_range = R"doc()doc";

static const char *__doc_nanogui_Slider_m_range = R"doc()doc";

static const char *__doc_nanogui_Slider_m_value = R"doc()doc";

static const char *__doc_nanogui_Slider_mouse_button_event = R"doc()doc";

static const char *__doc_nanogui_Slider_mouse_drag_event = R"doc()doc";

static const char *__doc_nanogui_Slider_preferred_size = R"doc()doc";

static const char *__doc_nanogui_Slider_range = R"doc()doc";

static const char *__doc_nanogui_Slider_set_callback = R"doc()doc";

static const char *__doc_nanogui_Slider_set_final_callback = R"doc()doc";

static const char *__doc_nanogui_Slider_set_highlight_color = R"doc()doc";

static const char *__doc_nanogui_Slider_set_highlighted_range = R"doc()doc";

static const char *__doc_nanogui_Slider_set_range = R"doc()doc";

static const char *__doc_nanogui_Slider_set_value = R"doc()doc";

static const char *__doc_nanogui_Slider_value = R"doc()doc";

static const char *__doc_nanogui_StackedWidget = R"doc(A stack widget.)doc";

static const char *__doc_nanogui_StackedWidget_StackedWidget = R"doc()doc";

static const char *__doc_nanogui_StackedWidget_add_child = R"doc()doc";

static const char *__doc_nanogui_StackedWidget_m_selected_index = R"doc()doc";

static const char *__doc_nanogui_StackedWidget_perform_layout = R"doc()doc";

static const char *__doc_nanogui_StackedWidget_preferred_size = R"doc()doc";

static const char *__doc_nanogui_StackedWidget_selected_index = R"doc()doc";

static const char *__doc_nanogui_StackedWidget_set_selected_index = R"doc()doc";

static const char *__doc_nanogui_TabHeader = R"doc(A Tab navigable widget.)doc";

static const char *__doc_nanogui_TabHeader_ClickLocation = R"doc(The location in which the Widget will be facing.)doc";

static const char *__doc_nanogui_TabHeader_ClickLocation_LeftControls = R"doc()doc";

static const char *__doc_nanogui_TabHeader_ClickLocation_RightControls = R"doc()doc";

static const char *__doc_nanogui_TabHeader_ClickLocation_TabButtons = R"doc()doc";

static const char *__doc_nanogui_TabHeader_TabButton = R"doc(Implementation class of the actual tab buttons.)doc";

static const char *__doc_nanogui_TabHeader_TabButton_StringView = R"doc(Helper struct to represent the TabButton.)doc";

static const char *__doc_nanogui_TabHeader_TabButton_StringView_first = R"doc()doc";

static const char *__doc_nanogui_TabHeader_TabButton_StringView_last = R"doc()doc";

static const char *__doc_nanogui_TabHeader_TabButton_TabButton = R"doc()doc";

static const char *__doc_nanogui_TabHeader_TabButton_calculate_visible_string = R"doc()doc";

static const char *__doc_nanogui_TabHeader_TabButton_draw_active_border_at = R"doc()doc";

static const char *__doc_nanogui_TabHeader_TabButton_draw_at_position = R"doc()doc";

static const char *__doc_nanogui_TabHeader_TabButton_draw_inactive_border_at = R"doc()doc";

static const char *__doc_nanogui_TabHeader_TabButton_label = R"doc()doc";

static const char *__doc_nanogui_TabHeader_TabButton_m_header = R"doc()doc";

static const char *__doc_nanogui_TabHeader_TabButton_m_label = R"doc()doc";

static const char *__doc_nanogui_TabHeader_TabButton_m_size = R"doc()doc";

static const char *__doc_nanogui_TabHeader_TabButton_m_visible_text = R"doc()doc";

static const char *__doc_nanogui_TabHeader_TabButton_m_visible_width = R"doc()doc";

static const char *__doc_nanogui_TabHeader_TabButton_preferred_size = R"doc()doc";

static const char *__doc_nanogui_TabHeader_TabButton_set_label = R"doc()doc";

static const char *__doc_nanogui_TabHeader_TabButton_set_size = R"doc()doc";

static const char *__doc_nanogui_TabHeader_TabButton_size = R"doc()doc";

static const char *__doc_nanogui_TabHeader_TabHeader = R"doc()doc";

static const char *__doc_nanogui_TabHeader_active_button_area =
R"doc(Returns a pair of Vectors describing the top left (pair.first) and the
bottom right (pair.second) positions of the rectangle containing the
active tab button. Returns two zero vectors if the active button is
not visible.)doc";

static const char *__doc_nanogui_TabHeader_active_iterator = R"doc()doc";

static const char *__doc_nanogui_TabHeader_active_iterator_2 = R"doc()doc";

static const char *__doc_nanogui_TabHeader_active_tab = R"doc()doc";

static const char *__doc_nanogui_TabHeader_add_tab = R"doc(Inserts a tab at the end of the tabs collection.)doc";

static const char *__doc_nanogui_TabHeader_add_tab_2 = R"doc(Inserts a tab into the tabs collection at the specified index.)doc";

static const char *__doc_nanogui_TabHeader_calculate_visible_end = R"doc(Given the beginning of the visible tabs, calculate the end.)doc";

static const char *__doc_nanogui_TabHeader_callback = R"doc()doc";

static const char *__doc_nanogui_TabHeader_draw = R"doc()doc";

static const char *__doc_nanogui_TabHeader_draw_controls = R"doc()doc";

static const char *__doc_nanogui_TabHeader_ensure_tab_visible =
R"doc(Recalculate the visible range of tabs so that the tab with the
specified index is visible. The tab with the specified index will
either be the first or last visible one depending on the position
relative to the old visible range.)doc";

static const char *__doc_nanogui_TabHeader_font = R"doc()doc";

static const char *__doc_nanogui_TabHeader_is_tab_visible = R"doc()doc";

static const char *__doc_nanogui_TabHeader_locate_click = R"doc()doc";

static const char *__doc_nanogui_TabHeader_m_active_tab = R"doc()doc";

static const char *__doc_nanogui_TabHeader_m_callback = R"doc()doc";

static const char *__doc_nanogui_TabHeader_m_font = R"doc()doc";

static const char *__doc_nanogui_TabHeader_m_overflowing = R"doc()doc";

static const char *__doc_nanogui_TabHeader_m_tab_buttons = R"doc()doc";

static const char *__doc_nanogui_TabHeader_m_visible_end = R"doc()doc";

static const char *__doc_nanogui_TabHeader_m_visible_start = R"doc()doc";

static const char *__doc_nanogui_TabHeader_mouse_button_event = R"doc()doc";

static const char *__doc_nanogui_TabHeader_on_arrow_left = R"doc()doc";

static const char *__doc_nanogui_TabHeader_on_arrow_right = R"doc()doc";

static const char *__doc_nanogui_TabHeader_overflowing = R"doc()doc";

static const char *__doc_nanogui_TabHeader_perform_layout = R"doc()doc";

static const char *__doc_nanogui_TabHeader_preferred_size = R"doc()doc";

static const char *__doc_nanogui_TabHeader_remove_tab =
R"doc(Removes the tab with the specified label and returns the index of the
label. Returns -1 if there was no such tab)doc";

static const char *__doc_nanogui_TabHeader_remove_tab_2 = R"doc(Removes the tab with the specified index.)doc";

static const char *__doc_nanogui_TabHeader_set_active_tab = R"doc()doc";

static const char *__doc_nanogui_TabHeader_set_callback =
R"doc(Sets the callable objects which is invoked when a tab button is
pressed. The argument provided to the callback is the index of the
tab.)doc";

static const char *__doc_nanogui_TabHeader_set_font = R"doc()doc";

static const char *__doc_nanogui_TabHeader_tab_count = R"doc()doc";

static const char *__doc_nanogui_TabHeader_tab_index =
R"doc(Retrieves the index of a specific tab label. Returns the number of
tabs (tabs_count) if there is no such tab.)doc";

static const char *__doc_nanogui_TabHeader_tab_iterator = R"doc()doc";

static const char *__doc_nanogui_TabHeader_tab_iterator_2 = R"doc()doc";

static const char *__doc_nanogui_TabHeader_tab_label_at = R"doc(Retrieves the label of the tab at a specific index.)doc";

static const char *__doc_nanogui_TabHeader_visible_begin = R"doc()doc";

static const char *__doc_nanogui_TabHeader_visible_begin_2 = R"doc()doc";

static const char *__doc_nanogui_TabHeader_visible_button_area =
R"doc(Returns a pair of Vectors describing the top left (pair.first) and the
bottom right (pair.second) positions of the rectangle containing the
visible tab buttons.)doc";

static const char *__doc_nanogui_TabHeader_visible_end = R"doc()doc";

static const char *__doc_nanogui_TabHeader_visible_end_2 = R"doc()doc";

static const char *__doc_nanogui_TabWidget =
R"doc(A wrapper around the widgets TabHeader and StackedWidget which hooks
the two classes together.)doc";

static const char *__doc_nanogui_TabWidget_TabWidget = R"doc()doc";

static const char *__doc_nanogui_TabWidget_active_tab = R"doc()doc";

static const char *__doc_nanogui_TabWidget_add_tab =
R"doc(Inserts a tab at the end of the tabs collection and associates it with
the provided widget.)doc";

static const char *__doc_nanogui_TabWidget_add_tab_2 =
R"doc(Inserts a tab into the tabs collection at the specified index and
associates it with the provided widget.)doc";

static const char *__doc_nanogui_TabWidget_callback = R"doc()doc";

static const char *__doc_nanogui_TabWidget_create_tab =
R"doc(Creates a new tab with the specified name and returns a pointer to the
layer.)doc";

static const char *__doc_nanogui_TabWidget_create_tab_2 = R"doc()doc";

static const char *__doc_nanogui_TabWidget_draw = R"doc()doc";

static const char *__doc_nanogui_TabWidget_ensure_tab_visible =
R"doc(This function can be invoked to ensure that the tab with the provided
index the is visible, i.e to track the given tab. Forwards to the tab
header widget. This function should be used whenever the client wishes
to make the tab header follow a newly added tab, as the content of the
new tab is made visible but the tab header does not track it by
default.)doc";

static const char *__doc_nanogui_TabWidget_m_callback = R"doc()doc";

static const char *__doc_nanogui_TabWidget_m_content = R"doc()doc";

static const char *__doc_nanogui_TabWidget_m_header = R"doc()doc";

static const char *__doc_nanogui_TabWidget_perform_layout = R"doc()doc";

static const char *__doc_nanogui_TabWidget_preferred_size = R"doc()doc";

static const char *__doc_nanogui_TabWidget_remove_tab =
R"doc(Removes the tab with the specified label and returns the index of the
label. Returns whether the removal was successful.)doc";

static const char *__doc_nanogui_TabWidget_remove_tab_2 = R"doc(Removes the tab with the specified index.)doc";

static const char *__doc_nanogui_TabWidget_set_active_tab = R"doc()doc";

static const char *__doc_nanogui_TabWidget_set_callback =
R"doc(Sets the callable objects which is invoked when a tab is changed. The
argument provided to the callback is the index of the new active tab.)doc";

static const char *__doc_nanogui_TabWidget_tab = R"doc()doc";

static const char *__doc_nanogui_TabWidget_tab_2 = R"doc()doc";

static const char *__doc_nanogui_TabWidget_tab_count = R"doc()doc";

static const char *__doc_nanogui_TabWidget_tab_index =
R"doc(Retrieves the index of a specific tab using a widget pointer. Returns
-1 if there is no such tab.)doc";

static const char *__doc_nanogui_TabWidget_tab_label_at = R"doc(Retrieves the label of the tab at a specific index.)doc";

static const char *__doc_nanogui_TabWidget_tab_label_index =
R"doc(Retrieves the index of a specific tab using its tab label. Returns -1
if there is no such tab.)doc";

static const char *__doc_nanogui_TextBox = R"doc(Fancy text box with builtin regular expression-based validation.)doc";

static const char *__doc_nanogui_TextBox_Alignment = R"doc(How to align the text in the text box.)doc";

static const char *__doc_nanogui_TextBox_Alignment_Center = R"doc()doc";

static const char *__doc_nanogui_TextBox_Alignment_Left = R"doc()doc";

static const char *__doc_nanogui_TextBox_Alignment_Right = R"doc()doc";

static const char *__doc_nanogui_TextBox_SpinArea = R"doc(The location (if any) for the spin area.)doc";

static const char *__doc_nanogui_TextBox_SpinArea_Bottom = R"doc()doc";

static const char *__doc_nanogui_TextBox_SpinArea_None = R"doc()doc";

static const char *__doc_nanogui_TextBox_SpinArea_Top = R"doc()doc";

static const char *__doc_nanogui_TextBox_TextBox = R"doc()doc";

static const char *__doc_nanogui_TextBox_alignment = R"doc()doc";

static const char *__doc_nanogui_TextBox_callback = R"doc(Set the change callback)doc";

static const char *__doc_nanogui_TextBox_check_format = R"doc()doc";

static const char *__doc_nanogui_TextBox_copy_selection = R"doc()doc";

static const char *__doc_nanogui_TextBox_cursor_index2Position = R"doc()doc";

static const char *__doc_nanogui_TextBox_default_value = R"doc()doc";

static const char *__doc_nanogui_TextBox_delete_selection = R"doc()doc";

static const char *__doc_nanogui_TextBox_draw = R"doc()doc";

static const char *__doc_nanogui_TextBox_editable = R"doc()doc";

static const char *__doc_nanogui_TextBox_focus_event = R"doc()doc";

static const char *__doc_nanogui_TextBox_format = R"doc(Return the underlying regular expression specifying valid formats)doc";

static const char *__doc_nanogui_TextBox_keyboard_character_event = R"doc()doc";

static const char *__doc_nanogui_TextBox_keyboard_event = R"doc()doc";

static const char *__doc_nanogui_TextBox_m_alignment = R"doc()doc";

static const char *__doc_nanogui_TextBox_m_callback = R"doc()doc";

static const char *__doc_nanogui_TextBox_m_committed = R"doc()doc";

static const char *__doc_nanogui_TextBox_m_cursor_pos = R"doc()doc";

static const char *__doc_nanogui_TextBox_m_default_value = R"doc()doc";

static const char *__doc_nanogui_TextBox_m_editable = R"doc()doc";

static const char *__doc_nanogui_TextBox_m_format = R"doc()doc";

static const char *__doc_nanogui_TextBox_m_last_click = R"doc()doc";

static const char *__doc_nanogui_TextBox_m_mouse_down_modifier = R"doc()doc";

static const char *__doc_nanogui_TextBox_m_mouse_down_pos = R"doc()doc";

static const char *__doc_nanogui_TextBox_m_mouse_drag_pos = R"doc()doc";

static const char *__doc_nanogui_TextBox_m_mouse_pos = R"doc()doc";

static const char *__doc_nanogui_TextBox_m_selection_pos = R"doc()doc";

static const char *__doc_nanogui_TextBox_m_spinnable = R"doc()doc";

static const char *__doc_nanogui_TextBox_m_text_offset = R"doc()doc";

static const char *__doc_nanogui_TextBox_m_units = R"doc()doc";

static const char *__doc_nanogui_TextBox_m_units_image = R"doc()doc";

static const char *__doc_nanogui_TextBox_m_valid_format = R"doc()doc";

static const char *__doc_nanogui_TextBox_m_value = R"doc()doc";

static const char *__doc_nanogui_TextBox_m_value_temp = R"doc()doc";

static const char *__doc_nanogui_TextBox_mouse_button_event = R"doc()doc";

static const char *__doc_nanogui_TextBox_mouse_drag_event = R"doc()doc";

static const char *__doc_nanogui_TextBox_mouse_motion_event = R"doc()doc";

static const char *__doc_nanogui_TextBox_paste_from_clipboard = R"doc()doc";

static const char *__doc_nanogui_TextBox_position2Cursor_index = R"doc()doc";

static const char *__doc_nanogui_TextBox_preferred_size = R"doc()doc";

static const char *__doc_nanogui_TextBox_set_alignment = R"doc()doc";

static const char *__doc_nanogui_TextBox_set_callback = R"doc()doc";

static const char *__doc_nanogui_TextBox_set_default_value = R"doc()doc";

static const char *__doc_nanogui_TextBox_set_editable = R"doc()doc";

static const char *__doc_nanogui_TextBox_set_format = R"doc(Specify a regular expression specifying valid formats)doc";

static const char *__doc_nanogui_TextBox_set_spinnable = R"doc()doc";

static const char *__doc_nanogui_TextBox_set_theme = R"doc(Set the Theme used to draw this widget)doc";

static const char *__doc_nanogui_TextBox_set_units = R"doc()doc";

static const char *__doc_nanogui_TextBox_set_units_image = R"doc()doc";

static const char *__doc_nanogui_TextBox_set_value = R"doc()doc";

static const char *__doc_nanogui_TextBox_spin_area = R"doc()doc";

static const char *__doc_nanogui_TextBox_spinnable = R"doc()doc";

static const char *__doc_nanogui_TextBox_units = R"doc()doc";

static const char *__doc_nanogui_TextBox_units_image = R"doc()doc";

static const char *__doc_nanogui_TextBox_update_cursor = R"doc()doc";

static const char *__doc_nanogui_TextBox_value = R"doc()doc";

static const char *__doc_nanogui_Theme = R"doc(Storage class for basic theme-related properties.)doc";

static const char *__doc_nanogui_Theme_Theme = R"doc()doc";

static const char *__doc_nanogui_Theme_m_border_dark = R"doc()doc";

static const char *__doc_nanogui_Theme_m_border_light = R"doc()doc";

static const char *__doc_nanogui_Theme_m_border_medium = R"doc()doc";

static const char *__doc_nanogui_Theme_m_button_corner_radius = R"doc()doc";

static const char *__doc_nanogui_Theme_m_button_font_size = R"doc()doc";

static const char *__doc_nanogui_Theme_m_button_gradient_bot_focused = R"doc()doc";

static const char *__doc_nanogui_Theme_m_button_gradient_bot_pushed = R"doc()doc";

static const char *__doc_nanogui_Theme_m_button_gradient_bot_unfocused = R"doc()doc";

static const char *__doc_nanogui_Theme_m_button_gradient_top_focused = R"doc()doc";

static const char *__doc_nanogui_Theme_m_button_gradient_top_pushed = R"doc()doc";

static const char *__doc_nanogui_Theme_m_button_gradient_top_unfocused = R"doc()doc";

static const char *__doc_nanogui_Theme_m_disabled_text_color = R"doc()doc";

static const char *__doc_nanogui_Theme_m_drop_shadow = R"doc()doc";

static const char *__doc_nanogui_Theme_m_font_bold = R"doc()doc";

static const char *__doc_nanogui_Theme_m_font_icons = R"doc()doc";

static const char *__doc_nanogui_Theme_m_font_normal = R"doc()doc";

static const char *__doc_nanogui_Theme_m_icon_color = R"doc()doc";

static const char *__doc_nanogui_Theme_m_standard_font_size = R"doc()doc";

static const char *__doc_nanogui_Theme_m_tab_border_width = R"doc()doc";

static const char *__doc_nanogui_Theme_m_tab_button_horizontal_padding = R"doc()doc";

static const char *__doc_nanogui_Theme_m_tab_button_vertical_padding = R"doc()doc";

static const char *__doc_nanogui_Theme_m_tab_control_width = R"doc()doc";

static const char *__doc_nanogui_Theme_m_tab_inner_margin = R"doc()doc";

static const char *__doc_nanogui_Theme_m_tab_max_button_width = R"doc()doc";

static const char *__doc_nanogui_Theme_m_tab_min_button_width = R"doc()doc";

static const char *__doc_nanogui_Theme_m_text_box_font_size = R"doc()doc";

static const char *__doc_nanogui_Theme_m_text_color = R"doc()doc";

static const char *__doc_nanogui_Theme_m_text_color_shadow = R"doc()doc";

static const char *__doc_nanogui_Theme_m_transparent = R"doc()doc";

static const char *__doc_nanogui_Theme_m_window_corner_radius = R"doc()doc";

static const char *__doc_nanogui_Theme_m_window_drop_shadow_size = R"doc()doc";

static const char *__doc_nanogui_Theme_m_window_fill_focused = R"doc()doc";

static const char *__doc_nanogui_Theme_m_window_fill_unfocused = R"doc()doc";

static const char *__doc_nanogui_Theme_m_window_header_gradient_bot = R"doc()doc";

static const char *__doc_nanogui_Theme_m_window_header_gradient_top = R"doc()doc";

static const char *__doc_nanogui_Theme_m_window_header_height = R"doc()doc";

static const char *__doc_nanogui_Theme_m_window_header_sep_bot = R"doc()doc";

static const char *__doc_nanogui_Theme_m_window_header_sep_top = R"doc()doc";

static const char *__doc_nanogui_Theme_m_window_popup = R"doc()doc";

static const char *__doc_nanogui_Theme_m_window_popup_transparent = R"doc()doc";

static const char *__doc_nanogui_Theme_m_window_title_focused = R"doc()doc";

static const char *__doc_nanogui_Theme_m_window_title_unfocused = R"doc()doc";

static const char *__doc_nanogui_ToolButton = R"doc(Simple radio+toggle button with an icon.)doc";

static const char *__doc_nanogui_ToolButton_ToolButton = R"doc()doc";

static const char *__doc_nanogui_VScrollPanel =
R"doc(Adds a vertical scrollbar around a widget that is too big to fit into
a certain area.)doc";

static const char *__doc_nanogui_VScrollPanel_VScrollPanel = R"doc()doc";

static const char *__doc_nanogui_VScrollPanel_draw = R"doc()doc";

static const char *__doc_nanogui_VScrollPanel_m_child_preferred_height = R"doc()doc";

static const char *__doc_nanogui_VScrollPanel_m_scroll = R"doc()doc";

static const char *__doc_nanogui_VScrollPanel_m_update_layout = R"doc()doc";

static const char *__doc_nanogui_VScrollPanel_mouse_drag_event = R"doc()doc";

static const char *__doc_nanogui_VScrollPanel_perform_layout = R"doc()doc";

static const char *__doc_nanogui_VScrollPanel_preferred_size = R"doc()doc";

static const char *__doc_nanogui_VScrollPanel_scroll_event = R"doc()doc";

static const char *__doc_nanogui_Widget =
R"doc(Base class of all widgets.

Widget is the base class of all widgets in ``nanogui``. It can also be
used as an panel to arrange an arbitrary number of child widgets using
a layout generator (see Layout).)doc";

static const char *__doc_nanogui_Widget_Widget = R"doc(Construct a new widget with the given parent widget)doc";

static const char *__doc_nanogui_Widget_absolute_position = R"doc(Return the absolute position on screen)doc";

static const char *__doc_nanogui_Widget_add = R"doc(Variadic shorthand notation to construct and add a child widget)doc";

static const char *__doc_nanogui_Widget_add_child =
R"doc(Add a child widget to the current widget at the specified index.

This function almost never needs to be called by hand, since the
constructor of Widget automatically adds the current widget to its
parent)doc";

static const char *__doc_nanogui_Widget_add_child_2 = R"doc(Convenience function which appends a widget at the end)doc";

static const char *__doc_nanogui_Widget_child_at = R"doc(Retrieves the child at the specific position)doc";

static const char *__doc_nanogui_Widget_child_at_2 = R"doc(Retrieves the child at the specific position)doc";

static const char *__doc_nanogui_Widget_child_count = R"doc(Return the number of child widgets)doc";

static const char *__doc_nanogui_Widget_child_index = R"doc(Returns the index of a specific child or -1 if not found)doc";

static const char *__doc_nanogui_Widget_children = R"doc(Return the list of child widgets of the current widget)doc";

static const char *__doc_nanogui_Widget_contains = R"doc(Check if the widget contains a certain position)doc";

static const char *__doc_nanogui_Widget_cursor = R"doc(Return a pointer to the cursor of the widget)doc";

static const char *__doc_nanogui_Widget_draw = R"doc(Draw the widget (and all child widgets))doc";

static const char *__doc_nanogui_Widget_enabled = R"doc(Return whether or not this widget is currently enabled)doc";

static const char *__doc_nanogui_Widget_find_widget = R"doc(Determine the widget located at the given position value (recursive))doc";

static const char *__doc_nanogui_Widget_fixed_height = R"doc()doc";

static const char *__doc_nanogui_Widget_fixed_size = R"doc(Return the fixed size (see set_fixed_size()))doc";

static const char *__doc_nanogui_Widget_fixed_width = R"doc()doc";

static const char *__doc_nanogui_Widget_focus_event =
R"doc(Handle a focus change event (default implementation: record the focus
status, but do nothing))doc";

static const char *__doc_nanogui_Widget_focused = R"doc(Return whether or not this widget is currently focused)doc";

static const char *__doc_nanogui_Widget_font_size =
R"doc(Return current font size. If not set the default of the current theme
will be returned)doc";

static const char *__doc_nanogui_Widget_has_font_size = R"doc(Return whether the font size is explicitly specified for this widget)doc";

static const char *__doc_nanogui_Widget_height = R"doc(Return the height of the widget)doc";

static const char *__doc_nanogui_Widget_keyboard_character_event = R"doc(Handle text input (UTF-32 format) (default implementation: do nothing))doc";

static const char *__doc_nanogui_Widget_keyboard_event = R"doc(Handle a keyboard event (default implementation: do nothing))doc";

static const char *__doc_nanogui_Widget_layout = R"doc(Return the used Layout generator)doc";

static const char *__doc_nanogui_Widget_layout_2 = R"doc(Return the used Layout generator)doc";

static const char *__doc_nanogui_Widget_m_children = R"doc()doc";

static const char *__doc_nanogui_Widget_m_cursor = R"doc()doc";

static const char *__doc_nanogui_Widget_m_enabled = R"doc()doc";

static const char *__doc_nanogui_Widget_m_fixed_size = R"doc()doc";

static const char *__doc_nanogui_Widget_m_focused = R"doc()doc";

static const char *__doc_nanogui_Widget_m_font_size = R"doc()doc";

static const char *__doc_nanogui_Widget_m_layout = R"doc()doc";

static const char *__doc_nanogui_Widget_m_mouse_focus = R"doc()doc";

static const char *__doc_nanogui_Widget_m_parent = R"doc()doc";

static const char *__doc_nanogui_Widget_m_pos = R"doc()doc";

static const char *__doc_nanogui_Widget_m_size = R"doc()doc";

static const char *__doc_nanogui_Widget_m_theme = R"doc()doc";

static const char *__doc_nanogui_Widget_m_tooltip = R"doc()doc";

static const char *__doc_nanogui_Widget_m_visible = R"doc()doc";

static const char *__doc_nanogui_Widget_mouse_button_event =
R"doc(Handle a mouse button event (default implementation: propagate to
children))doc";

static const char *__doc_nanogui_Widget_mouse_drag_event = R"doc(Handle a mouse drag event (default implementation: do nothing))doc";

static const char *__doc_nanogui_Widget_mouse_enter_event =
R"doc(Handle a mouse enter/leave event (default implementation: record this
fact, but do nothing))doc";

static const char *__doc_nanogui_Widget_mouse_motion_event =
R"doc(Handle a mouse motion event (default implementation: propagate to
children))doc";

static const char *__doc_nanogui_Widget_parent = R"doc(Return the parent widget)doc";

static const char *__doc_nanogui_Widget_parent_2 = R"doc(Return the parent widget)doc";

static const char *__doc_nanogui_Widget_perform_layout =
R"doc(Invoke the associated layout generator to properly place child
widgets, if any)doc";

static const char *__doc_nanogui_Widget_position = R"doc(Return the position relative to the parent widget)doc";

static const char *__doc_nanogui_Widget_preferred_size = R"doc(Compute the preferred size of the widget)doc";

static const char *__doc_nanogui_Widget_remove_child = R"doc(Remove a child widget by index)doc";

static const char *__doc_nanogui_Widget_remove_child_2 = R"doc(Remove a child widget by value)doc";

static const char *__doc_nanogui_Widget_request_focus = R"doc(Request the focus to be moved to this widget)doc";

static const char *__doc_nanogui_Widget_scroll_event =
R"doc(Handle a mouse scroll event (default implementation: propagate to
children))doc";

static const char *__doc_nanogui_Widget_set_cursor = R"doc(Set the cursor of the widget)doc";

static const char *__doc_nanogui_Widget_set_enabled = R"doc(Set whether or not this widget is currently enabled)doc";

static const char *__doc_nanogui_Widget_set_fixed_height = R"doc(Set the fixed height (see set_fixed_size()))doc";

static const char *__doc_nanogui_Widget_set_fixed_size =
R"doc(Set the fixed size of this widget

If nonzero, components of the fixed size attribute override any values
computed by a layout generator associated with this widget. Note that
just setting the fixed size alone is not enough to actually change its
size; this is done with a call to set_size or a call to
perform_layout() in the parent widget.)doc";

static const char *__doc_nanogui_Widget_set_fixed_width = R"doc(Set the fixed width (see set_fixed_size()))doc";

static const char *__doc_nanogui_Widget_set_focused = R"doc(Set whether or not this widget is currently focused)doc";

static const char *__doc_nanogui_Widget_set_font_size = R"doc(Set the font size of this widget)doc";

static const char *__doc_nanogui_Widget_set_height = R"doc(Set the height of the widget)doc";

static const char *__doc_nanogui_Widget_set_layout = R"doc(Set the used Layout generator)doc";

static const char *__doc_nanogui_Widget_set_parent = R"doc(Set the parent widget)doc";

static const char *__doc_nanogui_Widget_set_position = R"doc(Set the position relative to the parent widget)doc";

static const char *__doc_nanogui_Widget_set_size = R"doc(set the size of the widget)doc";

static const char *__doc_nanogui_Widget_set_theme = R"doc(Set the Theme used to draw this widget)doc";

static const char *__doc_nanogui_Widget_set_tooltip = R"doc()doc";

static const char *__doc_nanogui_Widget_set_visible =
R"doc(Set whether or not the widget is currently visible (assuming all
parents are visible))doc";

static const char *__doc_nanogui_Widget_set_width = R"doc(Set the width of the widget)doc";

static const char *__doc_nanogui_Widget_size = R"doc(Return the size of the widget)doc";

static const char *__doc_nanogui_Widget_theme = R"doc(Return the Theme used to draw this widget)doc";

static const char *__doc_nanogui_Widget_theme_2 = R"doc(Return the Theme used to draw this widget)doc";

static const char *__doc_nanogui_Widget_tooltip = R"doc()doc";

static const char *__doc_nanogui_Widget_visible =
R"doc(Return whether or not the widget is currently visible (assuming all
parents are visible))doc";

static const char *__doc_nanogui_Widget_visible_recursive =
R"doc(Check if this widget is currently visible, taking parent widgets into
account)doc";

static const char *__doc_nanogui_Widget_width = R"doc(Return the width of the widget)doc";

static const char *__doc_nanogui_Widget_window = R"doc(Walk up the hierarchy and return the parent window)doc";

static const char *__doc_nanogui_Window = R"doc(Top-level window widget.)doc";

static const char *__doc_nanogui_Window_Window = R"doc()doc";

static const char *__doc_nanogui_Window_button_panel = R"doc(Return the panel used to house window buttons)doc";

static const char *__doc_nanogui_Window_center = R"doc(Center the window in the current Screen)doc";

static const char *__doc_nanogui_Window_dispose = R"doc(Dispose the window)doc";

static const char *__doc_nanogui_Window_draw = R"doc(Draw the window)doc";

static const char *__doc_nanogui_Window_m_button_panel = R"doc()doc";

static const char *__doc_nanogui_Window_m_drag = R"doc()doc";

static const char *__doc_nanogui_Window_m_modal = R"doc()doc";

static const char *__doc_nanogui_Window_m_title = R"doc()doc";

static const char *__doc_nanogui_Window_modal = R"doc(Is this a model dialog?)doc";

static const char *__doc_nanogui_Window_mouse_button_event =
R"doc(Handle mouse events recursively and bring the current window to the
top)doc";

static const char *__doc_nanogui_Window_mouse_drag_event = R"doc(Handle window drag events)doc";

static const char *__doc_nanogui_Window_perform_layout =
R"doc(Invoke the associated layout generator to properly place child
widgets, if any)doc";

static const char *__doc_nanogui_Window_preferred_size = R"doc(Compute the preferred size of the widget)doc";

static const char *__doc_nanogui_Window_refresh_relative_placement =
R"doc(Internal helper function to maintain nested window position values;
overridden in Popup)doc";

static const char *__doc_nanogui_Window_scroll_event =
R"doc(Accept scroll events and propagate them to the widget under the mouse
cursor)doc";

static const char *__doc_nanogui_Window_set_modal = R"doc(Set whether or not this is a modal dialog)doc";

static const char *__doc_nanogui_Window_set_title = R"doc(Set the window title)doc";

static const char *__doc_nanogui_Window_title = R"doc(Return the window title)doc";

static const char *__doc_nanogui_active = R"doc(Return whether or not a main loop is currently active)doc";

static const char *__doc_nanogui_chdir_to_bundle_parent =
R"doc(Move to the application bundle's parent directory

This is function is convenient when deploying .app bundles on OSX. It
adjusts the file path to the parent directory containing the bundle.)doc";

static const char *__doc_nanogui_detail_FormWidget = R"doc()doc";

static const char *__doc_nanogui_detail_FormWidget_2 = R"doc()doc";

static const char *__doc_nanogui_detail_FormWidget_3 = R"doc()doc";

static const char *__doc_nanogui_detail_FormWidget_FormWidget = R"doc()doc";

static const char *__doc_nanogui_detail_FormWidget_FormWidget_2 = R"doc()doc";

static const char *__doc_nanogui_detail_FormWidget_FormWidget_3 = R"doc()doc";

static const char *__doc_nanogui_detail_FormWidget_set_callback = R"doc()doc";

static const char *__doc_nanogui_detail_FormWidget_set_editable = R"doc()doc";

static const char *__doc_nanogui_detail_FormWidget_set_editable_2 = R"doc()doc";

static const char *__doc_nanogui_detail_FormWidget_set_value = R"doc()doc";

static const char *__doc_nanogui_detail_FormWidget_set_value_2 = R"doc()doc";

static const char *__doc_nanogui_detail_FormWidget_value = R"doc()doc";

static const char *__doc_nanogui_detail_FormWidget_value_2 = R"doc()doc";

static const char *__doc_nanogui_file_dialog =
R"doc(Open a native file open/save dialog.

Parameter ``filetypes``:
    Pairs of permissible formats with descriptions like ``("png",
    "Portable Network Graphics")``.

Parameter ``save``:
    Set to ``True`` if you would like subsequent file dialogs to open
    at whatever folder they were in when they close this one.)doc";

static const char *__doc_nanogui_init =
R"doc(Static initialization; should be called once before invoking **any**
NanoGUI functions **if** you are having NanoGUI manage OpenGL / GLFW.
This method is effectively a wrapper call to ``glfwInit()``, so if you
are managing OpenGL / GLFW on your own *do not call this method*.

```
Refer to :ref:`nanogui_example_3` for how you might go about managing OpenGL
and GLFW on your own, while still using NanoGUI's classes.

```)doc";

static const char *__doc_nanogui_leave =
R"doc(Request the application main loop to terminate (e.g. if you detached
mainloop).)doc";

static const char *__doc_nanogui_load_image_directory =
R"doc(Load a directory of PNG images and upload them to the GPU (suitable
for use with ImagePanel))doc";

static const char *__doc_nanogui_mainloop =
R"doc(Enter the application main loop

Parameter ``refresh``:
    NanoGUI issues a redraw call whenever an keyboard/mouse/.. event
    is received. In the absence of any external events, it enforces a
    redraw once every ``refresh`` milliseconds. To disable the refresh
    timer, specify a negative value here.

Parameter ``detach``:
    This pararameter only exists in the Python bindings. When the
    active ``Screen`` instance is provided via the ``detach``
    parameter, the ``mainloop()`` function becomes non-blocking and
    returns immediately (in this case, the main loop runs in parallel
    on a newly created thread). This feature is convenient for
    prototyping user interfaces on an interactive Python command
    prompt. When ``detach != None``, the function returns an opaque
    handle that will release any resources allocated by the created
    thread when the handle's ``join()`` method is invoked (or when it
    is garbage collected).

Remark:
    Unfortunately, Mac OS X strictly requires all event processing to
    take place on the application's main thread, which is
    fundamentally incompatible with this type of approach. Thus,
    NanoGUI relies on a rather crazy workaround on Mac OS (kudos to
    Dmitriy Morozov): ``mainloop()`` launches a new thread as before
    but then uses libcoro to swap the thread execution environment
    (stack, registers, ..) with the main thread. This means that the
    main application thread is hijacked and processes events in the
    main loop to satisfy the requirements on Mac OS, while the thread
    that actually returns from this function is the newly created one
    (paradoxical, as that may seem). Deleting or ``join()``ing the
    returned handle causes application to wait for the termination of
    the main loop and then swap the two thread environments back into
    their initial configuration.)doc";

static const char *__doc_nanogui_nanogui_get_image = R"doc(Helper function used by nvg_image_icon)doc";

static const char *__doc_nanogui_nvg_is_font_icon =
R"doc(Determine whether an icon ID is a font-based icon (e.g. from the
entypo.ttf font))doc";

static const char *__doc_nanogui_nvg_is_image_icon = R"doc(Determine whether an icon ID is a texture loaded via nvg_image_icon)doc";

static const char *__doc_nanogui_operator_const_NVGcolor =
R"doc(Allows for conversion between nanogui::Color and the NanoVG NVGcolor
class.)doc";

static const char *__doc_nanogui_ref =
R"doc(Reference counting helper.

The *ref* template is a simple wrapper to store a pointer to an
object. It takes care of increasing and decreasing the object's
reference count as needed. When the last reference goes out of scope,
the associated object will be deallocated.

The advantage over C++ solutions such as ``std::shared_ptr`` is that
the reference count is very compactly integrated into the base object
itself.)doc";

static const char *__doc_nanogui_ref_get = R"doc(Return a const pointer to the referenced object)doc";

static const char *__doc_nanogui_ref_get_2 = R"doc(Return a pointer to the referenced object)doc";

static const char *__doc_nanogui_ref_m_ptr = R"doc()doc";

static const char *__doc_nanogui_ref_operator_T0 = R"doc(Return a pointer to the referenced object)doc";

static const char *__doc_nanogui_ref_operator_assign = R"doc(Move another reference into the current one)doc";

static const char *__doc_nanogui_ref_operator_assign_2 = R"doc(Overwrite this reference with another reference)doc";

static const char *__doc_nanogui_ref_operator_assign_3 = R"doc(Overwrite this reference with a pointer to another object)doc";

static const char *__doc_nanogui_ref_operator_bool = R"doc(Check if the object is defined)doc";

static const char *__doc_nanogui_ref_operator_eq = R"doc(Compare this reference with another reference)doc";

static const char *__doc_nanogui_ref_operator_eq_2 = R"doc(Compare this reference with a pointer)doc";

static const char *__doc_nanogui_ref_operator_mul = R"doc(Return a C++ reference to the referenced object)doc";

static const char *__doc_nanogui_ref_operator_mul_2 = R"doc(Return a const C++ reference to the referenced object)doc";

static const char *__doc_nanogui_ref_operator_ne = R"doc(Compare this reference with another reference)doc";

static const char *__doc_nanogui_ref_operator_ne_2 = R"doc(Compare this reference with a pointer)doc";

static const char *__doc_nanogui_ref_operator_sub = R"doc(Access the object referenced by this reference)doc";

static const char *__doc_nanogui_ref_operator_sub_2 = R"doc(Access the object referenced by this reference)doc";

static const char *__doc_nanogui_ref_ref = R"doc(Create a ``nullptr``-valued reference)doc";

static const char *__doc_nanogui_ref_ref_2 = R"doc(Construct a reference from a pointer)doc";

static const char *__doc_nanogui_ref_ref_3 = R"doc(Copy constructor)doc";

static const char *__doc_nanogui_ref_ref_4 = R"doc(Move constructor)doc";

static const char *__doc_nanogui_shutdown = R"doc(Static shutdown; should be called before the application terminates.)doc";

static const char *__doc_nanogui_utf8 =
R"doc(Convert a single UTF32 character code to UTF8.

```
NanoGUI uses this to convert the icon character codes
defined in :ref:`file_include_nanogui_entypo.h`.

```

Parameter ``c``:
    The UTF32 character to be converted.)doc";

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif

