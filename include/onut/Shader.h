#pragma once
// Onut
#include <onut/Resource.h>

// Third party
#include <onut/ForwardDeclaration.h>
OForwardDeclare(ContentManager)
OForwardDeclare(Shader)

namespace onut
{
    class Shader : public Resource
    {
    public:
        enum class Type
        {
            Vertex,
            Pixel
        };

        static OShaderRef createFromBinaryFile(const std::string& filename, Type in_type);
        static OShaderRef createFromSourceFile(const std::string& filename, Type in_type);
        static OShaderRef createFromBinaryData(const uint8_t* pData, uint32_t size, Type in_type);
        static OShaderRef createFromSource(const std::string& source, Type in_type);

        virtual ~Shader();

        Type getType() const;

    protected:
        Shader();

        Type m_type;

    private:
    };
};

#define OVertexShader onut::Shader::Type::Vertex
#define OPixelShader onut::Shader::Type::Pixel
