#include <iostream>
#include "engine/rules.h"

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

static void validate_ruleset(const std::map<int, std::map<int, int>>& ruleset) {
    for (auto const& [tile, neighbors] : ruleset) {
        int sum = 0;
        for (auto const& [neighbor, weight] : neighbors) sum += weight;
        if (sum != 10) std::cerr << "Error: Tile " << tile << " sums " << sum << "\n";
    }
}

// TODO - Rules - HeightFactors
// TODO - Rules - validate_height_factors()
static void validate_height_factors() {}

static void validate_constraints() {
    validate_ruleset(ruleset);
    validate_height_factors();
}
