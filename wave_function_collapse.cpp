#include <bits/stdc++.h>
#include "hex.h"
#include "cell.h"

using namespace std;

enum TileTypes {
    GRASS,
    FOREST,
    WATER,
    SAND
};

const map<int, set<int>> ruleset = {
    {GRASS, {GRASS, FOREST, WATER, SAND}},
    {FOREST, {GRASS, FOREST}},
    {WATER, {GRASS, WATER, SAND}},
    {SAND, {GRASS, SAND, WATER}}
};

set<int> tiles = {GRASS, FOREST, WATER, SAND};

// Fill hex map with Cell objects
void fill_map(vector<Cell> &hex_map, int size, int n_tiles) {
    for (int q = -size; q <= size; q++) {
        int r1 = max(-size, -q - size);
        int r2 = min(size, -q + size);
        for (int r = r1; r <= r2; r++) {
            int s = -q - r;
            Cell h(q, r, s, n_tiles, tiles);
            hex_map.push_back(h);
        }
    }
}

// Pick a random tile and collapse the cell
bool collapse(Cell &cell) {
    if (cell.possible_tiles.empty()) return false;

    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(0, cell.possible_tiles.size() - 1);

    auto it = cell.possible_tiles.begin();
    advance(it, dis(gen));
    int chosen_tile = *it;

    cell.possible_tiles = {chosen_tile};
    cell.entropy = 1;
    cell.collapsed = true;

    return true;
}

// Find neighbors using Hex::hex_neighbor and a coordinate map
vector<Cell*> get_neighbors(Cell &cell, map<tuple<int,int,int>, Cell*> &coord_map) {
    vector<Cell*> neighbors;
    for (int dir = 0; dir < 6; dir++) {
        Hex neighbor_hex = Hex::hex_neighbor(cell, dir);
        auto it = coord_map.find({neighbor_hex.get_q(), neighbor_hex.get_r(), neighbor_hex.get_s()});
        if (it != coord_map.end()) neighbors.push_back(it->second);
    }
    return neighbors;
}

// Find the lowest entropy cell
Cell* lowest_entropy_cell(vector<Cell> &hex_map) {
    Cell* lowest = nullptr;
    for (auto &c : hex_map) {
        if (!c.collapsed) {
            if (!lowest || c.entropy < lowest->entropy) {
                lowest = &c;
            }
        }
    }
    return lowest;
}

bool wave_function_collapse(vector<Cell> &hex_map, int size, int n_tiles) {
    fill_map(hex_map, size, n_tiles);

    // Build coordinate map for fast neighbor lookup
    map<tuple<int,int,int>, Cell*> coord_map;
    for (auto &c : hex_map) coord_map[{c.get_q(), c.get_r(), c.get_s()}] = &c;

    int n_collapsed = 0;
    int max_collapsed = hex_map.size();

    while (n_collapsed < max_collapsed) {
        Cell* cell = lowest_entropy_cell(hex_map);
        if (!cell) break;

        if(!collapse(*cell)) return false;
        n_collapsed++;

        for (Cell* neighbor : get_neighbors(*cell, coord_map)) {
            if (!neighbor->collapsed) {
                set<int> allowed;
                for (int t : neighbor->possible_tiles) {
                    if (ruleset.at(*cell->possible_tiles.begin()).count(t))
                        allowed.insert(t);
                }
                neighbor->possible_tiles = allowed;
                neighbor->entropy = neighbor->possible_tiles.size();
            }
        }
    }

    return true;
}

// SHOW IT ELSEWHERE

struct Point {
    const double x, y;
    Point(double x_, double y_): x(x_), y(y_) {}
};

struct Orientation {
    const double f0, f1, f2, f3;
    const double b0, b1, b2, b3;
    const double start_angle; // in multiples of 60°
    Orientation(double f0_, double f1_, double f2_, double f3_,
                double b0_, double b1_, double b2_, double b3_,
                double start_angle_)
    : f0(f0_), f1(f1_), f2(f2_), f3(f3_),
      b0(b0_), b1(b1_), b2(b2_), b3(b3_),
      start_angle(start_angle_) {}
};

struct Layout {
    const Orientation orientation;
    const Point size;
    const Point origin;
    Layout(Orientation orientation_, Point size_, Point origin_)
    : orientation(orientation_), size(size_), origin(origin_) {}
};

Point hex_to_pixel(Layout layout, Hex h) {
    const Orientation& M = layout.orientation;
    double x = (M.f0 * h.get_q() + M.f1 * h.get_r()) * layout.size.x;
    double y = (M.f2 * h.get_q() + M.f3 * h.get_r()) * layout.size.y;
    return Point(x + layout.origin.x, y + layout.origin.y);
}

const Orientation layout_flat
= Orientation(3.0 / 2.0, 0.0, sqrt(3.0) / 2.0, sqrt(3.0),
                2.0 / 3.0, 0.0, -1.0 / 3.0, sqrt(3.0) / 3.0,
                0.0);


int main() {
    int N;
    cin >> N;

    vector<Cell> hex_map;
    Layout layout(layout_flat, Point(5,5), Point(500, 500));



    if (wave_function_collapse(hex_map, N, tiles.size())) {
        //cout << "SUCCESS!!!" << endl;
        
        for (auto &c : hex_map){
            Point p = hex_to_pixel(layout, c);
            
            //cout << q << " " << r << " " << -q-r << endl;
            cout << p.x << " " << p.y << " " << *c.possible_tiles.begin() << endl;;
        }
        
    } else {
        cout << "yeah we lost" << endl;
    }
}