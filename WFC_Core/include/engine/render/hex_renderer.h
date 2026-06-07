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

    static constexpr int VERTICES_PER_HEX = 6;
    // Upper and lower hexes of the cell's hexagonal prism 3D representation
    static constexpr int HEXES_PER_CELL = 2;
    float radius;

    void draw_hex_map_frame(const HexMap& hex_map, int width, int height, const Camera& camera);

private:

    std::array<glm::vec2, HEXES_PER_CELL * VERTICES_PER_HEX> hex_vertices_offset_from_hex_center;
    unsigned int VAO, VBO, EBO_TRIANGLES, EBO_LINES;
    unsigned int shader_program;

    void setup_geometry();
    void setup_shaders();
    unsigned int compile_shader(unsigned int type, const char* source);
};

#endif
