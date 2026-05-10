#include "engine/opengl/window_manager.h"

#include <stdexcept>

// Callback to resize window
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

GLFWwindow* setup_window() {

    if (!glfwInit()) {
        throw std::runtime_error("Error on GLFW init");
    }

    // Sets GLWF up with OpenGL
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    #ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    #endif

    GLFWwindow* window = glfwCreateWindow(1000, 1000, "WFC_HEX", NULL, NULL);
    if (window == NULL) {
        throw std::runtime_error("Error on creating window");
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        throw std::runtime_error("Error on GLAD Load");
    }

    glViewport(0, 0, 1000, 1000);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    return window;
}

void clear_window(GLFWwindow* window, int* width, int* height) {

    // Clears buffer
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // Deals with window resizing
    glfwGetFramebufferSize(window, width, height);
}

void update_window(GLFWwindow* window) {
    // Send the new frame to the screen
    glfwSwapBuffers(window);
    glfwPollEvents();
}

