#include "item.h"

std::string Item::get_type_name() const
{
    switch (type) {
    case ItemType::Weapon:
        return "Weapon";
    case ItemType::Armor:
        return "Armor";
    case ItemType::Boot:
        return "Boot";
    case ItemType::Accessory:
        return "Accessory";
    case ItemType::Consumable:
        return "Consumable";
    case ItemType::Fusion_Item:
        return "Fusion Item";
    case ItemType::Incense_Card:
        return "Incense Card";
    case ItemType::Gift:
        return "Gift";
    case ItemType::Gemstone:
        return "Gemstone";
    case ItemType::Quest:
        return "Quest Item";
    case ItemType::Valuable:
        return "Valuable";
    case ItemType::Key:
        return "Key Item";
    case ItemType::Fusion_Spell:
        return "Fusion Spell";
    case ItemType::Skill_Card:
        return "Skill Card";
    case ItemType::Placeholder:
        return "Placeholder";
    default:
        return "Other";
    }
}