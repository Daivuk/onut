// Onut
#include <onut/Resource.h>

namespace onut
{
    Resource::Resource()
    {
    }

    Resource::~Resource()
    {
    }

    const std::string& Resource::getName() const
    {
        return m_name;
    }

    void Resource::setName(const std::string& name)
    {
        m_name = name;
    }

    const std::string& Resource::getFilename() const
    {
        return m_filename;
    }

    void Resource::setFilename(const std::string& filename)
    {
        m_filename = filename;
    }
};
