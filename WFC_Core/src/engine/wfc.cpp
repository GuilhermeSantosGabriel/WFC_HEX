#include "engine/wfc.h"
#include <random>
#include <stdexcept>

static void wave_function_collapse(HexMap& hex_map) {

    static std::random_device rd;
    static std::mt19937 gen(rd());

    int n_collapsed = 0;
    size_t max_collapsed = hex_map.cells.size();

    while (n_collapsed < int(max_collapsed)) {
        Cell* cell = lowest_entropy_cell(hex_map);
        if (!cell) break;

        collapse(*cell, gen);

        update_neighbors(*cell, hex_map);

        n_collapsed++;
    }
}

static Cell* lowest_entropy_cell(HexMap& hex_map) {
    Cell* lowest = nullptr;
    for (auto &c : hex_map.cells) {
        if (!c.collapsed) {
            if (!lowest || c.entropy < lowest->entropy) lowest = &c;
        }
    }
    return lowest;
}

static void collapse(Cell& cell, std::mt19937 &gen) {

    if (cell.possible_tiles.empty())
        throw std::runtime_error("WFC Error: A cell has zero possible tiles.");

    int tile = random_weighted_tile(cell, gen);
    cell.collapse(tile);
}

static int random_weighted_tile(Cell& cell, std::mt19937 &gen) {

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

static void update_neighbors(Cell& cell, HexMap& hex_map) {
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
