// Onut
#include <onut/Strings.h>

// STL
#include <algorithm>
#include <codecvt>

namespace onut
{
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>, wchar_t> g_stringConverter;

    std::wstring utf8ToUtf16(const std::string& utf8)
    {
        return g_stringConverter.from_bytes(utf8);
    }

    std::string utf16ToUtf8(const std::wstring& utf16)
    {
        return g_stringConverter.to_bytes(utf16);
    }

    std::vector<std::string> splitString(const std::string& in_string, char in_delimiter)
    {
        std::vector<std::string> elems;
        unsigned int start = 0;
        unsigned int end = 0;
        for (; end < in_string.length(); ++end)
        {
            if (in_string[end] == in_delimiter)
            {
                if (end - start)
                {
                    elems.push_back(in_string.substr(start, end - start));
                }
                start = end + 1;
            }
        }
        if (start < in_string.length())
        {
            if (end - start)
            {
                elems.push_back(in_string.substr(start, end - start));
            }
        }
        return elems;
    }

    std::string removeChars(const std::string& str, const std::string& charsToRemove)
    {
        auto ret = str;
        for (decltype(charsToRemove.size()) i = 0; i < charsToRemove.size(); ++i)
        {
            ret.erase(remove(ret.begin(), ret.end(), charsToRemove[i]), ret.end());
        }
        return ret;
    }

    std::string toUpper(const std::string& str)
    {
        auto ret = str;
        std::transform(ret.begin(), ret.end(), ret.begin(), ::toupper);
        return ret;
    }

    std::string toLower(const std::string& str)
    {
        auto ret = str;
        std::transform(ret.begin(), ret.end(), ret.begin(), ::tolower);
        return ret;
    }
}
