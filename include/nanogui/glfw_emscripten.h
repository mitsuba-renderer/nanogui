/*
    nanogui/glfw_emscripten.h -- Emulation of NanoGUI's GLFW extensions on
    top of the Emscripten GLFW port (contrib.glfw3)

    NanoGUI was developed by Wenzel Jakob <wenzel.jakob@epfl.ch>.
    The widget drawing code is based on the NanoVG demo application
    by Mikko Mononen.

    All rights reserved. Use of this source code is governed by a
    BSD-style license that can be found in the LICENSE.txt file.
*/

#pragma once

#include <GLFW/glfw3.h>
#include <GLFW/emscripten_glfw3.h>
#include <unordered_map>

#define GLFW_SCROLL_PRECISE      0x0001
#define GLFW_SCROLL_MOMENTUM     0x0002
#define GLFW_SCROLL_BEGIN        0x0004
#define GLFW_SCROLL_END          0x0008

typedef void (* GLFWscrollfunext)(GLFWwindow* window, double xoffset, double yoffset, int flags);

// Browsers expose no HDR metadata, report an SDR sRGB display with a 2.2 gamma
inline float glfwGetWindowSdrWhiteLevel(GLFWwindow *) { return 80.f; }
inline float glfwGetWindowMinLuminance(GLFWwindow *) { return 0.f; }
inline float glfwGetWindowMaxLuminance(GLFWwindow *) { return 0.f; }
inline uint32_t glfwGetWindowPrimaries(GLFWwindow *) { return 1; }
inline uint32_t glfwGetWindowTransfer(GLFWwindow *) { return 2; }

inline GLFWmonitor *glfwGetWindowCurrentMonitor(GLFWwindow *) {
    return glfwGetPrimaryMonitor();
}

inline std::unordered_map<GLFWwindow *, GLFWscrollfunext> &glfwScrollCallbacks() {
    static std::unordered_map<GLFWwindow *, GLFWscrollfunext> callbacks;
    return callbacks;
}

// Overload accepting the extended callback signature with a 'flags' argument
inline GLFWscrollfunext glfwSetScrollCallback(GLFWwindow *window, GLFWscrollfunext callback) {
    auto &callbacks = glfwScrollCallbacks();
    GLFWscrollfunext prev = callbacks[window];
    callbacks[window] = callback;

    glfwSetScrollCallback(window, [](GLFWwindow *w, double x, double y) {
        auto it = glfwScrollCallbacks().find(w);
        if (it != glfwScrollCallbacks().end() && it->second)
            it->second(w, x, y, 0);
    });

    return prev;
}
