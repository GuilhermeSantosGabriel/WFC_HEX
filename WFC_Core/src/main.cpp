#include <iostream>

#include "cli/args_parser.h"
#include "engine/rules.h"
#include "engine/hex_to_pixels.h"
#include "models/hexmap.h"
#include "engine/generators/wfc.h"
#include "engine/generators/river_generator.h"
#include "engine/noises/perlin.h"
#include "engine/noises/ridged.h"
#include "engine/render/hex_renderer.h"
#include "engine/render/simulation_runner.h"
#include "engine/render/window_manager.h"

int main(int argc, char *argv[]) {

    // Deals with CLI args
    // - Map radius
    // - OpenGl configs
    // - Seeds
    ArgsParser cli_args(argc, argv);

    // Validate generation rules
    validate_constraints();

    // Creates an empty HexMap
    Layout layout(layout_flat, Point(5,5), Point(500, 500));
    HexMap hex_map = HexMap::generate_uncollapsed_hex_map(layout, cli_args.map_radius);

    // Creates Noise Objects
    PerlinNoise height_factor_perlin(cli_args.hf_perlin_seed);

    PerlinNoise r_ridged_base(cli_args.river_ridged_seed);
    RidgedNoise river_ridged(r_ridged_base);

    // Creates Generators Objects
    RiverGenerator river_gen(
        hex_map, true,
        height_factor_perlin, river_ridged
    );
    WFC wfc(
        hex_map, cli_args.wfc_seed,
        height_factor_perlin
    );

    // Generation with visual rendering
    if (cli_args.opengl_render) {

        GLFWwindow* window = setup_window();
        HexRenderer hex_renderer = HexRenderer(4);

        run_step_visual_simulation(
            window, hex_renderer, cli_args.opengl_step_counter, hex_map, river_gen
        );

        run_step_visual_simulation(
            window, hex_renderer, cli_args.opengl_step_counter, hex_map, wfc
        );

        terminate_window(window);
    }

    // Blind Generation
    else {
        river_gen.generate_river();
        wfc.wave_function_collapse();
    }

    // Outputs the generated map
    hex_map.print_map();

    return 0;
}
