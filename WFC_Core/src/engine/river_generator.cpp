#include "engine/rules.h"
#include "engine/river_generator.h"
#include "engine/noises/ridged_multifractal.h"

using namespace std;

void generate_river(HexMap& hex_map) {

    float base = 0.0f;
    float amplitude = 400.0f;
    float frequency = 0.01f;

    float water_height = 15;

    float ridged_value;
    float height;

    vector<reference_wrapper<Cell>> water_cells;

    for (auto &c : hex_map.cells){

        ridged_value = 1 - ridged_multifractal(
            c.get_q()*frequency, c.get_r()*frequency
        );

        height = (int)(base + amplitude * ridged_value);

        if (height <= water_height) {
            height = water_height - 1;
            c.collapse(WATER);
            water_cells.push_back(c);
        }

        else {
            c.possible_tiles = {GRASS, SAND, FOREST};
            c.entropy = 3;
            c.collapsed = false;
        }

        c.set_height(height);
    }

    set_river_margin(hex_map, water_cells);
}

void set_river_margin(HexMap& hex_map, vector<reference_wrapper<Cell>> water_cells) {
    for (Cell& cell: water_cells) {
        for (Cell* neighbor : hex_map.get_neighbors(cell)) {
            if (
                !neighbor->collapsed
            ) cell.collapse(SAND);
        }
    }
}
