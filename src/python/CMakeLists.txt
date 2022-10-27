if (NOT NANOGUI_BUILD_SHARED)
  # Need PIC code in libnanogui & GLFW even when compiled as a static library
  set_target_properties(nanogui PROPERTIES POSITION_INDEPENDENT_CODE ON)

  if (NANOGUI_BUILD_GLFW)
    set_target_properties(glfw_objects PROPERTIES POSITION_INDEPENDENT_CODE ON)
  endif()
endif()

add_definitions(-DNANOGUI_PYTHON)

nanobind_add_module(nanogui-python MODULE
  NB_STATIC
  LTO
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
  py_doc.h)

set_target_properties(nanogui-python PROPERTIES OUTPUT_NAME nanogui_ext)
target_link_libraries(nanogui-python PRIVATE nanogui)

set_target_properties(nanogui-python PROPERTIES
  LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/../../nanogui)

if (CMAKE_COMPILER_IS_GNUCC)
  # Quench warnings on GCC
  target_compile_options(nanogui-python PRIVATE -Wno-unused-variable)
endif()

if (NANOGUI_INSTALL)
  install(TARGETS nanogui-python
          LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR} 
          ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR})
  set_target_properties(nanogui-python PROPERTIES INSTALL_RPATH "${NANOGUI_ORIGIN}")
endif()
