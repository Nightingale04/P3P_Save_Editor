#ifndef SKILL_H
#define SKILL_H

#include <cstdint>
#include <string>

struct Skill
{
    std::uint16_t id = 0;
    std::string name;
    std::string description;
    bool is_hidden = false;
    Skill(std::uint16_t p_id, std::string p_name, std::string p_description, bool p_is_hidden)
        : id(p_id)
        , name(std::move(p_name))
        , description(std::move(p_description))
        , is_hidden(p_is_hidden)
    {}
};
#endif // SKILL_H
