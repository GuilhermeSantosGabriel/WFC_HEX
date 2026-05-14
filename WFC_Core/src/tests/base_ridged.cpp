#include <iostream>

#include "cli/args_parser.h"
#include "engine/rules.h"
#include "engine/hex_to_pixels.h"
#include "models/hexmap.h"
#include "engine/noises/noises.h"


int main(int argc, char *argv[]) {

    ArgsParser cli_args(argc, argv);

    Layout layout(layout_flat, Point(5,5), Point(500, 500));
    HexMap hex_map = HexMap::generate_empty_hex_map(layout, cli_args.map_radius);

    RidgedNoise river_ridged(cli_args.river_ridged_seed);

    float base = 0.0f;
    float amplitude = 120.0f;
    float frequency = 0.1f;
    float ridged_value;

    for (auto &c : hex_map.cells){

        ridged_value = river_ridged.sample(
            c.get_q()*frequency, c.get_r()*frequency
        );

        c.set_height(
            (int)(base + amplitude * ridged_value)
        );
    }

    hex_map.print_map();
}
