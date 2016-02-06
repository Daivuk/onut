#pragma once
#include <string>

#include "onut/forward_declaration.h"
OForwardDeclare(Resource);

namespace onut
{
    class Resource
    {
    public:
        virtual ~Resource();

        const std::string& getName() const;

    protected:
        friend class ContentManager;

        Resource();

        void setName(const std::string& name);

    private:
        std::string m_name;
    };
};
