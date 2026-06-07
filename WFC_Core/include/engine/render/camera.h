#ifndef CAMERA_H
#define CAMERA_H

#include <external/glm/glm.hpp>
#include <external/glm/gtc/matrix_transform.hpp>

class Camera {
public:

    Camera(glm::vec3 position);

    glm::vec3 position;
    // pitch in degrees
    float pitch;
    // yaw in degrees
    float yaw;
    // Positive change in y moves camera up in relation to initial camera orientation
    static constexpr glm::vec3 AXIS_VECTOR_UP_NORMALIZED = {0.0f, 1.0f, 0.0f};
    static constexpr float CAMERA_SPEED_MAP_UNITS_PER_SECOND = 1500.0f;

    glm::vec3 direction_normalized() const;
    glm::mat4 view_matrix() const;
    void translate(glm::vec3 direction, float delta_time);
    void rotate(float yaw_change, float pitch_change);
};

#endif
