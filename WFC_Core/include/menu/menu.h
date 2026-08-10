#ifndef MENU_H
#define MENU_H

#include "external/GLFW/glfw3.h"
#include "engine/config/EngineConfig.h"
#include "engine/config/TileRegistry.h"

void open_config_menu(GLFWwindow* window, EngineConfig* config, TileRegistry* tile_registry);

#endif
