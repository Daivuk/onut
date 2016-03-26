#pragma once
// STL
#include <string>
#include <vector>

namespace onut
{
    std::wstring utf8ToUtf16(const std::string& utf8);
    std::string utf16ToUtf8(const std::wstring& utf16);
    std::vector<std::string> splitString(const std::string& in_string, char in_delimiter);
    std::vector<std::string> splitString(const std::string& in_string, const std::string& in_delimiters);
    std::string removeChars(const std::string& str, const std::string& charsToRemove);
    std::string toUpper(const std::string& str);
    std::string toLower(const std::string& str);
}
