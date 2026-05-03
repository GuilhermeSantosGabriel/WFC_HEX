#include <bits/stdc++.h>

#include "models/cell.h"
#include "engine/hex_to_pixels.h"
#include "engine/wfc.h"

using namespace std;

int main() {

        for (auto &c : hex_map){

            c.set_height(cell_height(c));

            Point p = hex_to_pixel(layout, c);
            
            //cout << q << " " << r << " " << -q-r << endl;
            cout << p.x << " " << p.y << " " << c.get_height() << " " << *c.possible_tiles.begin() << endl;;
        }
        

}
