#ifndef RIVER_GENERATOR_H
#define RIVER_GENERATOR_H

#include "models/hexmap.h"
#include "engine/opengl/window_manager.h"
#include "engine/opengl/hex_renderer.h"

void generate_river(HexMap& hex_map, bool sand_margin, GLFWwindow* window, HexRenderer& hex_renderer);

void generate_river_step(Cell& c, bool sand_margin);

#endif
