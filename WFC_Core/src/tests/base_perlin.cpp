#include <iostream>

#include "engine/rules.h"
#include "engine/hex_to_pixels.h"
#include "models/hexmap.h"
#include "engine/noises/noises.h"

using namespace std;


int main() {

    int radius;
    cerr << "Type map radius (size): ";
    cin >> radius;

    Layout layout(layout_flat, Point(5,5), Point(500, 500));
    HexMap hex_map = HexMap::generate_empty_hex_map(layout, radius);

    float base = 0.0f;
    float amplitude = 70.0f;
    float frequency = 0.1f;

    PerlinNoise perlin(27);

    for (auto &c : hex_map.cells){

        c.set_height(
            (int)(base + amplitude * perlin.normalized_perlin(
                c.get_q() * frequency, c.get_r() * frequency
            ))
        );
    }

    hex_map.print_map();
}
