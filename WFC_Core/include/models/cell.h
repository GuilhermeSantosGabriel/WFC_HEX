#ifndef CELL_H
#define CELL_H

#include <set>
#include "hex.h"

class Cell : public Hex {
public:
    double x, y;
    int entropy;
    std::set<int> possible_tiles;
    bool collapsed;

    // Constructor
    Cell(int q, int r, int s,
        double x_coord, double y_coord,
        int height,
        int e, const std::set<int>& t);

    void collapse(int tile);
};

#endif