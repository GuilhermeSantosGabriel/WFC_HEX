#ifndef RIVER_GENERATOR_H
#define RIVER_GENERATOR_H

#include "models/hexmap.h"
#include "models/cell.h"

using namespace std;

void generate_river(HexMap& hex_map);

void set_river_margin(HexMap& hex_map, vector<reference_wrapper<Cell>> water_cells);

#endif
