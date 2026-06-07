#include "engine/render/camera.h"

Camera::Camera(glm::vec3 position) : position(position) {
    yaw = -90.0f;
    pitch = 0.0f;
}

glm::vec3 Camera::direction_normalized() const {
    glm::vec3 direction;
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    return glm::normalize(direction);
}

glm::mat4 Camera::view_matrix() const {
    return glm::lookAt(position, position + direction_normalized(), AXIS_VECTOR_UP_NORMALIZED);
}

void Camera::translate(glm::vec3 direction, float delta_time) {
    if (glm::length(direction) == 0.0f) {
        return;
    }

    direction = glm::normalize(direction);
    const glm::vec3 axis_vector_front_normalized = direction_normalized();
    const glm::vec3 axis_vector_right_normalized = glm::normalize(
        glm::cross(axis_vector_front_normalized, AXIS_VECTOR_UP_NORMALIZED)
    );

    const float movement_distance_map_units = CAMERA_SPEED_MAP_UNITS_PER_SECOND * delta_time;

    position += movement_distance_map_units * direction.x * axis_vector_right_normalized;
    position += movement_distance_map_units * direction.y * axis_vector_front_normalized;
    position += movement_distance_map_units * direction.z * AXIS_VECTOR_UP_NORMALIZED;
}

void Camera::rotate(float yaw_change, float pitch_change) {
    const float yaw_new = yaw + yaw_change;
    yaw = yaw_new;

    const float pitch_new = pitch + pitch_change;
    constexpr float PITCH_MIN = -89.0f;
    constexpr float PITCH_MAX = 89.0f;
    // Avoid weird camera direction flips
    if (pitch_new < PITCH_MIN) {
        pitch = PITCH_MIN;
    } else if (pitch_new > PITCH_MAX) {
        pitch = PITCH_MAX;
    } else {
        pitch = pitch_new;
    }
}
