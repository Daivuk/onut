#pragma once
#include <string>
#include <vector>

namespace onut
{
    std::wstring                utf8ToUtf16(const std::string& utf8);
    std::string                 utf16ToUtf8(const std::wstring& utf16);

    std::vector<std::string>    splitString(const std::string& in_string, char in_delimiter);
    std::vector<std::string>    splitString(const std::string& in_string, const std::string& in_delimiters);

    template<bool TuseAssert = true>
    std::string                 findFile(const std::string& name, const std::string& lookIn = ".", bool deepSearch = true);
    std::string                 getPath(const std::string& filename);

    // Once supported, use constexpr here
    int                         hash(const char* pStr);
}
