#include <iostream>

#include "engine/rules.h"
#include "engine/hex_to_pixels.h"
#include "models/hexmap.h"
#include "engine/generators/generators.h"
#include "engine/noises/perlin.h"
#include "engine/noises/ridged_multifractal.h"
#include "engine/opengl/window_manager.h"
#include "engine/opengl/hex_renderer.h"
#include "engine/opengl/simulation_runner.h"

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

    unsigned int seed = 27;
    PerlinNoise height_factor_perlin(seed);
    RidgedNoise river_ridged(seed);

    RiverGenerator river_gen(
        hex_map, true,
        height_factor_perlin, river_ridged
    );
    run_step_visual_simulation(
        window, hex_renderer, 100, hex_map, river_gen
    );
    // river_gen.generate_river();

    WFC wfc(
        hex_map, seed,
        height_factor_perlin
    );
    run_step_visual_simulation(
        window, hex_renderer, 100, hex_map, wfc
    );
    // wfc.wave_function_collapse();

    hex_map.print_map();

    terminate_window(window);
}
