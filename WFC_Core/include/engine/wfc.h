#ifndef WFC_H
#define WFC_H

#include "models/cell.h"
#include "engine/rules.h"



std::vector<Cell*> get_neighbors(Cell &cell, std::map<std::tuple<int,int,int>, Cell*> &coord_map);

bool wave_function_collapse(std::vector<Cell> &hex_map, int size, int n_tiles);

int cell_height(Cell hex_cell);

#endif
