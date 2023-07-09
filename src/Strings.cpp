// Onut
#include <onut/Strings.h>

// STL
#include <algorithm>
#if defined(WIN32)
#include <codecvt>
#endif
#include <regex>

namespace onut
{
#if defined(WIN32)
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>, wchar_t> g_stringConverter;

    std::wstring utf8ToUtf16(const std::string& utf8)
    {
        return g_stringConverter.from_bytes(utf8);
    }

    std::string utf16ToUtf8(const std::wstring& utf16)
    {
        return g_stringConverter.to_bytes(utf16);
    }
#endif

    std::vector<std::string> splitString(const std::string& in_string, char in_delimiter, bool in_removeEmptyElements)
    {
        std::vector<std::string> elems;
        unsigned int start = 0;
        unsigned int end = 0;
        for (; end < in_string.length(); ++end)
        {
            if (in_string[end] == in_delimiter)
            {
                if (end - start || !in_removeEmptyElements)
                {
                    elems.push_back(in_string.substr(start, end - start));
                }
                start = end + 1;
            }
        }
        if (!in_removeEmptyElements && start == in_string.length())
        {
            elems.push_back("");
        }
        if (start < in_string.length())
        {
            if (end - start || !in_removeEmptyElements)
            {
                elems.push_back(in_string.substr(start, end - start));
            }
        }
        return elems;
    }

    std::vector<std::string> splitString(const std::string& in_string, const std::string& in_delimeters)
    {
        std::vector<std::string> elems;
        unsigned int start = 0;
        unsigned int end = 0;
        for (; end < in_string.length(); ++end)
        {
            for (auto c : in_delimeters)
            {
                if (in_string[end] == c)
                {
                    if (end - start)
                    {
                        elems.push_back(in_string.substr(start, end - start));
                    }
                    start = end + 1;
                }
            }
        }
        if (start == in_string.length())
        {
            elems.push_back("");
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

    std::string trim(const std::string& str)
    {
        auto s = str.find_first_not_of(" \t\n\r");
        if (s == std::string::npos)
        {
            return "";
        }
        std::string ret;
        if (s > 0)
        {
            ret = str.substr(s);
        }
        else
        {
            ret = str;
        }
        s = ret.find_last_not_of(" \t\n\r");
        return ret.substr(0, s + 1);
    }

    std::string join(const std::vector<std::string>& strings, const std::string& separator)
    {
        if (strings.empty()) return "";
        std::string ret = strings[0];
        for (size_t i = 1, len = strings.size(); i < len; ++i)
        {
            ret += separator + strings[i];
        }
        return ret;
    }

    size_t utf8Length(const std::string& str)
    {
        int len = 0;
        auto s = str.data();
        while (*s) len += (*s++ & 0xc0) != 0x80;
        return len;
    }

    size_t utf8Pos(const std::string& str, size_t pos)
    {
        auto s = str.data();
        while (*s && pos)
        {
            if (*s & 0x80)
            {
                if ((*s & 0xE0) == 0xC0)
                {
                    ++s;
                }
                else if ((*s & 0xF0) == 0xE0)
                {
                    s += 2;
                }
                else
                {
                    s += 3;
                }
            }
            --pos;
            ++s;
        }

        return s - str.data();
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

    void stripOutComments(std::string& source)
    {
        // Group
        std::smatch m1;
        std::smatch m2;
        size_t offset = 0;
        while (std::regex_search(source.cbegin() + offset, source.cend(), m1, std::regex("\\/\\*")))
        {
            offset += m1.position();
            if (std::regex_search(source.cbegin() + offset, source.cend(), m2, std::regex("\\*\\/")))
            {
                source.erase(source.begin() + offset,
                             source.begin() + offset + m2.position() + m2.length());
            }
            else break;
        }

        // Single
        offset = 0;
        while (std::regex_search(source.cbegin() + offset, source.cend(), m1, std::regex("\\/\\/.*")))
        {
            offset += m1.position();
            source.erase(source.begin() + offset,
                         source.begin() + offset + m1.length());
        }
    }

    std::string stripOutComments(const std::string& source)
    {
        auto ret = source;
        stripOutComments(ret);
        return std::move(ret);
    }

    void stripOutAllButComments(std::string& source)
    {
        source = stripOutAllButComments((const std::string&)source);
    }

    std::string stripOutAllButComments(const std::string& source)
    {
        std::string ret;

        int state = 0;

        for (auto c : source)
        {
            switch (state)
            {
                case 0:
                    if (c == '/') state = 1;
                    break;
                case 1:
                    if (c == '/') state = 2;
                    else if (c == '*') state = 3;
                    else state = 0;
                    break;
                case 2:
                    if (c == '\n') state = 0;
                    else ret += c;
                    break;
                case 3:
                    if (c == '*') state = 4;
                    else ret += c;
                    break;
                case 4:
                    if (c == '/') state = 0;
                    else
                    {
                        ret += c;
                        state = 3;
                    }
                    break;
            }
        }

        return std::move(ret);
    }

    void replace(std::string& source, const std::string& reg, const std::string& substitution)
    {
        source = std::regex_replace(source, std::regex(reg), substitution);
    }

    std::string replace(const std::string& source, const std::string& reg, const std::string& substitution)
    {
        auto ret = source;
        replace(ret, reg, substitution);
        return std::move(ret);
    }
}
