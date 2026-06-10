#ifndef SIMULATION_RUNNER_H
#define SIMULATION_RUNNER_H

#include "engine/generators/generators.h"

#include <functional>
#include "engine/render/window_manager.h"
#include "engine/render/hex_renderer.h"
#include "engine/render/camera.h"


void run_step_visual_simulation(
    GLFWwindow* window,
    HexRenderer& hex_renderer,
    unsigned int step_counter,
    HexMap& hex_map,
    StepGenerator& step_generator,
    double visual_start_timestamp,
    Camera& camera,
    bool persist_after_generator_finished
);

#endif
