#include "engine/hex_to_pixels.h"

using namespace std;

Point hex_to_pixel(Layout layout, Hex h) {
    const Orientation& M = layout.orientation;
    double x = (M.f0 * h.get_q() + M.f1 * h.get_r()) * layout.size.x;
    double y = (M.f2 * h.get_q() + M.f3 * h.get_r()) * layout.size.y;
    return Point(x + layout.origin.x, y + layout.origin.y);
}
