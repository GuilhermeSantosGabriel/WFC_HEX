#include <iostream>

#include "engine/rules.h"
#include "engine/hex_to_pixels.h"
#include "models/hexmap.h"
#include "engine/river_generator.h"
#include "engine/wfc.h"
#include "engine/opengl/window_manager.h"
#include "engine/opengl/hex_renderer.h"

using namespace std;

int main() {

    int radius;
    cerr << "Type map radius (size): ";
    cin >> radius;

    Layout layout(layout_flat, Point(5,5), Point(500, 500));
    HexMap hex_map = HexMap::generate_empty_hex_map(layout, radius);
    GLFWwindow* window = setup_window();
    HexRenderer hex_renderer = HexRenderer(4);

    unsigned int seed = 27;
    PerlinNoise perlin(seed);
    RidgedNoise ridged(seed);

    generate_river(hex_map, false, window, hex_renderer, perlin, ridged);

    for (auto &cell : hex_map.cells) {
        if (!cell.collapsed) cell.collapse(EMPTY);
    }

    hex_map.print_map();

    glfwDestroyWindow(window);
    glfwTerminate();
}
