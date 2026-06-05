#include "engine/render/render.h"

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (action == GLFW_PRESS && key == GLFW_KEY_Q) {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
}

void run_step_visual_simulation(
    GLFWwindow* window,
    HexRenderer& hex_renderer,
    unsigned int step_counter,
    HexMap& hex_map,
    StepGenerator& step_generator,
    bool persist_after_generator_finished
) {

    int window_height, window_width;
    unsigned int counter = 0;

    while (
        !glfwWindowShouldClose(window) &&
        step_generator.step()
    ) {

        if (counter >= step_counter) {
            clear_window(window, &window_width, &window_height);
            hex_renderer.draw_hex_map_frame(hex_map, window_width, window_height);
            update_window(window);
            counter = 0;
        }

        else counter++;
    }
    if (persist_after_generator_finished) {
        // Makes finished hex_map window stay open until closed by key_callback
        glfwSetKeyCallback(window, key_callback);

        while(!glfwWindowShouldClose(window)) {
            clear_window(window, &window_width, &window_height);
            hex_renderer.draw_hex_map_frame(hex_map, window_width, window_height);
            update_window(window);
        }
    }
}
