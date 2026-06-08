#include "engine/height_dealer.h"
#include "engine/rules.h"
#include "engine/noises/INoise.h"
#include <cassert>

void set_height_by_height_factors(Cell& cell, INoise& hf_noise) {
    assert(cell.collapsed && "Trying to set height of an uncollapsed cell!");

    int tile_type = *cell.possible_tiles.begin();

    HeightFactor hf = height_factors.at(tile_type);

    float frequency = 0.1f;
    int height = (int)(
        hf.base + hf.amplitude * hf_noise.sample(
            cell.get_q() * frequency, cell.get_r() * frequency
    ));

    cell.set_height(height);
}
