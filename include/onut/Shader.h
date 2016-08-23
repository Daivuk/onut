#pragma once
// Onut
#include <onut/Maths.h>
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

        static OShaderRef createFromFile(const std::string& filename, const OContentManagerRef& pContentManager);
        static OShaderRef createFromBinaryFile(const std::string& filename, Type in_type, const VertexElements& vertexElements = {});
        static OShaderRef createFromSourceFile(const std::string& filename, Type in_type, const VertexElements& vertexElements = {});
        static OShaderRef createFromBinaryData(const uint8_t* pData, uint32_t size, Type in_type, const VertexElements& vertexElements = {});
        static OShaderRef createFromSource(const std::string& source, Type in_type, const VertexElements& vertexElements = {});

        virtual ~Shader();

        Type getType() const;
        uint32_t getVertexSize() const;

        virtual int getUniformId(const std::string& varName) const = 0;
        virtual void setFloat(int varId, float value) = 0;
        virtual void setVector2(int varId, const Vector2& value) = 0;
        virtual void setVector3(int varId, const Vector3& value) = 0;
        virtual void setVector4(int varId, const Vector4& value) = 0;
        virtual void setMatrix(int varId, const Matrix& value) = 0;
        virtual void setFloat(const std::string& varName, float value) = 0;
        virtual void setVector2(const std::string& varName, const Vector2& value) = 0;
        virtual void setVector3(const std::string& varName, const Vector3& value) = 0;
        virtual void setVector4(const std::string& varName, const Vector4& value) = 0;
        virtual void setMatrix(const std::string& varName, const Matrix& value) = 0;

    protected:
        Shader();

        struct ParsedElement
        {
            uint32_t size;
            std::string name;
        };
        using ParsedElements = std::vector<ParsedElement>;

        struct ParsedUniform
        {
            std::string type;
            std::string name;
        };
        using ParsedUniforms = std::vector<ParsedUniform>;

        struct ParsedTexture
        {
            enum class Filter
            {
                Nearest,
                Linear,
                Bilinear,
                Trilinear,
                Anisotropic
            };
            enum class Repeat
            {
                Clamp,
                Wrap
            };
            int index;
            Filter filter = Filter::Trilinear;
            Repeat repeatX = Repeat::Wrap;
            Repeat repeatY = Repeat::Wrap;
            std::string name;
        };
        using ParsedTextures = std::vector<ParsedTexture>;

        struct ParsedVS
        {
            ParsedElements inputs;
            ParsedElements outputs;
            ParsedUniforms uniforms;
            std::string source;
        };

        struct ParsedPS
        {
            ParsedElements inputs;
            ParsedUniforms uniforms;
            ParsedTextures textures;
            std::string source;
        };

        static ParsedElements parseElements(std::string& content, const std::string& type);
        static ParsedUniforms parseUniforms(std::string& content);
        static ParsedTextures parseTextures(std::string& content);
        static ParsedVS parseVertexShader(const std::string& filename);
        static ParsedPS parsePixelShader(const std::string& filename);

        Type m_type;
        uint32_t m_vertexSize = 0;

    private:
    };
};

#define OVertexShader onut::Shader::Type::Vertex
#define OPixelShader onut::Shader::Type::Pixel

OShaderRef OGetShader(const std::string& name);
