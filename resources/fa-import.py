# Imports FontAwesome's 'all.css' and creates the 'icons.h' header file

import yaml

with open("icons.yml", 'r') as stream:
    try:
        data = yaml.safe_load(stream)
    except yaml.YAMLError as exc:
        print(exc)

icons = []
for key, value in data.items():
    if 'solid' in value['styles']:
        key = 'fa-' + key
        name = key.replace('-', '_').upper()
        spacer = " " * (40-len(name))
        icons.append((key, name, spacer, value['unicode']))


with open("icons.h", 'w') as f:
    f.write('''/*
    NanoGUI was developed by Wenzel Jakob <wenzel.jakob@epfl.ch>.
    The widget drawing code is based on the NanoVG demo application
    by Mikko Mononen.

    All rights reserved. Use of this source code is governed by a
    BSD-style license that can be found in the LICENSE.txt file.
*/

/* Developer note: need to make a change to this file?
 * Please raise an Issue on GitHub describing what needs to change.  This file
 * was generated, so the scripts that generated it needs to update as well.
 */

/**
 * \\file nanogui/icons.h
 *
 * \\brief This is a list of icon codes for the free variant of the
 * FontAwesome 5.10.1 font.
 *
 * \\rst
 *
 * This file defines the full listing of
 * `FontAwesome <https://raw.githubusercontent.com/FortAwesome/Font-Awesome>`_
 * icons available in NanoGUI.  Please note that if viewing the documentation
 * on the web, your browser may display the icons differently than what they
 * look like in NanoGUI.  Run the one of the :ref:`nanogui_example_icons`
 * executables to see what they all look like in NanoGUI.
 *
 * .. tip::
 *
 *    In C++, ``#include <nanogui/icons.h>`` to gain access to the ``#define``
 *    shown in these docs and simply reference the names as listed in the
 *    documentation.  To use the icons from Python, declare
 *    ``from nanogui import icons`` and reference the icons as follows:
 *    ``icons.FA_ASTERISK``.
 *
 * The following icons are available:
 *
 * .. raw:: html
 *
 *    <center>
 *      <div class="wy-table-responsive">
 *        <table class="docutils" border=1>
 *          <colgroup>
 *            <col width="90%" align="center"/>
 *            <col width="10%" align="center" />
 *          </colgroup>
 *          <thead valign="bottom">
 *            <tr class="row-odd">
 *              <th class="head" align="center">Definition</th>
 *              <th class="head" align="center">Icon</th>
 *            </tr>
 *          </thead>
 *          <tbody valign="top">
''')

    for i, icon in enumerate(icons):
        evenodd = 'even' if i % 2 == 0 else 'odd'
        name_orig = icon[0]
        name_new = icon[1]
        f.write(f''' *            <tr class="row-{evenodd}">
 *              <td align="right"><code>{name_new}</code></td>
 *              <td align="center"><span class="{name_orig}"></span></td>
 *            </tr>
''')
    f.write(''' *          </tbody>
 *        </table>
 *      </div><!-- wy-table-responsive -->
 *    </center>
 *
 * \endrst
 */

#pragma once

// prevent individual pages from being generated for all of these
#if !defined(DOXYGEN_SHOULD_SKIP_THIS)

''')
    for name_orig, name_new, spacer, hexcode in icons:
        f.write(f'#define {name_new}{spacer}0x{hexcode}\n')
    f.write('\n#endif // DOXYGEN_SHOULD_SKIP_THIS\n')


with open('icons.cpp', 'w') as f:
    f.write('''#ifdef NANOGUI_PYTHON

#include "python.h"

/* Developer note: need to make a change to this file?
 * Please raise an Issue on GitHub describing what needs to change.  This file
 * was generated, so the scripts that generated it needs to update as well.
 */

void register_entypo(py::module &m) {
    /* Entypo constants */
    {
        #define C(name) g.attr(#name) = py::int_(name);
        py::module g = m.def_submodule("icons");
''')
    for name_orig, name_new, spacer, hexcode in icons:
        f.write(f'        C({name_new});\n')
    f.write('''        #undef C
    }
}

#endif
''')
with open('example_icons.cpp', 'w') as f:
    for name_orig, name_new, spacer, hexcode in icons:
        f.write(f'        ADD_BUTTON({name_new});\n')
