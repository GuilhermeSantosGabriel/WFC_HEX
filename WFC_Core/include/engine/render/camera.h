#ifndef CAMERA_H
#define CAMERA_H

#include <external/glm/glm.hpp>
#include <external/glm/gtc/matrix_transform.hpp>

class Camera {
public:

    Camera(glm::vec3 position);

    glm::vec3 position;
    // Positive change in y moves camera up
    inline static glm::vec3 axis_vector_up_normalized;
    // Negative change in z moves camera forward
    inline static glm::vec3 axis_vector_front_normalized;
    static constexpr float camera_speed_map_units_per_second = 300.0f;

    glm::mat4 view_matrix() const;
    float delta_time();
    void translate(glm::vec2 direction, float delta_time);

private:
    float last_frame_timestamp;
};

#endif
