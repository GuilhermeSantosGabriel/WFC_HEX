#ifndef RULES_H
#define RULES_H

#include <map>
#include <set>
#include <unordered_map>

enum TileTypes {
    EMPTY,
    GRASS,
    FOREST,
    WATER,
    SAND
};

extern std::map<int, std::map<int, int>> ruleset;
extern std::set<int> tiles;

struct HeightFactor {
    float amplitude;
    float base;
};
extern std::unordered_map<int, HeightFactor> height_factors;

void validate_constraints();

#endif
