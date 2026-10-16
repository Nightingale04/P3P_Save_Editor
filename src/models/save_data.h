#ifndef SAVEDATA_H
#define SAVEDATA_H

#include "persona.h"
#include <cstdint>
#include <filesystem>
#include <string>
#include <vector>

struct SaveData
{
    std::string surname;
    std::string firstname;
    std::uint32_t yen = 0;
    std::uint8_t mc_level = 1;
    std::uint32_t mc_xp = 0;

    std::uint16_t academics = 0;
    std::uint16_t charm = 0;
    std::uint16_t courage = 0;

    std::vector<Persona> active_personas;
    std::vector<uint8_t> inventory;
    std::vector<Persona> compendium;

    bool is_pc_save = false;
    std::filesystem::path file_path;
    std::vector<std::uint8_t> raw_buffer;
};

#endif // SAVEDATA_H
