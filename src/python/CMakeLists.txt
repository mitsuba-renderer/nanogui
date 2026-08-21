if (NOT NANOGUI_BUILD_SHARED)
  # Need PIC code in libnanogui, GLFW, and nfd even when compiled as a static library
  set_target_properties(nanogui PROPERTIES POSITION_INDEPENDENT_CODE ON)
  set_target_properties(nfd PROPERTIES POSITION_INDEPENDENT_CODE ON)

  if (NANOGUI_BUILD_GLFW)
    set_target_properties(glfw PROPERTIES POSITION_INDEPENDENT_CODE ON)
  endif()
endif()

add_definitions(-DNANOGUI_PYTHON)

nanobind_add_module(nanogui-python MODULE
  BACKEND_MODULE nanobind_backend
  NB_DOMAIN nanogui
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
  quad.cpp
  vector.cpp
  chroma.cpp
  ema.cpp
  python.h
  py_doc.h)

set_target_properties(nanogui-python PROPERTIES OUTPUT_NAME nanogui_ext)
target_link_libraries(nanogui-python PRIVATE nanogui)

set(NANOGUI_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/../../nanogui)

if (MSVC)
  set_target_properties(nanogui-python PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY_RELEASE        ${NANOGUI_OUTPUT_DIRECTORY}
    RUNTIME_OUTPUT_DIRECTORY_DEBUG          ${NANOGUI_OUTPUT_DIRECTORY}
    RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO ${NANOGUI_OUTPUT_DIRECTORY}
    RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL     ${NANOGUI_OUTPUT_DIRECTORY}
    LIBRARY_OUTPUT_DIRECTORY_RELEASE        ${NANOGUI_OUTPUT_DIRECTORY}
    LIBRARY_OUTPUT_DIRECTORY_DEBUG          ${NANOGUI_OUTPUT_DIRECTORY}
    LIBRARY_OUTPUT_DIRECTORY_RELWITHDEBINFO ${NANOGUI_OUTPUT_DIRECTORY}
    LIBRARY_OUTPUT_DIRECTORY_MINSIZEREL     ${NANOGUI_OUTPUT_DIRECTORY})

    # On windows, the nanogui DLL file must be part of the same directory or stub generation fails
    if (NANOGUI_BUILD_SHARED)
      add_custom_command(TARGET nanogui-python POST_BUILD
          COMMAND ${CMAKE_COMMAND} -E copy_if_different
          $<TARGET_FILE:nanogui>
          $<TARGET_FILE_DIR:nanogui-python>)
    endif()
else()
  set_target_properties(nanogui-python PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY ${NANOGUI_OUTPUT_DIRECTORY})
endif()

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

if (NOT (CMAKE_CURRENT_BINARY_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR))
  add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/../../nanogui/__init__.py
                     COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/../../nanogui/__init__.py
                                                      ${CMAKE_CURRENT_BINARY_DIR}/../../nanogui/__init__.py
                     DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/../../nanogui/__init__.py)
endif()

nanobind_add_stub(
  nanobind-python-stub
  MODULE nanogui
  PYTHON_PATH ${NANOGUI_OUTPUT_DIRECTORY}/..
  RECURSIVE

  OUTPUT
    ${NANOGUI_OUTPUT_DIRECTORY}/__init__.pyi
    ${NANOGUI_OUTPUT_DIRECTORY}/glfw.pyi
    ${NANOGUI_OUTPUT_DIRECTORY}/icons.pyi
    ${NANOGUI_OUTPUT_DIRECTORY}/ituth273.pyi
    ${NANOGUI_OUTPUT_DIRECTORY}/nanovg.pyi

  DEPENDS
    nanogui-python
    ${NANOGUI_OUTPUT_DIRECTORY}/__init__.py

  MARKER_FILE
    ${NANOGUI_OUTPUT_DIRECTORY}/py.typed

  OUTPUT_PATH
    ${NANOGUI_OUTPUT_DIRECTORY}
)

if (NANOGUI_INSTALL)
  install(
    FILES
      ${NANOGUI_OUTPUT_DIRECTORY}/__init__.py
      ${NANOGUI_OUTPUT_DIRECTORY}/__init__.pyi
      ${NANOGUI_OUTPUT_DIRECTORY}/glfw.pyi
      ${NANOGUI_OUTPUT_DIRECTORY}/icons.pyi
      ${NANOGUI_OUTPUT_DIRECTORY}/ituth273.pyi
      ${NANOGUI_OUTPUT_DIRECTORY}/nanovg.pyi
      ${NANOGUI_OUTPUT_DIRECTORY}/py.typed

    DESTINATION
      ${CMAKE_INSTALL_LIBDIR}
  )
endif()

mark_as_advanced(
  NB_CREATE_INSTALL_RULES
  NB_TEST
  NB_TEST_CUDA
  NB_TEST_FREE_THREADED
  NB_TEST_SANITIZERS_ASAN
  NB_TEST_SANITIZERS_TSAN
  NB_TEST_SANITIZERS_UBSAN
  NB_TEST_SHARED_BUILD
  NB_TEST_STABLE_ABI
  NB_USE_SUBMODULE_DEPS
)
