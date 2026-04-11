#include "engine/wfc.h"
#include "models/hex.h"
#include "engine/noise_generator/perlin.h"

#include <iostream>
#include <random>
#include <algorithm>
#include <tuple>
#include <cassert>

std::map<int, std::map<int, int>> ruleset = {

    {WATER,  {
        {WATER, 4}, {SAND, 6}
    }},

    {SAND,   {
        {WATER, 1}, {SAND, 5}, {GRASS, 4}
    }},

    {GRASS,  {
        {SAND, 3}, {GRASS, 4}, {FOREST, 3}
    }},

    {FOREST, {
        {GRASS, 3}, {FOREST, 7
    }}}

};

std::set<int> tiles = {GRASS, FOREST, WATER, SAND};

static void validate_rules(const std::map<int, std::map<int, int>>& rules) {
    for (auto const& [tile, neighbors] : rules) {
        int soma = 0;
        for (auto const& [neighbor, weight] : neighbors) soma += weight;
        if (soma != 10) std::cerr << "Error: Tile " << tile << " sums " << soma << "\n";
    }
}

static void fill_map(std::vector<Cell> &hex_map, int size, int n_tiles) {

    for (int q = -size; q <= size; q++) {
        int r1 = std::max(-size, -q - size);
        int r2 = std::min(size, -q + size);
        for (int r = r1; r <= r2; r++) {

            Cell h(q, r, -q - r, 0, n_tiles, tiles);

            hex_map.push_back(h);
        }
    }
}

static bool collapse(Cell &cell) {
    if (cell.possible_tiles.empty()) return false;

    static std::random_device rd;
    static std::mt19937 gen(rd());

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

    cell.possible_tiles = {chosen_tile};
    cell.entropy = 1;
    cell.collapsed = true;
    return true;
}

static std::vector<Cell*> get_neighbors(Cell &cell, std::map<std::tuple<int,int,int>, Cell*> &coord_map) {
    std::vector<Cell*> neighbors;
    for (int dir = 0; dir < 6; dir++) {
        Hex neighbor_hex = Hex::hex_neighbor(cell, dir);
        auto it = coord_map.find({neighbor_hex.get_q(), neighbor_hex.get_r(), neighbor_hex.get_s()});
        if (it != coord_map.end()) neighbors.push_back(it->second);
    }
    return neighbors;
}

static Cell* lowest_entropy_cell(std::vector<Cell> &hex_map) {
    Cell* lowest = nullptr;
    for (auto &c : hex_map) {
        if (!c.collapsed) {
            if (!lowest || c.entropy < lowest->entropy) lowest = &c;
        }
    }
    return lowest;
}

bool wave_function_collapse(std::vector<Cell> &hex_map, int size, int n_tiles) {
    validate_rules(ruleset);
    fill_map(hex_map, size, n_tiles);

    std::map<std::tuple<int,int,int>, Cell*> coord_map;
    for (auto &c : hex_map) coord_map[{c.get_q(), c.get_r(), c.get_s()}] = &c;

    int n_collapsed = 0;
    size_t max_collapsed = hex_map.size();

    while (n_collapsed < int(max_collapsed)) {
        Cell* cell = lowest_entropy_cell(hex_map);
        if (!cell) break;

        if(!collapse(*cell)) return false;
        n_collapsed++;

        for (Cell* neighbor : get_neighbors(*cell, coord_map)) {
            if (!neighbor->collapsed) {
                std::set<int> allowed;
                int current_tile = *cell->possible_tiles.begin();
                for (int t : neighbor->possible_tiles) {
                    if (ruleset[current_tile].count(t)) allowed.insert(t);
                }
                neighbor->possible_tiles = allowed;
                neighbor->entropy = neighbor->possible_tiles.size();
            }
        }
    }
    return true;
}

int cell_height(Cell hex_cell) {

    assert(hex_cell.collapsed);

    int tile_type = *hex_cell.possible_tiles.begin();

    float amplitude = 0.0f;
    switch (tile_type) {
        case WATER:
            amplitude = 0.0f;
        break;

        case SAND:
            amplitude = 15.0f;
        break;

        case GRASS:
            amplitude = 30.0f;
        break;

        case FOREST:
            amplitude = 50.0f;
        break;

        default:
            amplitude = 0.0f;
    }

    float frequency = 0.1f;
    int height = (int)(amplitude * normalized_perlin(
        hex_cell.get_q(),
        hex_cell.get_r(),
        frequency
    ));

    return height;
}
