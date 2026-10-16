#ifndef ARCANA_H
#define ARCANA_H

#include <cstdint>
#include <string>

struct Arcana
{
    std::uint8_t id = 0;
    std::string name;
    Arcana(std::uint8_t p_id, std::string p_name)
        : id(p_id)
        , name(std::move(p_name))
    {}
};

#endif // ARCANA_H
