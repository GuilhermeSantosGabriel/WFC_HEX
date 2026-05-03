#ifndef RULES_H
#define RULES_H

#include <map>
#include <set>

enum TileTypes {
    EMPTY,
    GRASS,
    FOREST,
    WATER,
    SAND
};

extern std::map<int, std::map<int, int>> ruleset;
extern std::set<int> tiles;

void validate_constraints();

#endif
