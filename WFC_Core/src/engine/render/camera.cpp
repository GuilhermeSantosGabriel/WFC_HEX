#include "engine/render/camera.h"

#include <external/GLFW/glfw3.h>

Camera::Camera(glm::vec3 position) : position(position) {
    axis_vector_up_normalized = glm::normalize(glm::vec3(0.0f, 1.0f, 0.0f));
    axis_vector_front_normalized = glm::normalize(glm::vec3(0.0f, 0.0f, -1.0f));
    last_frame_timestamp = glfwGetTime();
}

glm::mat4 Camera::view_matrix() const {
    return glm::lookAt(position, position + axis_vector_front_normalized, axis_vector_up_normalized);
}

float Camera::delta_time() {
    const float current_frame_timestamp = glfwGetTime();
    const float delta_time = current_frame_timestamp - last_frame_timestamp;
    last_frame_timestamp = current_frame_timestamp;
    return delta_time;
}

void Camera::translate(glm::vec2 direction, float delta_time) {
    if (glm::length(direction) == 0.0f) {
        return;
    }

    direction = glm::normalize(direction);
    // Cross-product of 2 normalized vectors is also a normalized vector
    const glm::vec3 axis_vector_right_normalized = glm::cross(
        axis_vector_front_normalized,
        axis_vector_up_normalized
    );

    position += delta_time * camera_speed_map_units_per_second * direction.x * axis_vector_right_normalized;
    position += delta_time * camera_speed_map_units_per_second * direction.y * axis_vector_up_normalized;
}
