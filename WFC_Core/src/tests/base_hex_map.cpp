#include <iostream>

#include "engine/rules.h"
#include "engine/hex_to_pixels.h"
#include "models/hexmap.h"

int main() {

    int radius;
    std::cerr << "Type map radius (size): ";
    std::cin >> radius;

    Layout layout(layout_flat, Point(5,5), Point(500, 500));
    HexMap hex_map = HexMap::generate_empty_hex_map(layout, radius);

    hex_map.print_map();
}
