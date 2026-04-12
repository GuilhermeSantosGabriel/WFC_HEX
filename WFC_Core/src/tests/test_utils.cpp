#include "models/cell.h"

void generate_empty_shell(std::vector<Cell>& hex_map, int size) {

    std::set<int> placeholder_tiles = {-1};

    for (int q = -size; q <= size; q++) {
        int r1 = std::max(-size, -q - size);
        int r2 = std::min(size, -q + size);
        for (int r = r1; r <= r2; r++) {
            hex_map.emplace_back(q, r, -q - r, 0, 1, placeholder_tiles);
        }
    }
}