#include <iostream>

#include "engine/rules.h"
#include "engine/hex_to_pixels.h"
#include "models/hexmap.h"
#include "engine/river_generator.h"
#include "engine/wfc.h"
#include "engine/opengl.h"

#include "engine/noises/ridged_multifractal.h"

using namespace std;

int main() {

    int radius;
    cerr << "Type map radius (size): ";
    cin >> radius;

    Layout layout(layout_flat, Point(5,5), Point(500, 500));
    HexMap hex_map = HexMap::generate_empty_hex_map(layout, radius);

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
            c.possible_tiles = {WATER};
            c.entropy = 1;
            c.collapsed = true;
            water_cells.push_back(c);
        } 
        else {
            c.possible_tiles = {GRASS, SAND, FOREST};
            c.entropy = 3;
            c.collapsed = false;
        }
        // else {
        //     c.possible_tiles = {-1};
        //     c.entropy = 1;
        //     c.collapsed = false;
        // }

        c.set_height(height);
    }

    for (Cell& cell: water_cells) {
        for (Cell* neighbor : hex_map.get_neighbors(cell)) {
            if (!neighbor->collapsed) {
                neighbor->possible_tiles = {SAND};
                neighbor->entropy = 1;
                neighbor->collapsed = true;
            }
        }
    }

    wave_function_collapse(hex_map);

    hex_map.print_map();
}
