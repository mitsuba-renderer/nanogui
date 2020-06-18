import os
import re
import sys
import platform
import subprocess

from setuptools import setup, Extension
from setuptools.command.build_ext import build_ext
from distutils.version import LooseVersion
from distutils.command.install_headers import install_headers

headers = [
    'include/nanogui/button.h',
    'include/nanogui/canvas.h',
    'include/nanogui/checkbox.h',
    'include/nanogui/colorpicker.h',
    'include/nanogui/colorwheel.h',
    'include/nanogui/combobox.h',
    'include/nanogui/common.h',
    'include/nanogui/formhelper.h',
    'include/nanogui/graph.h',
    'include/nanogui/icons.h',
    'include/nanogui/imagepanel.h',
    'include/nanogui/imageview.h',
    'include/nanogui/label.h',
    'include/nanogui/layout.h',
    'include/nanogui/messagedialog.h',
    'include/nanogui/metal.h',
    'include/nanogui/nanogui.h',
    'include/nanogui/object.h',
    'include/nanogui/opengl.h',
    'include/nanogui/popup.h',
    'include/nanogui/popupbutton.h',
    'include/nanogui/progressbar.h',
    'include/nanogui/python.h',
    'include/nanogui/renderpass.h',
    'include/nanogui/screen.h',
    'include/nanogui/shader.h',
    'include/nanogui/slider.h',
    'include/nanogui/tabwidget.h',
    'include/nanogui/textarea.h',
    'include/nanogui/textbox.h',
    'include/nanogui/texture.h',
    'include/nanogui/theme.h',
    'include/nanogui/toolbutton.h',
    'include/nanogui/traits.h',
    'include/nanogui/vector.h',
    'include/nanogui/vscrollpanel.h',
    'include/nanogui/widget.h',
    'include/nanogui/window.h'
]


class CMakeExtension(Extension):
    def __init__(self, name, sourcedir=''):
        Extension.__init__(self, name, sources=[])
        self.sourcedir = os.path.abspath(sourcedir)


class CMakeBuild(build_ext):
    def run(self):
        try:
            out = subprocess.check_output(['cmake', '--version'])
        except OSError:
            raise RuntimeError("CMake must be installed to build the following extensions: " +
                               ", ".join(e.name for e in self.extensions))

        if platform.system() == "Windows":
            cmake_version = LooseVersion(re.search(r'version\s*([\d.]+)', out.decode()).group(1))
            if cmake_version < '3.1.0':
                raise RuntimeError("CMake >= 3.1.0 is required on Windows")

        for ext in self.extensions:
            self.build_extension(ext)

    def build_extension(self, ext):
        extdir = os.path.abspath(os.path.dirname(self.get_ext_fullpath(ext.name)))
        cmake_args = ['-DPYTHON_EXECUTABLE=' + sys.executable,
                      '-DCMAKE_LIBRARY_OUTPUT_DIRECTORY=' + extdir,
                      '-DNANOGUI_BUILD_EXAMPLES=NO']

        cfg = 'Debug' if self.debug else 'Release'
        build_args = ['--config', cfg]

        if platform.system() == "Windows":
            cmake_args += ['-DCMAKE_LIBRARY_OUTPUT_DIRECTORY_{}={}'.format(cfg.upper(), extdir)]
            if sys.maxsize > 2**32:
                cmake_args += ['-A', 'x64']
            build_args += ['--', '/m']
        else:
            cmake_args += ['-DCMAKE_BUILD_TYPE=' + cfg]
            build_args += ['--', '-j2']

        env = os.environ.copy()
        env['CXXFLAGS'] = '{} -DVERSION_INFO=\\"{}\\"'.format(env.get('CXXFLAGS', ''),
                                                              self.distribution.get_version())
        if not os.path.exists(self.build_temp):
            os.makedirs(self.build_temp)
        subprocess.check_call(['cmake', ext.sourcedir] + cmake_args, cwd=self.build_temp, env=env)
        subprocess.check_call(['cmake', '--build', '.'] + build_args, cwd=self.build_temp)

class InstallHeaders(install_headers):
    """Use custom header installer because the default one flattens subdirectories"""
    def run(self):
        if not self.distribution.headers:
            return

        for header in self.distribution.headers:
            subdir = os.path.dirname(os.path.relpath(header, 'include/nanogui'))
            install_dir = os.path.join(self.install_dir, subdir)
            self.mkpath(install_dir)
            (out, _) = self.copy_file(header, install_dir)
            self.outfiles.append(out)

setup(
    name='nanogui',
    version='0.0.1',
    author='Wenzel Jakob',
    author_email='wenzel.jakob@epfl.ch',
    description='A minimalistic GUI library for OpenGL, GLES 2, and Metal',
    long_description='',
    ext_modules=[CMakeExtension('nanogui')],
    headers=headers,
    cmdclass=dict(build_ext=CMakeBuild, install_headers=InstallHeaders),
    zip_safe=False
)
