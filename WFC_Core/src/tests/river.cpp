#include <bits/stdc++.h>

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

    float base = 0.0f;
    float amplitude = 60.0f;
    float frequency = 0.1f;

    float ridged_value;
    float height;

    for (auto &c : hex_map){

        ridged_value = 1 - ridged_multifractal(
            c.get_q()*frequency, c.get_r()*frequency
        );

        height = (int)(base + amplitude * ridged_value);

        if (height < 3) {
            height = 0;
            c.possible_tiles = {WATER};
            c.entropy = 1;
            c.collapsed = true;
        } else {
            c.possible_tiles = {SAND, GRASS, FOREST};
            c.entropy = 3;
            c.collapsed = false;
        }

        c.set_height(height);
    }

    if (wave_function_collapse(hex_map, size, tiles.size())) {
    // if (true) {
        for (auto &c : hex_map){
            Point p = hex_to_pixel(layout, c);
            cout << p.x << " " << p.y << " " << c.get_height() << " " << *c.possible_tiles.begin() << endl;
        }
    } else {
        cout << "yeah we lost" << endl;
    }


}
