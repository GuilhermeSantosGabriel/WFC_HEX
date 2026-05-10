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

    validate_constraints();

    Layout layout(layout_flat, Point(5,5), Point(500, 500));
    HexMap hex_map = HexMap::generate_uncollapsed_hex_map(layout, radius);

    GLFWwindow* window = setup_window();
    HexRenderer hex_renderer = HexRenderer(4);
    WFC wfc = WFC(27);

    generate_river(hex_map, true, window, hex_renderer);

    wfc.wave_function_collapse(hex_map, window, hex_renderer);

    hex_map.print_map();

    glfwDestroyWindow(window);
    glfwTerminate();
}
