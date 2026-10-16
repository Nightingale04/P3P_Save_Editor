#ifndef ITEM_REPOSITORY_H
#define ITEM_REPOSITORY_H
#include "models/item.h"
#include <vector>

class ItemRepository
{
public:
    static ItemRepository &instance()
    {
        static ItemRepository item_db;
        return item_db;
    }
    bool load();
    const Item& get_by_id(std::uint16_t id) const;
    std::vector<const Item *> get_by_type(ItemType type) const;
    std::vector<const Item *> get_debug() const;

private:
    ItemRepository() = default;
    std::vector<Item> items_;
};

#endif // ITEM_REPOSITORY_H
