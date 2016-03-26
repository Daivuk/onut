#pragma once
// STL
#include <cinttypes>
#include <string>
#include <vector>

namespace onut
{
    std::string findFile(const std::string& name, const std::string& lookIn = ".", bool deepSearch = true);
    std::string getPath(const std::string& filename);
    std::string getFilename(const std::string& path);
    std::string getExtension(const std::string& filename);
    std::string makeRelativePath(const std::string& path, const std::string& relativeTo);
    std::vector<uint8_t> getFileData(const std::string& filename);
    bool fileExists(const std::string& filename);
}
