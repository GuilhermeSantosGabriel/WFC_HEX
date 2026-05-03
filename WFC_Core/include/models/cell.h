#ifndef CELL_H
#define CELL_H

#include <set>
#include "hex.h"

class Cell : public Hex {
public:
    int entropy;
    std::set<int> possible_tiles;
    bool collapsed;
    double x, y;

    // Constructor
    Cell(int q, int r, int s,
        double x_coord, double y_coord,
        int height,
        int e, const std::set<int>& t);

    void update_possible_tiles();

    void collapse();
};

#endif