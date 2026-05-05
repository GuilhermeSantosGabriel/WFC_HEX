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
    float amplitude = 120.0f;
    float frequency = 0.1f;
    float ridged_value;

    for (auto &c : hex_map.cells){

        ridged_value = ridged_multifractal(
            c.get_q()*frequency, c.get_r()*frequency
        );

        c.set_height(
            (int)(base + amplitude * ridged_value)
        );
    }

    hex_map.print_map();
}
