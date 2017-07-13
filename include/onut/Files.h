#ifndef FILES_H_INCLUDED
#define FILES_H_INCLUDED

// STL
#include <cinttypes>
#include <string>
#include <vector>

namespace onut
{
    struct FileType
    {
        std::string typeName;
        std::string extension;
    };

    using FileTypes = std::vector<FileType>;

    std::string findFile(const std::string& name, const std::string& lookIn = ".", bool deepSearch = true);
    std::vector<std::string> findAllFiles(const std::string& lookIn = ".", const std::string& extension = "*", bool deepSearch = true);
    std::string getPath(const std::string& filename);
    std::string getFilename(const std::string& path);
    std::string getFilenameWithoutExtension(const std::string& path);
    std::string getExtension(const std::string& filename);
    std::string makeRelativePath(const std::string& path, const std::string& relativeTo);
    std::vector<uint8_t> getFileData(const std::string& filename);
    bool fileExists(const std::string& filename);
    std::string showOpenDialog(const std::string& caption, const FileTypes& extensions, const std::string& defaultFilename = "");
    std::string showSaveAsDialog(const std::string& caption, const FileTypes& extensions, const std::string& defaultFilename = "");
    std::string showOpenFolderDialog(const std::string& caption, const std::string& defaultPath = "");
    bool createFolder(const std::string& fullPath);
    bool copyFile(const std::string& from, const std::string& to);
    bool createTextFile(const std::string& path, const std::string& content);
    void showInExplorer(const std::string& path);

    enum class MessageBoxLevel
    {
        Info,
        Warning,
        Error,
        Question
    };

    enum class MessageBoxType
    {
        Ok,
        OkCancel,
        YesNo,
        YesNoCancel
    };

    enum class MessageBoxReturn : int
    {
        CancelNo = 0,
        OkYes = 1,
        No = 2
    };

    MessageBoxReturn showMessageBox(const std::string& title, const std::string& message, MessageBoxType type = MessageBoxType::Ok, MessageBoxLevel level = MessageBoxLevel::Info);
}

#endif
