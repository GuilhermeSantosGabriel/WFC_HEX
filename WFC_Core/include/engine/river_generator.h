#ifndef RIVER_GENERATOR_H
#define RIVER_GENERATOR_H

#include "models/hexmap.h"
#include "engine/opengl/window_manager.h"
#include "engine/opengl/hex_renderer.h"
#include "engine/noises/perlin.h"
#include "engine/noises/ridged_multifractal.h"

void generate_river(HexMap& hex_map, bool sand_margin, GLFWwindow* window, HexRenderer& hex_renderer, PerlinNoise& perlin, RidgedNoise& ridged);

void generate_river_step(Cell& c, bool sand_margin, PerlinNoise& perlin, RidgedNoise& ridged);

#endif
