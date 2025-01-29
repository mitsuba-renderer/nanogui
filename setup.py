# -*- coding: utf-8 -*-

from __future__ import print_function

import sys, re, os

try:
    from skbuild import setup
    import nanobind
except ImportError:
    print("The preferred way to invoke 'setup.py' is via pip, as in 'pip "
          "install .'. If you wish to run the setup script directly, you must "
          "first install the build dependencies listed in pyproject.toml!",
          file=sys.stderr)
    raise

VERSION_REGEX = re.compile(
    r"^\s*#\s*define\s+NANOGUI_VERSION_([A-Z]+)\s+(.*)$", re.MULTILINE)

this_directory = os.path.abspath(os.path.dirname(__file__))

with open(os.path.join("include/nanogui/common.h")) as f:
    matches = dict(VERSION_REGEX.findall(f.read()))
    nanogui_version = "{MAJOR}.{MINOR}.{PATCH}".format(**matches)

long_description = \
'''NanoGUI is a minimalistic cross-platform widget library for OpenGL 3+, GLES
2/3, and Metal. It supports automatic layout generation, stateful C++ lambdas
callbacks, a variety of useful widget types and Retina-capable rendering on
Apple devices thanks to NanoVG by Mikko Mononen. Python bindings of all
functionality are provided using nanobind.'''

setup(
    name="nanogui",
    version=nanogui_version,
    author="Wenzel Jakob",
    author_email="wenzel.jakob@epfl.ch",
    description='A minimalistic GUI library for OpenGL, GLES 2, and Metal',
    url="https://github.com/mitsuba-renderer/nanogui",
    license="BSD",
    long_description=long_description,
    long_description_content_type='text/markdown',
    cmake_args=[
        '-DCMAKE_INSTALL_LIBDIR=nanogui',
        '-DCMAKE_INSTALL_BINDIR=nanogui',
        '-DCMAKE_INSTALL_INCLUDEDIR=nanogui/include',
        '-DCMAKE_INSTALL_DATAROOTDIR=nanogui/share',
        '-DNANOGUI_BUILD_EXAMPLES=OFF'
    ],
    packages=['nanogui']
)
