#include <iostream>

#include "engine/rules.h"
#include "engine/hex_to_pixels.h"
#include "models/hexmap.h"
#include "engine/river_generator.h"
#include "engine/wfc.h"
#include "engine/opengl.h"

using namespace std;

int main() {

    int radius;
    cerr << "Type map radius (size): ";
    cin >> radius;

    Layout layout(layout_flat, Point(5,5), Point(500, 500));
    HexMap hex_map = HexMap::generate_empty_hex_map(layout, radius);

    hex_map.print_map();
}
