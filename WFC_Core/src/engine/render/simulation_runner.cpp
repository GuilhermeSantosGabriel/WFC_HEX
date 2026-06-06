#include "engine/render/render.h"

// Processes keyboard input and resets delta_time through camera.pan call
void input_keyboard_process(GLFWwindow* window, Camera& camera) {
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GL_TRUE);
        return;
    }

    glm::vec2 direction = glm::vec2(0.0f, 0.0f);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        direction.y -= 1.0f;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        direction.y += 1.0f;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        direction.x -= 1.0f;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        direction.x += 1.0f;
    }
    const float delta_time = camera.delta_time();
    camera.translate(direction, delta_time);
}

void run_step_visual_simulation(
    GLFWwindow* window,
    HexRenderer& hex_renderer,
    unsigned int step_counter,
    HexMap& hex_map,
    StepGenerator& step_generator,
    Camera& camera,
    bool persist_after_generator_finished
) {

    int window_height, window_width;
    unsigned int counter = 0;

    while (
        !glfwWindowShouldClose(window) &&
        step_generator.step()
    ) {
        input_keyboard_process(window, camera);
        if (counter >= step_counter) {
            clear_window(window, &window_width, &window_height);
            hex_renderer.draw_hex_map_frame(hex_map, window_width, window_height, camera);
            update_window(window);
            counter = 0;
        }

        else counter++;
    }
    if (persist_after_generator_finished) {
        // Reset last frame timestamp to avoid camera movement crazy jump
        (void)camera.delta_time();
        while(!glfwWindowShouldClose(window)) {
            input_keyboard_process(window, camera);
            clear_window(window, &window_width, &window_height);
            hex_renderer.draw_hex_map_frame(hex_map, window_width, window_height, camera);
            update_window(window);
        }
    }
}
