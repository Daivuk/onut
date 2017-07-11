#ifndef SHADER_H_INCLUDED
#define SHADER_H_INCLUDED

// Onut
#include <onut/SampleMode.h>
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

        enum class VarType : int
        {
            Unknown = 0,
            Float = 1,
            Float2 = 2,
            Float3 = 3,
            Float4 = 4,
            Matrix = 16
        };

        struct VertexElement
        {
            VarType type;
            std::string semanticName;

            VertexElement(VarType in_type, const std::string& in_semanticName = "ELEMENT");
        };
        using VertexElements = std::vector<VertexElement>;

        static OShaderRef createFromFile(const std::string& filename, const OContentManagerRef& pContentManager);
        static OShaderRef createFromBinaryFile(const std::string& filename, Type in_type, const VertexElements& vertexElements = {});
        static OShaderRef createFromSourceFile(const std::string& filename, Type in_type, const VertexElements& vertexElements = {});
        static OShaderRef createFromBinaryData(const uint8_t* pData, uint32_t size, Type in_type, const VertexElements& vertexElements = {});
        static OShaderRef createFromSource(const std::string& source, Type in_type, const VertexElements& vertexElements = {});
        static OShaderRef createFromNativeSource(const std::string& source, Type in_type, const VertexElements& vertexElements = {});

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

    public:
        struct ParsedElement
        {
            VarType type;
            std::string name;
        };
        using ParsedElements = std::vector<ParsedElement>;

        struct ParsedUniform
        {
            VarType type;
            std::string name;
        };
        using ParsedUniforms = std::vector<ParsedUniform>;

        struct ParsedTexture
        {
            int index;
            sample::Filtering filter = sample::Filtering::Trilinear;
            sample::AddressMode repeatX = sample::AddressMode::Wrap;
            sample::AddressMode repeatY = sample::AddressMode::Wrap;
            std::string name;
        };
        using ParsedTextures = std::vector<ParsedTexture>;

        enum class Intrinsic
        {
            None,
            Radians,
            Degrees,
            Sin,
            Cos,
            Tan,
            ASin,
            ACos,
            ATan,
            ATan2,
            Pow,
            Exp,
            Log,
            Exp2,
            Log2,
            Sqrt,
            RSqrt,
            Abs,
            Sign,
            Floor,
            Ceil,
            Frac,
            Mod,
            Min,
            Max,
            Clamp,
            Saturate,
            Lerp,
            Step,
            SmoothStep,
            Length,
            Distance,
            Dot,
            Cross,
            Normalize,
            FaceForward,
            Reflect,
            Refract,
            Any,
            All,
            Mul,
            Round
        };

        struct Token
        {
            VarType type;
            std::string str;
            Intrinsic intrinsic;
            std::vector<std::vector<Token>> intrinsicArguments;
        };

        struct Function
        {
            VarType type;
            std::string typeName;
            std::string name;
            std::vector<Token> arguments;
            std::vector<Token> body;
        };

        struct Const
        {
            VarType type;
            std::string typeName;
            std::string name;
            bool isArray;
            std::vector<Token> arguments;
            std::vector<Token> arrayArguments;
        };

        struct StructMember
        {
            VarType type;
            std::string typeName;
            std::string name;
        };

        struct Struct
        {
            std::string name;
            std::vector<StructMember> members;
        };

        struct Parsed
        {
            ParsedElements inputs;
            ParsedUniforms uniforms;
            Function mainFunction;
            std::vector<Function> functions;
            std::vector<Struct> structs;
            std::vector<Const> consts;
        };

        struct ParsedVS : public Parsed
        {
            ParsedElements outputs;
        };

        struct ParsedPS : public Parsed
        {
            ParsedTextures textures;
        };

        static ParsedTextures parseTextures(std::string& content);
        static ParsedVS parseVertexShader(const std::string& content);
        static ParsedPS parsePixelShader(const std::string& content);

    protected:
        friend class RendererGL;

        Type m_type;
        uint32_t m_vertexSize = 0;

    private:
    };
};

#define OVertexShader onut::Shader::Type::Vertex
#define OPixelShader onut::Shader::Type::Pixel

OShaderRef OGetShader(const std::string& name);

#endif
