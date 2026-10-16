#ifndef PERSONA_H
#define PERSONA_H

#include <array>
#include <cstdint>
#include <string>

struct Persona
{
    std::uint16_t id = 0;
    std::uint16_t flag = 1;
    std::string name;
    std::uint8_t arcana = 0;
    std::uint8_t level = 0;
    std::uint8_t base_level = 0;
    std::uint32_t total_xp = 0;
    std::array<std::uint16_t, 8> skills;
    std::uint8_t st = 0;
    std::uint8_t ma = 0;
    std::uint8_t en = 0;
    std::uint8_t ag = 0;
    std::uint8_t lu = 0;
    bool is_hidden = false;

    Persona(std::uint16_t p_id,
            std::uint16_t p_flag,
            std::string p_name,
            std::uint8_t p_arcana,
            std::uint8_t p_level,
            std::uint8_t p_base_level,
            std::uint32_t p_total_xp,
            std::array<std::uint16_t, 8> p_skills,
            std::uint8_t p_st,
            std::uint8_t p_ma,
            std::uint8_t p_en,
            std::uint8_t p_ag,
            std::uint8_t p_lu,
            bool p_is_hidden)
        : id(p_id)
        , flag(p_flag)
        , name(std::move(p_name))
        , arcana(p_arcana)
        , level(p_level)
        , base_level(p_base_level)
        , total_xp(p_total_xp)
        , skills(p_skills)
        , st(p_st)
        , ma(p_ma)
        , en(p_en)
        , ag(p_ag)
        , lu(p_lu)
        , is_hidden(p_is_hidden)
    {}
};

#endif // PERSONA_H
