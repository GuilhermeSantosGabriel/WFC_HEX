#include "models/cell.h"

Cell::Cell(int q, int r, int s,
           int e,
           const std::set<int>& t)
    : Hex(q, r, s), entropy(e), possible_tiles(t), collapsed(false) {}