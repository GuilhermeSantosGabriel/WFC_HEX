#ifndef HEX_RENDERER_H
#define HEX_RENDERER_H

#include "models/hexmap.h"

#include <vector>
#include <cmath>
#include <external/glad/glad.h>
#include <external/glm/glm.hpp>
#include <external/glm/gtc/matrix_transform.hpp>

class HexRenderer {

public:

    HexRenderer(float hex_radius);

    void draw_hex_map_frame(HexMap& hex_map, int width, int height);

private:

    unsigned int VAO, VBO;
    unsigned int shader_program;
    float radius;

    void setup_geometry();
    void setup_shaders();
    unsigned int compile_shader(unsigned int type, const char* source);
};

#endif
