#ifndef PERSONA_REPOSITORY_H
#define PERSONA_REPOSITORY_H

#include "models/persona.h"
#include <vector>

class PersonaRepository
{
public:
    static PersonaRepository &instance()
    {
        static PersonaRepository persona_db;
        return persona_db;
    }
    bool load();
    const Persona& get_by_id(std::uint16_t id) const;
    std::vector<const Persona*> get_obtainable_personas() const;

private:
    PersonaRepository() = default;
    std::vector<Persona> personas_;
};

#endif // PERSONA_REPOSITORY_H
