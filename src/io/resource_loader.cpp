#include "resource_loader.h"
#include <QFile>

namespace resource_loader {
std::vector<std::uint8_t> load_resource_bytes(const char *path)
{
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly))
        return {};
    const QByteArray b = file.readAll();
    return {b.begin(), b.end()};
}
} // namespace resource_loader