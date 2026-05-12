#include "engine/generators/river_generator.h"

#include "engine/rules.h"
#include "engine/noises/ridged_multifractal.h"
#include "engine/height_dealer.h"


RiverGenerator::RiverGenerator(
    HexMap& hex_map_s, bool sand_margin_s,
    PerlinNoise& hf_perlin, RidgedNoise& r_ridged
):
    hex_map(hex_map_s), sand_margin(sand_margin_s),
    height_factors_perlin(hf_perlin), river_ridged(r_ridged)
{}

void RiverGenerator::generate_river() {
    while(step());
}

bool RiverGenerator::step() {

    if (this->current_index >= hex_map.cells.size()) {
        return false;
    }

    Cell& c = hex_map.cells[this->current_index];

    float base = 0.0f;
    float amplitude = 400.0f;
    float frequency = 0.01f;

    float water_height = 15;

    float ridged_value = 1 - river_ridged.sample(
        c.get_q()*frequency, c.get_r()*frequency
    );

    float height = (int)(base + amplitude * ridged_value);

    float sand_margin_height = water_height + 5;

    if (height <= water_height) c.collapse(WATER);

    else if (sand_margin && height <= sand_margin_height) c.collapse(SAND);

    else {
        c.possible_tiles = {GRASS, SAND, FOREST};
        c.entropy = 3;
        c.collapsed = false;
        this->current_index++;
        return true;
    }

    set_height_by_height_factors(c, height_factors_perlin);

    this->current_index++;
    return true;
}
