#include "threshold_repository.h"
#include "io/resource_loader.h"
#include <cstdint>
#include <iostream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

namespace {
constexpr const char *XP_THRESHOLDS_FILEPATH = ":/resources/xp_thresholds.json";
}

bool ThresholdRepository::load()
{
    const std::vector<std::uint8_t> bytes = resource_loader::load_resource_bytes(
        XP_THRESHOLDS_FILEPATH);
    if (bytes.empty())
        return false;

    try {
        json data = json::parse(bytes.begin(), bytes.end());
        xp_thresholds_.clear();

        for (const auto &[key, value] : data.items()) {
            std::uint8_t base_level = static_cast<std::uint8_t>(std::stoi(key));
            xp_thresholds_[base_level] = value.get<std::vector<std::uint32_t>>();
        }
        return true;
    } catch (const json::exception &e) {
        std::cerr << e.what() << '\n';
        return false;
    }
}

std::uint8_t ThresholdRepository::get_level_by_xp(std::uint8_t base_level, std::uint32_t xp) const
{
    const auto &xp_table = xp_thresholds_.at(base_level);
    if (xp >= xp_table.back()) {
        return 99;
    }
    auto it_bound = std::upper_bound(xp_table.begin(), xp_table.end(), xp);
    std::uint8_t level = static_cast<std::uint8_t>(it_bound - xp_table.begin());
    return level;
}

std::uint32_t ThresholdRepository::get_xp_by_level(std::uint8_t base_level, std::uint8_t level) const
{
    const std::vector<std::uint32_t> &xp_table = xp_thresholds_.at(base_level);
    return xp_table[level - 1];
}
