# python/example1.py -- Python version of an example application that shows
# how to use the various widget classes. For a C++ implementation, see
# '../src/example1.cpp'.
#
# NanoGUI was developed by Wenzel Jakob <wenzel@inf.ethz.ch>.
# The widget drawing code is based on the NanoVG demo application
# by Mikko Mononen.
#
# All rights reserved. Use of this source code is governed by a
# BSD-style license that can be found in the LICENSE.txt file.

import nanogui
import math
import time
import gc
import numpy as np

from nanogui import Color, ColorPicker, Screen, Window, GroupLayout, \
                    BoxLayout, ToolButton, Label, Button, Widget, \
                    Popup, PopupButton, CheckBox, MessageDialog, \
                    VScrollPanel, ImagePanel, ImageView, ComboBox, \
                    ProgressBar, Slider, TextBox, ColorWheel, Graph, \
                    GridLayout, Alignment, Orientation, TabWidget, \
                    IntBox, RenderPass, Shader, Texture

from nanogui import glfw, icons

# A simple counter, used for dynamic tab creation with TabWidget callback
counter = 1


class TestApp(Screen):
    def __init__(self):
        super(TestApp, self).__init__((1024, 768), "NanoGUI Test")
        self.shader = None

        window = Window(self, "Button demo")
        window.set_position((15, 15))
        window.set_layout(GroupLayout())

        Label(window, "Push buttons", "sans-bold")
        b = Button(window, "Plain button")

        def cb():
            print("pushed!")
        b.set_callback(cb)

        b = Button(window, "Styled", icons.FA_ROCKET)
        b.set_background_color(Color(0, 0, 1.0, 0.1))
        b.set_callback(cb)

        Label(window, "Toggle buttons", "sans-bold")
        b = Button(window, "Toggle me")
        b.set_flags(Button.Flags.ToggleButton)

        def change_cb(state):
            print("Toggle button state: %s" % str(state))
        b.set_change_callback(change_cb)

        Label(window, "Radio buttons", "sans-bold")
        b = Button(window, "Radio button 1")
        b.set_flags(Button.Flags.RadioButton)
        b = Button(window, "Radio button 2")
        b.set_flags(Button.Flags.RadioButton)

        Label(window, "A tool palette", "sans-bold")
        tools = Widget(window)
        tools.set_layout(BoxLayout(Orientation.Horizontal,
                                  Alignment.Middle, 0, 6))

        ToolButton(tools, icons.FA_CLOUD)
        ToolButton(tools, icons.FA_FAST_FORWARD)
        ToolButton(tools, icons.FA_COMPASS)
        ToolButton(tools, icons.FA_UTENSILS)

        Label(window, "Popup buttons", "sans-bold")
        popup_btn = PopupButton(window, "Popup", icons.FA_FLASK)
        popup = popup_btn.popup()
        popup.set_layout(GroupLayout())
        Label(popup, "Arbitrary widgets can be placed here")
        CheckBox(popup, "A check box")
        # popup right
        popup_btn = PopupButton(popup, "Recursive popup", icons.FA_CHART_PIE)
        popup_right = popup_btn.popup()
        popup_right.set_layout(GroupLayout())
        CheckBox(popup_right, "Another check box")
        # popup left
        popup_btn = PopupButton(popup, "Recursive popup", icons.FA_DNA)
        popup_btn.set_side(Popup.Side.Left)
        popup_left = popup_btn.popup()
        popup_left.set_layout(GroupLayout())
        CheckBox(popup_left, "Another check box")

        window = Window(self, "Basic widgets")
        window.set_position((200, 15))
        window.set_layout(GroupLayout())

        Label(window, "Message dialog", "sans-bold")
        tools = Widget(window)
        tools.set_layout(BoxLayout(Orientation.Horizontal,
                                  Alignment.Middle, 0, 6))

        def cb2(result):
            print("Dialog result: %i" % result)

        b = Button(tools, "Info")

        def cb():
            dlg = MessageDialog(self, MessageDialog.Type.Information, "Title",
                                "This is an information message")
            dlg.set_callback(cb2)
        b.set_callback(cb)

        b = Button(tools, "Warn")

        def cb():
            dlg = MessageDialog(self, MessageDialog.Type.Warning, "Title",
                                "This is a warning message")
            dlg.set_callback(cb2)
        b.set_callback(cb)

        b = Button(tools, "Ask")

        def cb():
            dlg = MessageDialog(self, MessageDialog.Type.Warning, "Title",
                                "This is a question message", "Yes", "No",
                                True)
            dlg.set_callback(cb2)
        b.set_callback(cb)

        import os
        import sys
        os.chdir(sys.path[0])
        try:
            icons_data = nanogui.load_image_directory(self.nvg_context(), "icons")
        except:
            try:
                icons_data = nanogui.load_image_directory(self.nvg_context(), "../icons")
            except:
                icons_data = nanogui.load_image_directory(self.nvg_context(), "../resources/icons")


        Label(window, "Image panel & scroll panel", "sans-bold")
        image_panel_btn = PopupButton(window, "Image Panel")
        image_panel_btn.set_icon(icons.FA_IMAGES)
        popup = image_panel_btn.popup()
        vscroll = VScrollPanel(popup)
        img_panel = ImagePanel(vscroll)
        img_panel.set_images(icons_data)
        popup.set_fixed_size((245, 150))

        img_window = Window(self, "Selected image")
        img_window.set_position((710, 15))
        img_window.set_layout(GroupLayout())

        img_view = ImageView(img_window)
        img_view.set_image(Texture(icons_data[0][1] + ".png",
                                   Texture.InterpolationMode.Trilinear,
                                   Texture.InterpolationMode.Nearest))
        img_view.center()

        def cb(i):
            print("Selected item %i" % i)
            img_view.set_image(Texture(icons_data[i][1] + ".png",
                                       Texture.InterpolationMode.Trilinear,
                                       Texture.InterpolationMode.Nearest))
        img_panel.set_callback(cb)

        Label(window, "File dialog", "sans-bold")
        tools = Widget(window)
        tools.set_layout(BoxLayout(Orientation.Horizontal,
                                  Alignment.Middle, 0, 6))
        b = Button(tools, "Open")
        valid = [("png", "Portable Network Graphics"), ("txt", "Text file")]

        def cb():
            result = nanogui.file_dialog(valid, False)
            print("File dialog result = %s" % result)

        b.set_callback(cb)
        b = Button(tools, "Save")

        def cb():
            result = nanogui.file_dialog(valid, True)
            print("File dialog result = %s" % result)

        b.set_callback(cb)

        Label(window, "Combo box", "sans-bold")
        ComboBox(window, ["Combo box item 1", "Combo box item 2",
                          "Combo box item 3"])
        Label(window, "Check box", "sans-bold")

        def cb(state):
            print("Check box 1 state: %s" % state)
        chb = CheckBox(window, "Flag 1", cb)
        chb.set_checked(True)

        def cb(state):
            print("Check box 2 state: %s" % state)
        CheckBox(window, "Flag 2", cb)

        Label(window, "Progress bar", "sans-bold")
        self.progress = ProgressBar(window)

        Label(window, "Slider and text box", "sans-bold")

        panel = Widget(window)
        panel.set_layout(BoxLayout(Orientation.Horizontal,
                                  Alignment.Middle, 0, 20))

        slider = Slider(panel)
        slider.set_value(0.5)
        slider.set_fixed_width(80)

        text_box = TextBox(panel)
        text_box.set_fixed_size((60, 25))
        text_box.set_value("50")
        text_box.set_units("%")
        text_box.set_font_size(20)
        text_box.set_alignment(TextBox.Alignment.Right)

        def cb(value):
            text_box.set_value("%i" % int(value * 100))
        slider.set_callback(cb)

        def cb(value):
            print("Final slider value: %i" % int(value * 100))
        slider.set_final_callback(cb)

        window = Window(self, "Misc. widgets")
        window.set_position((425, 15))
        window.set_layout(GroupLayout())

        tab_widget = TabWidget(window)
        layer = Widget(tab_widget)
        layer.set_layout(GroupLayout())
        tab_widget.append_tab("Color Wheel", layer)

        Label(layer, "Color wheel widget", "sans-bold")
        ColorWheel(layer)

        layer = Widget(tab_widget)
        layer.set_layout(GroupLayout())
        tab_widget.append_tab("Function Graph", layer)
        Label(layer, "Function graph widget", "sans-bold")

        graph = Graph(layer, "Some function")
        graph.set_header("E = 2.35e-3")
        graph.set_footer("Iteration 89")
        values = [0.5 * (0.5 * math.sin(i / 10.0) +
                         0.5 * math.cos(i / 23.0) + 1)
                  for i in range(100)]
        graph.set_values(values)

        # Dummy tab used to represent the last tab button.
        plus_id = tab_widget.append_tab("+", Widget(tab_widget))

        def tab_cb(index):
            if index == plus_id:
                global counter
                # When the "+" tab has been clicked, simply add a new tab.
                tab_name  = "Dynamic {0}".format(counter)
                layer_dyn = Widget(tab_widget)
                layer_dyn.set_layout(GroupLayout())
                new_id = tab_widget.insert_tab(tab_widget.tab_count() - 1,
                                               tab_name, layer_dyn)
                Label(layer_dyn, "Function graph widget", "sans-bold")
                graph_dyn = Graph(layer_dyn, "Dynamic function")

                graph_dyn.set_header("E = 2.35e-3")
                graph_dyn.set_footer("Iteration {0}".format(index*counter))
                values_dyn = [0.5 * abs((0.5 * math.sin(i / 10.0 + counter)) +
                                        (0.5 * math.cos(i / 23.0 + 1 + counter)))
                              for i in range(100)]
                graph_dyn.set_values(values_dyn)
                counter += 1
                # We must invoke the layout manager after adding tabs dynamically
                self.perform_layout()
                tab_widget.set_selected_id(new_id)

        tab_widget.set_callback(tab_cb)

        window = Window(self, "Grid of small widgets")
        window.set_position((425, 300))
        layout = GridLayout(Orientation.Horizontal, 2,
                            Alignment.Middle, 15, 5)
        layout.set_col_alignment(
            [Alignment.Maximum, Alignment.Fill])
        layout.set_spacing(0, 10)
        window.set_layout(layout)

        Label(window, "Floating point :", "sans-bold")
        float_box = TextBox(window)
        float_box.set_editable(True)
        float_box.set_fixed_size((100, 20))
        float_box.set_value("50")
        float_box.set_units("GiB")
        float_box.set_default_value("0.0")
        float_box.set_font_size(16)
        float_box.set_format("[-]?[0-9]*\\.?[0-9]+")

        Label(window, "Positive integer :", "sans-bold")
        int_box = IntBox(window)
        int_box.set_editable(True)
        int_box.set_fixed_size((100, 20))
        int_box.set_value(50)
        int_box.set_units("Mhz")
        int_box.set_default_value("0")
        int_box.set_font_size(16)
        int_box.set_format("[1-9][0-9]*")
        int_box.set_spinnable(True)
        int_box.set_min_value(1)
        int_box.set_value_increment(2)

        Label(window, "Checkbox :", "sans-bold")

        cb = CheckBox(window, "Check me")
        cb.set_font_size(16)
        cb.set_checked(True)

        Label(window, "Combo box :", "sans-bold")
        cobo = ComboBox(window, ["Item 1", "Item 2", "Item 3"])
        cobo.set_font_size(16)
        cobo.set_fixed_size((100, 20))

        Label(window, "Color picker :", "sans-bold")
        cp = ColorPicker(window, Color(255, 120, 0, 255))
        cp.set_fixed_size((100, 20))

        def cp_final_cb(color):
            print(
                "ColorPicker Final Callback: [{0}, {1}, {2}, {3}]".format(color.r,
                                                                          color.g,
                                                                          color.b,
                                                                          color.w)
            )

        cp.set_final_callback(cp_final_cb)

        # setup a fast callback for the color picker widget on a new window
        # for demonstrative purposes
        window = Window(self, "Color Picker Fast Callback")
        window.set_position((425, 300))
        layout = GridLayout(Orientation.Horizontal, 2,
                            Alignment.Middle, 15, 5)
        layout.set_col_alignment(
            [Alignment.Maximum, Alignment.Fill])
        layout.set_spacing(0, 10)
        window.set_layout(layout)
        window.set_position((425, 500))
        Label(window, "Combined: ")
        b = Button(window, "ColorWheel", icons.FA_INFINITY)
        Label(window, "Red: ")
        red_int_box = IntBox(window)
        red_int_box.set_editable(False)
        Label(window, "Green: ")
        green_int_box = IntBox(window)
        green_int_box.set_editable(False)
        Label(window, "Blue: ")
        blue_int_box = IntBox(window)
        blue_int_box.set_editable(False)
        Label(window, "Alpha: ")
        alpha_int_box = IntBox(window)

        def cp_fast_cb(color):
            b.set_background_color(color)
            b.set_text_color(color.contrasting_color())
            red = int(color.r * 255.0)
            red_int_box.set_value(red)
            green = int(color.g * 255.0)
            green_int_box.set_value(green)
            blue = int(color.b * 255.0)
            blue_int_box.set_value(blue)
            alpha = int(color.w * 255.0)
            alpha_int_box.set_value(alpha)

        cp.set_callback(cp_fast_cb)

        self.perform_layout()

        self.render_pass = RenderPass([self])
        self.render_pass.set_clear_color(0, Color(0.3, 0.3, 0.32, 1.0))

        if nanogui.api == 'opengl':
            vertex_shader = """
            #version 330
            uniform mat4 mvp;
            in vec3 position;
            void main() {
                gl_Position = mvp * vec4(position, 1.0);
            }"""

            fragment_shader = """
            #version 330
            out vec4 color;
            uniform float intensity;
            void main() {
                color = vec4(vec3(intensity), 1.0);
            }"""
        elif nanogui.api == 'gles2' or nanogui.api == 'gles3':
            vertex_shader = """
            precision highp float;
            uniform mat4 mvp;
            attribute vec3 position;
            void main() {
                gl_Position = mvp * vec4(position, 1.0);
            }"""

            fragment_shader = """
            precision highp float;
            uniform float intensity;
            void main() {
                gl_FragColor = vec4(vec3(intensity), 1.0);
            }"""
        elif nanogui.api == 'metal':
            vertex_shader = """
            using namespace metal;
            struct VertexOut {
                float4 position [[position]];
            };

            vertex VertexOut vertex_main(const device packed_float3 *position,
                                         constant float4x4 &mvp,
                                         uint id [[vertex_id]]) {
                VertexOut vert;
                vert.position = mvp * float4(position[id], 1.f);
                return vert;
            }"""

            fragment_shader = """
            using namespace metal;
            fragment float4 fragment_main(const constant float &intensity) {
                return float4(intensity);
            }"""

        self.shader = Shader(
            self.render_pass,
            # An identifying name
            "A simple shader",
            vertex_shader,
            fragment_shader
        )

        self.shader.set_buffer("indices", np.array([0, 1, 2, 2, 3, 0], dtype=np.uint32))
        self.shader.set_buffer("position", np.array(
            [[-1, -1, 0],
             [1, -1, 0],
             [1, 1, 0],
             [-1, 1, 0]],
            dtype=np.float32
        ))

        self.shader.set_buffer("intensity", np.array(0.5, dtype=np.float32))

    def draw(self, ctx):
        self.progress.set_value(math.fmod(time.time() / 10, 1))
        super(TestApp, self).draw(ctx)

    def draw_contents(self):
        if self.shader is None:
            return
        self.render_pass.resize(self.framebuffer_size())
        s = self.size()
        with self.render_pass:
            mvp = nanogui.scale([s[1] / float(s[0]) * 0.25, 0.25, 0.25]) @ \
                  nanogui.rotate([0, 0, 1], glfw.getTime())
            self.shader.set_buffer("mvp", np.float32(mvp.T))
            with self.shader:
                self.shader.draw_array(Shader.PrimitiveType.Triangle, 0, 6, True)


    def keyboard_event(self, key, scancode, action, modifiers):
        if super(TestApp, self).keyboard_event(key, scancode,
                                              action, modifiers):
            return True
        if key == glfw.KEY_ESCAPE and action == glfw.PRESS:
            self.set_visible(False)
            return True
        return False

if __name__ == "__main__":
    nanogui.init()
    test = TestApp()
    test.draw_all()
    test.set_visible(True)
    nanogui.mainloop(refresh=1 / 60.0 * 1000)
    del test
    gc.collect()
    nanogui.shutdown()
