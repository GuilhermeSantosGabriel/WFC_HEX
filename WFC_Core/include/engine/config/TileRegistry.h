#ifndef TILE_REGISTR_H
#define TILE_REGISTR_H

#include <cstdint>
#include <cassert>
#include <map>

#include <fstream>
#include "external/nlohmann/json.hpp"
using json = nlohmann::json;


class TileConfig {
public:

    unsigned int id;

    unsigned int r;
    unsigned int g;
    unsigned int b;
    unsigned int a;

    float height_base;
    float height_amplitude;

    // Map of <TileConfig.id, weight(0 to 10)>
    std::map<unsigned int, unsigned int> neighbors;

    TileConfig(
        unsigned int id_s,
        unsigned int r_s, unsigned int g_s, unsigned int b_s, unsigned int a_s,
        float hb_s, float ha_s,
        std::map<unsigned int, unsigned int> n
    ):
        id(id_s),
        r(r_s), g(g_s), b(b_s), a(a_s),
        height_base(hb_s), height_amplitude(ha_s),
        neighbors(n)
    {
        assert(hb_s >= 0 && "Negative Base height found in tile "+id_s);
        assert(ha_s >= 0 && "Negative Base amplitude found in tile"+id_s);

        int sum = 0;
        for (auto const& [_, n_weight] : n) sum += n_weight;
        assert(sum == 10 && "Unavailable weight map for tile "+id_s);
    }
};

class TileRegistry {
public:

    std::map<int, TileConfig> config_map;

    // TODO - implement TileRegistry fromJson
    TileRegistry fromJson(char* path) {

        std::ifstream f(path);
        json data = json::parse(f);

    };
};

#endif
