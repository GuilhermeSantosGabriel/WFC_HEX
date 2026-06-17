#ifndef TILE_REGISTR_H
#define TILE_REGISTR_H

#include <cstdint>
#include <cassert>
#include <map>
#include <fstream>
#include <stdexcept>
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
    static TileRegistry fromJson() {

        TileRegistry registry;

        std::string config_path = "assets/tile_registry.json"; 
        std::ifstream file(config_path);
        if (!file.is_open()) {
            throw std::runtime_error("CRITICAL: Unable to open config file at " + config_path);
        }

        json data = json::parse(file);

        if (!data.contains("tiles")) {
            throw std::runtime_error("CRITICAL: Invalid JSON format. Root object must contain a 'tiles'.");
        }

        json available_tiles_ids = json::array();
        for (auto it = data["tiles"].begin(); it != data["tiles"].end(); ++it) {
            available_tiles_ids.push_back(it.key());
        }

        for (auto& [key, value] : data["tiles"].items()) {

            try {
                unsigned int id = int(&key);

                unsigned int r = value['r'];
                unsigned int g = value['g'];
                unsigned int b = value['b'];
                unsigned int a = value['a'];

                float hb = value['height_base'];
                float ha = value['height_amplitude'];

                std::map<unsigned int,unsigned int> n;
                for (auto& [n_key, n_value] : value["neighbors"].items()) {
                    if (!available_tiles_ids.contains(n_key)) {
                        throw std::runtime_error(
                            "CRITICAL: Invalid JSON format. Neighbor tile" +
                            n_key + " inside of tile " + n_value +
                            " not configured"
                        );
                    }
                    // TODO - continue here
                }

                TileConfig tile_config_aux(
                    
                );
            }
            catch(const std::exception& e) {

            }
        }
    };
};

#endif
