#ifndef WFC_H
#define WFC_H

#include "engine/generators/interface/step_generator.h"
#include "models/hexmap.h"
#include "models/cell.h"
#include "engine/rules.h"
#include "engine/opengl/window_manager.h"
#include "engine/opengl/hex_renderer.h"
#include "engine/noises/perlin.h"

#include <random>

class WFC : public StepGenerator {
private:

    HexMap& hex_map;

    PerlinNoise& height_factor_perlin;

    inline static std::mt19937 gen;

public:
    WFC(HexMap& hex_map_s, unsigned int wfc_seed, PerlinNoise& hf_perlin);

    void wave_function_collapse();

    bool step();

    Cell* lowest_entropy_cell();
    void wfc_collapse(Cell& cell);
    int random_weighted_tile(Cell& cell);
    void update_neighbors(Cell& cell);
};

#endif
