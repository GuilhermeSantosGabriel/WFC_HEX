#ifndef CELL_H
#define CELL_H

#include <set>
#include "hex.h"

class Cell : public Hex {
public:
    int entropy;
    std::set<int> possible_tiles;
    bool collapsed;

    // Constructor
    Cell(int q, int r, int s,
         int e,
         const std::set<int>& t);
};

#endif // CELL_H