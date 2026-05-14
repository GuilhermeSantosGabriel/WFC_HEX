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

    float base = 0.0f;
    float amplitude = 70.0f;
    float frequency = 0.1f;

    PerlinNoise height_factor_perlin(cli_args.hf_perlin_seed);

    for (auto &c : hex_map.cells){

        c.set_height(
            (int)(base + amplitude * height_factor_perlin.normalized_perlin(
                c.get_q() * frequency, c.get_r() * frequency
            ))
        );
    }

    hex_map.print_map();
}
