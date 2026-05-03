#include "models/cell.h"
#include "engine/hex_to_pixels.h"

Cell::Cell(
    int q, int r, int s,
    double x_coord, double y_coord,
    int height,
    int e, const std::set<int>& t)
:
    Hex(q, r, s, height),
    x(x_coord), y(y_coord),
    entropy(e), possible_tiles(t), collapsed(false) {}


// TODO - Cell - update_possible_tiles()
void Cell::update_possible_tiles() {

}

void Cell::collapse(int tile) {
    possible_tiles = {tile};
    entropy = 1;
    collapsed = true;
}
