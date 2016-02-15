#include "onut/Shader.h"

namespace onut
{
    Shader::Shader()
    {
    }

    Shader::~Shader()
    {
    }

    Shader::Type Shader::getType() const
    {
        return m_type;
    }
};
