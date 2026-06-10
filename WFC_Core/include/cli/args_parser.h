#ifndef ARGS_PARSER_H
#define ARGS_PARSER_H

#include <external/argparse/argparse.hpp>

class ArgsParser {
public:

    int map_radius;
    bool opengl_render;
    unsigned int opengl_step_counter;
    unsigned int wfc_seed;
    unsigned int hf_perlin_seed;
    unsigned int river_ridged_seed;

    ArgsParser(int argc, char *argv[]);
};

#endif
