#ifndef STRINGS_H_INCLUDED
#define STRINGS_H_INCLUDED

// STL
#include <string>
#include <vector>

namespace onut
{
#if defined(WIN32)
    std::wstring utf8ToUtf16(const std::string& utf8);
    std::string utf16ToUtf8(const std::wstring& utf16);
#endif
    std::vector<std::string> splitString(const std::string& in_string, char in_delimiter, bool in_removeEmptyElements = true);
    std::vector<std::string> splitString(const std::string& in_string, const std::string& in_delimiters);
    std::string removeChars(const std::string& str, const std::string& charsToRemove);
    std::string toUpper(const std::string& str);
    std::string toLower(const std::string& str);
    std::string trim(const std::string& str);

    // Regex stuff
    void stripOutComments(std::string& source);
    std::string stripOutComments(const std::string& source);
    void replace(std::string& source, const std::string& reg, const std::string& substitution);
    std::string replace(const std::string& source, const std::string& reg, const std::string& substitution);
}

#endif
