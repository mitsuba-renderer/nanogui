# python/example2.py -- Python version of an example application that shows
# how to use the form helper class. For a C++ implementation, see
# '../src/example2.cpp'.
#
# NanoGUI was developed by Wenzel Jakob <wenzel@inf.ethz.ch>.
# The widget drawing code is based on the NanoVG demo application
# by Mikko Mononen.
#
# All rights reserved. Use of this source code is governed by a
# BSD-style license that can be found in the LICENSE.txt file.

import nanogui
import math
import gc

from nanogui import Screen, FormHelper

bvar = True
ivar = 12345678
dvar = math.pi
strvar = 'A string'
strvar2 = ''
enumvar = 1
colvar = nanogui.Color(.5, .5, .7, 1)


def make_accessors(name):
    def setter(value):
        globals()[name] = value

    def getter():
        return globals()[name]
    return setter, getter

nanogui.init()

use_gl_4_1 = False # Set to True to create an OpenGL 4.1 context.
if use_gl_4_1:
    # NanoGUI presents many options for you to utilize at your discretion.
    # See include/nanogui/screen.h for what all of the options are.
    screen = Screen((500, 700), 'NanoGUI test [GL 4.1]', gl_major=4, gl_minor=1)
else:
    screen = Screen((500, 700), 'NanoGUI test')

gui = FormHelper(screen)
window = gui.add_window((10, 10), 'Form helper example')

gui.add_group('Basic types')
gui.add_bool_variable('bool', *make_accessors('bvar'))
gui.add_string_variable('string', *make_accessors('strvar'))
gui.add_string_variable('placeholder', *make_accessors('strvar2')).set_placeholder('placeholder')

gui.add_group('Validating fields')
gui.add_int_variable('int', *make_accessors('ivar'))
gui.add_double_variable('double', *make_accessors('dvar'))

gui.add_group('Complex types')
gui.add_enum_variable('Enumeration', *make_accessors('enumvar')) \
   .set_items(['Item 1', 'Item 2', 'Item 3'])
gui.add_color_variable('Color', *make_accessors('colvar'))

gui.add_group('Other widgets')


def cb():
    print('Button pressed.')
gui.add_button('A button', cb)

screen.set_visible(True)
screen.perform_layout()
window.center()

nanogui.mainloop(refresh=0)
screen = gui = window = None
gc.collect()
nanogui.shutdown()
