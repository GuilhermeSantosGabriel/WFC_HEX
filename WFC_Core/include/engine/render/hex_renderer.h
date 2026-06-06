#ifndef HEX_RENDERER_H
#define HEX_RENDERER_H

#include "models/hexmap.h"
#include "engine/render/camera.h"

#include <array>
#include <cmath>
#include <external/glad/glad.h>
#include <external/glm/glm.hpp>
#include <external/glm/gtc/matrix_transform.hpp>

class HexRenderer {

public:

    HexRenderer(float hex_radius);

    void draw_hex_map_frame(const HexMap& hex_map, int width, int height, const Camera& camera);

private:

    std::array<float, 12> hex_vertices_offset_from_hex_center;
    unsigned int VAO, VBO, EBO;
    unsigned int shader_program;
    float radius;

    void setup_geometry();
    void setup_shaders();
    unsigned int compile_shader(unsigned int type, const char* source);
};

#endif
