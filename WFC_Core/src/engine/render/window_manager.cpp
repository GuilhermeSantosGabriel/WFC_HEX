#include "engine/render/render.h"

#include <stdexcept>
#include <iostream>
#include <ostream>

// Callback to resize window
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

GLFWwindow* setup_window() {

    glfwSetErrorCallback([](int error, const char* description) {
        std::cerr << "GLFW Error (" << error << "): " << description << std::endl;
    });

    if (!glfwInit()) {
        throw std::runtime_error("Error on GLFW init");
    }

    // Sets GLWF up with OpenGL
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // MSAA to reduce flickering
    glfwWindowHint(GLFW_SAMPLES, 4);

    #ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    #endif

    GLFWwindow* window = glfwCreateWindow(1000, 1000, "WFC_HEX", NULL, NULL);
    if (window == NULL) {
        throw std::runtime_error("Error on creating window");
    }

    glfwMakeContextCurrent(window);
    // Enables vsync
    glfwSwapInterval(1);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        throw std::runtime_error("Error on GLAD Load");
    }

    glViewport(0, 0, 1000, 1000);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // Capture and hide mouse cursor
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Enable z-buffer to avoid having to sort triangles by distance to the camera
    // before drawing them
    glEnable(GL_DEPTH_TEST);
    // Enable MSAA to reduce flickering
    glEnable(GL_MULTISAMPLE);

    return window;
}

void clear_window(GLFWwindow* window, int* width, int* height) {

    // Clears buffer
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Deals with window resizing
    glfwGetFramebufferSize(window, width, height);
}

void update_window(GLFWwindow* window) {
    // Send the new frame to the screen
    glfwSwapBuffers(window);
    glfwPollEvents();
}

void terminate_window(GLFWwindow* window) {
    glfwDestroyWindow(window);
    glfwTerminate();
}
