#ifndef OPEN_GL_H
#define OPEN_GL_H

#include "models/hexmap.h"

#include <external/glad/glad.h>
#include <external/GLFW/glfw3.h>

GLFWwindow* setup_window();

void render_hex_map(HexMap hex_map, GLFWwindow* window);

#endif
