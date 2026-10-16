#ifndef SKILL_REPOSITORY_H
#define SKILL_REPOSITORY_H

#include "models/skill.h"
#include <vector>

class SkillRepository
{
public:
    static SkillRepository &instance()
    {
        static SkillRepository skill_db;
        return skill_db;
    }
    bool load();
    const Skill& get_by_id(std::uint16_t id) const;
    std::vector<const Skill*> get_obtainable_skills() const;
private:
    SkillRepository() = default;
    std::vector<Skill> skills_;
};

#endif // SKILL_REPOSITORY_H
