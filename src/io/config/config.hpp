#ifndef CONFIG_H
#define CONFIG_H

#include <string>
#include <json/json.hpp>
#include <stdint.h>

struct GeneralConfig {
    int threadsToSpawn;
    std::string randomSimulationDestinationDb;
    uint64_t numberOfPhotonsPerSimulation;
};


void to_json(nlohmann::json&, const GeneralConfig&);
void from_json(nlohmann::json&, GeneralConfig&);

Material materialFromLayersJSON(nlohmann::json);

#endif