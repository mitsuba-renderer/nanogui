# python/example_icons.py -- Python version of an example application that shows
# all available Entypo icons as they would appear in NanoGUI itself.  For a C++
# implementation, see '../src/example_icons.cpp'.
#
# NanoGUI was developed by Wenzel Jakob <wenzel.jakob@epfl.ch>.
# The widget drawing code is based on the NanoVG demo application
# by Mikko Mononen.
#
# All rights reserved. Use of this source code is governed by a
# BSD-style license that can be found in the LICENSE.txt file.

# Developer note: need to make a change to this file?
# Please raise an Issue on GitHub describing what needs to change.  This file
# was generated, so the scripts that generated it needs to update as well.

import gc

import nanogui
from nanogui import Screen, Window, Widget, GridLayout, VScrollPanel, Button
from nanogui import icons

if __name__ == "__main__":
    nanogui.init()

    width      = 1000
    half_width = width // 2
    height     = 800

    # create a fixed size screen with one window
    screen = Screen((width, height), "NanoGUI Icons", False)
    window = Window(screen, "All Icons")
    window.set_position((0, 0))
    window.set_fixed_size((width, height))

    # attach a vertical scroll panel
    vscroll = VScrollPanel(window)
    vscroll.set_fixed_size((width, height))

    # vscroll should only have *ONE* child. this is what `wrapper` is for
    wrapper = Widget(vscroll)
    wrapper.set_fixed_size((width, height))
    wrapper.set_layout(GridLayout())  # defaults: 2 columns

    # NOTE: using `dir` as done below is not good practice.
    # It is used here because it is convenient for enumerating all available
    # icons -- see 'example_icons.cpp' for an alternative way.
    for name in dir(icons):
        if name.startswith("FA_"):
            b = Button(wrapper, "icons.{0}".format(name), getattr(icons, name))
            b.set_icon_position(Button.IconPosition.Left)
            b.set_fixed_width(half_width)

    screen.perform_layout()
    screen.draw_all()
    screen.set_visible(True)

    nanogui.mainloop()

    del screen
    gc.collect()

    nanogui.shutdown()
