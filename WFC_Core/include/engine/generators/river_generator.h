#ifndef RIVER_GENERATOR_H
#define RIVER_GENERATOR_H

#include "engine/generators/IStepGenerator.h"
#include "engine/noises/INoise.h"
#include "models/hexmap.h"


class RiverGenerator : public IStepGenerator {
private:

    HexMap& hex_map;
    size_t current_index = 0;

    bool sand_margin;

    // Height Factors Noise
    INoise& hf_noise;
    // River Noise
    INoise& r_noise;

public:

    RiverGenerator(
        HexMap& hex_map_s, bool sand_margin_s,
        INoise& hf_n, INoise& r_n
    );

    void generate_river();

    bool step();
};

#endif
