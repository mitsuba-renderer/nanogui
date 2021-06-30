from importlib import import_module as _import

_import('nanogui.nanogui_ext')

def get_cmake_dir():
    from os import path
    file_dir = path.abspath(path.dirname(__file__))
    cmake_path = path.join(file_dir, "share", "cmake", "nanogui")
    if not path.exists(cmake_path):
        raise ImportError("Cannot find NanoGUI CMake directory")
    return cmake_path
