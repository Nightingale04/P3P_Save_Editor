#include "arcana_repository.h"
#include "io/resource_loader.h"
#include <cstdint>
#include <iostream>
#include <nlohmann/json.hpp>
#include <string>

using json = nlohmann::json;

namespace {
constexpr const char *ARCANAS_FILEPATH = ":/resources/arcanas.json";
}

bool ArcanaRepository::load()
{
    const std::vector<std::uint8_t> bytes = resource_loader::load_resource_bytes(ARCANAS_FILEPATH);
    if (bytes.empty())
        return false;

    try {
        json data = json::parse(bytes.begin(), bytes.end());
        arcanas_.clear();

        for (const auto &arcana : data) {
            std::uint8_t id = arcana["id"].get<std::uint8_t>();
            std::string name = arcana["name"].get<std::string>();

            arcanas_.emplace_back(id, name);
        }
        return true;
    } catch (const json::exception &e) {
        std::cerr << e.what() << '\n';
        return false;
    }
}

const Arcana& ArcanaRepository::get_by_id(std::uint8_t id) const
{
    return arcanas_[id];
}