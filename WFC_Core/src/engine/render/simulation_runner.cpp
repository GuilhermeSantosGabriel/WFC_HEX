#include "engine/render/render.h"

// Processes all keyboard input
void input_keyboard_process(GLFWwindow* window, Camera& camera, float delta_time) {
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

    double last_frame_timestamp = glfwGetTime();

    while (
        !glfwWindowShouldClose(window) &&
        step_generator.step()
    ) {
        const double current_frame_timestamp = glfwGetTime();
        const float delta_time = static_cast<float>(
            current_frame_timestamp - last_frame_timestamp
        );
        last_frame_timestamp = current_frame_timestamp;

        input_keyboard_process(window, camera, delta_time);
        if (counter >= step_counter) {
            clear_window(window, &window_width, &window_height);
            hex_renderer.draw_hex_map_frame(hex_map, window_width, window_height, camera);
            update_window(window);
            counter = 0;
        }

        else counter++;
    }
    if (persist_after_generator_finished) {
        while(!glfwWindowShouldClose(window)) {
            const double current_frame_timestamp = glfwGetTime();
            const float delta_time = static_cast<float>(
                current_frame_timestamp - last_frame_timestamp
            );
            last_frame_timestamp = current_frame_timestamp;

            input_keyboard_process(window, camera, delta_time);
            clear_window(window, &window_width, &window_height);
            hex_renderer.draw_hex_map_frame(hex_map, window_width, window_height, camera);
            update_window(window);
        }
    }
}
