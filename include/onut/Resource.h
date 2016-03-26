#pragma once
// STL
#include <string>

// Forward
#include <onut/ForwardDeclaration.h>
OForwardDeclare(Resource);

namespace onut
{
    class Resource
    {
    public:
        virtual ~Resource();

        void setName(const std::string& name);
        const std::string& getName() const;

        void setFilename(const std::string& filename);
        const std::string& getFilename() const;

    protected:
        Resource();

    private:
        std::string m_name;
        std::string m_filename;
    };
};
