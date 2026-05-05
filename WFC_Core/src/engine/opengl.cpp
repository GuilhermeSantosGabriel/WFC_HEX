#include "engine/opengl.h"

#include <stdexcept>

GLFWwindow* setup_window() {

    if (!glfwInit()) {
        throw std::runtime_error("Error on GLFW init");
    }

    // Sets GLWF up with OpenGL
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(
        800, 600,
        "WFC_HEX",
        NULL,
        NULL
    );
    if (window == NULL) {
        throw std::runtime_error("Error on creating window");
    }

    return window;
}

// TODO - render_hex_map(HexMap)
void render_hex_map(HexMap hex_map, GLFWwindow* window) {

    glfwMakeContextCurrent(window);

    // loads openGL with GLAD
    gladLoadGL();
    // Sets viewport
    glViewport(0, 0, 800, 600);
    // Sets background color
    glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
    // Clears back buffer and alocates the specified color
    glClear(GL_COLOR_BUFFER_BIT);
    // Swaps back buffer (loaded with background color) with front buffer
    glfwSwapBuffers(window);

    int counter = 0;
    int color_swap = 0;
    while (!glfwWindowShouldClose(window)) {
        if (counter == 200000) {
            if (color_swap == 0) {
                glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
                glClear(GL_COLOR_BUFFER_BIT);
                glfwSwapBuffers(window);
                color_swap = 1;
            } else if (color_swap == 1) {
                glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
                glClear(GL_COLOR_BUFFER_BIT);
                glfwSwapBuffers(window);
                color_swap = 0;
            }

            counter = 0;
        }
        counter++;

        glfwPollEvents();
    }
}
