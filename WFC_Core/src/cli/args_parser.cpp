#include "cli/args_parser.h"
#include <optional>
#include <random>
#include <string>


ArgsParser::ArgsParser(int argc, char *argv[]) {

    argparse::ArgumentParser program("wfc_hex");

    program.add_argument("--map-radius")
        .help("Defines the radius of the map that will be generated, the greater, the more time it takes to generate. Default: 45")
        .default_value(std::string("45"))
    ;
    program.add_argument("--opengl-render")
        .help("Show OpenGL real-time rendering")
        .flag()
    ;
    program.add_argument("--opengl-step-counter")
        .help("Defines the rate that OpenGL updates the rendering. The greater the faster. Default: 100")
        .default_value(std::string("100"))
    ;
    program.add_argument("--wfc-seed")
        .help("Wave Function Collapse generation seed")
    ;
    program.add_argument("--hf-perlin-seed")
        .help("Height Factors perlin noise generation seed")
    ;
    program.add_argument("--river-ridged-seed")
        .help("River Ridged noise generation seed")
    ;

    try {
        program.parse_args(argc, argv);
    }
    catch (const std::exception& err) {
        std::cerr << err.what() << std::endl;
        std::cerr << program;
        throw err;
    }

    this->opengl_render = program.get<bool>("--opengl-render");

    try {
        this->opengl_step_counter = std::stoi(program.get<std::string>("--opengl-step-counter"));
        this->map_radius = std::stoi(program.get<std::string>("--map-radius"));
    } catch (...) {
        this->opengl_step_counter = 100;
    }

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<unsigned int> dist(0, 0xFFFFFFFF);
    unsigned int base_random = dist(gen);

    if (program.present("--wfc-seed")) {
        std::string s = program.get<std::string>("--wfc-seed");
        this->wfc_seed = std::stoul(s);
    } else  this->wfc_seed = base_random;

    if (program.present("--hf-perlin-seed")) {
        std::string s = program.get<std::string>("--hf-perlin-seed");
        this->hf_perlin_seed = std::stoul(s);
    } else this->hf_perlin_seed = base_random;

    if (program.present("--river-ridged-seed")) {
        std::string s = program.get<std::string>("--river-ridged-seed");
        this->river_ridged_seed = std::stoul(s);
    } else this->river_ridged_seed = base_random;
}
