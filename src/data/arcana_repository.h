#ifndef ARCANA_REPOSITORY_H
#define ARCANA_REPOSITORY_H

#include "models/arcana.h"
#include <cstdint>
#include <vector>

class ArcanaRepository
{
public:
    static ArcanaRepository &instance()
    {
        static ArcanaRepository arcana_db;
        return arcana_db;
    }

    bool load();
    const Arcana& get_by_id(std::uint8_t id) const;

private:
    ArcanaRepository() = default;
    std::vector<Arcana> arcanas_;
};

#endif // ARCANA_REPOSITORY_H
