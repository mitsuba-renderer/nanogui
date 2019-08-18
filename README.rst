NanoGUI
========================================================================================
|docs| |travis| |appveyor|

.. |docs| image:: https://readthedocs.org/projects/nanogui/badge/?version=latest
    :target: http://nanogui.readthedocs.org/en/latest/?badge=latest
    :alt: Docs

.. |travis| image:: https://travis-ci.org/wjakob/nanogui.svg?branch=master
   :target: https://travis-ci.org/wjakob/nanogui
   :alt: Travis Build Status

.. |appveyor| image:: https://ci.appveyor.com/api/projects/status/m8h3uyvdb4ej2i02/branch/master?svg=true
   :target: https://ci.appveyor.com/project/wjakob/nanogui/branch/master
   :alt: Appveyor Build Status

.. begin_brief_description

NanoGUI is a minimalistic cross-platform widget library for OpenGL 3+, GLES 2/3,
and Metal. It supports automatic layout generation, stateful C++ lambdas
callbacks, a variety of useful widget types and Retina-capable rendering on
Apple devices thanks to NanoVG_ by Mikko Mononen. Python bindings of all
functionality are provided using pybind11_.

**Note**: This repository contains an Enoki_-compatible port of the original
NanoGUI_.

This repository incorporates a number of additional changes that go beyond the
choice of vector library:

1. A different set of naming conventions is used for function and variable
   names that feels more natural in a mixed C++ & Python environment.
   (specifically, ``underscore_case`` for methods and variables rather than
   ``camelCase``).

2. GUI Rendering now provides backends for OpenGL 3+, GLES 2/3, and Metal. GLES 2
   support allows NanoGUI to run on ARM devices including the Raspberry Pi
   and in browsers via WebGL. The Metal backend supports modern Macs, iPhones, etc.

   NanoGUI includes generic wrappers around shaders and textures that work for
   all of these frameworks.

3. The event loop is much more conservative by default and only issues redraw
   calls when explicitly requested by an event callback.

4. Python integration: the library comes with a ``pip``-compatible ``setup.py``
   installation script.

5. WebAssembly code generation works out of the box (requires Emscripten),
   enabling powerful UI development for the web. See Tekari_ for an example of
   such an application.

6. Significantly revamped tab widget (supports right-click context menus,
   draggable, and closeable tabs) and image view widget.

7. The Entypo_ icon font has been replaced by FontAwesome_ (v5.10.1).

.. _NanoVG: https://github.com/memononen/NanoVG
.. _pybind11: https://github.com/wjakob/pybind11
.. _NanoGUI: https://github.com/wjakob/nanogui
.. _Enoki: https://github.com/mitsuba-renderer/enoki
.. _Tekari: https://rgl.epfl.ch/tekari?url=%2F%2Frgl.s3.eu-central-1.amazonaws.com%2Fmedia%2Fuploads%2Fwjakob%2F2018%2F08%2F27%2Firidescent-paper.txt&log=1
.. _Entypo: http://www.entypo.com
.. _FontAwesome: https://github.com/FortAwesome/Font-Awesome

.. end_brief_description

- `Documentation <https://nanogui.readthedocs.io>`_

.. contents:: Contents
   :local:
   :backlinks: none

Example screenshot
----------------------------------------------------------------------------------------

.. image:: https://github.com/wjakob/nanogui/raw/master/resources/screenshot.png
   :alt: Screenshot of Example 1.
   :align: center

Description
----------------------------------------------------------------------------------------

.. begin_long_description

NanoGUI builds on GLFW_ for cross-platform context creation and event handling,
GLAD_ to access OpenGL functionality on Windows, Enoki_ for basic vector types,
and NanoVG_/MetalNanoVG_ to draw 2D primitives.

Note that the dependency library NanoVG already includes some basic example code to draw
good-looking static widgets; what NanoGUI does is to flesh it out into a complete GUI
toolkit with event handling, layout generation, etc.

NanoGUI currently works on Mac OS X (Clang) Linux (GCC or Clang) and Windows (Visual
Studio ≥ 2017); it requires a recent C++17 capable compiler. All dependencies are
jointly built using a CMake-based build system.

.. _GLFW: http://www.glfw.org/
.. _GLAD: https://github.com/Dav1dde/glad
.. _Enoki: https://github.com/mitsuba-renderer/enoki
.. _MetalNanoVG: https://github.com/ollix/MetalNanoVG

.. end_long_description

Creating widgets
----------------------------------------------------------------------------------------

NanoGUI makes it easy to instantiate widgets, set layout constraints, and
register event callbacks using high-level C++17 code. For instance, the
following two lines from the included example application add a new button to
an existing window `window` and register an event callback.

.. code-block:: cpp

   Button *b = new Button(window, "Plain button");
   b->set_callback([] { cout << "pushed!" << endl; });


The following lines from the example application create the coupled
slider and text box on the bottom of the second window (see the screenshot).

.. code-block:: cpp

   /* Create an empty panel with a horizontal layout */
   Widget *panel = new Widget(window);
   panel->set_layout(new BoxLayout(BoxLayout::Horizontal, BoxLayout::Middle, 0, 20));

   /* Add a slider and set defaults */
   Slider *slider = new Slider(panel);
   slider->set_value(0.5f);
   slider->set_fixed_width(80);

   /* Add a textbox and set defaults */
   TextBox *tb = new TextBox(panel);
   tb->set_fixed_size(Vector2i(60, 25));
   tb->set_value("50");
   tb->set_units("%");

   /* Propagate slider changes to the text box */
   slider->set_callback([tb](float value) {
       tb->set_value(std::to_string((int) (value * 100)));
   });


The Python version of this same piece of code looks like this:

.. code-block:: py

   # Create an empty panel with a horizontal layout
   panel = Widget(window)
   panel.set_layout(BoxLayout(BoxLayout.Horizontal, BoxLayout.Middle, 0, 20))

   # Add a slider and set defaults
   slider = Slider(panel)
   slider.set_value(0.5)
   slider.set_fixed_width(80)

   # Add a textbox and set defaults
   tb = TextBox(panel)
   tb.set_fixed_size(Vector2i(60, 25))
   tb.set_value("50")
   tb.set_units("%")

   # Propagate slider changes to the text box
   def cb(value):
       tb.set_value("%i" % int(value * 100))
   slider.set_callback(cb)

"Simple mode"
----------------------------------------------------------------------------------------

Christian Schüller contributed a convenience class that makes it possible to
create AntTweakBar-style variable manipulators using just a few lines of code.
For instance, the source code below was used to create the following example
application.

.. image:: https://github.com/wjakob/nanogui/raw/master/resources/screenshot2.png
   :alt: Screenshot
   :align: center


.. code-block:: cpp

   /// dvar, bar, strvar, etc. are double/bool/string/.. variables

   FormHelper *gui = new FormHelper(screen);
   ref<Window> window = gui->add_window(Vector2i(10, 10), "Form helper example");
   gui->add_group("Basic types");
   gui->add_variable("bool", bvar);
   gui->add_variable("string", strvar);

   gui->add_group("Validating fields");
   gui->add_variable("int", ivar);
   gui->add_variable("float", fvar);
   gui->add_variable("double", dvar);

   gui->add_group("Complex types");
   gui->add_variable("Enumeration", enumval, enabled)
      ->setItems({"Item 1", "Item 2", "Item 3"});
   gui->add_variable("Color", colval);

   gui->add_group("Other widgets");
   gui->add_button("A button", [](){ std::cout << "Button pressed." << std::endl; });

   screen->set_visible(true);
   screen->perform_layout();
   window->center();

Compiling
----------------------------------------------------------------------------------------

Clone the repository and all dependencies (with ``git clone --recursive``),
run CMake to generate Makefiles or CMake/Visual Studio project files, and
the rest should just work automatically.

On Debian/Ubuntu, make sure that you have installed the following packages

.. code-block:: bash

   $ apt-get install cmake xorg-dev libglu1-mesa-dev

To also get the Python bindings, you'll need to run

.. code-block:: bash

   $ apt-get install python-dev

On RedHat/Fedora, make sure that you have installed the following packages

.. code-block:: bash

   $ sudo dnf install cmake mesa-libGLU-devel libXi-devel libXcursor-devel libXinerama-devel libXrandr-devel xorg-x11-server-devel

To also get the Python bindings, you'll need to run

.. code-block:: bash

   $ sudo dnf install python3-devel

License
----------------------------------------------------------------------------------------

.. begin_license

NanoGUI is provided under a BSD-style license that can be found in the LICENSE_
file. By using, distributing, or contributing to this project, you agree to the
terms and conditions of this license.

.. _LICENSE: https://github.com/wjakob/nanogui/blob/master/LICENSE.txt

Note that NanoGUI ships with several fonts that use different (though similarly
unencumbered) licenses, in particular `Roboto
<https://github.com/google/roboto/>`_, `Inconsolata
<https://github.com/googlefonts/Inconsolata>`_, and the free version of the
`Font Awesome icon font <https://github.com/FortAwesome/Font-Awesome>`_
(v5.10.1). The latter two are distributed under the `SIL Open Font License
Version 1.1 <https://opensource.org/licenses/OFL-1.1>`_, while Roboto is
distributed under the `Apache 2.0
<https://opensource.org/licenses/Apache-2.0>`_ license.

.. end_license
