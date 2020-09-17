cmake_minimum_required (VERSION 3.13..3.18)

project(nanogui
  DESCRIPTION
    "NanoGUI"
  LANGUAGES
    CXX C
)

if (NOT IS_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/ext/glfw/src")
  message(FATAL_ERROR "The NanoGUI dependency repositories (GLFW, etc.) are missing! "
    "You probably did not clone the project with --recursive. It is possible to recover "
    "by calling \"git submodule update --init --recursive\"")
endif()

if (NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message(STATUS "Setting build type to 'Release' as none was specified.")
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build." FORCE)
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release"
    "MinSizeRel" "RelWithDebInfo")
endif()
string(TOUPPER "${CMAKE_BUILD_TYPE}" U_CMAKE_BUILD_TYPE)

set(NANOGUI_MASTER_PROJECT OFF)
if (${CMAKE_CURRENT_SOURCE_DIR} STREQUAL ${CMAKE_SOURCE_DIR})
  set(NANOGUI_MASTER_PROJECT ON)
endif()

if (WIN32)
  set(NANOGUI_BUILD_GLAD_DEFAULT ON)
else()
  set(NANOGUI_BUILD_GLAD_DEFAULT OFF)
endif()

set(NANOGUI_BUILD_SHARED_DEFAULT ON)
set(NANOGUI_BUILD_PYTHON_DEFAULT ON)
set(NANOGUI_BUILD_GLFW_DEFAULT ON)

if (CMAKE_CXX_COMPILER MATCHES "/em\\+\\+(-[a-zA-Z0-9.])?$")
  set(CMAKE_CXX_COMPILER_ID "Emscripten")
  set(NANOGUI_BUILD_SHARED_DEFAULT OFF)
  set(NANOGUI_BUILD_PYTHON_DEFAULT OFF)
  set(NANOGUI_BUILD_GLAD_DEFAULT   OFF)
  set(NANOGUI_BUILD_GLFW_DEFAULT   OFF)

  set(CMAKE_STATIC_LIBRARY_SUFFIX ".bc")
  set(CMAKE_EXECUTABLE_SUFFIX ".bc")
  set(CMAKE_CXX_CREATE_STATIC_LIBRARY "<CMAKE_CXX_COMPILER> -o <TARGET> <LINK_FLAGS> <OBJECTS>")
  if (U_CMAKE_BUILD_TYPE MATCHES REL)
    add_compile_options(-O3 -DNDEBUG)
  endif()
endif()

option(NANOGUI_BUILD_EXAMPLES            "Build NanoGUI example application?" ON)
option(NANOGUI_BUILD_SHARED              "Build NanoGUI as a shared library?" ${NANOGUI_BUILD_SHARED_DEFAULT})
option(NANOGUI_BUILD_PYTHON              "Build a Python plugin for NanoGUI?" ${NANOGUI_BUILD_PYTHON_DEFAULT})
option(NANOGUI_BUILD_GLAD                "Build GLAD OpenGL loader library? (needed on Windows)" ${NANOGUI_BUILD_GLAD_DEFAULT})
option(NANOGUI_BUILD_GLFW                "Build GLFW?" ${NANOGUI_BUILD_GLFW_DEFAULT})
option(NANOGUI_INSTALL                   "Install NanoGUI on `make install`?" ON)

if (NOT NANOGUI_BACKEND)
  if (CMAKE_SYSTEM_PROCESSOR MATCHES "armv7" OR
      CMAKE_SYSTEM_PROCESSOR MATCHES "aarch64" OR
      CMAKE_CXX_COMPILER MATCHES "/em\\+\\+(-[a-zA-Z0-9.])?$")
    set(NANOGUI_BACKEND_DEFAULT "GLES 2")
  elseif (APPLE)
    set(NANOGUI_BACKEND_DEFAULT "Metal")
  else()
    set(NANOGUI_BACKEND_DEFAULT "OpenGL")
  endif()

  set(NANOGUI_BACKEND ${NANOGUI_BACKEND_DEFAULT} CACHE STRING "Choose the backend used for rendering (OpenGL/GLES 2/GLES 3/Metal)" FORCE)
endif()

set_property(CACHE NANOGUI_BACKEND PROPERTY STRINGS "OpenGL" "GLES 2" "GLES 3" "Metal")

# Required libraries, flags, and include files for compiling and linking against nanogui (all targets)
set(NANOGUI_EXTRA_INCS "")

include(CheckCXXCompilerFlag)
include(CheckCXXSourceRuns)

macro(CHECK_CXX_COMPILER_AND_LINKER_FLAGS _RESULT _CXX_FLAGS _LINKER_FLAGS)
  set(CMAKE_REQUIRED_FLAGS ${_CXX_FLAGS})
  set(CMAKE_REQUIRED_LIBRARIES ${_LINKER_FLAGS})
  set(CMAKE_REQUIRED_QUIET TRUE)
  check_cxx_source_runs("#include <iostream>\nint main(int argc, char **argv) { std::cout << \"test\"; return 0; }" ${_RESULT})
  set(CMAKE_REQUIRED_FLAGS "")
  set(CMAKE_REQUIRED_LIBRARIES "")
endmacro()

if (NANOGUI_BACKEND MATCHES "(OpenGL|GLES 2|GLES 3)")
  list(APPEND NANOGUI_EXTRA
    src/texture_gl.cpp src/shader_gl.cpp
    src/renderpass_gl.cpp src/opengl.cpp
    src/opengl_check.h
  )
endif()

if (NANOGUI_BACKEND STREQUAL "OpenGL")
  set(NANOGUI_BACKEND_DEFS -DNANOGUI_USE_OPENGL)
  set(NANOGUI_GLOB "resources/*.gl")
  message(STATUS "NanoGUI: using OpenGL backend.")
elseif (NANOGUI_BACKEND STREQUAL "GLES 2")
  set(NANOGUI_BACKEND_DEFS -DNANOGUI_USE_GLES -DNANOGUI_GLES_VERSION=2)
  set(NANOGUI_GLOB "resources/*.gles")
  message(STATUS "NanoGUI: using GLES 2 backend.")
elseif (NANOGUI_BACKEND STREQUAL "GLES 3")
  set(NANOGUI_BACKEND_DEFS -DNANOGUI_USE_GLES -DNANOGUI_GLES_VERSION=3)
  set(NANOGUI_GLOB "resources/*.gles")
  message(STATUS "NanoGUI: using GLES 3 backend.")
elseif (NANOGUI_BACKEND STREQUAL "Metal")
  set(NANOGUI_BACKEND_DEFS -DNANOGUI_USE_METAL)
  list(APPEND NANOGUI_EXTRA
    ext/nanovg_metal/src/nanovg_mtl.m ext/nanovg_metal/src/nanovg_mtl.h
    src/texture_metal.mm src/shader_metal.mm src/renderpass_metal.mm
  )
  set(NANOGUI_GLOB "resources/*.metal")
  include_directories(ext/nanovg_metal/src)
  message(STATUS "NanoGUI: using Metal backend.")
endif()

# Shared library mode: add dllimport/dllexport flags to all symbols
if (NANOGUI_BUILD_SHARED)
  message(STATUS "NanoGUI: building shared library.")
else()
  message(STATUS "NanoGUI: building static library.")
endif()

if (MSVC)
  # Disable annoying MSVC warnings (all targets)
  add_definitions(/D "_CRT_SECURE_NO_WARNINGS")

  # Parallel build on MSVC (all targets)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP")
endif()

# Compile with compiler warnings turned on
if (MSVC)
  if (CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
    string(REGEX REPLACE "/W[0-4]" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
  else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
  endif()
elseif (CMAKE_CXX_COMPILER_ID MATCHES "Clang" OR CMAKE_CXX_COMPILER_ID MATCHES "GNU")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
endif()

# Use C++17, visibility=hidden by default, interprocedural optimization
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_VISIBILITY_PRESET hidden)

# Prefer libc++ in conjunction with Clang
if (CMAKE_CXX_COMPILER_ID MATCHES "Clang" AND NOT CMAKE_CXX_FLAGS MATCHES "-stdlib=libc\\+\\+")
  CHECK_CXX_COMPILER_AND_LINKER_FLAGS(HAS_LIBCPP "-stdlib=libc++" "-stdlib=libc++")
  if (HAS_LIBCPP)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++ -D_LIBCPP_VERSION")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -stdlib=libc++")
    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -stdlib=libc++")
    message(STATUS "NanoGUI: using libc++.")
  else()
    CHECK_CXX_COMPILER_AND_LINKER_FLAGS(HAS_LIBCPP_AND_CPPABI "-stdlib=libc++" "-stdlib=libc++ -lc++abi")
    if (HAS_LIBCPP_AND_CPPABI)
      set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++ -D_LIBCPP_VERSION")
      set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -stdlib=libc++ -lc++abi")
      set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -stdlib=libc++ -lc++abi")
      message(STATUS "NanoGUI: using libc++ and libc++abi.")
    else()
      message(FATAL_ERROR "When Clang is used to compile NanoGUI, libc++ must be available -- GCC's libstdc++ is not supported! (please insteall the libc++ development headers, provided e.g. by the packages 'libc++-dev' and 'libc++abi-dev' on Debian/Ubuntu).")
    endif()
  endif()
endif()

# Compile GLFW
if (NANOGUI_BUILD_GLFW)
  set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL " " FORCE)
  set(GLFW_BUILD_TESTS OFF CACHE BOOL " " FORCE)
  set(GLFW_BUILD_DOCS OFF CACHE BOOL " " FORCE)
  set(GLFW_BUILD_INSTALL OFF CACHE BOOL " " FORCE)
  set(GLFW_INSTALL OFF CACHE BOOL " " FORCE)
  set(GLFW_USE_CHDIR OFF CACHE BOOL " " FORCE)
  set(BUILD_SHARED_LIBS ${NANOGUI_BUILD_SHARED} CACHE BOOL " " FORCE)

  if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    # Quench annoying deprecation warnings when compiling GLFW on OSX
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-deprecated-declarations")
  endif()

  add_subdirectory(ext/glfw)

  # Two targets have now been defined: `glfw_objects`, which will be merged into
  # NanoGUI at the end, and `glfw`.  The `glfw` target is the library itself
  # (e.g., libglfw.so), but can be skipped as we do not need to link against it
  # (because we merge `glfw_objects` into NanoGUI).  Skipping is required for
  # XCode, but preferable for all build systems (reduces build artifacts).
  set_target_properties(glfw PROPERTIES EXCLUDE_FROM_ALL 1 EXCLUDE_FROM_DEFAULT_BUILD 1)

  mark_as_advanced(
    GLFW_BUILD_DOCS GLFW_BUILD_EXAMPLES GLFW_BUILD_INSTALL GLFW_BUILD_TESTS
    GLFW_DOCUMENT_INTERNALS GLFW_INSTALL GLFW_USE_CHDIR GLFW_USE_MENUBAR
    GLFW_USE_OSMESA GLFW_VULKAN_STATIC GLFW_USE_RETINA GLFW_USE_MIR
    BUILD_SHARED_LIBS USE_MSVC_RUNTIME_LIBRARY_DLL)
endif()

if (NANOGUI_BUILD_GLAD)
  # Build and include GLAD on Windows
  list(APPEND NANOGUI_EXTRA
    ext/glad/src/glad.c
    ext/glad/include/glad/glad.h
    ext/glad/include/KHR/khrplatform.h)
  if (MSVC)
    set_source_files_properties(ext/glad/src/glad.c
      PROPERTIES COMPILE_FLAGS /wd4055)
  endif()
endif()

if (${CMAKE_SYSTEM_NAME} MATCHES "BSD")
  include_directories(/usr/local/include)
  link_directories(/usr/local/lib)
  if (${CMAKE_SYSTEM_NAME} MATCHES "OpenBSD")
    include_directories(/usr/X11R6/include/)
    link_directories(/usr/X11R6/lib)
  endif()
endif()

# Required core libraries on various platforms
if (${CMAKE_CXX_COMPILER_ID} MATCHES "Emscripten")
  # No dependencies
elseif (WIN32)
  list(APPEND NANOGUI_LIBS opengl32)
elseif (APPLE)
  find_library(cocoa_library Cocoa)
  find_library(iokit_library IOKit)
  list(APPEND NANOGUI_LIBS ${cocoa_library} ${iokit_library})
  mark_as_advanced(cocoa_library iokit_library)

  if (NANOGUI_BACKEND STREQUAL "OpenGL")
    find_library(corevideo_library CoreVideo)
    find_library(opengl_library OpenGL)

    # Disable OpenGL deprecation warnings
    add_definitions(-DGL_SILENCE_DEPRECATION)
    list(APPEND NANOGUI_LIBS ${opengl_library} ${corevideo_library})
    mark_as_advanced(opengl_library corevideo_library)
  else()
    find_library(metal_library Metal)
    find_library(quartzcore_library QuartzCore)
    list(APPEND NANOGUI_LIBS ${metal_library} ${quartzcore_library})
    mark_as_advanced(metal_library quartzcore_library)
  endif()
  list(APPEND NANOGUI_EXTRA src/darwin.mm)
elseif (CMAKE_SYSTEM MATCHES "Linux" OR CMAKE_SYSTEM_NAME MATCHES "BSD")
  list(APPEND NANOGUI_LIBS X11 pthread)
  if (NANOGUI_BACKEND STREQUAL "OpenGL")
    list(APPEND NANOGUI_LIBS GL)
  elseif (NANOGUI_BACKEND STREQUAL "GLES 2")
    list(APPEND NANOGUI_LIBS GLESv2)
  elseif (NANOGUI_BACKEND STREQUAL "GLES 3")
    list(APPEND NANOGUI_LIBS GLESv3)
  endif()
  if (NOT CMAKE_SYSTEM_NAME MATCHES "OpenBSD")
    list(APPEND NANOGUI_LIBS rt)
  endif()
  if (CMAKE_SYSTEM MATCHES "Linux")
    list(APPEND NANOGUI_LIBS dl)
  endif()
endif()

# Run simple cmake converter to put font files into the data segment

# Glob up resource files
file(GLOB resources
  "${CMAKE_CURRENT_SOURCE_DIR}/resources/*.ttf"
  "${NANOGUI_GLOB}")

file(MAKE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/resources")

# Precompile .metal shaders to .metallib files
foreach(fname_in IN LISTS resources)
  if (NANOGUI_BACKEND STREQUAL "Metal" AND fname_in MATCHES "\\.metal")
    get_filename_component(fname_out ${fname_in} NAME)
    set(fname_out "${CMAKE_CURRENT_BINARY_DIR}/resources/${fname_out}lib")
    add_custom_command(
        OUTPUT ${fname_out}
        DEPENDS ${fname_in}
        COMMAND xcrun -sdk macosx metal -std=osx-metal2.0 -O3 "${fname_in}" -o "${fname_out}"
        VERBATIM
    )
  else()
    set(fname_out "${fname_in}")
  endif()
  list(APPEND resources_processed ${fname_out})
endforeach()

# Concatenate resource files into a comma separated string
string(REGEX REPLACE "([^\\]|^);" "\\1," resources_string "${resources_processed}")
string(REGEX REPLACE "[\\](.)" "\\1" resources_string "${resources_string}")

# Create command line for running bin2c cmake script
set(bin2c_cmdline
  -DOUTPUT_C=nanogui_resources.cpp
  -DOUTPUT_H=nanogui_resources.h
  "-DINPUT_FILES=${resources_string}"
  -P "${CMAKE_CURRENT_SOURCE_DIR}/resources/bin2c.cmake")


# Run bin2c on resource files
add_custom_command(
  OUTPUT nanogui_resources.cpp nanogui_resources.h
  COMMAND ${CMAKE_COMMAND} ARGS ${bin2c_cmdline}
  DEPENDS ${resources} ${resources_processed}
  COMMENT "Running bin2c"
  PRE_BUILD VERBATIM)

# Needed to generated files
include_directories(${CMAKE_CURRENT_BINARY_DIR})

# Set library type
if (NANOGUI_BUILD_SHARED)
  set(NANOGUI_LIBRARY_TYPE "SHARED")
else()
  set(NANOGUI_LIBRARY_TYPE "STATIC")
endif()

if (APPLE AND CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  # Use automatic reference counting for Objective-C portions
  add_compile_options(-fobjc-arc)
endif()

if (NANOGUI_BUILD_GLFW)
  list(APPEND NANOGUI_EXTRA $<TARGET_OBJECTS:glfw_objects>)
endif()

# Compile main NanoGUI library
add_library(nanogui ${NANOGUI_LIBRARY_TYPE} 
  # Merge NanoVG into the NanoGUI library
  ext/nanovg/src/nanovg.c
  ext/nanovg/src/stb_image.h

  # Merge GLAD into the NanoGUI library (only if needed)
  ${NANOGUI_EXTRA}

  # Fonts etc.
  nanogui_resources.cpp
  include/nanogui/common.h src/common.cpp
  include/nanogui/widget.h src/widget.cpp
  include/nanogui/theme.h src/theme.cpp
  include/nanogui/layout.h src/layout.cpp
  include/nanogui/screen.h src/screen.cpp
  include/nanogui/label.h src/label.cpp
  include/nanogui/window.h src/window.cpp
  include/nanogui/popup.h src/popup.cpp
  include/nanogui/checkbox.h src/checkbox.cpp
  include/nanogui/button.h src/button.cpp
  include/nanogui/popupbutton.h src/popupbutton.cpp
  include/nanogui/combobox.h src/combobox.cpp
  include/nanogui/progressbar.h src/progressbar.cpp
  include/nanogui/slider.h src/slider.cpp
  include/nanogui/messagedialog.h src/messagedialog.cpp
  include/nanogui/textbox.h src/textbox.cpp
  include/nanogui/textarea.h src/textarea.cpp
  include/nanogui/imagepanel.h src/imagepanel.cpp
  include/nanogui/vscrollpanel.h src/vscrollpanel.cpp
  include/nanogui/colorwheel.h src/colorwheel.cpp
  include/nanogui/colorpicker.h src/colorpicker.cpp
  include/nanogui/graph.h src/graph.cpp
  include/nanogui/tabwidget.h src/tabwidget.cpp
  include/nanogui/canvas.h src/canvas.cpp
  include/nanogui/texture.h src/texture.cpp
  include/nanogui/shader.h src/shader.cpp
  include/nanogui/imageview.h src/imageview.cpp
  include/nanogui/traits.h src/traits.cpp
  include/nanogui/renderpass.h
  include/nanogui/formhelper.h
  include/nanogui/icons.h
  include/nanogui/toolbutton.h
  include/nanogui/opengl.h
  include/nanogui/nanogui.h
)

target_compile_definitions(nanogui
  PUBLIC
    ${NANOGUI_BACKEND_DEFS}

  PRIVATE
    -DNANOGUI_BUILD
    -DNVG_STB_IMAGE_IMPLEMENTATION
)

target_include_directories(nanogui
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include

  PRIVATE
    ext/nanovg/src
    ${CMAKE_CURRENT_BINARY_DIR}
)

if (NANOGUI_BUILD_SHARED)
  target_compile_definitions(nanogui
    PUBLIC
      -DNANOGUI_SHARED -DNVG_SHARED
    PRIVATE
      -DNVG_BUILD
  )

  if (NANOGUI_BUILD_GLAD)
    target_compile_definitions(nanogui
      PUBLIC
        -DGLAD_GLAPI_EXPORT
      PRIVATE
        -DGLAD_GLAPI_EXPORT_BUILD
    )
  endif()

  if (NANOGUI_BUILD_GLFW)
    target_compile_definitions(nanogui PRIVATE _GLFW_BUILD_DLL)
  endif()
endif()

target_include_directories(nanogui PUBLIC
  "${CMAKE_CURRENT_SOURCE_DIR}/ext/nanovg/src")

if (NANOGUI_BUILD_GLAD)
  target_compile_definitions(nanogui PUBLIC -DNANOGUI_GLAD)
  target_include_directories(nanogui
    PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/ext/glad/include")
endif()

if (NANOGUI_BUILD_GLFW)
  target_include_directories(nanogui
    PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/ext/glfw/include")
endif()

if (NANOGUI_BUILD_SHARED)
  target_link_libraries(nanogui PRIVATE ${NANOGUI_LIBS})
  set_property(TARGET nanogui PROPERTY INTERPROCEDURAL_OPTIMIZATION ON)
  set_property(TARGET nanogui PROPERTY DEFINE_SYMBOL "")
else()
  target_link_libraries(nanogui PUBLIC ${NANOGUI_LIBS})
endif()

# Auto-detect GLES include directory on Raspberry PI
if (EXISTS /opt/vc/include)
  target_include_directories(nanogui PUBLIC /opt/vc/include)
endif()

if (NANOGUI_INSTALL)
  install(TARGETS nanogui
          LIBRARY DESTINATION lib
          ARCHIVE DESTINATION lib)

  install(DIRECTORY include/nanogui DESTINATION include
          FILES_MATCHING PATTERN "*.h")
endif()

# Quench warnings while compiling NanoVG
if (CMAKE_COMPILER_IS_GNUCC)
  set_source_files_properties(ext/nanovg/src/nanovg.c PROPERTIES COMPILE_FLAGS -Wno-unused-result)
elseif(MSVC)
  set_source_files_properties(ext/nanovg/src/nanovg.c PROPERTIES COMPILE_FLAGS "/wd4005 /wd4456 /wd4457")
endif()

# Build example application if desired
if (NANOGUI_BUILD_EXAMPLES)
  add_executable(example1      src/example1.cpp)
  add_executable(example2      src/example2.cpp)
  add_executable(example3      src/example3.cpp)
  add_executable(example4      src/example4.cpp)
  add_executable(example_icons src/example_icons.cpp)

  target_link_libraries(example1      nanogui)
  target_link_libraries(example2      nanogui)
  target_link_libraries(example3      nanogui ${NANOGUI_LIBS}) # For OpenGL
  target_link_libraries(example4      nanogui)
  target_link_libraries(example_icons nanogui)

  # Copy icons for example application
  file(COPY resources/icons DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
endif()

if (NANOGUI_BUILD_PYTHON)
  message(STATUS "NanoGUI: building the Python plugin.")
  if (NOT TARGET pybind11::module)
    find_package(Python COMPONENTS Interpreter Development REQUIRED)

    # Allow overriding the pybind11 library used to compile NanoGUI
    set(NANOGUI_PYBIND11_DIR "${CMAKE_CURRENT_SOURCE_DIR}/ext/pybind11"
      CACHE STRING "Path containing the 'pybind11' library used to compile nanogui.")

    add_subdirectory(${NANOGUI_PYBIND11_DIR} ext/pybind11)

    mark_as_advanced(
      USE_PYTHON_INCLUDE_DIR PYBIND11_INSTALL PYBIND11_TEST PYBIND11_NOPYTHON
      PYBIND11_FINDPYTHON PYBIND11_PYTHON_VERSION PYBIND11_WERROR
      NANOGUI_PYBIND11_DIR
    )
  endif()

  add_subdirectory(python)
else()
  message(STATUS "NanoGUI: not building the Python plugin.")
endif()

if (NANOGUI_BUILD_PYTHON AND NANOGUI_MASTER_PROJECT)
  # Create documentation for python plugin (optional target for developers)
  string(REPLACE " " ";" MKDOC_CXX_FLAGS ${CMAKE_CXX_FLAGS})
  get_property(MKDOC_INCLUDE_DIRECTORIES DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY INCLUDE_DIRECTORIES)
  get_property(MKDOC_COMPILE_DEFINITIONS DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY COMPILE_DEFINITIONS)

  foreach (value ${MKDOC_INCLUDE_DIRECTORIES})
    list(APPEND MKDOC_CXX_FLAGS -I${value})
  endforeach()

  # Make sure platform specific code gets kept in py_doc.h (specifically __doc_nanogui_chdir_to_bundle_parent)
  list(APPEND MKDOC_COMPILE_DEFINITIONS "DOXYGEN_DOCUMENTATION_BUILD")
  foreach (value ${MKDOC_COMPILE_DEFINITIONS})
    list(APPEND MKDOC_CXX_FLAGS -D${value})
  endforeach()

  add_custom_target(docstrings USES_TERMINAL COMMAND
    ${Python_EXECUTABLE} -m pybind11_mkdoc -std=c++17 -stdlib=libc++
    ${MKDOC_CXXFLAGS}
    ${NANOGUI_BACKEND_DEFS}
    -I${PROJECT_SOURCE_DIR}/include
    -I${PROJECT_SOURCE_DIR}/ext/glfw/include
    -I${PROJECT_SOURCE_DIR}/ext/nanovg/src
    ${PROJECT_SOURCE_DIR}/include/nanogui/*.h
    -o ${CMAKE_CURRENT_SOURCE_DIR}/python/py_doc.h
  )
endif()

# vim: set et ts=2 sw=2 ft=cmake nospell:
