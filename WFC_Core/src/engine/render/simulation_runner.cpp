#include "engine/render/render.h"

void run_step_visual_simulation(
    GLFWwindow* window,
    HexRenderer& hex_renderer,
    unsigned int step_counter,
    HexMap& hex_map,
    StepGenerator& step_generator
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
}