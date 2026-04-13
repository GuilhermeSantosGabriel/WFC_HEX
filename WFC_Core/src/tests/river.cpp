#include <bits/stdc++.h>
#include <functional>

#include "models/cell.h"
#include "engine/noise_generator/perlin.h"
#include "engine/noise_generator/ridged_multifractal.h"
#include "engine/hex_to_pixels.h"
#include "engine/wfc.h"

#include "tests/test_utils.h"

using namespace std;

int main() {

    int size;
    cerr << "Type map radius: ";
    cin >> size;

    Layout layout(layout_flat, Point(5,5), Point(500, 500));
    vector<Cell> hex_map;

    generate_empty_shell(hex_map, size);

    std::map<std::tuple<int,int,int>, Cell*> coord_map;
    for (auto &c : hex_map) coord_map[{c.get_q(), c.get_r(), c.get_s()}] = &c;

    float base = 0.0f;
    float amplitude = 400.0f;
    float frequency = 0.01f;

    float water_height = 15;

    float ridged_value;
    float height;

    vector<reference_wrapper<Cell>> water_cells;

    for (auto &c : hex_map){

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
        for (Cell* neighbor : get_neighbors(cell, coord_map)) {
            if (!neighbor->collapsed) {
                neighbor->possible_tiles = {SAND};
                neighbor->entropy = 1;
                neighbor->collapsed = true;
            }
        }
    }

    if (wave_function_collapse(hex_map, size, tiles.size())) {
        for (auto &c : hex_map){
            Point p = hex_to_pixel(layout, c);
            cout << p.x << " " << p.y << " " << c.get_height() << " " << *c.possible_tiles.begin() << endl;
        }
    } else {
        cout << "yeah we lost" << endl;
    }


}
