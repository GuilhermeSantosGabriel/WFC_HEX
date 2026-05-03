#include <bits/stdc++.h>

#include "models/cell.h"
#include "engine/hex_to_pixels.h"

#include "tests/test_utils.h"

using namespace std;

int main() {

    int size;
    cerr << "Type map radius: ";
    cin >> size;

    Layout layout(layout_flat, Point(5,5), Point(500, 500));
    vector<Cell> hex_map;

    generate_empty_shell(hex_map, size);

    for (auto &c : hex_map){

        Point p = hex_to_pixel(layout, c);

        cout << p.x << " " << p.y << " " << c.get_height() << " " << *c.possible_tiles.begin() << endl;
    }
}
