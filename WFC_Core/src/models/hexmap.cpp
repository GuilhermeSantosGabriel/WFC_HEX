#include <cassert>
#include "models/hexmap.h"
#include "engine/rules.h"

HexMap::HexMap(int radius_s) :
    radius(radius_s)
{
    assert(radius >= 0);
    cells.reserve(3 * radius * (radius + 1) + 1);
}

int HexMap::get_radius() const { return radius; }

Cell generate_unit_cell(
    Layout layout, int q, int r, std::set<int>& initial_tiles
) {
    Hex aux_h = Hex(q, r, -q - r, 0);
    Point p = hex_to_pixel(layout, aux_h);
    return Cell(
        q, r, -q - r,
        p.x, p.y,
        0,
        initial_tiles.size(), initial_tiles
    );
}

std::vector<Cell*> HexMap::get_neighbors(Cell& center) {
    std::vector<Cell*> neighbors;

    int center_q = center.get_q();
    int center_r = center.get_r();
    int center_s = center.get_s();
    
    static const std::vector<std::tuple<int, int, int>> directions = {
        {+1, -1, 0}, {+1, 0, -1}, {0, +1, -1},
        {-1, +1, 0}, {-1, 0, +1}, {0, -1, +1}
    };

    for (const auto& dir : directions) {
        int nq = center_q + std::get<0>(dir);
        int nr = center_r + std::get<1>(dir);
        int ns = center_s + std::get<2>(dir);

        auto it = coord_map.find({nq, nr, ns});
        if (it != coord_map.end()) {
            neighbors.push_back(&cells[it->second]);
        }
    }

    return neighbors;
}

HexMap HexMap::generate_uncollapsed_hex_map(Layout layout, int radius) {

    HexMap new_map = HexMap(radius);

    for (int q = -radius; q <= radius; q++) {
        int r1 = std::max(-radius, -q - radius);
        int r2 = std::min(radius, -q + radius);
        for (int r = r1; r <= r2; r++) {

            Cell c = generate_unit_cell(layout, q, r, tiles);

            new_map.cells.push_back(c);
            new_map.coord_map[{q, r, -q-r}] = new_map.cells.size() - 1;
        }
    }

    return new_map;
}

HexMap HexMap::generate_empty_hex_map(Layout layout, int radius) {

    HexMap new_map = HexMap(radius);
    std::set<int> empty_tile = {EMPTY};

    for (int q = -radius; q <= radius; q++) {
        int r1 = std::max(-radius, -q - radius);
        int r2 = std::min(radius, -q + radius);
        for (int r = r1; r <= r2; r++) {

            Cell c = generate_unit_cell(layout, q, r, empty_tile);

            new_map.cells.push_back(c);
            new_map.coord_map[{q, r, -q-r}] = new_map.cells.size() - 1;
        }
    }

    return new_map;
}
