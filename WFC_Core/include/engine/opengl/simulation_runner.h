#include <functional>
#include "engine/opengl/window_manager.h"
#include "engine/opengl/hex_renderer.h"

void run_visual_simulation(
    GLFWwindow* window,
    HexRenderer& hex_renderer,
    unsigned int step_counter,
    HexMap& hex_map,
    std::function<bool()> step_logic
);
