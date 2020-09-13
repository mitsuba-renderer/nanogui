if (NOT NANOGUI_BUILD_SHARED)
  # Need PIC code in libnanogui & GLFW even when compiled as a static library
  set_target_properties(nanogui PROPERTIES POSITION_INDEPENDENT_CODE ON)

  if (NANOGUI_BUILD_GLFW)
    set_target_properties(glfw_objects PROPERTIES POSITION_INDEPENDENT_CODE ON)
  endif()
endif()

if (NANOGUI_BUILD_PYTHON AND (APPLE OR CMAKE_SYSTEM MATCHES "Linux"))
  # Include coroutine support for running the mainloop in detached mode
  add_definitions(-DCORO_SJLJ)
  include_directories(../ext/coro)
  set(NANOGUI_PYTHON_EXTRA ../ext/coro/coro.c)
endif()

add_definitions(-DNANOGUI_PYTHON)

pybind11_add_module(nanogui-python MODULE THIN_LTO OPT_SIZE
  main.cpp
  glfw.cpp
  icons.cpp
  color.cpp
  widget.cpp
  layout.cpp
  basics.cpp
  button.cpp
  tabs.cpp
  textbox.cpp
  textarea.cpp
  theme.cpp
  formhelper.cpp
  misc.cpp
  canvas.cpp
  nanovg.cpp
  render.cpp
  vector.cpp
  python.h
  py_doc.h
  ${NANOGUI_PYTHON_EXTRA})

set_target_properties(nanogui-python PROPERTIES OUTPUT_NAME nanogui)
target_link_libraries(nanogui-python PRIVATE nanogui)

if (CMAKE_COMPILER_IS_GNUCC)
  # Quench warnings on GCC
  target_compile_options(nanogui-python PRIVATE -Wno-unused-variable)
endif()

if (NANOGUI_INSTALL)
  install(TARGETS nanogui-python
          LIBRARY DESTINATION lib
          ARCHIVE DESTINATION lib)
endif()
