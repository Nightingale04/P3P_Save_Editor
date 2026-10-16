#ifndef SAVE_FILE_HANDLER_H
#define SAVE_FILE_HANDLER_H
#include "models/save_data.h"
#include <optional>
#include <filesystem>

namespace save_file_handler{
    std::optional<SaveData> read_save_file(const std::filesystem::path &filepath);
    bool write_save_file(const SaveData &data);
}

#endif // SAVE_FILE_HANDLER_H
