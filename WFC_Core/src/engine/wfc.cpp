#include "engine/wfc.h"
#include "engine/opengl/window_manager.h"
#include "engine/opengl/hex_renderer.h"
#include "engine/height_dealer.h"
#include <stdexcept>
#include <cassert>

void wave_function_collapse(HexMap& hex_map, GLFWwindow* window, HexRenderer& hex_renderer) {

    static std::random_device rd;
    static std::mt19937 gen(rd());

    int n_collapsed = 0;
    size_t max_collapsed = hex_map.cells.size();

    int window_height, window_width;
    int counter = 0;
    while (n_collapsed < int(max_collapsed)) {

        Cell* cell = lowest_entropy_cell(hex_map);
        if (!cell) break;

        collapse(*cell, gen);
        set_height_by_height_factors(*cell);
        update_neighbors(*cell, hex_map);
        n_collapsed++;

        if (counter == 100) {
            clear_window(window, &window_width, &window_height);
            hex_renderer.draw_hex_map_frame(hex_map, window_width, window_height);
            update_window(window);
            counter = 0;
        } else counter++;
    }

    // Keeps window open waiting the user
    while (!glfwWindowShouldClose(window)) {
        clear_window(window, &window_width, &window_height);
        hex_renderer.draw_hex_map_frame(hex_map, window_width, window_height);
        update_window(window);
    }
}

Cell* lowest_entropy_cell(HexMap& hex_map) {
    Cell* lowest = nullptr;
    for (auto &c : hex_map.cells) {
        if (!c.collapsed) {
            if (!lowest || c.entropy < lowest->entropy) lowest = &c;
        }
    }
    return lowest;
}

void collapse(Cell& cell, std::mt19937 &gen) {

    if (cell.possible_tiles.empty())
        throw std::runtime_error("WFC Error: A cell has zero possible tiles.");

    int tile = random_weighted_tile(cell, gen);
    cell.collapse(tile);
}

int random_weighted_tile(Cell& cell, std::mt19937 &gen) {

    int total_weight = 0;
    std::map<int, int> current_weights;

    for (int tile : cell.possible_tiles) {
        int w = ruleset[tile][tile]; 
        if (w == 0) w = 1;
        current_weights[tile] = w;
        total_weight += w;
    }

    std::uniform_int_distribution<> dis(0, total_weight - 1);
    int random_roll = dis(gen);

    int chosen_tile = *cell.possible_tiles.begin();
    int cumulative_sum = 0;

    for (auto const& [tile, weight] : current_weights) {
        cumulative_sum += weight;
        if (random_roll < cumulative_sum) {
            chosen_tile = tile;
            break;
        }
    }

    return chosen_tile;
}

void update_neighbors(Cell& cell, HexMap& hex_map) {
    for (Cell* neighbor : hex_map.get_neighbors(cell)) {
        if (!neighbor->collapsed) {
            std::set<int> allowed;
            int current_tile = *cell.possible_tiles.begin();
            for (int t : neighbor->possible_tiles) {
                if (ruleset[current_tile].count(t)) allowed.insert(t);
            }
            neighbor->possible_tiles = allowed;
            neighbor->entropy = neighbor->possible_tiles.size();
        }
    }
}
