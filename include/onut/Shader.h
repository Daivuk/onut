#pragma once
// Onut
#include <onut/Resource.h>

// STL
#include <vector>

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

        struct VertexElement
        {
            uint32_t size;
            std::string semanticName;

            VertexElement(uint32_t in_size, const std::string& in_semanticName = "ELEMENT");
        };
        using VertexElements = std::vector<VertexElement>;

        static OShaderRef createFromBinaryFile(const std::string& filename, Type in_type, const VertexElements& vertexElements = {});
        static OShaderRef createFromSourceFile(const std::string& filename, Type in_type, const VertexElements& vertexElements = {});
        static OShaderRef createFromBinaryData(const uint8_t* pData, uint32_t size, Type in_type, const VertexElements& vertexElements = {});
        static OShaderRef createFromSource(const std::string& source, Type in_type, const VertexElements& vertexElements = {});

        virtual ~Shader();

        Type getType() const;
        uint32_t getVertexSize() const;

    protected:
        Shader();

        Type m_type;
        uint32_t m_vertexSize = 0;

    private:
    };
};

#define OVertexShader onut::Shader::Type::Vertex
#define OPixelShader onut::Shader::Type::Pixel
