#include <bits/stdc++.h>

#include "models/cell.h"
#include "engine/hex_to_pixels.h"
#include "engine/wfc.h"

using namespace std;

int main() {

    int N;
    cerr << "Type map radius (N): ";
    cin >> N;

    vector<Cell> hex_map;
    Layout layout(layout_flat, Point(5,5), Point(500, 500));


    if (wave_function_collapse(hex_map, N, tiles.size())) {
        //cout << "SUCCESS!!!" << endl;
        
        for (auto &c : hex_map){
            Point p = hex_to_pixel(layout, c);
            
            //cout << q << " " << r << " " << -q-r << endl;
            cout << p.x << " " << p.y << " " << *c.possible_tiles.begin() << endl;;
        }
        
    } else {
        cout << "yeah we lost" << endl;
    }
}
