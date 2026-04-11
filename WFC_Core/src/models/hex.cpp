#include "models/hex.h"
#include <cassert>

/* ALL OF THESE FUNCTIONS WERE BASED ON https://www.redblobgames.com/grids/hexagons/implementation.html#map-optimized-storage */

// Constructor
Hex::Hex(int q_s, int r_s, int s_s, int height_s) : q(q_s), r(r_s), s(s_s), height(height_s) {
    assert(q + r + s == 0); // this is a characteristic of the cube coordinate system used
}

// Getters
int Hex::get_q() const { return q; }
int Hex::get_r() const { return r; }
int Hex::get_s() const { return s; }
int Hex::get_height() { return height; }

// Setters
void Hex::set_height(int height_s) {
    assert(height_s >= 0);
    height = height_s;
}

// Operators
Hex Hex::operator+(const Hex& other) const {
    return Hex(q + other.q, r + other.r, s + other.s, height + other.height);
}

Hex Hex::operator-(const Hex& other) const {
    return Hex(q - other.q, r - other.r, s - other.s, height - other.height);
}

bool Hex::operator==(const Hex& other) const {
    return q == other.q && r == other.r && s == other.s && height == other.height;
}

bool Hex::operator!=(const Hex& other) const {
    return !(*this == other);
}

// Static member definition
const std::vector<Hex> Hex::hex_directions = {
    Hex(1, 0, -1, 0), Hex(1, -1, 0, 0), Hex(0, -1, 1, 0),
    Hex(-1, 0, 1, 0), Hex(-1, 1, 0, 0), Hex(0, 1, -1, 0)
};

// Static functions
Hex Hex::hex_direction(int direction) {
    assert(0 <= direction && direction < 6);
    return hex_directions[direction];
}

Hex Hex::hex_neighbor(const Hex& hex, int direction) {
    return hex + hex_direction(direction);
}
