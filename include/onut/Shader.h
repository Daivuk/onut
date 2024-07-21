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
            UInt = 5,
            UInt4 = 6,
            Int = 7,
            Int4 = 8,
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

        struct UInt4
        {
            union
            {
                struct
                {
                    uint32_t x, y, z, w;
                };
                uint32_t v[4];
            };
        };
        struct Int4
        {
            union
            {
                struct
                {
                    int32_t x, y, z, w;
                };
                int32_t v[4];
            };
        };

#define DECL_UNIFORM_SETTER(_type, _typename) \
        virtual void set ## _typename(int varId, const _type& value) {}; \
        virtual void set ## _typename(const std::string& varName, const _type& value) {}; \
        virtual void set ## _typename ## Array(int varId, const _type* values, int count) {}; \
        virtual void set ## _typename ## Array(const std::string& varName, const _type* values, int count) {};

        DECL_UNIFORM_SETTER(float, Float);
        DECL_UNIFORM_SETTER(Vector2, Vector2);
        DECL_UNIFORM_SETTER(Vector3, Vector3);
        DECL_UNIFORM_SETTER(Vector4, Vector4);
        DECL_UNIFORM_SETTER(uint32_t, UInt);
        DECL_UNIFORM_SETTER(UInt4, UInt4);
        DECL_UNIFORM_SETTER(int32_t, Int);
        DECL_UNIFORM_SETTER(Int4, Int4);
        DECL_UNIFORM_SETTER(Matrix, Matrix);

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
            int count = 1;
        };
        using ParsedUniforms = std::vector<ParsedUniform>;

        enum class ParsedTextureType
        {
            Float4,
            UInt
        };

        struct ParsedTexture
        {
            int index;
            sample::Filtering filter = sample::Filtering::Trilinear;
            sample::AddressMode repeatX = sample::AddressMode::Wrap;
            sample::AddressMode repeatY = sample::AddressMode::Wrap;
            VarType type = VarType::Float4; 
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
            ParsedTextures textures;
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
