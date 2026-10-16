#include "persona_repository.h"
#include "io/resource_loader.h"
#include "nlohmann/json.hpp"
#include <iostream>
#include <vector>

using json = nlohmann::json;

namespace {
constexpr const char *PERSONAS_FILEPATH = ":/resources/personas.json";
}

bool PersonaRepository::load()
{
    const std::vector<std::uint8_t> bytes = resource_loader::load_resource_bytes(PERSONAS_FILEPATH);
    if (bytes.empty())
        return false;

    try {
        json data = json::parse(bytes.begin(), bytes.end());
        personas_.clear();

        for (const auto &persona : data) {
            std::uint8_t id = persona["id"].get<std::uint8_t>();
            std::uint16_t flag = 1;
            std::string name = persona["name"].get<std::string>();
            std::uint8_t arcana = persona["arcana_id"].get<std::uint8_t>();
            std::uint8_t level = persona["base_level"].get<std::uint8_t>();
            std::uint8_t base_level = persona["base_level"].get<std::uint8_t>();
            std::uint32_t total_xp = 0;
            std::array<std::uint16_t, 8> skills = {0};
            for (std::size_t i = 0; i < 8; ++i) {
                std::string key = "skill" + std::to_string(i + 1);
                skills[i] = persona[key].get<std::uint16_t>();
            }
            std::uint8_t st = persona["st"].get<std::uint8_t>();
            std::uint8_t ma = persona["ma"].get<std::uint8_t>();
            std::uint8_t en = persona["en"].get<std::uint8_t>();
            std::uint8_t ag = persona["ag"].get<std::uint8_t>();
            std::uint8_t lu = persona["lu"].get<std::uint8_t>();
            bool is_hidden = persona["is_hidden"].get<bool>();

            personas_.emplace_back(id,
                                   flag,
                                   name,
                                   arcana,
                                   level,
                                   base_level,
                                   total_xp,
                                   skills,
                                   st,
                                   ma,
                                   en,
                                   ag,
                                   lu,
                                   is_hidden);
        }
        return true;
    } catch (const json::exception &e) {
        std::cerr << e.what() << '\n';
        return false;
    }
}

const Persona &PersonaRepository::get_by_id(std::uint16_t id) const
{
    return personas_[id];
}

std::vector<const Persona *> PersonaRepository::get_obtainable_personas() const
{
    std::vector<const Persona *> personas;
    const size_t compendium_size = 175;
    personas.reserve(compendium_size);
    for (size_t i = 0; i < compendium_size; ++i) {
        personas.push_back(&personas_[i]);
    }
    return personas;
}
