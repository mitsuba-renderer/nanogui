cmake_minimum_required (VERSION 2.8.12)

project(nanogui)

if (POLICY CMP0058)
  cmake_policy(SET CMP0058 NEW) # Ninja requires custom command byproducts to be explicit.
endif()
if (POLICY CMP0068)
  cmake_policy(SET CMP0068 NEW) # RPATH settings on macOS don't affect install_name.
endif()

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

if (WIN32)
  set(NANOGUI_USE_GLAD_DEFAULT ON)
else()
  set(NANOGUI_USE_GLAD_DEFAULT OFF)
endif()

set(NANOGUI_BUILD_SHARED_DEFAULT OFF)
set(NANOGUI_USE_PYTHON_DEFAULT   ON)
set(NANOGUI_BUILD_GLFW_DEFAULT   ON)

if (CMAKE_CXX_COMPILER MATCHES "/em\\+\\+(-[a-zA-Z0-9.])?$")
  set(CMAKE_CXX_COMPILER_ID "Emscripten")
  set(NANOGUI_BUILD_SHARED_DEFAULT OFF)
  set(NANOGUI_USE_PYTHON_DEFAULT   OFF)
  set(NANOGUI_USE_GLAD_DEFAULT     OFF)
  set(NANOGUI_BUILD_GLFW_DEFAULT   OFF)

  set(CMAKE_STATIC_LIBRARY_SUFFIX ".bc")
  set(CMAKE_EXECUTABLE_SUFFIX ".bc")
  set(CMAKE_CXX_CREATE_STATIC_LIBRARY "<CMAKE_CXX_COMPILER> -o <TARGET> <LINK_FLAGS> <OBJECTS>")
  if (U_CMAKE_BUILD_TYPE MATCHES REL)
    add_compile_options(-O3 -DNDEBUG)
  endif()
endif()

option(NANOGUI_BUILD_EXAMPLES "Build NanoGUI example application?" ON)
option(NANOGUI_BUILD_SHARED   "Build NanoGUI as a shared library?" ${NANOGUI_BUILD_SHARED_DEFAULT})
option(NANOGUI_BUILD_PYTHON   "Build a Python plugin for NanoGUI?" ${NANOGUI_USE_PYTHON_DEFAULT})
option(NANOGUI_USE_GLAD       "Use Glad OpenGL loader library?" ${NANOGUI_USE_GLAD_DEFAULT})
option(NANOGUI_BUILD_GLFW     "Build GLFW" ${NANOGUI_BUILD_GLFW_DEFAULT})
option(NANOGUI_INSTALL        "Install NanoGUI on `make install`?" ON)

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

set(NANOGUI_PYTHON_VERSION "" CACHE STRING "Python version to use for compiling the Python plugin")

# Allow overriding the pybind11 and Enoki version used to compile NanoGUI
set(ENOKI_PYBIND11_DIR "${CMAKE_CURRENT_SOURCE_DIR}/ext/enoki/ext/pybind11"
  CACHE STRING "Path containing the 'pybind11' library used to compile Enoki.")
set(NANOGUI_PYBIND11_DIR "${ENOKI_PYBIND11_DIR}"
  CACHE STRING "Path containing the 'pybind11' library used to compile nanogui.")
set(NANOGUI_ENOKI_DIR "${CMAKE_CURRENT_SOURCE_DIR}/ext/enoki"
  CACHE STRING "Path containing the 'enoki' library used to compile nanogui.")
mark_as_advanced(ENOKI_PYBIND11_DIR NANOGUI_PYBIND11_DIR NANOGUI_ENOKI_DIR)

# Required libraries, flags, and include files for compiling and linking against nanogui (all targets)
set(NANOGUI_EXTRA_LIBS "")
set(NANOGUI_EXTRA_DEFS "")
set(NANOGUI_EXTRA_INCS "")
set(NANOGUI_EXTRA_GLOB "")

# Platform-dependent files for libnanogui
set(LIBNANOGUI_EXTRA_SOURCE "")
set(LIBNANOGUI_PYTHON_EXTRA_SOURCE "")

if (APPLE AND NANOGUI_BUILD_SHARED)
  set(CMAKE_MACOSX_RPATH ON)
endif()

set(NANOGUI_MASTER_PROJECT OFF)
if (${CMAKE_CURRENT_SOURCE_DIR} STREQUAL ${CMAKE_SOURCE_DIR})
  set(NANOGUI_MASTER_PROJECT ON)
endif()

if (EXISTS /opt/vc/include)
  # Auto-detect GLES include directory on Raspberry PI
  include_directories(/opt/vc/include)
  list(APPEND NANOGUI_EXTRA_INCS "/opt/vc/include")
endif()

include(CheckCXXCompilerFlag)
include(CheckCXXSourceRuns)

if (NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message(STATUS "Setting build type to 'Release' as none was specified.")
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build." FORCE)
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release"
    "MinSizeRel" "RelWithDebInfo")
endif()
string(TOUPPER "${CMAKE_BUILD_TYPE}" U_CMAKE_BUILD_TYPE)

macro(CHECK_CXX_COMPILER_AND_LINKER_FLAGS _RESULT _CXX_FLAGS _LINKER_FLAGS)
  set(CMAKE_REQUIRED_FLAGS ${_CXX_FLAGS})
  set(CMAKE_REQUIRED_LIBRARIES ${_LINKER_FLAGS})
  set(CMAKE_REQUIRED_QUIET TRUE)
  check_cxx_source_runs("#include <iostream>\nint main(int argc, char **argv) { std::cout << \"test\"; return 0; }" ${_RESULT})
  set(CMAKE_REQUIRED_FLAGS "")
  set(CMAKE_REQUIRED_LIBRARIES "")
endmacro()

add_subdirectory("${NANOGUI_ENOKI_DIR}" "ext_build/enoki")

enoki_set_compile_flags()
enoki_set_native_flags()

# Python support: add NANOGUI_PYTHON flag to all targets
if (NANOGUI_BUILD_PYTHON)
  list(APPEND NANOGUI_EXTRA_DEFS -DNANOGUI_PYTHON)
endif()

if (NANOGUI_BACKEND MATCHES "(OpenGL|GLES 2|GLES 3)")
  list(APPEND LIBNANOGUI_EXTRA_SOURCE
    src/texture_gl.cpp src/shader_gl.cpp
    src/renderpass_gl.cpp src/opengl.cpp
    src/opengl_check.h
  )
endif()

if (NANOGUI_BACKEND STREQUAL "OpenGL")
  list(APPEND NANOGUI_EXTRA_DEFS -DNANOGUI_USE_OPENGL)
  list(APPEND NANOGUI_EXTRA_GLOB "resources/*.gl")
  message(STATUS "NanoGUI: using OpenGL backend.")
endif()

if (NANOGUI_BACKEND STREQUAL "GLES 2")
  list(APPEND NANOGUI_EXTRA_DEFS -DNANOGUI_USE_GLES -DNANOGUI_GLES_VERSION=2)
  list(APPEND NANOGUI_EXTRA_GLOB "resources/*.gles")
  message(STATUS "NanoGUI: using GLES 2 backend.")
endif()

if (NANOGUI_BACKEND STREQUAL "GLES 3")
  list(APPEND NANOGUI_EXTRA_DEFS -DNANOGUI_USE_GLES -DNANOGUI_GLES_VERSION=3)
  list(APPEND NANOGUI_EXTRA_GLOB "resources/*.gles")
  message(STATUS "NanoGUI: using GLES 3 backend.")
endif()

if (NANOGUI_BACKEND STREQUAL "Metal")
  list(APPEND NANOGUI_EXTRA_DEFS -DNANOGUI_USE_METAL)
  list(APPEND LIBNANOGUI_EXTRA_SOURCE
    ext/nanovg_metal/src/nanovg_mtl.m ext/nanovg_metal/src/nanovg_mtl.h
    src/texture_metal.mm src/shader_metal.mm src/renderpass_metal.mm
  )
  list(APPEND NANOGUI_EXTRA_GLOB "resources/*.metal")
  include_directories(ext/nanovg_metal/src)
  message(STATUS "NanoGUI: using Metal backend.")
endif()

# Shared library mode: add dllimport/dllexport flags to all symbols
if (NANOGUI_BUILD_SHARED)
  message(STATUS "NanoGUI: building shared library.")
  list(APPEND NANOGUI_EXTRA_DEFS -DNANOGUI_SHARED -DNVG_SHARED -DGLAD_GLAPI_EXPORT)
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

if (CMAKE_CXX_COMPILER_ID MATCHES "^(GNU|Clang|Emscripten|Intel)$")
  CHECK_CXX_COMPILER_FLAG("-std=c++17" HAS_CPP17_FLAG)

  if (HAS_CPP17_FLAG)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
  else()
    CHECK_CXX_COMPILER_FLAG("-std=c++1z" HAS_CPP1Z_FLAG)
    if (HAS_CPP1Z_FLAG)
      set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1z")
    else()
      message(FATAL_ERROR "Unsupported compiler -- nanogui requires C++17 support!")
    endif()
  endif()
elseif(MSVC)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++17")
endif()


# Various optimizations for shared library release builds
if (NANOGUI_BUILD_SHARED)
  if (U_CMAKE_BUILD_TYPE MATCHES REL AND CMAKE_CXX_COMPILER_ID MATCHES "^(GNU|Clang)$")
    # Set the default symbol visibility to hidden
    if (NOT CMAKE_CXX_FLAGS MATCHES "-fvisibility")
      set(CMAKE_CXX_FLAGS "-fvisibility=hidden ${CMAKE_CXX_FLAGS}")
    endif()

    # Enable link time optimization
    if (NOT CMAKE_CXX_FLAGS MATCHES "-flto")
      if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
        set(LTO_CXX_FLAGS "-flto=thin")
        set(LTO_LINKER_FLAGS "-flto=thin")
        if (NOT APPLE AND U_CMAKE_BUILD_TYPE MATCHES MINSIZEREL)
            # Clang Gold plugin does not support -Os
            set(LTO_CXX_FLAGS "${LTO_CXX_FLAGS} -O3")
        endif()
      else()
        set(LTO_CXX_FLAGS "-flto -fno-fat-lto-objects")
        set(LTO_LINKER_FLAGS "-flto")
      endif()

      CHECK_CXX_COMPILER_AND_LINKER_FLAGS(HAS_LTO ${LTO_CXX_FLAGS} ${LTO_LINKER_FLAGS})

      if (HAS_LTO)
        message(STATUS "NanoGUI: LTO support enabled.")
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${LTO_LINKER_FLAGS}")
        set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} ${LTO_LINKER_FLAGS}")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${LTO_CXX_FLAGS}")
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${LTO_CXX_FLAGS}")
      else()
        message(STATUS "NanoGUI: LTO not supported by the compiler.")
      endif()
    endif()
  elseif(MSVC)
    set(Configurations RELEASE RELWITHDEBINFO MINSIZEREL)
    set(LinkTypes EXE SHARED MODULE STATIC)
    foreach(Configuration ${Configurations})
      set("CMAKE_CXX_FLAGS_${Configuration}" "${CMAKE_CXX_FLAGS_${Configuration}} /GL")
      foreach(LinkType ${LinkTypes})
        set("CMAKE_${LinkType}_LINKER_FLAGS_${Configuration}" "${CMAKE_${LinkType}_LINKER_FLAGS_${Configuration}} /LTCG")
      endforeach()
    endforeach()
    message(STATUS "NanoGUI: LTO support enabled.")
  endif()
endif()

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

  add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/ext/glfw"  "ext_build/glfw")

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
  )
endif()

if (NANOGUI_USE_GLAD)
  # Build and include GLAD on Windows
  list(APPEND LIBNANOGUI_EXTRA_SOURCE
   "${CMAKE_CURRENT_SOURCE_DIR}/ext/glad/src/glad.c"
   "${CMAKE_CURRENT_SOURCE_DIR}/ext/glad/include/glad/glad.h"
   "${CMAKE_CURRENT_SOURCE_DIR}/ext/glad/include/KHR/khrplatform.h")
  if (MSVC)
    set_source_files_properties("${CMAKE_CURRENT_SOURCE_DIR}/ext/glad/src/glad.c"
      PROPERTIES COMPILE_FLAGS "/wd4055 ")
  endif()
  include_directories(ext/glad/include)
  list(APPEND NANOGUI_EXTRA_DEFS -DNANOGUI_GLAD)
  list(APPEND NANOGUI_EXTRA_INCS "${CMAKE_CURRENT_SOURCE_DIR}/ext/glad/include")
endif()

list(APPEND NANOGUI_EXTRA_INCS
  "${CMAKE_CURRENT_SOURCE_DIR}/ext/glfw/include"
  "${CMAKE_CURRENT_SOURCE_DIR}/ext/nanovg/src"
  "${NANOGUI_ENOKI_DIR}/include"
)

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
  list(APPEND NANOGUI_EXTRA_LIBS opengl32)
elseif (APPLE)
  find_library(cocoa_library Cocoa)
  find_library(iokit_library IOKit)
  list(APPEND NANOGUI_EXTRA_LIBS ${cocoa_library} ${iokit_library})
  mark_as_advanced(cocoa_library iokit_library)

  if (NANOGUI_BACKEND STREQUAL "OpenGL")
    find_library(corevideo_library CoreVideo)
    find_library(opengl_library OpenGL)

    # Disable OpenGL deprecation warnings
    add_definitions(-DGL_SILENCE_DEPRECATION)
    list(APPEND NANOGUI_EXTRA_LIBS ${opengl_library} ${corevideo_library})
    mark_as_advanced(opengl_library corevideo_library)
  else()
    find_library(metal_library Metal)
    find_library(quartzcore_library QuartzCore)
    list(APPEND NANOGUI_EXTRA_LIBS ${metal_library} ${quartzcore_library})
    mark_as_advanced(metal_library quartzcore_library)
  endif()
  list(APPEND LIBNANOGUI_EXTRA_SOURCE src/darwin.mm)
elseif (CMAKE_SYSTEM MATCHES "Linux" OR CMAKE_SYSTEM_NAME MATCHES "BSD")
  list(APPEND NANOGUI_EXTRA_LIBS Xxf86vm Xrandr Xinerama Xcursor Xi X11 pthread)
  if (NANOGUI_BACKEND STREQUAL "OpenGL")
    list(APPEND NANOGUI_EXTRA_LIBS GL)
  elseif (NANOGUI_BACKEND STREQUAL "GLES 2")
    list(APPEND NANOGUI_EXTRA_LIBS GLESv2)
  elseif (NANOGUI_BACKEND STREQUAL "GLES 3")
    list(APPEND NANOGUI_EXTRA_LIBS GLESv3)
  endif()
  if (NOT CMAKE_SYSTEM_NAME MATCHES "OpenBSD")
    list(APPEND NANOGUI_EXTRA_LIBS rt)
  endif()
  if (CMAKE_SYSTEM MATCHES "Linux")
    list(APPEND NANOGUI_EXTRA_LIBS dl)
  endif()
endif()

include_directories(
  ${NANOGUI_ENOKI_DIR}/include
  ext/nanovg/src
  include
  ${CMAKE_CURRENT_BINARY_DIR}
)

if (NANOGUI_BUILD_GLFW)
  include_directories(ext/glfw/include)
endif()

# Run simple cmake converter to put font files into the data segment

# Glob up resource files
file(GLOB resources
  "${CMAKE_CURRENT_SOURCE_DIR}/resources/*.ttf"
  ${NANOGUI_EXTRA_GLOB}
)

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

add_definitions(${NANOGUI_EXTRA_DEFS})

# Compile main NanoGUI library
add_library(nanogui-obj OBJECT
  # Merge NanoVG into the NanoGUI library
  ext/nanovg/src/nanovg.c
  ext/nanovg/src/stb_image.h
  # Merge GLAD into the NanoGUI library (only if needed)
  ${LIBNANOGUI_EXTRA_SOURCE}
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
  include/nanogui/renderpass.h
  include/nanogui/formhelper.h
  include/nanogui/icons.h
  include/nanogui/toolbutton.h
  include/nanogui/opengl.h
  include/nanogui/arcball.h
  include/nanogui/nanogui.h
)

set(NANOGUI_LIB_CONTENTS $<TARGET_OBJECTS:nanogui-obj>)
if (CMAKE_GENERATOR STREQUAL Xcode)
  # XCode has a serious bug where the XCode project produces an invalid target
  # that will not get linked if it consists only of objects from object libraries,
  # it will not generate any products (executables, libraries). The only work
  # around is to add a dummy source file to the library definition. This is an
  # XCode, not a CMake bug. See: https://itk.org/Bug/view.php?id=14044

  set(NANOGUI_XCODE_DUMMY ${CMAKE_CURRENT_BINARY_DIR}/xcode_dummy.cpp)
  file(WRITE ${NANOGUI_XCODE_DUMMY} "")
  set(NANOGUI_LIB_CONTENTS ${NANOGUI_LIB_CONTENTS} ${CMAKE_CURRENT_BINARY_DIR}/xcode_dummy.cpp)
endif()

if (NANOGUI_BUILD_GLFW)
  set(NANOGUI_LIB_CONTENTS ${NANOGUI_LIB_CONTENTS} $<TARGET_OBJECTS:glfw_objects>)
endif()

add_library(nanogui ${NANOGUI_LIBRARY_TYPE} ${NANOGUI_LIB_CONTENTS})

if (NANOGUI_BUILD_SHARED)
  set_property(TARGET nanogui-obj PROPERTY POSITION_INDEPENDENT_CODE ON)
endif()

# Compile/link flags for NanoGUI
set_property(TARGET nanogui-obj APPEND PROPERTY COMPILE_DEFINITIONS "NANOGUI_BUILD;NVG_BUILD;NVG_STB_IMAGE_IMPLEMENTATION")

if (NANOGUI_USE_GLAD AND NANOGUI_BUILD_SHARED)
  set_property(TARGET nanogui-obj APPEND PROPERTY COMPILE_DEFINITIONS
    "GLAD_GLAPI_EXPORT;GLAD_GLAPI_EXPORT_BUILD")
endif()

if (NANOGUI_BUILD_SHARED)
  target_link_libraries(nanogui ${NANOGUI_EXTRA_LIBS})
endif()

if (NANOGUI_INSTALL)
  install(TARGETS nanogui
          LIBRARY DESTINATION lib
          ARCHIVE DESTINATION lib)

  install(DIRECTORY include/nanogui DESTINATION include
          FILES_MATCHING PATTERN "*.h")
endif()

if (NANOGUI_BUILD_SHARED)
  # When GLFW is merged into the NanoGUI library, this flag must be specified
  set_property(TARGET nanogui-obj APPEND PROPERTY COMPILE_DEFINITIONS "_GLFW_BUILD_DLL;NVG_SHARED")
endif()

if (NANOGUI_BUILD_SHARED AND NOT ${U_CMAKE_BUILD_TYPE} MATCHES DEB)
  # Platform-specific strip flags for reducing the library size.
  if (APPLE)
    # Strip .dylib library on OSX
    add_custom_command(TARGET nanogui POST_BUILD COMMAND strip -u -r "$<TARGET_FILE_DIR:nanogui>/$<TARGET_FILE_NAME:nanogui>")
  elseif(UNIX)
    # Strip .so library on Linux
    add_custom_command(TARGET nanogui POST_BUILD COMMAND strip "$<TARGET_FILE_DIR:nanogui>/$<TARGET_FILE_NAME:nanogui>")
  endif()
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
  target_link_libraries(example1      nanogui ${NANOGUI_EXTRA_LIBS})
  target_link_libraries(example2      nanogui ${NANOGUI_EXTRA_LIBS})
  target_link_libraries(example3      nanogui ${NANOGUI_EXTRA_LIBS})
  target_link_libraries(example4      nanogui ${NANOGUI_EXTRA_LIBS})
  target_link_libraries(example_icons nanogui ${NANOGUI_EXTRA_LIBS})

  # Copy icons for example application
  file(COPY resources/icons DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
endif()

if (NANOGUI_BUILD_PYTHON)
  if (APPLE OR CMAKE_SYSTEM MATCHES "Linux")
    # Include coroutine support for running the mainloop in detached mode
    add_definitions(-DCORO_SJLJ)
    include_directories(ext/coro)
    list(APPEND LIBNANOGUI_PYTHON_EXTRA_SOURCE ext/coro/coro.c)
  endif()

  # Try to autodetect Python (can be overridden manually if needed)
  list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/ext/enoki/ext/pybind11/tools")
  set(Python_ADDITIONAL_VERSIONS 3.7 3.6 3.5 3.4)
  find_package(PythonLibsNew ${NANOGUI_PYTHON_VERSION})
  if (NOT PYTHONLIBS_FOUND)
    # Python not found -- disable the plugin
    set(NANOGUI_BUILD_PYTHON OFF CACHE BOOL "Build a Python plugin for NanoGUI?" FORCE)
    message(WARNING "NanoGUI: not building the Python plugin!")
  else()
    message(STATUS "NanoGUI: building the Python plugin.")
  endif()
else()
  message(STATUS "NanoGUI: not building the Python plugin.")
endif()

if (NANOGUI_BUILD_PYTHON)
  # Need PIC code in libnanogui even when compiled as a static library
  set_target_properties(nanogui-obj PROPERTIES POSITION_INDEPENDENT_CODE ON)
  if (NANOGUI_BUILD_GLFW)
    set_target_properties(glfw_objects PROPERTIES POSITION_INDEPENDENT_CODE ON)
  endif()

  include_directories("${NANOGUI_PYBIND11_DIR}/include" ${PYTHON_INCLUDE_DIR})

  add_library(nanogui-python-obj OBJECT
    python/main.cpp
    python/glfw.cpp
    python/icons.cpp
    python/color.cpp
    python/widget.cpp
    python/layout.cpp
    python/basics.cpp
    python/button.cpp
    python/tabs.cpp
    python/textbox.cpp
    python/textarea.cpp
    python/theme.cpp
    python/formhelper.cpp
    python/misc.cpp
    python/canvas.cpp
    python/nanovg.cpp
    python/render.cpp
    python/python.h python/py_doc.h
    ${LIBNANOGUI_PYTHON_EXTRA_SOURCE})

  add_library(nanogui-python SHARED $<TARGET_OBJECTS:nanogui-python-obj>)
  set_property(TARGET nanogui-python-obj PROPERTY POSITION_INDEPENDENT_CODE ON)
  set_target_properties(nanogui-python PROPERTIES OUTPUT_NAME "nanogui")
  target_link_libraries(nanogui-python nanogui ${NANOGUI_EXTRA_LIBS})

  # Quench warnings on GCC
  if (CMAKE_COMPILER_IS_GNUCC)
    set_property(TARGET nanogui-python-obj APPEND PROPERTY COMPILE_OPTIONS "-Wno-unused-variable")
  endif()

  if (${CMAKE_CURRENT_SOURCE_DIR} STREQUAL ${CMAKE_CURRENT_BINARY_DIR})
  set_target_properties(nanogui-python PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/python)
  endif()
  # The prefix and extension are provided by FindPythonLibsNew.cmake
  set_target_properties(nanogui-python PROPERTIES PREFIX "${PYTHON_MODULE_PREFIX}")
  set_target_properties(nanogui-python PROPERTIES SUFFIX "${PYTHON_MODULE_EXTENSION}")

  if (WIN32)
    # Set output path
    set_target_properties(nanogui-python PROPERTIES ARCHIVE_OUTPUT_DIRECTORY_RELEASE "Release/python")
    set_target_properties(nanogui-python PROPERTIES ARCHIVE_OUTPUT_DIRECTORY_DEBUG "Debug/python")
    set_target_properties(nanogui-python PROPERTIES ARCHIVE_OUTPUT_DIRECTORY_MINSIZEREL "MinSizeRel/python")
    set_target_properties(nanogui-python PROPERTIES ARCHIVE_OUTPUT_DIRECTORY_RELWITHDEBINFO "RelWithDebInfo/python")
    set_target_properties(nanogui-python PROPERTIES RUNTIME_OUTPUT_DIRECTORY_RELEASE "Release/python")
    set_target_properties(nanogui-python PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG "Debug/python")
    set_target_properties(nanogui-python PROPERTIES RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL "MinSizeRel/python")
    set_target_properties(nanogui-python PROPERTIES RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO "RelWithDebInfo/python")

    # Link against the Python shared library
    target_link_libraries(nanogui-python ${PYTHON_LIBRARY})

    if (MSVC)
      # Optimize for size, /bigobj is needed for due to the heavy template metaprogramming in pybind11
      set_property(TARGET nanogui-python-obj APPEND PROPERTY COMPILE_OPTIONS
        "/bigobj" "$<$<CONFIG:Release>:/Os>" "$<$<CONFIG:MinSizeRel>:/Os>"
        "$<$<CONFIG:RelWithDebInfo>:/Os>")
    endif()
  elseif(UNIX)
    # Optimize for size
    if (U_CMAKE_BUILD_TYPE MATCHES REL)
      set_property(TARGET nanogui-python-obj APPEND PROPERTY COMPILE_OPTIONS "-Os")
    endif()

    # Strip unnecessary sections of the binary on Linux/Mac OS
    if(APPLE)
      set_target_properties(nanogui-python PROPERTIES MACOSX_RPATH ".")
      set_target_properties(nanogui-python PROPERTIES LINK_FLAGS "-undefined dynamic_lookup ")

      if (NOT ${U_CMAKE_BUILD_TYPE} MATCHES DEB)
        add_custom_command(TARGET nanogui-python POST_BUILD COMMAND strip -u -r $<TARGET_FILE:nanogui-python>)
      endif()
    else()
      if (NOT ${U_CMAKE_BUILD_TYPE} MATCHES DEB)
        add_custom_command(TARGET nanogui-python POST_BUILD COMMAND strip $<TARGET_FILE:nanogui-python>)
      endif()
    endif()
  endif()

  if (NANOGUI_INSTALL)
    install(TARGETS nanogui-python
            LIBRARY DESTINATION lib
            ARCHIVE DESTINATION lib)
  endif()
endif()

if (NOT NANOGUI_MASTER_PROJECT)
  # This project is included from somewhere else. Export NANOGUI_EXTRA_LIBS variable
  set(NANOGUI_EXTRA_LIBS ${NANOGUI_EXTRA_LIBS} PARENT_SCOPE)
  set(NANOGUI_EXTRA_DEFS ${NANOGUI_EXTRA_DEFS} PARENT_SCOPE)
  set(NANOGUI_EXTRA_INCS ${NANOGUI_EXTRA_INCS} PARENT_SCOPE)
elseif (NANOGUI_BUILD_PYTHON)
  # Create documentation for python plugin (optional target for developers)
  string(REPLACE " " ";" MKDOC_CXX_FLAGS_LIST ${CMAKE_CXX_FLAGS})
  get_property(MKDOC_INCLUDE_DIRECTORIES DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY INCLUDE_DIRECTORIES)
  get_property(MKDOC_COMPILE_DEFINITIONS DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY COMPILE_DEFINITIONS)

  foreach (value ${MKDOC_INCLUDE_DIRECTORIES})
    list(APPEND MKDOC_CXX_FLAGS_LIST -I${value})
  endforeach()

  # Make sure platform specific code gets kept in py_doc.h (specifically __doc_nanogui_chdir_to_bundle_parent)
  list(APPEND MKDOC_COMPILE_DEFINITIONS "DOXYGEN_DOCUMENTATION_BUILD")
  foreach (value ${MKDOC_COMPILE_DEFINITIONS})
    list(APPEND MKDOC_CXX_FLAGS_LIST -D${value})
  endforeach()

  add_custom_target(mkdoc USES_TERMINAL COMMAND
    python3 ${PROJECT_SOURCE_DIR}/docs/mkdoc_rst.py
      ${MKDOC_CXX_FLAGS_LIST}
      ${PROJECT_SOURCE_DIR}/include/nanogui/*.h
      > ${CMAKE_CURRENT_SOURCE_DIR}/python/py_doc.h)
endif()

# vim: set et ts=2 sw=2 ft=cmake nospell:
