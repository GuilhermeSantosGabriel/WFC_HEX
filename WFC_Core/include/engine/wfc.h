#ifndef WFC_H
#define WFC_H

#include "models/hexmap.h"
#include "models/cell.h"
#include "engine/rules.h"
#include "engine/opengl/window_manager.h"
#include "engine/opengl/hex_renderer.h"

#include <random>

void wave_function_collapse(HexMap& hex_map, GLFWwindow* window, HexRenderer& hex_renderer);

Cell* lowest_entropy_cell(HexMap& hex_map);

void collapse(Cell& cell, std::mt19937 &gen);

int random_weighted_tile(Cell& cell, std::mt19937 &gen);

void update_neighbors(Cell& cell, HexMap& hex_map);

void set_height(Cell& cell);

#endif
