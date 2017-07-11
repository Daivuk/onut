// Onut
#include <onut/ContentManager.h>
#include <onut/Log.h>
#include <onut/Files.h>
#include <onut/Shader.h>
#include <onut/Strings.h>

// STL
#include <cassert>
#include <map>
#include <regex>

// --BEGIN--
#define STB_C_LEX_C_DECIMAL_INTS    Y   //  "0|[1-9][0-9]*"                        CLEX_intlit
#define STB_C_LEX_C_HEX_INTS        Y   //  "0x[0-9a-fA-F]+"                       CLEX_intlit
#define STB_C_LEX_C_OCTAL_INTS      Y   //  "[0-7]+"                               CLEX_intlit
#define STB_C_LEX_C_DECIMAL_FLOATS  Y   //  "[0-9]*(.[0-9]*([eE][-+]?[0-9]+)?)     CLEX_floatlit
#define STB_C_LEX_C99_HEX_FLOATS    N   //  "0x{hex}+(.{hex}*)?[pP][-+]?{hex}+     CLEX_floatlit
#define STB_C_LEX_C_IDENTIFIERS     Y   //  "[_a-zA-Z][_a-zA-Z0-9]*"               CLEX_id
#define STB_C_LEX_C_DQ_STRINGS      N   //  double-quote-delimited strings with escapes  CLEX_dqstring
#define STB_C_LEX_C_SQ_STRINGS      N   //  single-quote-delimited strings with escapes  CLEX_ssstring
#define STB_C_LEX_C_CHARS           N   //  single-quote-delimited character with escape CLEX_charlits
#define STB_C_LEX_C_COMMENTS        Y   //  "/* comment */"
#define STB_C_LEX_CPP_COMMENTS      Y   //  "// comment to end of line\n"
#define STB_C_LEX_C_COMPARISONS     Y   //  "==" CLEX_eq  "!=" CLEX_noteq   "<=" CLEX_lesseq  ">=" CLEX_greatereq
#define STB_C_LEX_C_LOGICAL         Y   //  "&&"  CLEX_andand   "||"  CLEX_oror
#define STB_C_LEX_C_SHIFTS          Y   //  "<<"  CLEX_shl      ">>"  CLEX_shr
#define STB_C_LEX_C_INCREMENTS      Y   //  "++"  CLEX_plusplus "--"  CLEX_minusminus
#define STB_C_LEX_C_ARROW           N   //  "->"  CLEX_arrow
#define STB_C_LEX_EQUAL_ARROW       N   //  "=>"  CLEX_eqarrow
#define STB_C_LEX_C_BITWISEEQ       Y   //  "&="  CLEX_andeq    "|="  CLEX_oreq     "^="  CLEX_xoreq
#define STB_C_LEX_C_ARITHEQ         Y   //  "+="  CLEX_pluseq   "-="  CLEX_minuseq
//  "*="  CLEX_muleq    "/="  CLEX_diveq    "%=" CLEX_modeq
//  if both STB_C_LEX_SHIFTS & STB_C_LEX_ARITHEQ:
//                      "<<=" CLEX_shleq    ">>=" CLEX_shreq

#define STB_C_LEX_PARSE_SUFFIXES    N   // letters after numbers are parsed as part of those numbers, and must be in suffix list below
#define STB_C_LEX_DECIMAL_SUFFIXES  ""  // decimal integer suffixes e.g. "uUlL" -- these are returned as-is in string storage
#define STB_C_LEX_HEX_SUFFIXES      ""  // e.g. "uUlL"
#define STB_C_LEX_OCTAL_SUFFIXES    ""  // e.g. "uUlL"
#define STB_C_LEX_FLOAT_SUFFIXES    ""  //

#define STB_C_LEX_0_IS_EOF             N  // if Y, ends parsing at '\0'; if N, returns '\0' as token
#define STB_C_LEX_INTEGERS_AS_DOUBLES  Y  // parses integers as doubles so they can be larger than 'int', but only if STB_C_LEX_STDLIB==N
#define STB_C_LEX_MULTILINE_DSTRINGS   N  // allow newlines in double-quoted strings
#define STB_C_LEX_MULTILINE_SSTRINGS   N  // allow newlines in single-quoted strings
#define STB_C_LEX_USE_STDLIB           Y  // use strtod,strtol for parsing #s; otherwise inaccurate hack
#define STB_C_LEX_DOLLAR_IDENTIFIER    N  // allow $ as an identifier character
#define STB_C_LEX_FLOAT_NO_DECIMAL     Y  // allow floats that have no decimal point if they have an exponent

#define STB_C_LEX_DEFINE_ALL_TOKEN_NAMES  N   // if Y, all CLEX_ token names are defined, even if never returned
// leaving it as N should help you catch config bugs

#define STB_C_LEX_DISCARD_PREPROCESSOR    Y   // discard C-preprocessor directives (e.g. after prepocess
// still have #line, #pragma, etc)

//#define STB_C_LEX_ISWHITE(str)    ... // return length in bytes of whitespace characters if first char is whitespace

#define STB_C_LEXER_DEFINITIONS         // This line prevents the header file from replacing your definitions
// --END--

// Third party
#define STB_C_LEXER_IMPLEMENTATION
#include <stb/stb_c_lexer.h>

namespace onut
{
    static std::string readShaderFileContent(const std::string& filename)
    {
        FILE* pFic;
#if defined(WIN32)
        fopen_s(&pFic, filename.c_str(), "rb");
#else
        pFic = fopen(filename.c_str(), "rb");
#endif
        assert(pFic);
        fseek(pFic, 0, SEEK_END);
        auto filesize = static_cast<size_t>(ftell(pFic));
        fseek(pFic, 0, SEEK_SET);
        std::string content;
        content.resize(filesize + 1);
        fread(const_cast<char*>(content.data()), 1, filesize, pFic);
        content[filesize] = '\0';
        fclose(pFic);

        return std::move(content);
    }

    OShaderRef Shader::createFromFile(const std::string& filename, const OContentManagerRef& pContentManager)
    {
        // Set the shader type
        auto ext = onut::getExtension(filename);
        if (ext == "VS")
        {
            return createFromSourceFile(filename, OVertexShader);
        }
        else if (ext == "PS")
        {
            return createFromSourceFile(filename, OPixelShader);
        }
        else
        {
            assert(false);
        }

        return nullptr;
    }

    OShaderRef Shader::createFromSourceFile(const std::string& filename, Type in_type, const VertexElements& vertexElements)
    {
        OLog("Creating Shader " + filename);
        auto content = readShaderFileContent(filename);
        return createFromSource(content, in_type, vertexElements);
    }

    Shader::VertexElement::VertexElement(VarType in_type, const std::string& in_semanticName)
        : type(in_type)
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

    Shader::ParsedTextures Shader::parseTextures(std::string& content)
    {
        ParsedTextures ret;

        std::smatch match;
        size_t offset = 0;
        while (std::regex_search(content.cbegin() + offset, content.cend(), match, std::regex("Texture([\\d]+)\\s+([\\w]+)\\s+\\{")))
        {
            ParsedTexture texture;

            texture.index = std::stoi(match[1].str());
            texture.name = match[2].str();

            std::smatch match2;
            size_t propsStart = offset + match.position() + match.length();
            if (std::regex_search(content.cbegin() + propsStart, content.cend(), match2, std::regex("\\}")))
            {
                auto propsEnd = propsStart + match2.position() + match2.length();
                std::smatch matchProperty;
                while (std::regex_search(content.cbegin() + propsStart, content.cbegin() + propsEnd, matchProperty, std::regex("([\\w]+)\\s*=\\s*([\\w]+)\\s*;")))
                {
                    const auto& propName = matchProperty[1].str();
                    const auto& propValue = matchProperty[2].str();
                    if (propName == "filter")
                    {
                        if (propValue == "nearest")
                        {
                            texture.filter = ParsedTexture::Filter::Nearest;
                        }
                        else if (propValue == "linear")
                        {
                            texture.filter = ParsedTexture::Filter::Linear;
                        }
                        else if (propValue == "bilinear")
                        {
                            texture.filter = ParsedTexture::Filter::Bilinear;
                        }
                        else if (propValue == "trilinear")
                        {
                            texture.filter = ParsedTexture::Filter::Trilinear;
                        }
                        else if (propValue == "anisotropic")
                        {
                            texture.filter = ParsedTexture::Filter::Anisotropic;
                        }
                        else assert(false); // Invalid filter
                    }
                    else if (propName == "repeat")
                    {
                        if (propValue == "clamp")
                        {
                            texture.repeatX = texture.repeatY = ParsedTexture::Repeat::Clamp;
                        }
                        else if (propValue == "wrap")
                        {
                            texture.repeatX = texture.repeatY = ParsedTexture::Repeat::Wrap;
                        }
                        else assert(false); // Invalid repeat
                    }
                    else if (propName == "repeatX")
                    {
                        if (propValue == "clamp")
                        {
                            texture.repeatX = ParsedTexture::Repeat::Clamp;
                        }
                        else if (propValue == "wrap")
                        {
                            texture.repeatX = ParsedTexture::Repeat::Wrap;
                        }
                        else assert(false); // Invalid repeat
                    }
                    else if (propName == "repeatY")
                    {
                        if (propValue == "clamp")
                        {
                            texture.repeatY = ParsedTexture::Repeat::Clamp;
                        }
                        else if (propValue == "wrap")
                        {
                            texture.repeatY = ParsedTexture::Repeat::Wrap;
                        }
                        else assert(false); // Invalid repeat
                    }
                    else assert(false); // Invalid texture sampler property

                    propsStart += matchProperty.position() + matchProperty.length();
                }

                propsStart = propsEnd;
            }
            else assert(false); // No matching closing } to texture sample

            content[offset + match.position()] = '/';
            content[offset + match.position() + 1] = '*';
            content[propsStart - 2] = '*';
            content[propsStart - 1] = '/';

            offset += match.position() + match.length() + match2.position() + match2.length();

            ret.push_back(texture);
        }

        return std::move(ret);
    }

    static void shaderError(stb_lexer& lexer, const std::string& msg)
    {
        stb_lex_location loc;
        stb_c_lexer_get_location(&lexer, lexer.where_firstchar, &loc);
        OLogE("(" + std::to_string(loc.line_number) + "," + std::to_string(loc.line_offset) + ") " + msg);
    }

    static bool parseElement(Shader::ParsedElement& element, stb_lexer& lexer)
    {
        if (!stb_c_lexer_get_token(&lexer))
        {
            shaderError(lexer, "Expected type");
            return false;
        }
        if (lexer.token != CLEX_id)
        {
            shaderError(lexer, "Expected type");
            return false;
        }
        if (strcmp(lexer.string, "float") == 0) element.type = Shader::VarType::Float;
        else if (strcmp(lexer.string, "float2") == 0) element.type = Shader::VarType::Float2;
        else if (strcmp(lexer.string, "float3") == 0) element.type = Shader::VarType::Float3;
        else if (strcmp(lexer.string, "float4") == 0) element.type = Shader::VarType::Float4;
        else
        {
            shaderError(lexer, "Expected type of float, float2, float3 or float4");
            return false;
        }
        if (!stb_c_lexer_get_token(&lexer))
        {
            shaderError(lexer, "Expected identifier");
            return false;
        }
        if (lexer.token != CLEX_id)
        {
            shaderError(lexer, "Expected identifier");
            return false;
        }
        if (!stb_c_lexer_get_token(&lexer))
        {
            shaderError(lexer, "Expected ;");
            return false;
        }
        if ((char)lexer.token != ';')
        {
            shaderError(lexer, "Expected ;");
            return false;
        }
        element.name = lexer.string;
        return true;
    }

    static bool parseUniform(Shader::ParsedUniform& uniform, stb_lexer& lexer)
    {
        if (!stb_c_lexer_get_token(&lexer))
        {
            shaderError(lexer, "Expected type");
            return false;
        }
        if (lexer.token != CLEX_id)
        {
            shaderError(lexer, "Expected type");
            return false;
        }
        if (strcmp(lexer.string, "float") == 0) uniform.type = Shader::VarType::Float;
        else if (strcmp(lexer.string, "float2") == 0) uniform.type = Shader::VarType::Float2;
        else if (strcmp(lexer.string, "float3") == 0) uniform.type = Shader::VarType::Float3;
        else if (strcmp(lexer.string, "float4") == 0) uniform.type = Shader::VarType::Float4;
        else if (strcmp(lexer.string, "matrix") == 0) uniform.type = Shader::VarType::Matrix;
        else
        {
            shaderError(lexer, "Expected type of float, float2, float3, float4 or matrix");
            return false;
        }
        if (!stb_c_lexer_get_token(&lexer))
        {
            shaderError(lexer, "Expected identifier");
            return false;
        }
        if (lexer.token != CLEX_id)
        {
            shaderError(lexer, "Expected identifier");
            return false;
        }
        if (!stb_c_lexer_get_token(&lexer))
        {
            shaderError(lexer, "Expected ;");
            return false;
        }
        if ((char)lexer.token != ';')
        {
            shaderError(lexer, "Expected ;");
            return false;
        }
        uniform.name = lexer.string;
        return true;
    }

    static bool parseTexture(Shader::ParsedTexture& texture, stb_lexer& lexer)
    {
        texture.index = (int)lexer.string[7] - (int)'0';

        // Name
        if (!stb_c_lexer_get_token(&lexer))
        {
            shaderError(lexer, "Expected identifier");
            return false;
        }
        if (lexer.token != CLEX_id)
        {
            shaderError(lexer, "Expected identifier");
            return false;
        }
        texture.name = lexer.string;

        // If that's it, leave it to defaults
        if (!stb_c_lexer_get_token(&lexer))
        {
            shaderError(lexer, "Expected ;");
            return false;
        }
        if ((char)lexer.token == ';')
        {
            return true;
        }

        if ((char)lexer.token != '{')
        {
            shaderError(lexer, "Expected ; or {");
            return false;
        }

        while (true)
        {
            if (!stb_c_lexer_get_token(&lexer))
            {
                shaderError(lexer, "Expected }");
                return false;
            }
            if ((char)lexer.token == '}')
            {
                break; // All good so far
            }

            // Property name
            if (lexer.token != CLEX_id)
            {
                shaderError(lexer, "Expected identifier");
                return false;
            }
            std::string propName = onut::toLower(lexer.string);

            // =
            if (!stb_c_lexer_get_token(&lexer))
            {
                shaderError(lexer, "Expected =");
                return false;
            }
            if ((char)lexer.token != '=')
            {
                shaderError(lexer, "Expected =");
                return false;
            }

            // Property value
            if (!stb_c_lexer_get_token(&lexer))
            {
                shaderError(lexer, "Expected identifier");
                return false;
            }
            if (lexer.token != CLEX_id)
            {
                shaderError(lexer, "Expected identifier");
                return false;
            }
            std::string propValue = onut::toLower(lexer.string);

            if (!stb_c_lexer_get_token(&lexer))
            {
                shaderError(lexer, "Expected ;");
                return false;
            }
            if ((char)lexer.token != ';')
            {
                shaderError(lexer, "Expected ;");
                return false;
            }

            // Set property
            if (propName == "filter")
            {
                if (propValue == "nearest")
                {
                    texture.filter = Shader::ParsedTexture::Filter::Nearest;
                }
                else if (propValue == "linear")
                {
                    texture.filter = Shader::ParsedTexture::Filter::Linear;
                }
                else if (propValue == "bilinear")
                {
                    texture.filter = Shader::ParsedTexture::Filter::Bilinear;
                }
                else if (propValue == "trilinear")
                {
                    texture.filter = Shader::ParsedTexture::Filter::Trilinear;
                }
                else if (propValue == "anisotropic")
                {
                    texture.filter = Shader::ParsedTexture::Filter::Anisotropic;
                }
                else
                {
                    shaderError(lexer, "Invalid filter");
                    return false;
                }
            }
            else if (propName == "repeat")
            {
                if (propValue == "clamp")
                {
                    texture.repeatX = texture.repeatY = Shader::ParsedTexture::Repeat::Clamp;
                }
                else if (propValue == "wrap")
                {
                    texture.repeatX = texture.repeatY = Shader::ParsedTexture::Repeat::Wrap;
                }
                else
                {
                    shaderError(lexer, "Invalid repeat");
                    return false;
                }
            }
            else if (propName == "repeatx")
            {
                if (propValue == "clamp")
                {
                    texture.repeatX = Shader::ParsedTexture::Repeat::Clamp;
                }
                else if (propValue == "wrap")
                {
                    texture.repeatX = Shader::ParsedTexture::Repeat::Wrap;
                }
                else
                {
                    shaderError(lexer, "Invalid repeat");
                    return false;
                }
            }
            else if (propName == "repeaty")
            {
                if (propValue == "clamp")
                {
                    texture.repeatY = Shader::ParsedTexture::Repeat::Clamp;
                }
                else if (propValue == "wrap")
                {
                    texture.repeatY = Shader::ParsedTexture::Repeat::Wrap;
                }
                else
                {
                    shaderError(lexer, "Invalid repeat");
                    return false;
                }
            }
            else
            {
                shaderError(lexer, "Invalid texture sampler property");
                return false;
            }
        }

        return true;
    }

    static bool parseStruct(Shader::Struct& _struct, stb_lexer& lexer)
    {
        if (!stb_c_lexer_get_token(&lexer))
        {
            shaderError(lexer, "Expected identifier");
            return false;
        }
        if (lexer.token != CLEX_id)
        {
            shaderError(lexer, "Expected identifier");
            return false;
        }
        _struct.name = lexer.string;
        if (!stb_c_lexer_get_token(&lexer))
        {
            shaderError(lexer, "Expected {");
            return false;
        }
        if ((char)lexer.token != '{')
        {
            shaderError(lexer, "Expected {");
            return false;
        }
        while (true)
        {
            if (!stb_c_lexer_get_token(&lexer))
            {
                shaderError(lexer, "Expected }");
                return false;
            }
            if ((char)lexer.token == '}')
            {
                if (_struct.members.empty())
                {
                    shaderError(lexer, "Expected members");
                    return false;
                }
                break; // All good so far
            }
            // Member type
            if (lexer.token != CLEX_id)
            {
                shaderError(lexer, "Expected type");
                return false;
            }
            Shader::StructMember member;
            if (strcmp(lexer.string, "float") == 0) member.type = Shader::VarType::Float;
            else if (strcmp(lexer.string, "float2") == 0) member.type = Shader::VarType::Float2;
            else if (strcmp(lexer.string, "float3") == 0) member.type = Shader::VarType::Float3;
            else if (strcmp(lexer.string, "float4") == 0) member.type = Shader::VarType::Float4;
            else if (strcmp(lexer.string, "matrix") == 0) member.type = Shader::VarType::Matrix;
            else
            {
                member.type = Shader::VarType::Unknown;
                member.typeName = lexer.string;
            }
            // Member name
            if (!stb_c_lexer_get_token(&lexer))
            {
                shaderError(lexer, "Expected identifier");
                return false;
            }
            if (lexer.token != CLEX_id)
            {
                shaderError(lexer, "Expected identifier");
                return false;
            }
            member.name = lexer.string;
            // Check if we don't already have a member of the same name...
            for (const auto& otherMember : _struct.members)
            {
                if (otherMember.name == member.name)
                {
                    shaderError(lexer, "Duplicated member");
                    return false;
                }
            }
            _struct.members.push_back(member);
            if (!stb_c_lexer_get_token(&lexer))
            {
                shaderError(lexer, "Expected ;");
                return false;
            }
            if ((char)lexer.token != ';')
            {
                shaderError(lexer, "Expected ;");
                return false;
            }
        }
        if (!stb_c_lexer_get_token(&lexer))
        {
            shaderError(lexer, "Expected ;");
            return false;
        }
        if ((char)lexer.token != ';')
        {
            shaderError(lexer, "Expected ;");
            return false;
        }
        return true;
    }

    struct IntrinsicDesc
    {
        Shader::Intrinsic intrinsic;
        int argCount;
    };
    static std::map<std::string, IntrinsicDesc> INTRINSIC_DESCS = {
        { "radians",{ Shader::Intrinsic::Radians, 1 } },
        { "degrees",{ Shader::Intrinsic::Degrees, 1 } },
        { "sin",{ Shader::Intrinsic::Sin, 1 } },
        { "cos",{ Shader::Intrinsic::Cos, 1 } },
        { "tan",{ Shader::Intrinsic::Tan, 1 } },
        { "asin",{ Shader::Intrinsic::ASin, 1 } },
        { "acos",{ Shader::Intrinsic::ACos, 1 } },
        { "atan",{ Shader::Intrinsic::ATan, 1 } },
        { "atan2",{ Shader::Intrinsic::ATan2, 2 } },
        { "pow",{ Shader::Intrinsic::Pow, 2 } },
        { "exp",{ Shader::Intrinsic::Exp, 1 } },
        { "log",{ Shader::Intrinsic::Log, 1 } },
        { "exp2",{ Shader::Intrinsic::Exp2, 1 } },
        { "log2",{ Shader::Intrinsic::Log2, 1 } },
        { "sqrt",{ Shader::Intrinsic::Sqrt, 1 } },
        { "rsqrt",{ Shader::Intrinsic::RSqrt, 1 } },
        { "abs",{ Shader::Intrinsic::Abs, 1 } },
        { "sign",{ Shader::Intrinsic::Sign, 1 } },
        { "floor",{ Shader::Intrinsic::Floor, 1 } },
        { "ceil",{ Shader::Intrinsic::Ceil, 1 } },
        { "frac",{ Shader::Intrinsic::Frac, 1 } },
        { "mod",{ Shader::Intrinsic::Mod, 2 } },
        { "min",{ Shader::Intrinsic::Min, 2 } },
        { "max",{ Shader::Intrinsic::Max, 2 } },
        { "clamp",{ Shader::Intrinsic::Clamp, 3 } },
        { "saturate",{ Shader::Intrinsic::Saturate, 1 } },
        { "lerp",{ Shader::Intrinsic::Lerp, 3 } },
        { "step",{ Shader::Intrinsic::Step, 2 } },
        { "smoothstep",{ Shader::Intrinsic::SmoothStep, 3 } },
        { "length",{ Shader::Intrinsic::Length, 1 } },
        { "distance",{ Shader::Intrinsic::Distance, 2 } },
        { "dot",{ Shader::Intrinsic::Dot, 2 } },
        { "cross",{ Shader::Intrinsic::Cross, 2 } },
        { "normalize",{ Shader::Intrinsic::Normalize, 1 } },
        { "faceforward",{ Shader::Intrinsic::FaceForward, 3 } },
        { "reflect",{ Shader::Intrinsic::Reflect, 2 } },
        { "refract",{ Shader::Intrinsic::Refract, 3 } },
        { "any",{ Shader::Intrinsic::Any, 1 } },
        { "all",{ Shader::Intrinsic::All, 1 } },
        { "mul",{ Shader::Intrinsic::Mul, 2 } },
        { "round",{ Shader::Intrinsic::Round, 1 } }
    };

    static bool parseIntrinsic(Shader::Token& token, int argCount, stb_lexer& lexer);
    static bool parseToken(Shader::Token& token, stb_lexer& lexer)
    {
        token.intrinsic = Shader::Intrinsic::None;
        token.type = Shader::VarType::Unknown;
        if (lexer.token == CLEX_id)
        {
            token.str = lexer.string;
            auto it = INTRINSIC_DESCS.find(lexer.string);
            // Check if we call an instrinsic function
            if (it != INTRINSIC_DESCS.end())
            {
                token.intrinsic = it->second.intrinsic;
                if (!parseIntrinsic(token, it->second.argCount, lexer)) return false;
            }
            else if (strcmp(lexer.string, "float") == 0) token.type = Shader::VarType::Float;
            else if (strcmp(lexer.string, "float2") == 0) token.type = Shader::VarType::Float2;
            else if (strcmp(lexer.string, "float3") == 0) token.type = Shader::VarType::Float3;
            else if (strcmp(lexer.string, "float4") == 0) token.type = Shader::VarType::Float4;
            else if (strcmp(lexer.string, "matrix") == 0) token.type = Shader::VarType::Matrix;
        }
        else if (lexer.token == CLEX_intlit)
        {
            token.str = std::to_string(lexer.int_number) + ".0";
        }
        else if (lexer.token == CLEX_floatlit)
        {
            token.str = std::to_string(lexer.real_number);
        }
        else if (lexer.token == CLEX_eq) token.str = "==";
        else if (lexer.token == CLEX_noteq) token.str = "!=";
        else if (lexer.token == CLEX_lesseq) token.str = "<=";
        else if (lexer.token == CLEX_greatereq) token.str = ">=";
        else if (lexer.token == CLEX_andand) token.str = "&&";
        else if (lexer.token == CLEX_oror) token.str = "||";
        else if (lexer.token == CLEX_shl) token.str = "<<";
        else if (lexer.token == CLEX_shr) token.str = ">>";
        else if (lexer.token == CLEX_plusplus) token.str = "++";
        else if (lexer.token == CLEX_minusminus) token.str = "--";
        else if (lexer.token == CLEX_pluseq) token.str = "+=";
        else if (lexer.token == CLEX_minuseq) token.str = "-=";
        else if (lexer.token == CLEX_muleq) token.str = "*=";
        else if (lexer.token == CLEX_diveq) token.str = "/=";
        else if (lexer.token == CLEX_modeq)
        {
            shaderError(lexer, "Modulo operator % is unsupported. Use mod(x, y) intrinsic instead");
            return false;
        }
        else if (lexer.token == CLEX_andeq) token.str = "&=";
        else if (lexer.token == CLEX_oreq) token.str = "|=";
        else if (lexer.token == CLEX_xoreq) token.str = "^=";
        else
        {
            token.str = std::string() + (char)lexer.token;
        }
        return true;
    }

    static bool parseIntrinsic(Shader::Token& token, int argCount, stb_lexer& lexer)
    {
        if (!stb_c_lexer_get_token(&lexer))
        {
            shaderError(lexer, "Expected (");
            return false;
        }
        if ((char)lexer.token != '(')
        {
            shaderError(lexer, "Expected (");
            return false;
        }

        int deep = 1;
        while (true)
        {
            std::vector<Shader::Token> argument;
            while (true)
            {
                if (!stb_c_lexer_get_token(&lexer))
                {
                    shaderError(lexer, "Unexpected end of file");
                    return false;
                }
                if ((char)lexer.token == ',' && deep == 1)
                {
                    if (argument.empty())
                    {
                        shaderError(lexer, "Expected tokens");
                        return false;
                    }
                    token.intrinsicArguments.push_back(argument);
                    break; // Go for the next argument
                }
                if ((char)lexer.token == '(')
                {
                    ++deep;
                }
                if ((char)lexer.token == ')')
                {
                    --deep;
                    if (deep <= 0)
                    {
                        token.intrinsicArguments.push_back(argument);
                        break;
                    }
                }

                Shader::Token subToken;
                if (!parseToken(subToken, lexer)) return false;
                argument.push_back(subToken);
            }

            if (deep <= 0) break; // All good so far
        }

        if ((int)token.intrinsicArguments.size() != argCount)
        {
            shaderError(lexer, "Wrong argument count");
            return false;
        }

        return true;
    }

    static bool parseConst(Shader::Const& _const, stb_lexer& lexer)
    {
        // Const type
        if (!stb_c_lexer_get_token(&lexer))
        {
            shaderError(lexer, "Expected type");
            return false;
        }
        if (lexer.token != CLEX_id)
        {
            shaderError(lexer, "Expected type");
            return false;
        }
        else if (strcmp(lexer.string, "float") == 0) _const.type = Shader::VarType::Float;
        else if (strcmp(lexer.string, "float2") == 0) _const.type = Shader::VarType::Float2;
        else if (strcmp(lexer.string, "float3") == 0) _const.type = Shader::VarType::Float3;
        else if (strcmp(lexer.string, "float4") == 0) _const.type = Shader::VarType::Float4;
        else if (strcmp(lexer.string, "matrix") == 0) _const.type = Shader::VarType::Matrix;
        else
        {
            _const.type = Shader::VarType::Unknown;
            _const.typeName = lexer.string;
        }

        // const name
        if (!stb_c_lexer_get_token(&lexer))
        {
            shaderError(lexer, "Expected identifier");
            return false;
        }
        if (lexer.token != CLEX_id)
        {
            shaderError(lexer, "Expected identifier");
            return false;
        }
        _const.name = lexer.string;

        // Check if it's an array
        if (!stb_c_lexer_get_token(&lexer))
        {
            shaderError(lexer, "Expected identifier");
            return false;
        }
        if ((char)lexer.token == '[')
        {
            _const.isArray = true;
            while (true)
            {
                if (!stb_c_lexer_get_token(&lexer))
                {
                    shaderError(lexer, "Expected ]");
                    return false;
                }
                if ((char)lexer.token == ']')
                {
                    if (_const.arrayArguments.empty())
                    {
                        shaderError(lexer, "Expected array size");
                        return false;
                    }
                    if (!stb_c_lexer_get_token(&lexer))
                    {
                        shaderError(lexer, "Expected =");
                        return false;
                    }
                    break;
                }
                Shader::Token token;
                if (!parseToken(token, lexer)) return false;
                _const.arrayArguments.push_back(token);
            }
        }

        if ((char)lexer.token != '=')
        {
            shaderError(lexer, "Expected =");
            return false;
        }

        if (_const.isArray)
        {
            if (!stb_c_lexer_get_token(&lexer))
            {
                shaderError(lexer, "Expected { array initializer");
                return false;
            }
            if ((char)lexer.token != '{')
            {
                shaderError(lexer, "Expected { array initializer");
                return false;
            }
        }

        while (true)
        {
            if (!stb_c_lexer_get_token(&lexer))
            {
                shaderError(lexer, "Expected ;");
                return false;
            }
            if ((char)lexer.token == ';')
            {
                if (_const.arguments.empty())
                {
                    shaderError(lexer, "Expected value");
                    return false;
                }
                break;
            }
            if ((char)lexer.token == '}' && _const.isArray)
            {
                if (!stb_c_lexer_get_token(&lexer))
                {
                    shaderError(lexer, "Expected ;");
                    return false;
                }
                if ((char)lexer.token == ';')
                {
                    if (_const.arguments.empty())
                    {
                        shaderError(lexer, "Expected value");
                        return false;
                    }
                    break;
                }
                else
                {
                    shaderError(lexer, "Expected ;");
                    return false;
                }
            }

            Shader::Token token;
            if (!parseToken(token, lexer)) return false;
            _const.arguments.push_back(token);
        }

        return true;
    }

    static bool parseFunction(Shader::Function& _function, stb_lexer& lexer)
    {
        // Return type
        if (lexer.token != CLEX_id)
        {
            shaderError(lexer, "Expected type");
            return false;
        }
        else if (strcmp(lexer.string, "float") == 0) _function.type = Shader::VarType::Float;
        else if (strcmp(lexer.string, "float2") == 0) _function.type = Shader::VarType::Float2;
        else if (strcmp(lexer.string, "float3") == 0) _function.type = Shader::VarType::Float3;
        else if (strcmp(lexer.string, "float4") == 0) _function.type = Shader::VarType::Float4;
        else if (strcmp(lexer.string, "matrix") == 0) _function.type = Shader::VarType::Matrix;
        else
        {
            _function.type = Shader::VarType::Unknown;
            _function.typeName = lexer.string;
        }

        // Function  name
        if (!stb_c_lexer_get_token(&lexer))
        {
            shaderError(lexer, "Expected identifier");
            return false;
        }
        if (lexer.token != CLEX_id)
        {
            shaderError(lexer, "Expected identifier");
            return false;
        }
        _function.name = lexer.string;

        // Arguments
        if (!stb_c_lexer_get_token(&lexer))
        {
            shaderError(lexer, "Expected (");
            return false;
        }
        if ((char)lexer.token != '(')
        {
            shaderError(lexer, "Expected (");
            return false;
        }
        if (_function.name == "main")
        {
            if (!stb_c_lexer_get_token(&lexer))
            {
                shaderError(lexer, "Expected )");
                return false;
            }
            if ((char)lexer.token != ')')
            {
                shaderError(lexer, "Expected ), main function has no arguments");
                return false;
            }
        }
        else
        {
            while (true)
            {
                if (!stb_c_lexer_get_token(&lexer))
                {
                    shaderError(lexer, "Expected )");
                    return false;
                }
                if ((char)lexer.token == ')')
                {
                    break; // All good so far
                }
                Shader::Token token;
                if (!parseToken(token, lexer)) return false;
                _function.arguments.push_back(token);
            }
        }

        // Body
        if (!stb_c_lexer_get_token(&lexer))
        {
            shaderError(lexer, "Expected {");
            return false;
        }
        if ((char)lexer.token != '{')
        {
            shaderError(lexer, "Expected {");
            return false;
        }
        int deep = 1;
        while (true)
        {
            if (!stb_c_lexer_get_token(&lexer))
            {
                shaderError(lexer, "Unexpected end of file");
                return false;
            }
            if ((char)lexer.token == '{')
            {
                ++deep;
            }
            if ((char)lexer.token == '}')
            {
                --deep;
                if (deep <= 0) break; // All good so far
            }

            Shader::Token token;
            if (!parseToken(token, lexer)) return false;
            _function.body.push_back(token);
        }
        return true;
    }

    Shader::ParsedVS Shader::parseVertexShader(const std::string& in_content)
    {
        ParsedVS ret;
        stb_lexer lexer;
        char* string_store = new char[1024];
        stb_c_lexer_init(&lexer, in_content.c_str(), in_content.c_str() + in_content.size(), string_store, 1024);

        while (stb_c_lexer_get_token(&lexer))
        {
            char tokenC = (char)lexer.token;

            if (strcmp(lexer.string, "input") == 0)
            {
                ParsedElement input;
                if (!parseElement(input, lexer)) break;
                ret.inputs.push_back(input);
            }
            else if (strcmp(lexer.string, "output") == 0)
            {
                ParsedElement output;
                if (!parseElement(output, lexer)) break;
                ret.outputs.push_back(output);
            }
            else if (strcmp(lexer.string, "extern") == 0)
            {
                ParsedUniform uniform;
                if (!parseUniform(uniform, lexer)) break;
                ret.uniforms.push_back(uniform);
            }
            else if (strcmp(lexer.string, "const") == 0)
            {
                Const _const;
                if (!parseConst(_const, lexer)) break;
                ret.consts.push_back(_const);
            }
            else if (strcmp(lexer.string, "struct") == 0)
            {
                Struct _struct;
                if (!parseStruct(_struct, lexer)) break;
                ret.structs.push_back(_struct);
            }
            else if (strcmp(lexer.string, "void") == 0)
            {
                // This has to be the main function
                auto where = lexer.where_firstchar;
                if (!parseFunction(ret.mainFunction, lexer)) break;
                if (ret.mainFunction.name != "main")
                {
                    lexer.where_firstchar = where;
                    shaderError(lexer, "Only the main function have void as return type");
                    break;
                }
            }
            else if ((char)lexer.token == ';' || 
                     (char)lexer.token == '\0')
            {
                // That's fine, ignore it
            }
            else
            {
                // Nothing else than a function ca be here
                Function _function;
                if (!parseFunction(_function, lexer)) break;
                ret.functions.push_back(_function);
            }
        }

        delete[] string_store;

        return std::move(ret);
    }

    Shader::ParsedPS Shader::parsePixelShader(const std::string& in_content)
    {
        ParsedPS ret;
        stb_lexer lexer;
        char* string_store = new char[1024];
        stb_c_lexer_init(&lexer, in_content.c_str(), in_content.c_str() + in_content.size(), string_store, 1024);

        while (stb_c_lexer_get_token(&lexer))
        {
            char tokenC = (char)lexer.token;

            if (strcmp(lexer.string, "input") == 0)
            {
                ParsedElement input;
                if (!parseElement(input, lexer)) break;
                ret.inputs.push_back(input);
            }
            else if (strcmp(lexer.string, "extern") == 0)
            {
                ParsedUniform uniform;
                if (!parseUniform(uniform, lexer)) break;
                ret.uniforms.push_back(uniform);
            }
            else if (strcmp(lexer.string, "struct") == 0)
            {
                Struct _struct;
                if (!parseStruct(_struct, lexer)) break;
                ret.structs.push_back(_struct);
            }
            else if (strcmp(lexer.string, "const") == 0)
            {
                Const _const;
                if (!parseConst(_const, lexer)) break;
                ret.consts.push_back(_const);
            }
#if defined(WIN32)
            else if (_strnicmp(lexer.string, "texture", 7) == 0)
#else
            else if (strnicmp(lexer.string, "texture", 7) == 0)
#endif
            {
                std::string texI = lexer.string;
                if (texI.size() == 8 && texI[7] >= '0' && texI[7] <= '9')
                {
                    ParsedTexture texture;
                    if (!parseTexture(texture, lexer)) break;
                    ret.textures.push_back(texture);
                }
                else
                {
                    shaderError(lexer, "Expected texture index [0, 8]");
                    break;
                }
            }
            else if (strcmp(lexer.string, "void") == 0)
            {
                // This has to be the main function
                auto where = lexer.where_firstchar;
                if (!parseFunction(ret.mainFunction, lexer)) break;
                if (ret.mainFunction.name != "main")
                {
                    lexer.where_firstchar = where;
                    shaderError(lexer, "Only the main function have void as return type");
                    break;
                }
            }
            else if ((char)lexer.token == ';' ||
                (char)lexer.token == '\0')
            {
                // That's fine, ignore it
            }
            else
            {
                // Nothing else than a function can be here
                Function _function;
                if (!parseFunction(_function, lexer)) break;
                ret.functions.push_back(_function);
            }
        }

        delete[] string_store;

        return std::move(ret);
    }
};

OShaderRef OGetShader(const std::string& name)
{
    return oContentManager->getResourceAs<OShader>(name);
}
