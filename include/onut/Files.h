#ifndef FILES_H_INCLUDED
#define FILES_H_INCLUDED

// STL
#include <cinttypes>
#include <string>
#include <vector>

struct Vector2;
struct Vector3;
struct Vector4;
using Rect = Vector4;
struct iRect;
struct Color;
struct Matrix;
struct Point;

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
    std::string getSavePath();
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

    void writeInt8(int8_t val, FILE* pFile);
    void writeUInt8(uint8_t val, FILE* pFile);
    void writeInt16(int16_t val, FILE* pFile);
    void writeUInt16(uint16_t val, FILE* pFile);
    void writeInt32(int32_t val, FILE* pFile);
    void writeUInt32(uint32_t val, FILE* pFile);
    void writeInt64(int64_t val, FILE* pFile);
    void writeUInt64(uint64_t val, FILE* pFile);
    void writeFloat(float val, FILE* pFile);
    void writeDouble(double val, FILE* pFile);
    void writeBool(bool val, FILE* pFile);
    void writeString(const std::string& val, FILE* pFile);
    void writeVector2(const Vector2& val, FILE* pFile);
    void writeVector3(const Vector3& val, FILE* pFile);
    void writeVector4(const Vector4& val, FILE* pFile);
    void writeRect(const Rect& val, FILE* pFile);
    void writeiRect(const iRect& val, FILE* pFile);
    void writePoint(const Point& val, FILE* pFile);
    void writeColor(const Color& val, FILE* pFile);
    void writeMatrix(const Matrix& val, FILE* pFile);

    int8_t readInt8(FILE* pFile);
    uint8_t readUInt8(FILE* pFile);
    int16_t readInt16(FILE* pFile);
    uint16_t readUInt16(FILE* pFile);
    int32_t readInt32(FILE* pFile);
    uint32_t readUInt32(FILE* pFile);
    float readFloat(FILE* pFile);
    double readDouble(FILE* pFile);
    bool readBool(FILE* pFile);
    std::string readString(FILE* pFile);
    Vector2 readVector2(FILE* pFile);
    Vector3 readVector3(FILE* pFile);
    Vector4 readVector4(FILE* pFile);
    Rect readRect(FILE* pFile);
    iRect readiRect(FILE* pFile);
    Point readPoint(FILE* pFile);
    Color readColor(FILE* pFile);
    Matrix readMatrix(FILE* pFile);
}

#endif
