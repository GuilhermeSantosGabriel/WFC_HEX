#include <bits/stdc++.h> 
#include "hex.h"

using namespace std;

struct Point {
    const double x, y;
    Point(double x_, double y_): x(x_), y(y_) {}
};

struct Orientation {
    const double f0, f1, f2, f3;
    const double b0, b1, b2, b3;
    const double start_angle; // in multiples of 60°
    Orientation(double f0_, double f1_, double f2_, double f3_,
                double b0_, double b1_, double b2_, double b3_,
                double start_angle_)
    : f0(f0_), f1(f1_), f2(f2_), f3(f3_),
      b0(b0_), b1(b1_), b2(b2_), b3(b3_),
      start_angle(start_angle_) {}
};




struct Layout {
    const Orientation orientation;
    const Point size;
    const Point origin;
    Layout(Orientation orientation_, Point size_, Point origin_)
    : orientation(orientation_), size(size_), origin(origin_) {}
};



Point hex_to_pixel(Layout layout, Hex h) {
    const Orientation& M = layout.orientation;
    double x = (M.f0 * h.get_q() + M.f1 * h.get_r()) * layout.size.x;
    double y = (M.f2 * h.get_q() + M.f3 * h.get_r()) * layout.size.y;
    return Point(x + layout.origin.x, y + layout.origin.y);
}

namespace std {
    template <>
    struct hash<Hex> {
        size_t operator()(const Hex& h) const {
            hash<int> int_hash;
            size_t hq = int_hash(h.get_q());
            size_t hr = int_hash(h.get_r());
            // s is not needed because q + r + s = 0
            return hq ^ (hr + 0x9e3779b9 + (hq << 6) + (hq >> 2));
        }
    };
}

const Orientation layout_flat
= Orientation(3.0 / 2.0, 0.0, sqrt(3.0) / 2.0, sqrt(3.0),
                2.0 / 3.0, 0.0, -1.0 / 3.0, sqrt(3.0) / 3.0,
                0.0);

int main(){

    Layout layout(layout_flat, Point(5,5), Point(500, 500));

    int N;

    cerr << "Type map radius (N): "; 
    if (!(cin >> N)) return 0;

    unordered_set<Hex> hex_map;
    
    for (int q = -N; q <= N; q++) {
        int r1 = max(-N, -q - N);
        int r2 = min( N, -q + N);
        for (int r = r1; r <= r2; r++) {
            Hex h = Hex(q, r, -q-r);
            hex_map.insert(h);
            Point p = hex_to_pixel(layout, h);
            
            //cout << q << " " << r << " " << -q-r << endl;
            cout << p.x << " " << p.y << endl;
        }
    }
}
