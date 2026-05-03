#ifndef WFC_H
#define WFC_H

#include "models/hexmap.h"
#include "models/cell.h"
#include "engine/rules.h"

static void wave_function_collapse(HexMap& hex_map);

static Cell* lowest_entropy_cell(HexMap& hex_map);

static void collapse(Cell& cell, std::mt19937 &gen);

static int random_weighted_tile(Cell& cell, std::mt19937 &gen);

static void update_neighbors(Cell& cell, HexMap& hex_map);

#endif
