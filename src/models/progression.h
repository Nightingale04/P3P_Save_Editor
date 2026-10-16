#ifndef PROGRESSION_H
#define PROGRESSION_H

#include <array>
#include <cstdint>

namespace progression {

struct Rank
{
    std::uint16_t threshold;
    const char *name;
};

enum class SocialStat { Academic, Charm, Courage };

std::uint8_t get_social_rank(SocialStat stat, std::uint16_t points);
const std::array<Rank, 6> &get_social_table(SocialStat stat);

std::uint8_t get_mc_level(std::uint32_t xp);
std::uint32_t get_mc_xp(std::uint8_t level);

} // namespace progression

#endif // PROGRESSION_H
