#ifndef WFC_H
#define WFC_H

#include "models/hexmap.h"
#include "models/cell.h"
#include "engine/rules.h"

static void wave_function_collapse(HexMap& hex_map);

static Cell* get_next_uncollapsed_cell(HexMap& hex_map);

static void collapse(Cell& cell);

static int random_weighted_tile(Cell& cell);

static void update_neighbors(Cell& cell);

#endif
