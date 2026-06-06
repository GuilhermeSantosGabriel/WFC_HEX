#include "engine/render/camera.h"

Camera::Camera(glm::vec3 position) : position(position) {
    axis_vector_up_normalized = glm::normalize(glm::vec3(0.0f, 1.0f, 0.0f));
    axis_vector_front_normalized = glm::normalize(glm::vec3(0.0f, 0.0f, -1.0f));
}

glm::mat4 Camera::view_matrix() const {
    return glm::lookAt(position, position + axis_vector_front_normalized, axis_vector_up_normalized);
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
