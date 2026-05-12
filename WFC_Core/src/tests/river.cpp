#include <iostream>

#include "engine/rules.h"
#include "engine/hex_to_pixels.h"
#include "models/hexmap.h"
#include "engine/generators/generators.h"

using namespace std;

int main() {

    int radius;
    cerr << "Type map radius (size): ";
    cin >> radius;

    Layout layout(layout_flat, Point(5,5), Point(500, 500));
    HexMap hex_map = HexMap::generate_empty_hex_map(layout, radius);

    unsigned int seed = 27;
    PerlinNoise height_factor_perlin(seed);
    RidgedNoise river_ridged(seed);

    RiverGenerator river_gen(
        hex_map, false,
        height_factor_perlin, river_ridged
    );
    river_gen.generate_river();

    for (auto &cell : hex_map.cells) {
        if (!cell.collapsed) cell.collapse(EMPTY);
    }

    hex_map.print_map();
}
