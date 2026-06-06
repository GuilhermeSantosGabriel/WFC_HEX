#ifndef CAMERA_H
#define CAMERA_H

#include <external/glm/glm.hpp>
#include <external/glm/gtc/matrix_transform.hpp>

class Camera {
public:

    Camera(glm::vec3 position);

    glm::vec3 position;
    glm::vec3 direction_normalized;
    // Positive change in y moves camera up.
    static constexpr glm::vec3 axis_vector_up_normalized = glm::vec3(0.0f, 1.0f, 0.0f);
    // Negative change in z moves camera forward
    static constexpr glm::vec3 axis_vector_front_normalized = glm::vec3(0.0f, 0.0f, -1.0f);
    static constexpr float camera_speed_map_units_per_second = 300.0f;

    glm::mat4 view_matrix() const;
    void translate(glm::vec2 direction, float delta_time);
};

#endif
