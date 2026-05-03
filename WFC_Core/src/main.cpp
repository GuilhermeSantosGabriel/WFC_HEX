// TODO - reorganize with updated flow
//      - validate_constraints()
//      - generate_uncollapsed_hex_map() -> HexMap
//            - convert cells coords to (x, y)
//      - generate_rivers(HexMap)
//      - wave_function_collapse(HexMap)
//      - OpenGL.render_hex_map(HexMap)

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

    validate_constraints();

    Layout layout(layout_flat, Point(5,5), Point(500, 500));
    HexMap hex_map = HexMap::generate_uncollapsed_hex_map(layout, radius);

    generate_river(hex_map);

    wave_function_collapse(hex_map);

    render_hex_map(hex_map);
}
