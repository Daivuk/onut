// Onut
#include <onut/Files.h>
#include <onut/Strings.h>
#include <onut/Window.h>

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

    std::string getFilenameWithoutExtension(const std::string& path)
    {
        auto filename = getFilename(path);
        auto pos = filename.find_last_of('.');
        if (pos == std::string::npos) return filename;
        return filename.substr(0, pos);
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

#if defined(WIN32)
    std::string showOpenDialog(const std::string& caption, const FileTypes& extensions, const std::string& defaultFilename)
    {
        auto windowHandle = oWindow->getHandle();
        char szFileName[MAX_PATH] = {0};
        memcpy(szFileName, defaultFilename.c_str(), std::min(defaultFilename.size(), static_cast<size_t>(MAX_PATH - 1)));

        OPENFILENAMEA ofn = {0};
        ofn.lStructSize = sizeof(OPENFILENAMEA);
        ofn.hwndOwner = windowHandle;
        ofn.lStructSize = sizeof(ofn);
        ofn.lpstrFile = szFileName;
        ofn.nMaxFile = MAX_PATH;
        ofn.Flags = OFN_EXPLORER | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR;

        size_t totalCount = 0;
        for (auto& fileType : extensions)
        {
            totalCount += fileType.typeName.size();
            totalCount += fileType.extension.size();
        }

        char* szFilters = new char[21 + totalCount * 3 + 9 * extensions.size()];

        size_t currentOffset = 0;
        for (auto& fileType : extensions)
        {
            memcpy(szFilters + currentOffset, fileType.typeName.c_str(), fileType.typeName.size());
            currentOffset += fileType.typeName.size();
            memcpy(szFilters + currentOffset, " (*.", 4);
            currentOffset += 4;
            memcpy(szFilters + currentOffset, fileType.extension.c_str(), fileType.extension.size());
            currentOffset += fileType.extension.size();
            memcpy(szFilters + currentOffset, ")\0*.", 4);
            currentOffset += 4;
            memcpy(szFilters + currentOffset, fileType.extension.c_str(), fileType.extension.size());
            currentOffset += fileType.extension.size();
            memcpy(szFilters + currentOffset, "\0", 1);
            currentOffset += 1;
        }
        memcpy(szFilters + currentOffset, "All Files (*.*)\0*.*\0\0", 21);

        ofn.lpstrFilter = szFilters;
        std::string defaultExtension = extensions[0].extension;
        ofn.lpstrDefExt = defaultExtension.c_str();
        ofn.lpstrTitle = caption.c_str();

        // PNG Files (*.PNG)\0*.PNG\0All Files (*.*)\0*.*\0

        GetOpenFileNameA(&ofn);

        delete[] szFilters;

        return ofn.lpstrFile;
    }

    std::string showSaveAsDialog(const std::string& caption, const FileTypes& extensions, const std::string& defaultFilename)
    {
        auto windowHandle = oWindow->getHandle();
        char szFileName[MAX_PATH] = {0};
        memcpy(szFileName, defaultFilename.c_str(), std::min(defaultFilename.size(), static_cast<size_t>(MAX_PATH - 1)));

        OPENFILENAMEA ofn = {0};
        ofn.lStructSize = sizeof(OPENFILENAMEA);
        ofn.hwndOwner = windowHandle;
        ofn.lStructSize = sizeof(ofn);
        ofn.lpstrFile = szFileName;
        ofn.nMaxFile = MAX_PATH;
        ofn.Flags = OFN_EXPLORER | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR;

        size_t totalCount = 0;
        for (auto& fileType : extensions)
        {
            totalCount += fileType.typeName.size();
            totalCount += fileType.extension.size();
        }

        char* szFilters = new char[21 + totalCount * 3 + 9 * extensions.size()];

        size_t currentOffset = 0;
        for (auto& fileType : extensions)
        {
            memcpy(szFilters + currentOffset, fileType.typeName.c_str(), fileType.typeName.size());
            currentOffset += fileType.typeName.size();
            memcpy(szFilters + currentOffset, " (*.", 4);
            currentOffset += 4;
            memcpy(szFilters + currentOffset, fileType.extension.c_str(), fileType.extension.size());
            currentOffset += fileType.extension.size();
            memcpy(szFilters + currentOffset, ")\0*.", 4);
            currentOffset += 4;
            memcpy(szFilters + currentOffset, fileType.extension.c_str(), fileType.extension.size());
            currentOffset += fileType.extension.size();
            memcpy(szFilters + currentOffset, "\0", 1);
            currentOffset += 1;
        }
        memcpy(szFilters + currentOffset, "All Files (*.*)\0*.*\0\0", 21);

        ofn.lpstrFilter = szFilters;
        std::string defaultExtension = extensions[0].extension;
        ofn.lpstrDefExt = defaultExtension.c_str();
        ofn.lpstrTitle = caption.c_str();

        // PNG Files (*.PNG)\0*.PNG\0All Files (*.*)\0*.*\0

        GetSaveFileNameA(&ofn);

        delete[] szFilters;

        return ofn.lpstrFile;
    }
#endif
}
