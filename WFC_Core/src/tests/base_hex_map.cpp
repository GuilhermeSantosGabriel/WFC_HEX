#include <iostream>

#include "cli/args_parser.h"
#include "engine/rules.h"
#include "engine/hex_to_pixels.h"
#include "models/hexmap.h"

int main(int argc, char *argv[]) {

    ArgsParser cli_args(argc, argv);

    Layout layout(layout_flat, Point(5,5), Point(500, 500));
    HexMap hex_map = HexMap::generate_empty_hex_map(layout, cli_args.map_radius);

    hex_map.print_map();
}
