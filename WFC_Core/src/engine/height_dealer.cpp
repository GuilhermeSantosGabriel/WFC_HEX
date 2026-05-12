#include "engine/height_dealer.h"
#include "engine/rules.h"
#include "engine/noises/perlin.h"
#include <cassert>

void set_height_by_height_factors(Cell& cell, PerlinNoise& perlin) {
    assert(cell.collapsed && "Trying to set height of an uncollapsed cell!");

    int tile_type = *cell.possible_tiles.begin();

    HeightFactor hf = height_factors.at(tile_type);

    float frequency = 0.1f;
    int height = (int)(
        hf.base + hf.amplitude * perlin.normalized_perlin(
            cell.get_q() * frequency, cell.get_r() * frequency
    ));

    cell.set_height(height);
}
