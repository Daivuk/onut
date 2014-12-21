#pragma once
#include <string>
#include <vector>

namespace onut {
    std::wstring                utf8ToUtf16(const std::string& utf8);
    std::string                 utf16ToUtf8(const std::wstring& utf16);

    std::vector<std::string>    splitString(const std::string& in_string, char in_delimiter);

    std::string                 findFile(const std::string& name, const std::string& lookIn = "assets");

    // Once supported, use constexpr here
    int                         hash(const char* pStr);
}
