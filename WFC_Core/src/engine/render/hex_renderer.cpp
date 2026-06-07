#include "engine/render/render.h"
#include "engine/rules.h"

#include <iostream>

const char* vertexShaderSource = R"glsl(
    #version 460 core
    layout(location = 0) in vec3 aPos;
    layout(location = 0) uniform mat4 uProjection;
    void main() {
        gl_Position = uProjection * vec4(aPos, 1.0);
    }
)glsl";

const char* fragmentShaderSource = R"glsl(
    #version 460 core
    layout(location = 1) uniform vec3 uColor;
    out vec4 FragColor;
    void main() {
        FragColor = vec4(uColor, 1.0);
    }
)glsl";

constexpr int VERTICES_PER_HEX = HexRenderer::VERTICES_PER_HEX;

HexRenderer::HexRenderer(float hex_radius) : radius(hex_radius) {
    setup_shaders();
    setup_geometry();
    // Corner Points' angles - 60 degrees between each and the center
    // Hex corner vertex index visualization:
    //    2-----1
    //   /       \.
    //  3         0
    //   \       /.
    //    4-----5
    for (int i = 0; i < VERTICES_PER_HEX; ++i) {
        constexpr float ANGLE_BETWEEN_CONSECUTIVE_HEX_VERTICES_DEGREES = 60.0f;
        const float angle_radians = glm::radians(
            ANGLE_BETWEEN_CONSECUTIVE_HEX_VERTICES_DEGREES * i
        );

        const float x_offset = hex_radius * cos(angle_radians);
        const float y_offset = hex_radius * sin(angle_radians);
        const glm::vec2 xy_offset = {x_offset, y_offset};

        hex_vertices_offset_from_hex_center[i] = xy_offset;
        hex_vertices_offset_from_hex_center[i + VERTICES_PER_HEX] = xy_offset;
    }
}

unsigned int HexRenderer::compile_shader(unsigned int type, const char* source) {

    unsigned int id = glCreateShader(type);
    glShaderSource(id, 1, &source, nullptr);
    glCompileShader(id);

    // Error checking
    int success;
    char infoLog[512];
    glGetShaderiv(id, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(id, 512, nullptr, infoLog);
        std::cerr << "ERRO::SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    return id;
}

void HexRenderer::setup_shaders() {

    // Compile Shaders
    unsigned int vertex = compile_shader(GL_VERTEX_SHADER, vertexShaderSource);
    unsigned int fragment = compile_shader(GL_FRAGMENT_SHADER, fragmentShaderSource);

    // Creates and links Shader Program
    shader_program = glCreateProgram();
    glAttachShader(shader_program, vertex);
    glAttachShader(shader_program, fragment);
    glLinkProgram(shader_program);

    // Error checking
    int success;
    char infoLog[512];
    glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shader_program, 512, nullptr, infoLog);
        std::cerr << "ERRO::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    // Can delete because they are already linked to the program
    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

constexpr int TRIANGLES_PER_HEX = 4;
constexpr int VERTICES_PER_TRIANGLE = 3;
constexpr int VERTEX_INDICES_PER_HEX = TRIANGLES_PER_HEX * VERTICES_PER_TRIANGLE;
constexpr int HEXES_PER_CELL = HexRenderer::HEXES_PER_CELL;
constexpr int HEX_VERTEX_INDICES_PER_CELL = HEXES_PER_CELL *  VERTEX_INDICES_PER_HEX;

constexpr int TRIANGLES_PER_RECTANGLE = 2;
constexpr int VERTEX_INDICES_PER_RECTANGLE = TRIANGLES_PER_RECTANGLE * VERTICES_PER_TRIANGLE;
constexpr int RECTANGLES_PER_CELL = 6;
constexpr int RECTANGLE_VERTEX_INDICES_PER_CELL =  RECTANGLES_PER_CELL * VERTEX_INDICES_PER_RECTANGLE;

constexpr int VERTEX_INDICES_PER_CELL = HEX_VERTEX_INDICES_PER_CELL + RECTANGLE_VERTEX_INDICES_PER_CELL;

void HexRenderer::setup_geometry() {

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    // Hex corner vertex index visualization:
    //    2-----1
    //   /       \.
    //  3         0
    //   \       /
    //    4-----5
    //
    // Prism visualization (corresponding hex indices are connected when rendering):
    //    8-----7
    //   /       \.
    //  9         6
    //   \       /
    //   10-----11
    //
    //    2-----1
    //   /       \.
    //  3         0
    //   \       /
    //    4-----5
    constexpr std::array<unsigned int, VERTEX_INDICES_PER_CELL> HEX_VERTEX_INDICES = {
        // Lower hex
        0,  2,  1, // Triangle top right
        0,  3,  2, // 0, 2, 3, 5 Rectangle top left half
        0,  5,  3, // 0, 2, 3, 5 Rectangle bottom rigth half
        3,  5,  4, // Triangle bottom left
        // Upper hex
        6,  7,  8, // Triangle top right
        6,  8,  9, // 0, 2, 3, 5 Rectangle top left half
        6,  9, 11, // 0, 2, 3, 5 Rectangle bottom rigth half
        9, 10, 11, // Triangle bottom left
        // Side rectangle 0, 1, 6, 7
        0,  6,  1, // Bottom left half
        1,  6,  7, // Top right half
        // Side rectangle 1, 2, 7, 8
        1,  7,  2, // Bottom left half
        2,  7,  8, // Top right half
        // Side rectangle 2, 3, 8, 9
        2,  8,  3, // Bottom left half
        3,  8,  9, // Top right half
        // Side rectangle 3, 4, 9, 10
        3,  9,  4, // Bottom left half
        4,  9, 10, // Top right half
        // Side rectangle 4, 5, 10, 11
        4, 10, 5, // Bottom left half
        5, 10, 11, // Top right half
        // Side rectangle 0, 5, 6, 11
        5, 11, 0, // Bottom left half
        0, 11, 6  // Top right half
    };
    glBufferData(
        GL_ELEMENT_ARRAY_BUFFER,
        HEX_VERTEX_INDICES.size() * sizeof(unsigned int),
        HEX_VERTEX_INDICES.data(),
        GL_STATIC_DRAW
    );

    // Tells opengl that each vertex is x, y, z
    constexpr GLint aPos_element_count = 3;
    constexpr GLint aPos_size = aPos_element_count * sizeof(float);

    constexpr GLint aPos_location = 0;
    glVertexAttribPointer(
        aPos_location,
        aPos_element_count,
        GL_FLOAT,
        GL_FALSE,
        aPos_size,
        (void*)0
    );
    glEnableVertexAttribArray(aPos_location);
}

// Returns provided tile's RGB color
glm::vec3 tile_color(int tile) {
    switch (tile) {
    case GRASS:
        return glm::vec3(124.0f/255.0f, 255.0f/255.0f, 124.0f/255.0f);
    case FOREST:
        return glm::vec3(32.0f/255.0f, 87.0f/255.0f, 12.0f/255.0f);
    case WATER:
        return glm::vec3(0.0f, 0.0f, 1.0f);
    case SAND:
        return glm::vec3(1.0f, 1.0f, 0.0f);
    case EMPTY:
        return glm::vec3(1.0f, 1.0f, 1.0f);
    default:
        throw std::runtime_error("Invalid tile provided to tile_color");
    }
}

void HexRenderer::draw_hex_map_frame(const HexMap& hex_map, int width, int height, const Camera& camera) {

    glUseProgram(shader_program);

    constexpr float field_of_view = glm::radians(45.0f);
    const float aspect_ratio = static_cast<float>(width) / static_cast<float>(height);
    constexpr float near_plane = 0.1f;
    constexpr float far_plane = 100000.0f;
    glm::mat4 perspective_matrix = glm::perspective(
        field_of_view,
        aspect_ratio,
        near_plane,
        far_plane
    );
    // Flip projection y axis so initial camera orientation matches Python
    // visualization scripts output image
    perspective_matrix[1][1] *= -1.0f;

    const glm::mat4 projection = perspective_matrix * camera.view_matrix();
    constexpr GLint uProjection_location = 0;
    glUniformMatrix4fv(uProjection_location, 1, GL_FALSE, &projection[0][0]);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    constexpr float HEXMAP_SURFACE_HEIGHT = 0.0f;
    const float world_height_scale = HexRenderer::radius / 4;

    for (const auto& cell : hex_map.cells) {

        glm::vec3 cell_color = {0.9f, 0.9f, 0.9f};
        if (cell.collapsed) {
            // Because cell is collapsed, possible_tiles has only one element,
            // which is the cell's tile
            const int cell_tile = *cell.possible_tiles.begin();
            cell_color = tile_color(cell_tile);
        }
        constexpr GLint uColor_location = 1;
        glUniform3f(uColor_location, cell_color.x, cell_color.y, cell_color.z);

        const float cell_prism_height = cell.get_height() * world_height_scale;

        constexpr int FLOATS_PER_VERTEX = 3;
        std::array<float, HEXES_PER_CELL * VERTICES_PER_HEX * FLOATS_PER_VERTEX> corner_vertices;
        for (int i = 0; i < VERTICES_PER_HEX; ++i) {
            // lower hex x
            corner_vertices[FLOATS_PER_VERTEX * i] = cell.x +
                hex_vertices_offset_from_hex_center[i].x;
            // lower hex y
            corner_vertices[FLOATS_PER_VERTEX * i + 1] = cell.y +
                hex_vertices_offset_from_hex_center[i].y;
            // lower hex z
            corner_vertices[FLOATS_PER_VERTEX * i + 2] = HEXMAP_SURFACE_HEIGHT;

            // upper hex x
            corner_vertices[FLOATS_PER_VERTEX * (i + VERTICES_PER_HEX)] = cell.x +
                hex_vertices_offset_from_hex_center[i + VERTICES_PER_HEX].x;
            // upper hex y
            corner_vertices[FLOATS_PER_VERTEX * (i + VERTICES_PER_HEX) + 1] = cell.y +
                hex_vertices_offset_from_hex_center[i + VERTICES_PER_HEX].y;
            // upper hex z
            corner_vertices[FLOATS_PER_VERTEX * (i + VERTICES_PER_HEX) + 2] = cell_prism_height;
        }
        glBufferData(
            GL_ARRAY_BUFFER,
            corner_vertices.size() * sizeof(float),
            corner_vertices.data(),
            GL_STREAM_DRAW
        );

        constexpr void* EBO_OFFSET = 0;
        // Draw cell prism from 20 triangles in the order of EBO indices using the
        // corresponding VBO data of each EBO index
        glDrawElements(GL_TRIANGLES, VERTEX_INDICES_PER_CELL, GL_UNSIGNED_INT, EBO_OFFSET);
    }
}
