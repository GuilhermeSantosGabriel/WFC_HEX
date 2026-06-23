#ifndef TILE_REGISTRY_H
#define TILE_REGISTRY_H

#include <cstdint>
#include <cassert>
#include <map>
#include <fstream>
#include <stdexcept>
#include <iostream>
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

private:

    static json openJsonFile(std::string config_path) {
        std::ifstream file(config_path);
        if (!file.is_open()) {
            throw std::runtime_error("CRITICAL: Unable to open config file at " + config_path);
        }
        json data = json::parse(file);

        return data;
    }

    static void validateJsonData(json data) {
        if (!data.contains("tiles")) {
            throw std::runtime_error("CRITICAL: Invalid JSON format. Root object must contain a 'tiles'.");
        }
    }

    static TileConfig configFromJson(
        std::string key,
        nlohmann::json_abi_v3_12_0::json value,
        json full_data
    ) {

        try {
            unsigned int id = std::stoul(key);

            unsigned int r = value["r"].get<unsigned int>();
            unsigned int g = value["g"].get<unsigned int>();
            unsigned int b = value["b"].get<unsigned int>();
            unsigned int a = value["a"].get<unsigned int>();

            float hb = value["height_base"].get<float>();
            float ha = value["height_amplitude"].get<float>();

            std::map<unsigned int,unsigned int> neighbors;
            for (auto& [n_key, n_value] : value["neighbors"].items()) {

                if (!full_data["tiles"].contains(n_key)) {
                    throw std::runtime_error(
                        "CRITICAL: Neighbor " + n_key +
                        " not configured in tiles."
                    );
                }

                unsigned int neighbor_id = std::stoul(n_key);
                unsigned int neighbor_weight = n_value.get<unsigned int>();

                neighbors[neighbor_id] = neighbor_weight;
            }

            return TileConfig(
                id,
                r, g, b, a,
                hb, ha,
                neighbors
            );
        }

        catch(const std::exception& e) {
            std::cerr << "Error parsing tile " << key << ": " << e.what() << std::endl;
            throw;
        }
    }

public:

    std::map<unsigned int, TileConfig> config_map;

    static TileRegistry fromJson() {

        TileRegistry registry;

        // Reads the Tile Configuration JSON file
        std::string config_path = "assets/tile_registry.json"; 
        json data = TileRegistry::openJsonFile(config_path);

        // Validation - must contain 'tiles' dict in root
        TileRegistry::validateJsonData(data);

        std::map<int, TileConfig> config_map;
        for (auto& [key, value] : data["tiles"].items()) {

            TileConfig aux = TileRegistry::configFromJson(
                key, value, data
            );

            registry.config_map.emplace(
                aux.id, aux
            );
        }

        return registry;
    }
};

#endif

// TODO - implement better class organization in a new .cpp file
