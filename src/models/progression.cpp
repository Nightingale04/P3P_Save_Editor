#include "progression.h"
#include <algorithm>
#include <cstdlib>

namespace {
constexpr std::array<progression::Rank, 6> ACADEMIC_RANKS = {{{0, "Slacker"},
                                                              {20, "Average"},
                                                              {55, "Above Average"},
                                                              {100, "Smart"},
                                                              {155, "Intelligent"},
                                                              {230, "Genius"}}};
constexpr std::array<progression::Rank, 6> CHARM_RANKS = {{{0, "Plain"},
                                                           {15, "Unpolished/Ingenue"},
                                                           {30, "Confident/Unique"},
                                                           {45, "Smooth/Queen Bee"},
                                                           {70, "Popular/Angelic"},
                                                           {100, "Charismatic/Bewitching"}}};
constexpr std::array<progression::Rank, 6> COURAGE_RANKS = {{{0, "Timid"},
                                                             {15, "Ordinary"},
                                                             {30, "Determined"},
                                                             {45, "Tough"},
                                                             {60, "Fearless"},
                                                             {80, "Badass"}}};

constexpr std::array<std::uint32_t, 99> MC_XP_THRESHOLD
    = {0,       20,      47,      99,      185,     312,     490,     726,     1030,   1410,
       1873,    2429,    3085,    3851,    4735,    5744,    6888,    8174,    9612,   11210,
       12975,   14917,   17043,   19363,   21885,   24616,   27566,   30742,   34154,  37810,
       41717,   45885,   50321,   55035,   60035,   65328,   70924,   76830,   83056,  89610,
       96499,   103733,  111319,  119267,  127585,  136280,  145362,  154838,  164718, 175010,
       185721,  196861,  208437,  220459,  232935,  245872,  259280,  273166,  287540, 302410,
       317783,  333669,  350075,  367011,  384485,  402504,  421078,  440214,  459922, 480210,
       501085,  522557,  544633,  567323,  590635,  614576,  639156,  664382,  690264, 716810,
       744027,  771925,  800511,  829795,  859785,  890488,  921914,  954070,  986966, 1020610,
       1055009, 1090173, 1126109, 1162827, 1200335, 1238640, 1277752, 1317678, 1358428};
} // namespace

namespace progression {

const std::array<Rank, 6> &get_social_table(SocialStat stat)
{
    switch (stat) {
    case SocialStat::Academic:
        return ACADEMIC_RANKS;
    case SocialStat::Charm:
        return CHARM_RANKS;
    case SocialStat::Courage:
        return COURAGE_RANKS;
    }
    std::abort();
}

std::uint8_t get_social_rank(SocialStat stat, std::uint16_t points)
{
    const std::array<Rank, 6> &social_table = get_social_table(stat);
    if (points >= social_table.back().threshold) {
        return static_cast<std::uint8_t>(social_table.size() - 1);
    }
    auto it_bound = std::upper_bound(social_table.begin(),
                                     social_table.end(),
                                     points,
                                     [](std::uint16_t value, const Rank &rank) {
                                         return value < rank.threshold;
                                     });
    --it_bound;
    std::uint8_t rank = static_cast<std::uint8_t>(it_bound - social_table.begin());
    return rank;
}

std::uint8_t get_mc_level(std::uint32_t xp)
{
    if (xp >= MC_XP_THRESHOLD.back()) {
        return 99;
    }
    auto it_bound = std::upper_bound(MC_XP_THRESHOLD.begin(), MC_XP_THRESHOLD.end(), xp);
    std::uint8_t level = static_cast<std::uint8_t>(it_bound - MC_XP_THRESHOLD.begin());
    return level;
}

std::uint32_t get_mc_xp(std::uint8_t level)
{
    return MC_XP_THRESHOLD[level - 1];
}
} // namespace progression