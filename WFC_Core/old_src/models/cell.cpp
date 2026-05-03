#include "models/cell.h"

Cell::Cell(int q, int r, int s, int height,
           int e,
           const std::set<int>& t)
    : Hex(q, r, s, height), entropy(e), possible_tiles(t), collapsed(false) {}