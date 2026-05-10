#include "engine/opengl/hex_renderer.h"

#include <iostream>

const char* vertexShaderSource = R"glsl(
    #version 330 core
    layout (location = 0) in vec2 aPos;
    uniform mat4 projection;
    uniform vec2 offset;
    void main() {
        gl_Position = projection * vec4(aPos + offset, 0.0, 1.0);
    }
)glsl";

const char* fragmentShaderSource = R"glsl(
    #version 330 core
    out vec4 FragColor;
    uniform vec3 color;
    void main() {
        FragColor = vec4(color, 1.0);
    }
)glsl";

HexRenderer::HexRenderer(float hex_radius) : radius(hex_radius) {
    setup_shaders();
    setup_geometry();
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

void HexRenderer::setup_geometry() {

    std::vector<float> vertices;

    // Center point
    vertices.push_back(0.0f);
    vertices.push_back(0.0f);

    // Corner Points - 60 degrees between each and the center
    for (int i = 0; i <= 6; ++i) {
        float angle = i * 60.0f * (M_PI / 180.0f);
        vertices.push_back(radius * cos(angle));
        vertices.push_back(radius * sin(angle));
    }

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    // Tells opengl that each vertex is x and y
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
}

void HexRenderer::draw_hex_map_frame(HexMap& hex_map, int width, int height) {

    glUseProgram(shader_program);

    // NDC conversion matrix
    glm::mat4 projection = glm::ortho(0.0f, (float)width, (float)height, 0.0f, -1.0f, 1.0f);
    glUniformMatrix4fv(glGetUniformLocation(shader_program, "projection"), 1, GL_FALSE, &projection[0][0]);

    glBindVertexArray(VAO);

    for (const auto& cell : hex_map.cells) {

        // TODO - deal with color cell definition
        glm::vec3 color = cell.collapsed ? glm::vec3(0.2f, 0.8f, 0.2f) : glm::vec3(0.9f, 0.9f, 0.9f);

        // Sends cell data to shader
        glUniform2f(glGetUniformLocation(shader_program, "offset"), (float)cell.x, (float)cell.y);
        // glUniform3f(glGetUniformLocation(shader_program, "color"), color.x, color.y, color.z);

        // Draws the hex using triangle fan with 8 vertices
        // center + 6 corners + closing
        glDrawArrays(GL_TRIANGLE_FAN, 0, 8);
    }
}
