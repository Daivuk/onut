// Onut
#include <onut/ContentManager.h>
#include <onut/CSV.h>
#include <onut/Strings.h>

// STL
#include <cassert>
#include <fstream>

namespace onut
{
    OCSVRef CSV::createFromFile(const std::string& filename, const OContentManagerRef& pContentManager)
    {
        return std::shared_ptr<OCSV>(new OCSV(filename));
    }

    int CSV::getRowCount() const
    {
        return m_rowCount;
    }

    CSV::CSV(const std::string& filename)
    {
        std::string line;
        std::ifstream fic(filename);
        if (!fic) return;
        std::getline(fic, line);
        auto columnNames = onut::splitString(line, ',');
        while (!fic.eof())
        {
            std::getline(fic, line);
            auto valuesSplit = onut::splitString(line, ',', false);
            assert(valuesSplit.size() == columnNames.size()); // malformed
            for (size_t i = 0; i < columnNames.size(); ++i)
            {
                m_columnMap[columnNames[i]].push_back(valuesSplit[i]);
            }
            ++m_rowCount;
        }
        fic.close();
    }

    const std::string& CSV::getValue(const std::string& column, int row) const
    {
        static std::string empty("");
        const auto& itColumn = m_columnMap.find(column);
        if (itColumn == m_columnMap.end()) return empty;
        const auto& columnRows = itColumn->second;
        if (row < 0 || row >= static_cast<int>(columnRows.size())) return empty;
        return columnRows[row];
    }

    int CSV::getInt(const std::string& column, int row) const
    {
        try
        {
            return std::stoi(getValue(column, row));
        }
        catch (...)
        {
            return 0;
        }
    }

    float CSV::getFloat(const std::string& column, int row) const
    {
        try
        {
            return std::stof(getValue(column, row));
        }
        catch (...)
        {
            return 0.0f;
        }
    }

    double CSV::getDouble(const std::string& column, int row) const
    {
        try
        {
            return std::stod(getValue(column, row));
        }
        catch (...)
        {
            return 0.0;
        }
    }
}

OCSVRef OGetCSV(const std::string& name)
{
    return oContentManager->getResourceAs<OCSV>(name);
}
