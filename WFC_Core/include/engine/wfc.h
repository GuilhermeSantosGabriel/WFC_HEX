#ifndef WFC_H
#define WFC_H

#include "models/hexmap.h"
#include "models/cell.h"
#include "engine/rules.h"
#include "engine/opengl/window_manager.h"
#include "engine/opengl/hex_renderer.h"

#include <random>

class WFC{
private:
    inline static std::mt19937 gen;

public:
    WFC(unsigned int seed);

    void wave_function_collapse(HexMap& hex_map, GLFWwindow* window, HexRenderer& hex_renderer);

    bool wfc_step(HexMap& hex_map);

    Cell* lowest_entropy_cell(HexMap& hex_map);
    void collapse(Cell& cell);
    int random_weighted_tile(Cell& cell);
    void update_neighbors(Cell& cell, HexMap& hex_map);
};

#endif
