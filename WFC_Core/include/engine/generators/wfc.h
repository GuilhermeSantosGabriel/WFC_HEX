#ifndef WFC_H
#define WFC_H

#include "engine/generators/IStepGenerator.h"
#include "models/hexmap.h"
#include "models/cell.h"
#include "engine/rules.h"
#include "engine/noises/INoise.h"

#include <random>

class WFC : public IStepGenerator {
private:

    HexMap& hex_map;

    // Height Factors Noise
    INoise& hf_noise;

    inline static std::mt19937 gen;

public:
    WFC(HexMap& hex_map_s, unsigned int wfc_seed, INoise& hf_n);

    void wave_function_collapse();

    bool step();

    Cell* lowest_entropy_cell();
    void wfc_collapse(Cell& cell);
    int random_weighted_tile(Cell& cell);
    void update_neighbors(Cell& cell);
};

#endif
