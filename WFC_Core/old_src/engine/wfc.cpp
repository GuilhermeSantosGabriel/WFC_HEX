#include "engine/wfc.h"
#include "models/hex.h"
#include "engine/noises/perlin.h"

#include <cassert>


int cell_height(Cell hex_cell) {

    assert(hex_cell.collapsed);

    int tile_type = *hex_cell.possible_tiles.begin();

    float amplitude = 0.0f;
    float base = 0.0f;
    switch (tile_type) {
        case WATER:
            amplitude = 2.0f;
            base = 0.0f;
        break;

        case SAND:
            amplitude = 2.0f;
            base = 3.0f;
        break;

        case GRASS:
            amplitude = 8.0f;
            base = 6.0f;
        break;

        case FOREST:
            amplitude = 15.0f;
            base = 15.0f;
        break;

        default:
            base = 0.0f;
            amplitude = 30.0f;
    }

    float frequency = 0.1f;
    int height = (int)(base + amplitude * normalized_perlin(
        hex_cell.get_q(),
        hex_cell.get_r(),
        frequency
    ));

    return height;
}
