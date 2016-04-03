// Onut
#include <onut/Shader.h>

namespace onut
{
    Shader::VertexElement::VertexElement(uint32_t in_size, const std::string& in_semanticName)
        : size(in_size)
        , semanticName(in_semanticName)
    {
    }

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

    uint32_t Shader::getVertexSize() const
    {
        return m_vertexSize;
    }
};
