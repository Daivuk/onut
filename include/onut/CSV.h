#ifndef CSV_H_INCLUDED
#define CSV_H_INCLUDED

// Onut
#include <onut/Resource.h>

// STL
#include <unordered_map>
#include <vector>

// Forward
#include <onut/ForwardDeclaration.h>
OForwardDeclare(ContentManager)
OForwardDeclare(CSV)

namespace onut
{
    class CSV final : public Resource
    {
    public:
        static OCSVRef createFromFile(const std::string& filename, const OContentManagerRef& pContentManager);

        int getRowCount() const;
        const std::string& getValue(const std::string& column, int row) const;
        int getInt(const std::string& column, int row) const;
        float getFloat(const std::string& column, int row) const;
        double getDouble(const std::string& column, int row) const;

    private:
        CSV(const std::string& filename);

        using Column = std::vector<std::string>;
        using ColumnMap = std::unordered_map<std::string, Column>;

        ColumnMap m_columnMap;
        int m_rowCount = 0;
    };
}

OCSVRef OGetCSV(const std::string& name);

#endif
