#include "save_file_handler.h"
#include "data/persona_repository.h"
#include "models/save_data.h"
#include <cstdint>
#include <cstring>
#include <fstream>

namespace {
constexpr std::size_t OFFSET_MC_SURNAME = 0x0C;
constexpr std::size_t OFFSET_MC_FIRSTNAME = 0x1E;
constexpr std::size_t OFFSET_MC_LEVEL = 0x3E;
constexpr std::size_t OFFSET_SOCIAL_STATS = 0x74;
constexpr std::size_t OFFSET_MC_XP = 0x7C;
constexpr std::size_t OFFSET_INVENTORY = 0x4F0;
constexpr std::size_t OFFSET_SLOTS = 0x10CC;
constexpr std::size_t OFFSET_COMPENDIUM = 0x1378;
constexpr std::size_t OFFSET_YEN = 0x4A14;

constexpr std::size_t PADDING_PERSONA = 20;
constexpr std::size_t SIZE_PERSONA = 52;
constexpr std::size_t SIZE_INVENTORY = 2048;

template<typename T>
[[nodiscard]] inline T read_bytes_at(const std::uint8_t *ptr, std::size_t offset) noexcept
{
    T val;
    std::memcpy(&val, ptr + offset, sizeof(T));
    return val;
}

std::string read_ascii_string(const std::uint8_t *buffer)
{
    std::string text;
    for (std::size_t i = 0; i < 9 && buffer[i] != '\0'; ++i) {
        text += static_cast<char>(buffer[i]);
    }
    return text;
}

std::string read_js_string(const std::uint8_t *buffer)
{
    std::string text = "";
    for (int i = 0; i < 9; ++i) {
        const std::uint16_t num = read_bytes_at<std::uint16_t>(buffer, i * sizeof(std::uint16_t));
        const char ch = static_cast<char>(((num & 0xFF00) >> 8) - 0x60);
        if (ch != ' ' && ch != static_cast<char>(0xA0) && ch != '\0') {
            text += ch;
        }
    }
    return text;
}

static Persona read_persona_from_bytes(const std::uint8_t *persona_ptr)
{
    const std::uint16_t flag = read_bytes_at<std::uint16_t>(persona_ptr, 0);
    const std::uint16_t id = read_bytes_at<std::uint16_t>(persona_ptr, 2);
    const std::uint8_t level = persona_ptr[4];
    const std::uint32_t total_xp = read_bytes_at<std::uint32_t>(persona_ptr, 8);

    const Persona base_persona = PersonaRepository::instance().get_by_id(id);
    std::uint8_t base_level = base_persona.base_level;
    std::array<std::uint16_t, 8> skills;
    std::memcpy(skills.data(), persona_ptr + 12, sizeof(skills));

    const std::uint8_t st = persona_ptr[28];
    const std::uint8_t ma = persona_ptr[29];
    const std::uint8_t en = persona_ptr[30];
    const std::uint8_t ag = persona_ptr[31];
    const std::uint8_t lu = persona_ptr[32];

    return Persona(id,
                   flag,
                   base_persona.name,
                   base_persona.arcana,
                   level,
                   base_level,
                   total_xp,
                   skills,
                   st,
                   ma,
                   en,
                   ag,
                   lu,
                   base_persona.is_hidden);
}
} // namespace

namespace save_file_handler {
std::optional<SaveData> read_save_file(const std::filesystem::path &filepath)
{
    std::ifstream file(filepath, std::ios::binary | std::ios::ate);
    if (!file.is_open()) {
        return std::nullopt;
    }
    SaveData save;

    save.file_path = filepath;
    const std::size_t file_length = static_cast<size_t>(file.tellg());
    save.raw_buffer.resize(static_cast<std::size_t>(file_length));
    file.seekg(0, std::ios::beg);
    file.read(reinterpret_cast<char *>(save.raw_buffer.data()), file_length);

    const uint8_t *raw = save.raw_buffer.data();
    save.firstname = (raw[OFFSET_MC_FIRSTNAME] == 0x80)
                         ? read_js_string(raw + OFFSET_MC_FIRSTNAME)
                         : read_ascii_string(raw + OFFSET_MC_FIRSTNAME);

    save.surname = (raw[OFFSET_MC_SURNAME] == 0x80) ? read_js_string(raw + OFFSET_MC_SURNAME)
                                                    : read_ascii_string(raw + OFFSET_MC_SURNAME);
    save.mc_level = raw[OFFSET_MC_LEVEL];

    save.academics = read_bytes_at<std::uint16_t>(raw + OFFSET_SOCIAL_STATS, 0);
    save.charm = read_bytes_at<std::uint16_t>(raw + OFFSET_SOCIAL_STATS, 2);
    save.courage = read_bytes_at<std::uint16_t>(raw + OFFSET_SOCIAL_STATS, 4);

    save.mc_xp = read_bytes_at<std::uint32_t>(raw + OFFSET_MC_XP, 0);
    save.inventory.assign(raw + OFFSET_INVENTORY, raw + OFFSET_INVENTORY + SIZE_INVENTORY);

    save.active_personas.clear();
    for (std::size_t i = 0; i < 12; ++i) {
        const std::size_t slot_offset = OFFSET_SLOTS + (i * SIZE_PERSONA);
        const std::uint8_t *slot_persona = raw + slot_offset;

        const std::uint16_t flag = read_bytes_at<std::uint16_t>(slot_persona, 0);
        const std::uint16_t id = read_bytes_at<std::uint16_t>(slot_persona, 2);

        if (flag == 0 || id == 0) {
            break;
        }
        save.active_personas.push_back(read_persona_from_bytes(slot_persona));
    }

    save.compendium.clear();
    for (std::size_t i = 0; i < 174; ++i) {
        const std::size_t slot_offset = OFFSET_COMPENDIUM + (i * SIZE_PERSONA);
        const std::uint8_t *slot_persona = raw + slot_offset;
        save.compendium.push_back(read_persona_from_bytes(slot_persona));
    }
    save.yen = read_bytes_at<std::uint32_t>(raw + OFFSET_YEN, 0);
    return save;
}
} // namespace save_file_handler
