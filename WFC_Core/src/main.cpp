#include <iostream>

#include "cli/args_parser.h"
#include "engine/rules.h"
#include "engine/hex_to_pixels.h"
#include "models/hexmap.h"
#include "engine/generators/generators.h"
#include "engine/noises/noises.h"
#include "engine/render/render.h"
#include "engine/render/camera.h"

int main(int argc, char *argv[]) {

    ArgsParser cli_args(argc, argv);

    validate_constraints();

    Layout layout(layout_flat, Point(5.0, 5.0), Point(500.0, 500.0));
    HexMap hex_map = HexMap::generate_uncollapsed_hex_map(layout, cli_args.map_radius);

    PerlinNoise height_factor_perlin(cli_args.hf_perlin_seed);

    PerlinNoise r_ridged_base(cli_args.river_ridged_seed);
    RidgedNoise river_ridged(r_ridged_base);

    RiverGenerator river_gen(
        hex_map, true,
        height_factor_perlin, river_ridged
    );

    WFC wfc(
        hex_map, cli_args.wfc_seed,
        height_factor_perlin
    );

    if (cli_args.opengl_render) {

        GLFWwindow* window = setup_window();
        HexRenderer hex_renderer = HexRenderer(5.0f);
        constexpr glm::vec3 CAMERA_POSITION_INITIAL = {500.0f, 500.0f, 1000.0f};
        Camera camera = Camera(CAMERA_POSITION_INITIAL);
        const double visual_start_timestamp = glfwGetTime();

        run_step_visual_simulation(
            window, hex_renderer, cli_args.opengl_step_counter, hex_map, river_gen,
            visual_start_timestamp, camera, false
        );

        run_step_visual_simulation(
            window, hex_renderer, cli_args.opengl_step_counter, hex_map, wfc,
            visual_start_timestamp, camera, true
        );

        terminate_window(window);
    }

    else {
        river_gen.generate_river();
        wfc.wave_function_collapse();
    }

    hex_map.print_map();

    return 0;
}
