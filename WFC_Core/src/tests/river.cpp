#include <iostream>

#include "cli/args_parser.h"
#include "engine/rules.h"
#include "engine/hex_to_pixels.h"
#include "models/hexmap.h"
#include "engine/generators/generators.h"


int main(int argc, char *argv[]) {

    ArgsParser cli_args(argc, argv);

    Layout layout(layout_flat, Point(5,5), Point(500, 500));
    HexMap hex_map = HexMap::generate_empty_hex_map(layout, cli_args.map_radius);

    PerlinNoise height_factor_perlin(cli_args.hf_perlin_seed);
    RidgedNoise river_ridged(cli_args.river_ridged_seed);

    RiverGenerator river_gen(
        hex_map, true,
        height_factor_perlin, river_ridged
    );
    river_gen.generate_river();

    for (auto &cell : hex_map.cells) {
        if (!cell.collapsed) cell.collapse(EMPTY);
    }

    hex_map.print_map();
}
