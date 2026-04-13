#ifndef WFC_H
#define WFC_H

#include <vector>
#include <map>
#include <set>
#include "models/cell.h"

enum TileTypes {
    GRASS,
    FOREST,
    WATER,
    SAND
};

extern std::map<int, std::map<int, int>> ruleset;
extern std::set<int> tiles;

bool wave_function_collapse(std::vector<Cell> &hex_map, int size, int n_tiles);

int cell_height(Cell hex_cell);

#endif
