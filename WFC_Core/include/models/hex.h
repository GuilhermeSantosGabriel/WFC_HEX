#ifndef HEX_H
#define HEX_H

#include <vector>

class Hex
{
private:
    const int q, r, s;

public:
    // Constructor
    Hex(int q_s, int r_s, int s_s);

    // Getters
    int get_q() const;
    int get_r() const;
    int get_s() const;

    // Operators
    Hex operator+(const Hex& other) const;
    Hex operator-(const Hex& other) const;
    bool operator==(const Hex& other) const;
    bool operator!=(const Hex& other) const;

    // Direction helpers
    static const std::vector<Hex> hex_directions;

    static Hex hex_direction(int direction);
    static Hex hex_neighbor(const Hex& hex, int direction);
};

#endif