#include "skill_repository.h"
#include "io/resource_loader.h"
#include <iostream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

namespace {
constexpr const char *SKILLS_FILEPATH = ":/resources/skills.json";
}

bool SkillRepository::load()
{
    const std::vector<std::uint8_t> bytes = resource_loader::load_resource_bytes(SKILLS_FILEPATH);
    if (bytes.empty())
        return false;

    try {
        json data = json::parse(bytes.begin(), bytes.end());
        skills_.clear();

        for (const auto &skill : data) {
            std::uint16_t id = skill["id"].get<std::uint16_t>();
            std::string name = skill["name"].get<std::string>();
            std::string description = skill["description"].get<std::string>();
            bool is_hidden = skill["is_hidden"].get<bool>();

            skills_.emplace_back(id, name, description, is_hidden);
        }
        return true;
    } catch (const json::exception &e) {
        std::cerr << e.what() << '\n';
        return false;
    }
}

const Skill& SkillRepository::get_by_id(std::uint16_t id) const{
    return skills_[id];
}

std::vector<const Skill *> SkillRepository::get_obtainable_skills() const
{
    std::vector<const Skill *> skills;
    for (const Skill &skill : skills_) {
        if (!skill.is_hidden)
            skills.push_back(&skill);
    }
    return skills;
}
