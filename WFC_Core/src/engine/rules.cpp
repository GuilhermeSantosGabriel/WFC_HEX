#include "engine/rules.h"
#include <iostream>
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
        {GRASS, 3}, {FOREST, 7}
    }}

};

std::set<int> tiles = {GRASS, FOREST, SAND};

void validate_ruleset(const std::map<int, std::map<int, int>>& ruleset) {
    for (auto const& [tile, neighbors] : ruleset) {
        int sum = 0;
        for (auto const& [neighbor, weight] : neighbors) sum += weight;
        if (sum != 10) std::cerr << "Error: Tile " << tile << " sums " << sum << "\n";
    }
}

std::unordered_map<int, HeightFactor> height_factors = {

    {EMPTY, {0.0f, 30.0f}},

    {WATER, {2.0f, 0.0f}},

    {SAND, {2.0f, 3.0f}},

    {GRASS, {8.0f, 6.0f}},

    {FOREST, {15.0f, 15.0f}},

};

void validate_height_factors(std::unordered_map<int, HeightFactor> height_factors) {
    for (auto& [tile_type, factor] : height_factors) {
        assert(factor.base >= 0 && "Negative Base height found!");
        assert(factor.amplitude >= 0 && "Negative Base amplitude found!");
    }
}

void validate_constraints() {
    validate_ruleset(ruleset);
    validate_height_factors(height_factors);
}
