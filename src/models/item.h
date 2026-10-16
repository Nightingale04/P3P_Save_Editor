#ifndef ITEM_H
#define ITEM_H

#include <cstdint>
#include <string>

enum class ItemType : std::uint8_t {
    Placeholder = 0,
    Weapon = 1,
    Armor = 2,
    Boot = 3,
    Accessory = 4,
    Consumable = 5,
    Fusion_Item = 6,
    Incense_Card = 7,
    Gift = 8,
    Gemstone = 9,
    Quest = 10,
    Valuable = 11,
    Key = 12,
    Fusion_Spell = 13,
    Skill_Card = 14,
};

struct Item
{
    std::uint16_t id = 0;
    std::string name;
    std::string description;
    ItemType type = ItemType::Placeholder;
    bool is_debug = false;
    Item(std::uint16_t p_id,
         std::string p_name,
         std::string p_description,
         ItemType p_type,
         bool p_is_debug)
        : id(p_id)
        , name(std::move(p_name))
        , description(std::move(p_description))
        , type(p_type)
        , is_debug(p_is_debug)
    {}
    std::string get_type_name() const;
};

#endif // ITEM_H
