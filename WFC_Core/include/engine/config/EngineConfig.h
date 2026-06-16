#ifndef ENGINE_CONFIG_H
#define ENGINE_CONFIG_H

#include "external/argparse/argparse.hpp"
#include <random>
#include <string>
#include <iostream>

class EngineConfig {
private:
    static unsigned int generate_random_seed() {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<unsigned int> dist(0, 0xFFFFFFFF);
        return dist(gen);
    }

public:
    bool open_menu = true;
    int map_radius = 50;
    bool opengl_render = true;
    unsigned int opengl_step_counter = 100;
    unsigned int wfc_seed = generate_random_seed();
    unsigned int hf_perlin_seed = generate_random_seed();
    unsigned int river_ridged_seed = generate_random_seed();

    EngineConfig() = default;

    static EngineConfig from_cli(int argc, char* argv[]) {
        EngineConfig config;
        argparse::ArgumentParser program("wfc_hex");

        program.add_argument("--open-menu")
            .help("Open EngineConfig menu prior to the generation.")
            .flag();

        program.add_argument("--map-radius")
            .help("Defines the radius of the map that will be generated, the greater, the more time it takes to generate.");

        program.add_argument("--opengl-render")
            .help("Show OpenGL real-time rendering")
            .flag();

        program.add_argument("--opengl-step-counter")
            .help("Defines the rate that OpenGL updates the rendering. The greater the faster.");

        program.add_argument("--wfc-seed")
            .help("Wave Function Collapse generation seed");

        program.add_argument("--hf-perlin-seed")
            .help("Height Factors perlin noise generation seed");

        program.add_argument("--river-ridged-seed")
            .help("River Ridged noise generation seed");

        try {
            program.parse_args(argc, argv);
        }
        catch (const std::exception& err) {
            std::cerr << err.what() << std::endl;
            std::cerr << program;
            throw err;
        }

        config.open_menu = program.get<bool>("--open-menu");

        config.opengl_render = program.get<bool>("--opengl-render");

        try {
            config.opengl_step_counter = std::stoul(program.get<std::string>("--opengl-step-counter"));
            config.map_radius = std::stoi(program.get<std::string>("--map-radius"));
        } catch (...) {
            // Default values already set
        }

        unsigned int base_random = generate_random_seed();

        if (program.present("--wfc-seed")) {
            config.wfc_seed = std::stoul(program.get<std::string>("--wfc-seed"));
        } else {
            config.wfc_seed = base_random;
        }

        if (program.present("--hf-perlin-seed")) {
            config.hf_perlin_seed = std::stoul(program.get<std::string>("--hf-perlin-seed"));
        } else {
            config.hf_perlin_seed = base_random;
        }

        if (program.present("--river-ridged-seed")) {
            config.river_ridged_seed = std::stoul(program.get<std::string>("--river-ridged-seed"));
        } else {
            config.river_ridged_seed = base_random;
        }

        return config;
    }
};

#endif