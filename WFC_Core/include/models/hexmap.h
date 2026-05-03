#ifndef HEX_MAP_H
#define HEX_MAP_H

#include "models/cell.h"
#include <unordered_map>
#include "engine/hex_to_pixels.h"

struct HexHash {
    size_t operator()(const std::tuple<int, int, int>& t) const {
        auto [q, r, s] = t;
        std::size_t h1 = std::hash<int>{}(q);
        std::size_t h2 = std::hash<int>{}(r);
        std::size_t h3 = std::hash<int>{}(s);
        return h1 ^ (h2 << 1) ^ (h3 << 2);
    }
};

class HexMap {
private:
    const int radius;

public:
    std::vector<Cell> cells;
    std::unordered_map<std::tuple<int, int, int>, size_t, HexHash> coord_map;

    HexMap(int radius_s);

    int get_radius() const;

    std::vector<Cell*> get_neighbors(Cell& center);

    static HexMap generate_uncollapsed_hex_map(Layout layout, int radius);
    static HexMap generate_empty_hex_map(Layout layout, int radius);

};

#endif
