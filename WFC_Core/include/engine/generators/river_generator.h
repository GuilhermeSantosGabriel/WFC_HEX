#ifndef RIVER_GENERATOR_H
#define RIVER_GENERATOR_H

#include "engine/generators/interface/step_generator.h"
#include "models/hexmap.h"
#include "engine/opengl/window_manager.h"
#include "engine/opengl/hex_renderer.h"
#include "engine/noises/perlin.h"
#include "engine/noises/ridged_multifractal.h"


class RiverGenerator : public StepGenerator {
private:

    HexMap& hex_map;
    size_t current_index = 0;

    bool sand_margin;

    PerlinNoise& height_factors_perlin;
    RidgedNoise& river_ridged;

public:

    RiverGenerator(
        HexMap& hex_map_s, bool sand_margin_s,
        PerlinNoise& hf_perlin, RidgedNoise& r_ridged
    );

    void generate_river();

    bool step();
};

#endif
