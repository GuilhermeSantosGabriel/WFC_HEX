#include <bits/stdc++.h>

#include "models/cell.h"
#include "engine/hex_to_pixels.h"
#include "engine/wfc.h"

using namespace std;

int main() {

    int size;
    cerr << "Type map radius (size): ";
    cin >> size;

    vector<Cell> hex_map;
    Layout layout(layout_flat, Point(5,5), Point(500, 500));

    for (int q = -size; q <= size; q++) {
        int r1 = std::max(-size, -q - size);
        int r2 = std::min(size, -q + size);
        for (int r = r1; r <= r2; r++) {

            Cell h(q, r, -q - r, 0, tiles.size(), tiles);

            hex_map.push_back(h);
        }
    }

    if (wave_function_collapse(hex_map, size, tiles.size())) {
        //cout << "SUCCESS!!!" << endl;
        
        for (auto &c : hex_map){

            c.set_height(cell_height(c));

            Point p = hex_to_pixel(layout, c);
            
            //cout << q << " " << r << " " << -q-r << endl;
            cout << p.x << " " << p.y << " " << c.get_height() << " " << *c.possible_tiles.begin() << endl;;
        }
        
    } else {
        cout << "yeah we lost" << endl;
    }
}
