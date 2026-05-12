#ifndef WINDOW_MANAGER_H
#define WINDOW_MANAGER_H

#include "models/hexmap.h"

#include <external/glad/glad.h>
#include <external/GLFW/glfw3.h>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

GLFWwindow* setup_window();

void clear_window(GLFWwindow* window, int* width, int* height);

void update_window(GLFWwindow* window);

void terminate_window(GLFWwindow* window);

#endif
