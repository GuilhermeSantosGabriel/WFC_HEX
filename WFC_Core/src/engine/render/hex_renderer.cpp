#include "engine/render/render.h"
#include "engine/rules.h"

#include <algorithm>
#include <iostream>
#include <stdexcept>

std::array<glm::vec2, HexRenderer::VERTICES_PER_HEX> HexRenderer::hex_vertices_offset_from_hex_center;

constexpr const char* vertexShaderSource = R"glsl(
    #version 450 core
    layout(location = 0) in vec3 aPosition;
    layout(location = 1) in vec3 aNormal;

    layout(location = 0) uniform mat4 uProjection;

    out vec3 fragmentPosition;
    out vec3 fragmentNormal;

    void main() {
        fragmentPosition = aPosition;
        fragmentNormal = aNormal;
        gl_Position = uProjection * vec4(aPosition, 1.0);
    }
)glsl";

constexpr const char* fragmentShaderSource = R"glsl(
    #version 450 core
    layout(location = 1) uniform vec3 uViewPosition;
    layout(location = 2) uniform vec3 uSunDirection;
    layout(location = 3) uniform vec3 uColor;
    layout(location = 4) uniform bool uUseLighting;
    layout(location = 5) uniform float uLightAmbientIntensity;
    layout(location = 6) uniform float uLightDiffuseIntensity;
    layout(location = 7) uniform float uLightSpecularIntensity;
    layout(location = 8) uniform vec3 uMaterialAmbientColor;
    layout(location = 9) uniform vec3 uMaterialDiffuseColor;
    layout(location = 10) uniform vec3 uMaterialSpecularColor;
    layout(location = 11) uniform float uMaterialShininess;

    in vec3 fragmentPosition;
    // Fragment's face's normal vector. Vector is the same for all fragments in
    // the same face by having all vertices of each face have the exact same normal vector,
    // so prism faces look flat rather than smooth
    in vec3 fragmentNormal;

    out vec4 outFragmentColor;

    void main() {
        const float MINIMUM_LIGHT_COSINE = 0.0;
        const float COLOR_ALPHA = 1.0;

        if (!uUseLighting) {
            outFragmentColor = vec4(uColor, COLOR_ALPHA);
            return;
        }

        // Phong lighting used below
        vec3 normalizedNormal = normalize(fragmentNormal);
        vec3 lightDirection = normalize(uSunDirection);
        vec3 viewDirection = normalize(uViewPosition - fragmentPosition);
        vec3 reflectionDirection = reflect(-lightDirection, normalizedNormal);

        vec3 ambientColor = uLightAmbientIntensity * uMaterialAmbientColor;

        float diffuseCosine = max(dot(normalizedNormal, lightDirection), MINIMUM_LIGHT_COSINE);
        vec3 diffuseColor = uLightDiffuseIntensity * diffuseCosine * uMaterialDiffuseColor;

        float specularCosine = max(dot(viewDirection, reflectionDirection), MINIMUM_LIGHT_COSINE);
        float specularStrength = pow(specularCosine, uMaterialShininess);
        vec3 specularColor = uLightSpecularIntensity * specularStrength * uMaterialSpecularColor;

        outFragmentColor = vec4(ambientColor + diffuseColor + specularColor, COLOR_ALPHA);
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
    }
}

unsigned int HexRenderer::compile_shader(const unsigned int type, const char* source) {

    const unsigned int id = glCreateShader(type);
    glShaderSource(id, 1, &source, nullptr);
    glCompileShader(id);

    // Error checking
    int success;
    char infoLog[512];
    glGetShaderiv(id, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(id, 512, nullptr, infoLog);
        std::cerr << "ERRO::SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
        throw std::runtime_error("Error during shader compilation");
    }

    return id;
}

void HexRenderer::setup_shaders() {

    // Compile Shaders
    const unsigned int vertex = compile_shader(GL_VERTEX_SHADER, vertexShaderSource);
    const unsigned int fragment = compile_shader(GL_FRAGMENT_SHADER, fragmentShaderSource);

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
        throw std::runtime_error("Error during shader compilation");
    }

    // Can delete because they are already linked to the program
    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

constexpr int TRIANGLES_PER_HEX = 4;
constexpr int VERTICES_PER_TRIANGLE = 3;
constexpr int VERTEX_INDICES_PER_HEX = TRIANGLES_PER_HEX * VERTICES_PER_TRIANGLE;
constexpr int HEXES_PER_CELL = 2;
constexpr int HEX_VERTEX_INDICES_PER_CELL = HEXES_PER_CELL *  VERTEX_INDICES_PER_HEX;

constexpr int TRIANGLES_PER_RECTANGLE = 2;
constexpr int VERTEX_INDICES_PER_RECTANGLE = TRIANGLES_PER_RECTANGLE * VERTICES_PER_TRIANGLE;
constexpr int RECTANGLES_PER_CELL = 6;
constexpr int RECTANGLE_VERTEX_INDICES_PER_CELL =  RECTANGLES_PER_CELL * VERTEX_INDICES_PER_RECTANGLE;

constexpr int VERTEX_INDICES_PER_CELL = HEX_VERTEX_INDICES_PER_CELL + RECTANGLE_VERTEX_INDICES_PER_CELL;
constexpr int LIT_TRIANGLE_VERTEX_INDEX_START = VERTEX_INDICES_PER_HEX;
constexpr int LIT_VERTEX_INDICES_PER_CELL = VERTEX_INDICES_PER_CELL - LIT_TRIANGLE_VERTEX_INDEX_START;
constexpr int POSITION_FLOATS_PER_VERTEX = 3;
constexpr int NORMAL_FLOATS_PER_VERTEX = 3;
constexpr int FLOATS_PER_VERTEX = POSITION_FLOATS_PER_VERTEX + NORMAL_FLOATS_PER_VERTEX;

constexpr int LINE_VERTEX_INDICES_PER_EDGE = 2;
constexpr int HEXAGON_EDGE_COUNT = 6;
constexpr int VERTICAL_EDGES_PER_PRISM = 6;
constexpr int LINE_VERTEX_INDICES_PER_CELL = (HEXES_PER_CELL * HEXAGON_EDGE_COUNT +
    VERTICAL_EDGES_PER_PRISM) * LINE_VERTEX_INDICES_PER_EDGE;

constexpr std::array<unsigned int,
    // Prism vertices visualization (corresponding hex indices are connected when rendering):
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
    VERTEX_INDICES_PER_CELL> HEXAGONAL_PRISM_TRIANGLE_VERTEX_INDICES = {
    // lower hex
    0,  2,  1, // triangle top right
    0,  3,  2, // 0, 2, 3, 5 rectangle top left half
    0,  5,  3, // 0, 2, 3, 5 rectangle bottom rigth half
    3,  5,  4, // triangle bottom left
    // upper hex
    6,  7,  8, // triangle top right
    6,  8,  9, // 6, 8, 9, 11 rectangle top left half
    6,  9, 11, // 6, 8, 9, 11 rectangle bottom rigth half
    9, 10, 11, // triangle bottom left
    // side rectangle 0, 1, 6, 7
    0, 1, 6,
    1, 7, 6,
    // side rectangle 1, 2, 7, 8
    1, 2, 7,
    2, 8, 7,
    // side rectangle 2, 3, 8, 9
    2, 3, 8,
    3, 9, 8,
    // side rectangle 3, 4, 9, 10
    3, 4, 9,
    4, 10, 9,
    // side rectangle 4, 5, 10, 11
    4, 5, 10,
    5, 11, 10,
    // side rectangle 0, 5, 6, 11
    5, 0, 11,
    0, 6, 11
};

void HexRenderer::setup_geometry() {

    glGenVertexArrays(1, &VAO);

    glGenBuffers(1, &VBO);

    glGenBuffers(1, &EBO_LINES);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    constexpr std::array<unsigned int,
        LINE_VERTEX_INDICES_PER_CELL> HEXAGONAL_PRISM_LINE_VERTEX_INDICES = {
        // lower hex outline
        0, 1,
        1, 2,
        2, 3,
        3, 4,
        4, 5,
        5, 0,
        // upper hex outline
        6, 7,
        7, 8,
        8, 9,
        9, 10,
        10, 11,
        11, 6,
        // vertical edges
        0, 6,
        1, 7,
        2, 8,
        3, 9,
        4, 10,
        5, 11
    };
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_LINES);
    glBufferData(
        GL_ELEMENT_ARRAY_BUFFER,
        HEXAGONAL_PRISM_LINE_VERTEX_INDICES.size() * sizeof(unsigned int),
        HEXAGONAL_PRISM_LINE_VERTEX_INDICES.data(),
        GL_STATIC_DRAW
    );

    // Tells opengl that each vertex is position xyz and normal xyz
    constexpr GLint POSITION_ELEMENT_COUNT = 3;
    constexpr GLint NORMAL_ELEMENT_COUNT = 3;
    constexpr GLint FLOATS_PER_VERTEX = POSITION_ELEMENT_COUNT + NORMAL_ELEMENT_COUNT;
    constexpr GLint VERTEX_SIZE = FLOATS_PER_VERTEX * sizeof(float);

    constexpr GLint aPosition_location = 0;
    glVertexAttribPointer(
        aPosition_location,
        POSITION_ELEMENT_COUNT,
        GL_FLOAT,
        GL_FALSE,
        VERTEX_SIZE,
        (void*)0
    );
    glEnableVertexAttribArray(aPosition_location);

    constexpr GLint aNormal_location = 1;
    glVertexAttribPointer(
        aNormal_location,
        NORMAL_ELEMENT_COUNT,
        GL_FLOAT,
        GL_FALSE,
        VERTEX_SIZE,
        (void*)(POSITION_ELEMENT_COUNT * sizeof(float))
    );
    glEnableVertexAttribArray(aNormal_location);

    // Unbinding for cleanup
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

struct Material {
    glm::vec3 color_ambient;
    glm::vec3 color_diffuse;
    glm::vec3 color_specular;
    float shininess;
};

Material tile_material(int tile) {
    switch (tile) {
    case GRASS: {
        constexpr glm::vec3 COLOR_AMBIENT = {124.0f/255.0f, 255.0f/255.0f, 124.0f/255.0f};
        constexpr glm::vec3 COLOR_DIFFUSE = {124.0f/255.0f, 255.0f/255.0f, 124.0f/255.0f};
        constexpr glm::vec3 COLOR_SPECULAR = {0.08f, 0.08f, 0.08f};
        constexpr float SHININESS = 8.0f;
        return {COLOR_AMBIENT, COLOR_DIFFUSE, COLOR_SPECULAR, SHININESS};
    }
    case FOREST: {
        constexpr glm::vec3 COLOR_AMBIENT = {32.0f/255.0f, 87.0f/255.0f, 12.0f/255.0f};
        constexpr glm::vec3 COLOR_DIFFUSE = {32.0f/255.0f, 87.0f/255.0f, 12.0f/255.0f};
        constexpr glm::vec3 COLOR_SPECULAR = {0.03f, 0.03f, 0.03f};
        constexpr float SHININESS = 4.0f;
        return {COLOR_AMBIENT, COLOR_DIFFUSE, COLOR_SPECULAR, SHININESS};
    }
    case WATER: {
        constexpr glm::vec3 COLOR_AMBIENT = {0.0f, 0.0f, 1.0f};
        constexpr glm::vec3 COLOR_DIFFUSE = {0.0f, 0.15f, 0.65f};
        constexpr glm::vec3 COLOR_SPECULAR = {1.0f, 1.0f, 1.0f};
        constexpr float SHININESS = 64.0f;
        return {COLOR_AMBIENT, COLOR_DIFFUSE, COLOR_SPECULAR, SHININESS};
    }
    case SAND: {
        constexpr glm::vec3 COLOR_AMBIENT = {1.0f, 1.0f, 0.0f};
        constexpr glm::vec3 COLOR_DIFFUSE = {1.0f, 1.0f, 0.0f};
        constexpr glm::vec3 COLOR_SPECULAR = {0.25f, 0.25f, 0.18f};
        constexpr float SHININESS = 16.0f;
        return {COLOR_AMBIENT, COLOR_DIFFUSE, COLOR_SPECULAR, SHININESS};
    }
    case EMPTY: {
        constexpr glm::vec3 COLOR_AMBIENT = {0.9f, 0.9f, 0.9f};
        constexpr glm::vec3 COLOR_DIFFUSE = {0.9f, 0.9f, 0.9f};
        constexpr glm::vec3 COLOR_SPECULAR = {0.02f, 0.02f, 0.02f};
        constexpr float SHININESS = 4.0f;
        return {COLOR_AMBIENT, COLOR_DIFFUSE, COLOR_SPECULAR, SHININESS};
    }
    default:
        throw std::runtime_error("Invalid tile provided to tile_material");
    }
}

std::array<float, LIT_VERTEX_INDICES_PER_CELL * FLOATS_PER_VERTEX> build_lit_triangle_vertices(
    const std::array<glm::vec3, HEXES_PER_CELL * VERTICES_PER_HEX>& corner_positions
) {
    std::array<float, LIT_VERTEX_INDICES_PER_CELL * FLOATS_PER_VERTEX> vertices = {};

    for (int i = LIT_TRIANGLE_VERTEX_INDEX_START; i < VERTEX_INDICES_PER_CELL; i += VERTICES_PER_TRIANGLE) {
        const unsigned int first_index = HEXAGONAL_PRISM_TRIANGLE_VERTEX_INDICES[i];
        const unsigned int second_index = HEXAGONAL_PRISM_TRIANGLE_VERTEX_INDICES[i + 1];
        const unsigned int third_index = HEXAGONAL_PRISM_TRIANGLE_VERTEX_INDICES[i + 2];

        const glm::vec3 first_to_second = corner_positions[second_index] -
            corner_positions[first_index];
        const glm::vec3 first_to_third = corner_positions[third_index] -
            corner_positions[first_index];
        const glm::vec3 face_normal_raw = glm::cross(first_to_second, first_to_third);
        const glm::vec3 face_normal = glm::length(face_normal_raw) == 0.0f ?
            glm::vec3(0.0f, 0.0f, 1.0f) :
            glm::normalize(face_normal_raw);

        for (int triangle_vertex = 0; triangle_vertex < VERTICES_PER_TRIANGLE; ++triangle_vertex) {
            const unsigned int corner_index =
                HEXAGONAL_PRISM_TRIANGLE_VERTEX_INDICES[i + triangle_vertex];
            const int vertex_offset =
                (i - LIT_TRIANGLE_VERTEX_INDEX_START + triangle_vertex) * FLOATS_PER_VERTEX;
            vertices[vertex_offset] = corner_positions[corner_index].x;
            vertices[vertex_offset + 1] = corner_positions[corner_index].y;
            vertices[vertex_offset + 2] = corner_positions[corner_index].z;

            // All vertices have exact same normal vector so prism faces look flat rather than smooth
            vertices[vertex_offset + 3] = face_normal.x;
            vertices[vertex_offset + 4] = face_normal.y;
            vertices[vertex_offset + 5] = face_normal.z;
        }
    }

    return vertices;
}

std::array<float, HEXES_PER_CELL * VERTICES_PER_HEX * FLOATS_PER_VERTEX> build_outline_vertices(
    const std::array<glm::vec3, HEXES_PER_CELL * VERTICES_PER_HEX>& corner_positions
) {
    std::array<float, HEXES_PER_CELL * VERTICES_PER_HEX * FLOATS_PER_VERTEX> vertices;
    for (int i = 0; i < HEXES_PER_CELL * VERTICES_PER_HEX; ++i) {
        const int vertex_offset = FLOATS_PER_VERTEX * i;
        vertices[vertex_offset] = corner_positions[i].x;
        vertices[vertex_offset + 1] = corner_positions[i].y;
        vertices[vertex_offset + 2] = corner_positions[i].z;

        // Vertex normal xyz
        vertices[vertex_offset + 3] = 0.0f;
        vertices[vertex_offset + 4] = 0.0f;
        vertices[vertex_offset + 5] = 1.0f;
    }

    return vertices;
}

glm::vec3 hex_map_center(const HexMap& hex_map) {
    if (hex_map.cells.empty()) {
        return {0.0f, 0.0f, 0.0f};
    }

    glm::vec3 center = {0.0f, 0.0f, 0.0f};
    for (const auto& cell : hex_map.cells) {
        center.x += static_cast<float>(cell.x);
        center.y += static_cast<float>(cell.y);
    }

    return center / static_cast<float>(hex_map.cells.size());
}

constexpr int SUN_BILLBOARD_VERTEX_COUNT = TRIANGLES_PER_HEX * VERTICES_PER_TRIANGLE;

std::array<float, SUN_BILLBOARD_VERTEX_COUNT * FLOATS_PER_VERTEX> build_sun_billboard_vertices(
    const HexMap& hex_map,
    float hex_radius,
    glm::vec3 sun_direction_normalized,
    const Camera& camera
) {
    const float map_radius_world_units = static_cast<float>(hex_map.get_radius()) * hex_radius;
    const float sun_distance = std::max(map_radius_world_units * 3.0f, hex_radius * 12.0f);
    const glm::vec3 sun_center = hex_map_center(hex_map) + sun_direction_normalized * sun_distance;

    const glm::vec3 camera_front = camera.direction_normalized();
    const glm::vec3 camera_right = glm::normalize(
        glm::cross(camera_front, Camera::AXIS_VECTOR_UP_NORMALIZED)
    );
    const glm::vec3 camera_up = glm::normalize(glm::cross(camera_right, camera_front));
    const auto hex_offset_3d = [camera_right, camera_up](const glm::vec2& offset) {
        const float sun_size_scale = 8.0f;
        return (camera_right * offset.x + camera_up * offset.y) * sun_size_scale;
    };

    // Hex corner vertex index visualization:
    //    2-----1
    //   /       \.
    //  3         0
    //   \       /.
    //    4-----5
    const std::array<glm::vec3, SUN_BILLBOARD_VERTEX_COUNT> positions = {
         // triangle top right
        sun_center + hex_offset_3d(HexRenderer::hex_vertices_offset_from_hex_center[0]),
        sun_center + hex_offset_3d(HexRenderer::hex_vertices_offset_from_hex_center[1]),
        sun_center + hex_offset_3d(HexRenderer::hex_vertices_offset_from_hex_center[2]),
        // 0, 2, 3, 5 rectangle top left half
        sun_center + hex_offset_3d(HexRenderer::hex_vertices_offset_from_hex_center[0]),
        sun_center + hex_offset_3d(HexRenderer::hex_vertices_offset_from_hex_center[2]),
        sun_center + hex_offset_3d(HexRenderer::hex_vertices_offset_from_hex_center[3]),
        // 0, 2, 3, 5 rectangle bottom rigth half
        sun_center + hex_offset_3d(HexRenderer::hex_vertices_offset_from_hex_center[0]),
        sun_center + hex_offset_3d(HexRenderer::hex_vertices_offset_from_hex_center[3]),
        sun_center + hex_offset_3d(HexRenderer::hex_vertices_offset_from_hex_center[5]),
        // triangle bottom left
        sun_center + hex_offset_3d(HexRenderer::hex_vertices_offset_from_hex_center[3]),
        sun_center + hex_offset_3d(HexRenderer::hex_vertices_offset_from_hex_center[4]),
        sun_center + hex_offset_3d(HexRenderer::hex_vertices_offset_from_hex_center[5]),
    };

    std::array<float, SUN_BILLBOARD_VERTEX_COUNT * FLOATS_PER_VERTEX> vertices;
    for (int i = 0; i < SUN_BILLBOARD_VERTEX_COUNT; ++i) {
        const int vertex_offset = i * FLOATS_PER_VERTEX;
        vertices[vertex_offset] = positions[i].x;
        vertices[vertex_offset + 1] = positions[i].y;
        vertices[vertex_offset + 2] = positions[i].z;

        // Vertex normal xyz
        vertices[vertex_offset + 3] = 0.0f;
        vertices[vertex_offset + 4] = 0.0f;
        vertices[vertex_offset + 5] = 1.0f;
    }

    return vertices;
}

void HexRenderer::draw_hex_map_frame(
    const HexMap& hex_map,
    int width,
    int height,
    const Camera& camera,
    float elapsed_time_seconds
) {
    if (width <= 0 || height <= 0) {
        return;
    }

    glUseProgram(shader_program);

    constexpr float FIELD_OF_VIEW = glm::radians(45.0f);
    const float aspect_ratio = static_cast<float>(width) / static_cast<float>(height);
    constexpr float NEAR_PLANE = 0.1f;
    constexpr float FAR_PLANE = 100000.0f;
    glm::mat4 perspective_matrix = glm::perspective(
        FIELD_OF_VIEW,
        aspect_ratio,
        NEAR_PLANE,
        FAR_PLANE
    );
    // Flip projection y axis so initial camera orientation matches Python
    // visualization scripts output image
    perspective_matrix[1][1] *= -1.0f;

    const glm::mat4 projection = perspective_matrix * camera.view_matrix();
    constexpr GLint uProjection_location = 0;
    glUniformMatrix4fv(uProjection_location, 1, GL_FALSE, &projection[0][0]);

    constexpr GLint uViewPosition_location = 1;
    glUniform3f(uViewPosition_location, camera.position.x, camera.position.y, camera.position.z);

    constexpr float SUN_ROTATION_SPEED_DEGREES_PER_SECOND = 36.0f;
    const float sun_angle_radians =
        glm::radians(SUN_ROTATION_SPEED_DEGREES_PER_SECOND) * elapsed_time_seconds;
    // Sun begins pointing up from the perspective of camera initial direction
    constexpr glm::vec3 initial_sun_direction = glm::vec3(0.0f, 1.0f, 0.0f);
    const glm::mat4 sun_rotation_matrix = glm::rotate(
        // Start with identity matrix
        glm::mat4(1.0f),
        sun_angle_radians,
        // Rotate around initial camera right axis (sun rises from horizon on bottom
        // of camera initial field of view, and sets at opposite horizon)
        glm::vec3(1.0f, 0.0f, 0.0f)
    );

    const glm::vec3 sun_direction = glm::normalize(
        glm::vec3(sun_rotation_matrix * glm::vec4(initial_sun_direction, 0.0f))
    );
    constexpr GLint uSunDirection_location = 2;
    glUniform3f(uSunDirection_location, sun_direction.x, sun_direction.y, sun_direction.z);

    constexpr GLint uLightAmbientIntensity_location = 5;
    glUniform1f(uLightAmbientIntensity_location, 0.38f);

    constexpr GLint uLightDiffuseIntensity_location = 6;
    glUniform1f(uLightDiffuseIntensity_location, 0.42f);

    constexpr GLint uLightSpecularIntensity_location = 7;
    glUniform1f(uLightSpecularIntensity_location, 0.35f);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    GLint gl_depth_func_previous;
    glGetIntegerv(GL_DEPTH_FUNC, &gl_depth_func_previous);

    constexpr float HEXMAP_SURFACE_HEIGHT = 0.0f;
    const float world_height_scale = HexRenderer::radius / 4;

    for (const auto& cell : hex_map.cells) {

        // Because collapsed cells have only one possible tile, that tile is
        // used as the material source
        const int cell_tile = cell.collapsed ? *cell.possible_tiles.begin() : EMPTY;
        const Material cell_material = tile_material(cell_tile);


        constexpr GLint uMaterialAmbientColor_location = 8;
        glUniform3f(
            uMaterialAmbientColor_location,
            cell_material.color_ambient.x,
            cell_material.color_ambient.y,
            cell_material.color_ambient.z
        );

        constexpr GLint uMaterialDiffuseColor_location = 9;
        glUniform3f(
            uMaterialDiffuseColor_location,
            cell_material.color_diffuse.x,
            cell_material.color_diffuse.y,
            cell_material.color_diffuse.z
        );

        constexpr GLint uMaterialSpecularColor_location = 10;
        glUniform3f(
            uMaterialSpecularColor_location,
            cell_material.color_specular.x,
            cell_material.color_specular.y,
            cell_material.color_specular.z
        );

        constexpr GLint uMaterialShininess_location = 11;
        glUniform1f(
            uMaterialShininess_location,
            cell_material.shininess
        );

        constexpr GLint uUseLighting_location = 4;
        // Enable lighting for prism triangles
        glUniform1i(uUseLighting_location, GL_TRUE);

        const float cell_prism_height = cell.get_height() * world_height_scale;

        std::array<glm::vec3, HEXES_PER_CELL * VERTICES_PER_HEX> corner_positions;
        for (int i = 0; i < VERTICES_PER_HEX; ++i) {
            corner_positions[i] = {
                cell.x + hex_vertices_offset_from_hex_center[i].x,
                cell.y + hex_vertices_offset_from_hex_center[i].y,
                HEXMAP_SURFACE_HEIGHT
            };

            corner_positions[i + VERTICES_PER_HEX] = {
                cell.x + hex_vertices_offset_from_hex_center[i].x,
                cell.y + hex_vertices_offset_from_hex_center[i].y,
                cell_prism_height
            };
        }

        const std::array<float, LIT_VERTEX_INDICES_PER_CELL * FLOATS_PER_VERTEX> triangle_vertices =
            build_lit_triangle_vertices(corner_positions);
        glBufferData(
            GL_ARRAY_BUFFER,
            triangle_vertices.size() * sizeof(float),
            triangle_vertices.data(),
            GL_STREAM_DRAW
        );

        // No EBO used for drawing lit triangles
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        glDrawArrays(GL_TRIANGLES, 0, LIT_VERTEX_INDICES_PER_CELL);

        constexpr GLint uColor_location = 3;
        constexpr glm::vec3 OUTLINE_COLOR = {0.0f, 0.0f, 0.0f};
        glUniform3f(uColor_location, OUTLINE_COLOR.x, OUTLINE_COLOR.y, OUTLINE_COLOR.z);

        // Disable lighting for prism outlines
        glUniform1i(uUseLighting_location, GL_FALSE);

        const std::array<float, HEXES_PER_CELL * VERTICES_PER_HEX * FLOATS_PER_VERTEX>
            outline_vertices = build_outline_vertices(corner_positions);
        glBufferData(
            GL_ARRAY_BUFFER,
            outline_vertices.size() * sizeof(float),
            outline_vertices.data(),
            GL_STREAM_DRAW
        );

        glLineWidth(1.0f);

        constexpr void* EBO_OFFSET = 0;
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_LINES);
        // Without this depth function change, outlines may not be drawn because
        // they have same depth as hex surface
        glDepthFunc(GL_LEQUAL);
        // Draw cell prism outlines
        glDrawElements(GL_LINES, LINE_VERTEX_INDICES_PER_CELL, GL_UNSIGNED_INT, EBO_OFFSET);
        // Restore previous depth function
        glDepthFunc(gl_depth_func_previous);
    }

    constexpr GLint uUseLighting_location = 4;
    glUniform1i(uUseLighting_location, GL_FALSE);

    constexpr GLint uColor_location = 3;
    constexpr glm::vec3 SUN_COLOR = {1.0f, 0.86f, 0.22f};
    glUniform3f(uColor_location, SUN_COLOR.x, SUN_COLOR.y, SUN_COLOR.z);

    const std::array<float, SUN_BILLBOARD_VERTEX_COUNT * FLOATS_PER_VERTEX> sun_billboard_vertices =
        build_sun_billboard_vertices(hex_map, HexRenderer::radius, sun_direction, camera);
    glBufferData(
        GL_ARRAY_BUFFER,
        sun_billboard_vertices.size() * sizeof(float),
        sun_billboard_vertices.data(),
        GL_STREAM_DRAW
    );

    // Sun billboard vertices are drawn without EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glDrawArrays(GL_TRIANGLES, 0, SUN_BILLBOARD_VERTEX_COUNT);
}
