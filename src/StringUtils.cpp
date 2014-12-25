#include <codecvt>
#include <cassert>
#include "dirent.h"
#include "StringUtils.h"

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

    int hash(const char* pStr)
    {
        int hash = 0;
        static const int offset = 'a' - 1;
        while (pStr)
        {
            hash = hash << 1 | (*pStr - offset);
            ++pStr;
        }
        return hash;
    }

    template<>
    std::string findFile<true>(const std::string& name, const std::string& lookIn)
    {
        DIR *dir;
        struct dirent *ent;
        if ((dir = opendir(lookIn.c_str())) != NULL)
        {
            while ((ent = readdir(dir)) != NULL)
            {
                if (!strcmp(ent->d_name, "."))
                {
                    continue;
                }
                else if (!strcmp(ent->d_name, ".."))
                {
                    continue;
                }

                if (name == ent->d_name)
                {
                    auto ret = lookIn + "/" + ent->d_name;
                    closedir(dir);
                    return ret;
                }

                if (ent->d_type & DT_DIR)
                {
                    auto ret = findFile<true>(name, lookIn + "/" + ent->d_name);
                    if (!ret.empty())
                    {
                        closedir(dir);
                        return ret;
                    }
                }
            }
            closedir(dir);
        }
        else
        {
            assert(false);
        }

        return "";
    }

    template<>
    std::string findFile<false>(const std::string& name, const std::string& lookIn)
    {
        DIR *dir;
        struct dirent *ent;
        if ((dir = opendir(lookIn.c_str())) != NULL)
        {
            while ((ent = readdir(dir)) != NULL)
            {
                if (!strcmp(ent->d_name, "."))
                {
                    continue;
                }
                else if (!strcmp(ent->d_name, ".."))
                {
                    continue;
                }

                if (!_stricmp(name.c_str(), ent->d_name))
                {
                    auto ret = lookIn + "/" + ent->d_name;
                    closedir(dir);
                    return ret;
                }

                if (ent->d_type & DT_DIR)
                {
                    auto ret = findFile<false>(name, lookIn + "/" + ent->d_name);
                    if (!ret.empty())
                    {
                        closedir(dir);
                        return ret;
                    }
                }
            }
            closedir(dir);
        }

        return "";
    }
}
