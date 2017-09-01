# NanoGUI

NanoGUI is a minimalistic cross-platform widget library for OpenGL 3.x and GLES
2 (or higher).  It supports automatic layout generation, stateful C++11 lambdas
callbacks, a variety of useful widget types and high-DPI rendering on modern
display devices thanks to [NanoVG](https://github.com/memononen/NanoVG) by
Mikko Mononen.  Python bindings of all functionality are provided using
[pybind11](https://github.com/wjakob/pybind11).

**Note**: This repository contains an
[Enoki](https://github.com/mitsuba-renderer/enoki)-compatible port of the
original [NanoGUI](https://github.com/wjakob/nanogui).

This repository incorporates a number of additional changes that go beyond the
choice of vector library:

1. a different set of naming conventions is used for function and variable
   names that feels more natural in a mixed C++ & Python environment.
   (specifically, ``underscore_case`` rather than ``camelCase``).

2. both OpenGL and GLES 2 are supported. The latter e.g. allows NanoGUI to run
   on ARM devices including the Raspberry Pi.

3. the event loop is much more conservative by default and only issues redraw
   calls when explicitly requested by an event callback.

4. Python integration: the library comes with a ``pip``-compatible ``setup.py``
   installation script.

- [Documentation](https://nanogui.readthedocs.io)

## Example screenshot
![Screenshot](https://github.com/wjakob/nanogui/raw/master/resources/screenshot.png "Screenshot")

## Description
NanoGUI builds on [GLFW](http://www.glfw.org/) for cross-platform OpenGL context
creation and event handling, [GLAD](https://github.com/Dav1dde/glad) to use OpenGL
3.x/GLES2 windows, [Enoki](https://github.com/mitsuba-renderer/enoki) for
basic vector types, and [NanoVG](https://github.com/memononen/NanoVG) to draw
2D primitives.

Note that the dependency library NanoVG already includes some basic example
code to draw good-looking static widgets; what NanoGUI does is to flesh it
out into a complete GUI toolkit with event handling, layout generation, etc.

NanoGUI currently works on Mac OS X (Clang) Linux (GCC or Clang) and Windows
(Visual Studio ≥ 2017); it requires a recent C++14-capable compiler. All
dependencies are jointly built using a CMake-based build system.

## Creating widgets
NanoGUI makes it easy to instantiate widgets, set layout constraints, and
register event callbacks using high-level C++11 code. For instance, the
following two lines from the included example application add a new button to
an existing window `window` and register an event callback.
```C++
Button *b = new Button(window, "Plain button");
b->set_callback([] { cout << "pushed!" << endl; });
```

The following lines from the example application create the coupled
slider and text box on the bottom of the second window (see the screenshot).
```C++
/* Create an empty panel with a horizontal layout */
Widget *panel = new Widget(window);
panel->set_layout(new BoxLayout(BoxLayout::Horizontal, BoxLayout::Middle, 0, 20));

/* Add a slider and set defaults */
Slider *slider = new Slider(panel);
slider->set_value(0.5f);
slider->set_fixed_width(80);

/* Add a textbox and set defaults */
TextBox *textBox = new TextBox(panel);
textBox->set_fixed_size(Vector2i(60, 25));
textBox->set_value("50");
textBox->set_units("%");

/* Propagate slider changes to the text box */
slider->set_callback([textBox](float value) {
    textBox->set_value(std::to_string((int) (value * 100)));
});
```

The Python version of this same piece of code looks like this:
```Python
# Create an empty panel with a horizontal layout
panel = Widget(window)
panel.set_layout(BoxLayout(BoxLayout.Horizontal, BoxLayout.Middle, 0, 20))

# Add a slider and set defaults
slider = Slider(panel)
slider.set_value(0.5f)
slider.set_fixed_width(80)

# Add a textbox and set defaults
textBox = TextBox(panel)
textBox.set_fixed_size(Vector2i(60, 25))
textBox.set_value("50")
textBox.set_units("%")

# Propagate slider changes to the text box
def cb(value):
    textBox.set_value("%i" % int(value * 100))
slider.set_callback(cb)
```

## "Simple mode"

Christian Schüller contributed a convenience class that makes it possible to
create AntTweakBar-style variable manipulators using just a few lines of code.
For instance, the source code below was used to create the following example
application.

![Screenshot](https://github.com/wjakob/nanogui/raw/master/resources/screenshot2.png "Screenshot")


```C++
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
   ->set_items({"Item 1", "Item 2", "Item 3"});
gui->add_variable("Color", colval);

gui->add_group("Other widgets");
gui->add_button("A button", [](){ std::cout << "Button pressed." << std::endl; });

screen->set_visible(true);
screen->perform_layout();
window->center();
```

## Compiling
Clone the repository and all dependencies (with `git clone --recursive`),
run CMake to generate Makefiles or CMake/Visual Studio project files, and
the rest should just work automatically.

On Debian/Ubuntu, make sure that you have installed the following packages
```bash
$ apt-get install cmake xorg-dev libglu1-mesa-dev
```
To also get the Python bindings, you'll need to run
```bash
$ apt-get install python-dev
```
### License

nanogui is provided under a BSD-style license that can be found in the
``LICENSE.txt`` file. By using, distributing, or contributing to this project,
you agree to the terms and conditions of this license.
