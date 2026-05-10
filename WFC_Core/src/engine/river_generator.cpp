#include "engine/rules.h"
#include "engine/river_generator.h"
#include "engine/noises/ridged_multifractal.h"
#include "engine/height_dealer.h"

void generate_river(HexMap& hex_map, bool sand_margin, GLFWwindow* window, HexRenderer& hex_renderer) {

    int window_width, window_height;
    int counter = 0;
    for (auto &c : hex_map.cells){

        generate_river_step(c, sand_margin);

        if (counter == 100) {
            clear_window(window, &window_width, &window_height);
            hex_renderer.draw_hex_map_frame(hex_map, window_width, window_height);
            update_window(window);
            counter = 0;
        } else counter++;
    }
}

void generate_river_step(Cell& c, bool sand_margin) {

    float base = 0.0f;
    float amplitude = 400.0f;
    float frequency = 0.01f;

    float water_height = 15;

    float ridged_value = 1 - ridged_multifractal(
        c.get_q()*frequency, c.get_r()*frequency
    );

    float height = (int)(base + amplitude * ridged_value);

    float sand_margin_height = water_height + 5;

    if (height <= water_height) c.collapse(WATER);

    else if (sand_margin && height <= sand_margin_height) c.collapse(SAND);

    else {
        c.possible_tiles = {GRASS, SAND, FOREST};
        c.entropy = 3;
        c.collapsed = false;
        return;
    }

    set_height_by_height_factors(c);
}
