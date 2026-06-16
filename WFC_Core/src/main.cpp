#include <iostream>

#include "engine/render/camera.h"
#include "engine/render/hex_renderer.h"
#include "engine/render/simulation_runner.h"
#include "engine/render/window_manager.h"

#include "engine/config/EngineConfig.h"
#include "engine/rules.h"
#include "menu/menu.h"
#include "engine/hex_to_pixels.h"
#include "models/hexmap.h"
#include "engine/generators/wfc.h"
#include "engine/generators/river_generator.h"
#include "engine/noises/perlin.h"
#include "engine/noises/ridged.h"

int main(int argc, char *argv[]) {

    // Deals with flags and configs
    // - Map radius
    // - OpenGl configs
    // - Seeds
    EngineConfig config = EngineConfig::from_cli(argc, argv);

    // Validate generation rules
    validate_constraints();

    GLFWwindow* window = setup_window();
    if (config.open_menu) {
        open_config_menu(window, &config);
    }

    // Creates an empty HexMap
    Layout layout(layout_flat, Point(5.0, 5.0), Point(500.0, 500.0));
    HexMap hex_map = HexMap::generate_uncollapsed_hex_map(layout, config.map_radius);

    // Creates Noise Objects
    PerlinNoise height_factor_perlin(config.hf_perlin_seed);

    PerlinNoise r_ridged_base(config.river_ridged_seed);
    RidgedNoise river_ridged(r_ridged_base);

    // Creates Generators Objects
    RiverGenerator river_gen(
        hex_map, true,
        height_factor_perlin, river_ridged
    );
    WFC wfc(
        hex_map, config.wfc_seed,
        height_factor_perlin
    );

    // Generation with visual rendering
    if (config.opengl_render) {

        HexRenderer hex_renderer = HexRenderer(5.0f);
        constexpr glm::vec3 CAMERA_POSITION_INITIAL = {500.0f, 500.0f, 1000.0f};
        Camera camera = Camera(CAMERA_POSITION_INITIAL);
        const double visual_start_timestamp = glfwGetTime();

        run_step_visual_simulation(
            window, hex_renderer, config.opengl_step_counter, hex_map, river_gen,
            visual_start_timestamp, camera, false
        );

        run_step_visual_simulation(
            window, hex_renderer, config.opengl_step_counter, hex_map, wfc,
            visual_start_timestamp, camera, true
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
