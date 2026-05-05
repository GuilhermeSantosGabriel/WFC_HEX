#include <iostream>

#include "engine/rules.h"
#include "engine/hex_to_pixels.h"
#include "models/hexmap.h"
#include "engine/river_generator.h"
#include "engine/wfc.h"
#include "engine/opengl.h"

#include "engine/noises/perlin.h"

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

    for (auto &c : hex_map.cells){

        c.set_height(
            (int)(base + amplitude * normalized_perlin(
                c.get_q(), c.get_r(), frequency
            ))
        );
    }

    hex_map.print_map();
}
