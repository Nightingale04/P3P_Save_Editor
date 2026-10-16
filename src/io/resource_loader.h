#ifndef RESOURCE_LOADER_H
#define RESOURCE_LOADER_H

#include <cstdint>
#include <vector>

namespace resource_loader {
std::vector<std::uint8_t> load_resource_bytes(const char *path);
}

#endif // RESOURCE_LOADER_H
