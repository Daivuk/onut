// Onut
#include <onut/Files.h>
#include <onut/Strings.h>

// STL
#include <algorithm>
#include <fstream>
#include <sstream>

// Third party
#include <dirent/dirent.h>

namespace onut
{
    std::string findFile(const std::string& name, const std::string& lookIn, bool deepSearch)
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

                if (ent->d_type & DT_DIR && deepSearch)
                {
                    auto ret = findFile(name, lookIn + "/" + ent->d_name);
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

    std::string getPath(const std::string& filename)
    {
        return filename.substr(0, filename.find_last_of("\\/"));
    }

    std::string getFilename(const std::string& path)
    {
        auto pos = path.find_last_of("\\/");
        if (pos == std::string::npos) return path;
        return path.substr(pos + 1);
    }

    std::string getExtension(const std::string& filename)
    {
        auto pos = filename.find_last_of('.');
        if (pos == std::string::npos) return "";
        return toUpper(filename.substr(pos + 1));
    }

    std::string makeRelativePath(const std::string& in_path, const std::string& in_relativeTo)
    {
        auto path = in_path;
        std::replace(path.begin(), path.end(), '\\', '/');
        auto pathSplit = splitString(path, '/');

        auto relativeTo = in_relativeTo;
        std::replace(relativeTo.begin(), relativeTo.end(), '\\', '/');
        auto relativeSplit = splitString(relativeTo, '/');

        while (pathSplit.size() && relativeSplit.size() && pathSplit.front() == relativeSplit.front())
        {
            pathSplit.erase(pathSplit.begin());
            relativeSplit.erase(relativeSplit.begin());
        }

        std::stringstream ss;
        bool bFirst = true;
        for (auto& folder : relativeSplit)
        {
            if (!bFirst) ss << "/";
            bFirst = false;
            ss << "..";
        }
        for (auto& folder : pathSplit)
        {
            if (!bFirst) ss << "/";
            bFirst = false;
            ss << folder;
        }
        return std::move(ss.str());
    }

    std::vector<uint8_t> getFileData(const std::string& filename)
    {
        std::ifstream file(filename, std::ios::binary);
        std::vector<uint8_t> data = {std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>()};
        return std::move(data);
    }

    bool fileExists(const std::string& filename)
    {
        WIN32_FIND_DATAA FindFileData;
        HANDLE handle = FindFirstFileA(filename.c_str(), &FindFileData);
        bool found = handle != INVALID_HANDLE_VALUE;
        if (found)
        {
            //FindClose(&handle); this will crash
            FindClose(handle);
        }
        return found;
    }
}
