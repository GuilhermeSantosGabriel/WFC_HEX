#include "engine/render/simulation_runner.h"

#include <iostream>

struct MouseCallbackState {
    Camera& camera;
    double mouse_x_last;
    double mouse_y_last;
    // Used to avoid crazy jump on first mouse callback execution
    bool is_first_mouse_callback_execution = true;
};

// Processes all keyboard input
void input_keyboard_process(GLFWwindow* window, Camera& camera, float delta_time) {
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GL_TRUE);
        return;
    }

    // In relation to hexmap surface in initial orientation:
    // x: right, y: up, z: forward
    glm::vec3 direction = {0.0f, 0.0f, 0.0f};
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        direction.y += 1.0f;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        direction.y -= 1.0f;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        direction.x -= 1.0f;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        direction.x += 1.0f;
    }
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        direction.z -= 1.0f;
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
        direction.z += 1.0f;
    }
    camera.translate(direction, delta_time);
}

void mouse_callback(GLFWwindow* window, double mouse_x_current, double mouse_y_current) {
    auto callback_state = static_cast<MouseCallbackState*>(glfwGetWindowUserPointer(window));
    if (callback_state == nullptr) {
        std::cerr << "Mouse callback state is null" << '\n';
        return;
    }

    if (callback_state->is_first_mouse_callback_execution) {
        callback_state->mouse_x_last = mouse_x_current;
        callback_state->mouse_y_last = mouse_y_current;
        callback_state->is_first_mouse_callback_execution = false;
    }

    constexpr float MOUSE_SENSITIVITY = 0.1f;

    const float mouse_x_offset = mouse_x_current - callback_state->mouse_x_last;
    const float yaw_change = mouse_x_offset * MOUSE_SENSITIVITY;

    // mouse_y_offset would normally be inverted to match OpenGL coordinate system,
    // but projection matrix is flipped vertically during hexmap rendering
    const float mouse_y_offset = mouse_y_current - callback_state->mouse_y_last;
    const float pitch_change = mouse_y_offset * MOUSE_SENSITIVITY;

    callback_state->camera.rotate(yaw_change, pitch_change);
    callback_state->mouse_x_last = mouse_x_current;
    callback_state->mouse_y_last = mouse_y_current;
}

void run_step_visual_simulation(
    GLFWwindow* window,
    HexRenderer& hex_renderer,
    unsigned int step_counter,
    HexMap& hex_map,
    IStepGenerator& step_generator,
    double visual_start_timestamp,
    Camera& camera,
    bool persist_after_generator_finished
) {

    int window_height, window_width;
    unsigned int counter = 0;
    double last_frame_timestamp = glfwGetTime();

    MouseCallbackState mouse_callback_state = {camera, 0.0, 0.0, true};
    glfwSetWindowUserPointer(window, &mouse_callback_state);

    glfwSetCursorPosCallback(window, mouse_callback);

    while (
        !glfwWindowShouldClose(window) &&
        step_generator.step()
    ) {
        const double current_frame_timestamp = glfwGetTime();
        const auto delta_time = static_cast<float>(
            current_frame_timestamp - last_frame_timestamp
        );
        last_frame_timestamp = current_frame_timestamp;

        input_keyboard_process(window, camera, delta_time);
        if (counter >= step_counter) {
            const auto elapsed_time_seconds = static_cast<float>(
                current_frame_timestamp - visual_start_timestamp
            );
            clear_window(window, &window_width, &window_height);
            hex_renderer.draw_hex_map_frame(
                hex_map,
                window_width,
                window_height,
                camera,
                elapsed_time_seconds
            );
            update_window(window);
            counter = 0;
        }

        else counter++;
    }
    // This output isnt't visible when app is run through the bash script
    std::cout << "WFC finished" << '\n';

    // Sorry for this ugly boolean argument workaround
    if (persist_after_generator_finished) {
        while(!glfwWindowShouldClose(window)) {
            const double current_frame_timestamp = glfwGetTime();
            const auto delta_time = static_cast<float>(
                current_frame_timestamp - last_frame_timestamp
            );
            last_frame_timestamp = current_frame_timestamp;

            input_keyboard_process(window, camera, delta_time);
            const auto elapsed_time_seconds = static_cast<float>(
                current_frame_timestamp - visual_start_timestamp
            );
            clear_window(window, &window_width, &window_height);
            hex_renderer.draw_hex_map_frame(
                hex_map,
                window_width,
                window_height,
                camera,
                elapsed_time_seconds
            );
            update_window(window);
        }
    }

    glfwSetCursorPosCallback(window, nullptr);
    glfwSetWindowUserPointer(window, nullptr);
}
