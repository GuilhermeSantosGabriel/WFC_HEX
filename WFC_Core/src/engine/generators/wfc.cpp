#include "engine/generators/wfc.h"

#include "engine/opengl/window_manager.h"
#include "engine/opengl/hex_renderer.h"
#include "engine/height_dealer.h"

#include <stdexcept>
#include <cassert>



WFC::WFC(HexMap& hex_map_s, unsigned int wfc_seed, PerlinNoise& hf_perlin)
: hex_map(hex_map_s), height_factor_perlin(hf_perlin) {
    gen.seed(wfc_seed);
}

void WFC::wave_function_collapse() {
    while (step());
}

bool WFC::step() {

    Cell* cell = this->lowest_entropy_cell();
    if (!cell) return false;

    this->wfc_collapse(*cell);
    this->update_neighbors(*cell);
    set_height_by_height_factors(
        *cell, this->height_factor_perlin
    );

    return true;
}

Cell* WFC::lowest_entropy_cell() {
    Cell* lowest = nullptr;
    for (auto &c : hex_map.cells) {
        if (!c.collapsed) {
            if (!lowest || c.entropy < lowest->entropy) lowest = &c;
        }
    }
    return lowest;
}

void WFC::wfc_collapse(Cell& cell) {

    if (cell.possible_tiles.empty())
        throw std::runtime_error("WFC Error: A cell has zero possible tiles.");

    int tile = this->random_weighted_tile(cell);
    cell.collapse(tile);
}

int WFC::random_weighted_tile(Cell& cell) {

    int total_weight = 0;
    std::map<int, int> current_weights;

    for (int tile : cell.possible_tiles) {
        int w = ruleset[tile][tile]; 
        if (w == 0) w = 1;
        current_weights[tile] = w;
        total_weight += w;
    }

    std::uniform_int_distribution<> dis(0, total_weight - 1);
    int random_roll = dis(this->gen);

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

void WFC::update_neighbors(Cell& cell) {
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
