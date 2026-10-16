#include "item_repository.h"
#include "io/resource_loader.h"
#include <cstdint>
#include <iostream>
#include <nlohmann/json.hpp>
#include <string>

using json = nlohmann::json;

namespace {
constexpr const char *ITEMS_FILEPATH = ":/resources/items.json";
}

bool ItemRepository::load()
{
    const std::vector<std::uint8_t> bytes = resource_loader::load_resource_bytes(ITEMS_FILEPATH);
    if (bytes.empty())
        return false;

    try {
        json data = json::parse(bytes.begin(), bytes.end());
        items_.clear();

        for (const auto &item : data) {
            std::uint16_t id = item["id"].get<std::uint16_t>();
            std::string name = item["name"].get<std::string>();
            std::string description = item["description"].get<std::string>();
            ItemType type = static_cast<ItemType>(item["type"].get<std::uint8_t>());
            bool is_debug = item["is_debug"].get<bool>();

            items_.emplace_back(id, name, description, type, is_debug);
        }
        return true;
    } catch (const json::exception &e) {
        std::cerr << e.what() << '\n';
        return false;
    }
}

const Item& ItemRepository::get_by_id(std::uint16_t id) const
{
    return items_[id];
}

std::vector<const Item*> ItemRepository::get_by_type(ItemType type) const
{
    std::vector<const Item*> items;
    for (const Item &item : items_) {
        if (item.type == type && !item.is_debug)
            items.push_back(&item);
    }
    return items;
}

std::vector<const Item*> ItemRepository::get_debug()const{
    std::vector<const Item*> items;
    for (const Item &item : items_) {
        if (item.type != ItemType::Placeholder && item.is_debug)
            items.push_back(&item);
    }
    return items;
}