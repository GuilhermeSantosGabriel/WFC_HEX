#ifndef SIMULATION_RUNNER_H
#define SIMULATION_RUNNER_H

#include "engine/generators/IStepGenerator.h"

#include <functional>
#include "engine/render/window_manager.h"
#include "engine/render/hex_renderer.h"


void run_step_visual_simulation(
    GLFWwindow* window,
    HexRenderer& hex_renderer,
    unsigned int step_counter,
    HexMap& hex_map,
    IStepGenerator& step_generator
);

#endif
