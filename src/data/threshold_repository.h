#ifndef THRESHOLD_REPOSITORY_H
#define THRESHOLD_REPOSITORY_H
#include <cstdint>
#include <unordered_map>
#include <vector>

class ThresholdRepository
{
public:
    static ThresholdRepository &instance()
    {
        static ThresholdRepository threshold_db;
        return threshold_db;
    }
    bool load();
    std::uint8_t get_level_by_xp(std::uint8_t base_level, std::uint32_t xp) const;
    std::uint32_t get_xp_by_level(std::uint8_t base_leve, std::uint8_t level) const;

private:
    ThresholdRepository() = default;
    std::unordered_map<std::uint8_t, std::vector<std::uint32_t>> xp_thresholds_;
};

#endif // THRESHOLD_REPOSITORY_H
