#include <functional>
#include "engine/opengl/window_manager.h"
#include "engine/opengl/hex_renderer.h"

#include "engine/generators/generators.h"

void run_step_visual_simulation(
    GLFWwindow* window,
    HexRenderer& hex_renderer,
    unsigned int step_counter,
    HexMap& hex_map,
    StepGenerator& step_generator
);
