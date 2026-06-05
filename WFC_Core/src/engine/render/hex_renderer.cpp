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

constexpr int CORNER_VERTICES_PER_HEX = 6;

HexRenderer::HexRenderer(float hex_radius) : radius(hex_radius) {
    setup_shaders();
    setup_geometry();
    // Corner Points' offset - 60 degrees between each and the center
    // Hex corner vertex index visualization:
    //    2    1
    //    /----\.
    // 3 /      \ 0
    //   \      /
    //    \----/
    //    4    5
    for (int i = 0; i < CORNER_VERTICES_PER_HEX; ++i) {
        const float angle = i * 60.0f * (M_PI / 180.0f);
        // x offset
        hex_vertices_offset_from_hex_center[2 * i] = hex_radius * cos(angle);
        // y offset
        hex_vertices_offset_from_hex_center[2 * i + 1] = hex_radius * sin(angle);
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

void HexRenderer::setup_geometry() {

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    // Hex corner vertex index visualization:
    //    2    1
    //    /----\.
    // 3 /      \ 0
    //   \      /
    //    \----/
    //    4    5
    constexpr std::array<unsigned int, VERTEX_INDICES_PER_HEX> HEX_VERTEX_INDICES = {
        0, 1, 2, // Triangle top right
        0, 2, 3, // 0,2,3,5 Rectangle top left half
        0, 3, 5, // 0,2,3,5 Rectangle bottom rigth half
        3, 4, 5 // Triangle bottom left
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

void HexRenderer::draw_hex_map_frame(HexMap& hex_map, int width, int height) {

    glUseProgram(shader_program);

    // NDC conversion matrix
    glm::mat4 projection = glm::ortho(0.0f, (float)width, (float)height, 0.0f, -1.0f, 1.0f);
    constexpr GLint uProjection_location = 0;
    glUniformMatrix4fv(uProjection_location, 1, GL_FALSE, &projection[0][0]);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    for (const auto& cell : hex_map.cells) {

        glm::vec3 color = glm::vec3(0.9f, 0.9f, 0.9f);
        if (cell.collapsed) {
            // Because cell is collapsed, possible_tiles has only one element,
            // which is the cell's tile
            const int cell_tile = *cell.possible_tiles.begin();
            color = tile_color(cell_tile);
        }

        constexpr GLint uColor_location = 1;
        glUniform3f(uColor_location, color.x, color.y, color.z);

        constexpr int FLOATS_PER_VERTEX = 3;
        std::array<float, CORNER_VERTICES_PER_HEX * FLOATS_PER_VERTEX> corner_vertices;
        for (int i = 0; i < CORNER_VERTICES_PER_HEX; ++i) {
            // x
            corner_vertices[FLOATS_PER_VERTEX * i] = cell.x +
                hex_vertices_offset_from_hex_center[2 * i];
            // y
            corner_vertices[FLOATS_PER_VERTEX * i + 1] = cell.y +
                hex_vertices_offset_from_hex_center[2 * i + 1];
            // z
            corner_vertices[FLOATS_PER_VERTEX * i + 2] = 0.0f;
        }
        glBufferData(
            GL_ARRAY_BUFFER,
            corner_vertices.size() * sizeof(float),
            corner_vertices.data(),
            GL_STREAM_DRAW
        );

        constexpr void* EBO_OFFSET = 0;
        // Draw hex from 4 triangles in the order of EBO indices using the
        // corresponding VBO data of each EBO index
        glDrawElements(GL_TRIANGLES, VERTEX_INDICES_PER_HEX, GL_UNSIGNED_INT, EBO_OFFSET);
    }
}
