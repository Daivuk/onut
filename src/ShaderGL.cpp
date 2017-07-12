// Onut
#include <onut/ContentManager.h>
#include <onut/Files.h>
#include <onut/Log.h>
#include <onut/Renderer.h>
#include <onut/Strings.h>

// Private
#include "ShaderGL.h"
#include "RendererGL.h"

// STL
#include <cassert>
#include <regex>

#define SHADER_VERSION "120"

namespace onut
{
    static std::string getTypeName(Shader::VarType type)
    {
        switch (type)
        {
        case Shader::VarType::Float: return "float";
        case Shader::VarType::Float2: return "vec2";
        case Shader::VarType::Float3: return "vec3";
        case Shader::VarType::Float4: return "vec4";
        case Shader::VarType::Matrix: return "mat4";
        default: assert(false);
        }
        return "";
    }

    static void bakeTokens(std::string& source, const std::vector<Shader::Token>& tokens)
    {
        for (const auto& token : tokens)
        {
            if (token.intrinsic == Shader::Intrinsic::None)
            {
                if (token.type == Shader::VarType::Unknown)
                {
                    if (token.str == ";") source += ";\n";
                    else if (token.str == "[")
                    {
                        // This one is tricky, we need to cast to integer
                        source += "[int(";
                    }
                    else if (token.str == "]")
                    {
                        source += ")]";
                    }
                    else source += " " + token.str;
                }
                else
                {
                    source += " " + getTypeName(token.type);
                }
            }
            else
            {
#define INTRINSICT_IMPL_1(__enum__, __name__, __0__) \
    case Shader::Intrinsic::__enum__: \
        source += " " #__name__ "("; \
        bakeTokens(source, token.intrinsicArguments[__0__]); \
        source += ")"; \
        break

#define INTRINSICT_IMPL_2(__enum__, __name__, __0__, __1__) \
    case Shader::Intrinsic::__enum__: \
        source += " " #__name__ "("; \
        bakeTokens(source, token.intrinsicArguments[__0__]); \
        source += ","; \
        bakeTokens(source, token.intrinsicArguments[__1__]); \
        source += ")"; \
        break

#define INTRINSICT_IMPL_3(__enum__, __name__, __0__, __1__, __2__) \
    case Shader::Intrinsic::__enum__: \
        source += " " #__name__ "("; \
        bakeTokens(source, token.intrinsicArguments[__0__]); \
        source += ","; \
        bakeTokens(source, token.intrinsicArguments[__1__]); \
        source += ","; \
        bakeTokens(source, token.intrinsicArguments[__2__]); \
        source += ")"; \
        break

                switch (token.intrinsic)
                {
                    INTRINSICT_IMPL_1(Radians, radians, 0);
                    INTRINSICT_IMPL_1(Degrees, degrees, 0);
                    INTRINSICT_IMPL_1(Sin, sin, 0);
                    INTRINSICT_IMPL_1(Cos, cos, 0);
                    INTRINSICT_IMPL_1(Tan, tan, 0);
                    INTRINSICT_IMPL_1(ASin, asin, 0);
                    INTRINSICT_IMPL_1(ACos, acos, 0);
                    INTRINSICT_IMPL_1(ATan, atan, 0);
                    INTRINSICT_IMPL_2(ATan2, atan, 0, 1);
                    INTRINSICT_IMPL_2(Pow, pow, 0, 1);
                    INTRINSICT_IMPL_1(Exp, exp, 0);
                    INTRINSICT_IMPL_1(Log, log, 0);
                    INTRINSICT_IMPL_1(Exp2, exp2, 0);
                    INTRINSICT_IMPL_1(Log2, log2, 0);
                    INTRINSICT_IMPL_1(Sqrt, sqrt, 0);
                    INTRINSICT_IMPL_1(RSqrt, inversesqrt, 0);
                    INTRINSICT_IMPL_1(Abs, abs, 0);
                    INTRINSICT_IMPL_1(Sign, sign, 0);
                    INTRINSICT_IMPL_1(Floor, floor, 0);
                    INTRINSICT_IMPL_1(Ceil, ceil, 0);
                    INTRINSICT_IMPL_1(Frac, fract, 0);
                    INTRINSICT_IMPL_2(Mod, mod, 0, 1);
                    INTRINSICT_IMPL_2(Min, min, 0, 1);
                    INTRINSICT_IMPL_2(Max, max, 0, 1);
                    INTRINSICT_IMPL_3(Clamp, clamp, 0, 1, 2);
                    case Shader::Intrinsic::Saturate:
                        source += " clamp(";
                        bakeTokens(source, token.intrinsicArguments[0]);
                        source += "0.0, 1.0)";
                        break;
                    INTRINSICT_IMPL_3(Lerp, mix, 0, 1, 2);
                    INTRINSICT_IMPL_2(Step, step, 0, 1);
                    INTRINSICT_IMPL_3(SmoothStep, smoothstep, 0, 1, 2);
                    INTRINSICT_IMPL_1(Length, length, 0);
                    INTRINSICT_IMPL_2(Distance, distance, 0, 1);
                    INTRINSICT_IMPL_2(Dot, dot, 0, 1);
                    INTRINSICT_IMPL_2(Cross, cross, 0, 1);
                    INTRINSICT_IMPL_1(Normalize, normalize, 0);
                    INTRINSICT_IMPL_3(FaceForward, faceforward, 0, 1, 2);
                    INTRINSICT_IMPL_2(Reflect, reflect, 0, 1);
                    INTRINSICT_IMPL_3(Refract, refract, 0, 1, 2);
                    INTRINSICT_IMPL_1(Any, any, 0);
                    INTRINSICT_IMPL_1(All, all, 0);
                    case Shader::Intrinsic::Mul:
                        source += "((";
                        bakeTokens(source, token.intrinsicArguments[0]);
                        source += ") * (";
                        bakeTokens(source, token.intrinsicArguments[1]);
                        source += "))";
                        break;
                    case Shader::Intrinsic::Round:
                        source += " floor((";
                        bakeTokens(source, token.intrinsicArguments[0]);
                        source += ") + 0.5)";
                        break;
                    default: assert(false);
                }
            }
        }
    }

    OShaderRef Shader::createFromBinaryFile(const std::string& filename, Type in_type, const VertexElements& vertexElements)
    {
        return nullptr;
    }

    OShaderRef Shader::createFromBinaryData(const uint8_t* pData, uint32_t size, Type in_type, const VertexElements& vertexElements)
    {
        return nullptr;
    }

    OShaderRef Shader::createFromSource(const std::string& source, Type in_type, const VertexElements& vertexElements)
    {
        // Set the shader type
        if (in_type == OVertexShader)
        {
            auto parsed = parseVertexShader(source);
            std::string source;
            source.reserve(5000);

            // Add engine default constant buffers
            source += "#version " SHADER_VERSION "\n\n";
            source += "uniform mat4 oViewProjection;\n\n";

            // Add vertex elements
            VertexElements vertexElements;
            std::string elementStructsSource;
            int semanticIndex = 0;
            for (auto& element : parsed.inputs)
            {
                vertexElements.push_back({ element.type, "INPUT_ELEMENT" });
                switch (element.type)
                {
                //case VarType::Float: elementStructsSource += "layout(location = " + std::to_string(semanticIndex) + ") in float "; break;
                //case VarType::Float2: elementStructsSource += "layout(location = " + std::to_string(semanticIndex) + ") in vec2 "; break;
                //case VarType::Float3: elementStructsSource += "layout(location = " + std::to_string(semanticIndex) + ") in vec3 "; break;
                //case VarType::Float4: elementStructsSource += "layout(location = " + std::to_string(semanticIndex) + ") in vec4 "; break;
                case VarType::Float: elementStructsSource += "attribute float "; break;
                case VarType::Float2: elementStructsSource += "attribute vec2 "; break;
                case VarType::Float3: elementStructsSource += "attribute vec3 "; break;
                case VarType::Float4: elementStructsSource += "attribute vec4 "; break;
                default: assert(false);
                }
                elementStructsSource += element.name + ";\n";
                ++semanticIndex;
            }
            elementStructsSource += "\n";

            // Outputs
            semanticIndex = 0;
            for (auto& element : parsed.outputs)
            {
                switch (element.type)
                {
                //case VarType::Float: elementStructsSource += "layout(location = " + std::to_string(semanticIndex) + ") out float "; break;
                //case VarType::Float2: elementStructsSource += "layout(location = " + std::to_string(semanticIndex) + ") out vec2 "; break;
                //case VarType::Float3: elementStructsSource += "layout(location = " + std::to_string(semanticIndex) + ") out vec3 "; break;
                //case VarType::Float4: elementStructsSource += "layout(location = " + std::to_string(semanticIndex) + ") out vec4 "; break;
                case VarType::Float: elementStructsSource += "varying float "; break;
                case VarType::Float2: elementStructsSource += "varying vec2 "; break;
                case VarType::Float3: elementStructsSource += "varying vec3 "; break;
                case VarType::Float4: elementStructsSource += "varying vec4 "; break;
                default: assert(false);
                }
                elementStructsSource += "VARYING_ELEMENT" + std::to_string(semanticIndex) + ";\n";
                ++semanticIndex;
            }
            elementStructsSource += "\n";
            source += elementStructsSource;

            // Create uniforms
            ShaderGL::Uniforms uniforms;
            for (auto it = parsed.uniforms.begin(); it != parsed.uniforms.end(); ++it)
            {
                ShaderGL::Uniform uniform;
                uniform.name = it->name;
                uniform.type = it->type;
                source += "uniform " + getTypeName(it->type) + " " + uniform.name + ";\n";
                uniforms.push_back(uniform);
            }

            // Bake structures
            for (const auto& _struct : parsed.structs)
            {
                source += "struct " + _struct.name + "\n";
                source += "{\n";
                for (const auto& member : _struct.members)
                {
                    if (member.type == VarType::Unknown) source += "    " + member.typeName;
                    else source += "    " + getTypeName(member.type);
                    source += " " + member.name + ";\n";
                }
                source += "};\n\n";
            }

            // Bake constants
            for (const auto& _const : parsed.consts)
            {
                source += "const " + getTypeName(_const.type) + " " + _const.name;
                if (_const.isArray)
                {
                    source += "[int(";
                    bakeTokens(source, _const.arrayArguments);
                    source += ")] = " + getTypeName(_const.type) + "[](";
                    bakeTokens(source, _const.arguments);
                    source += ")";
                }
                else
                {
                    source += " = ";
                    bakeTokens(source, _const.arguments);
                }
                source += ";\n\n";
            }

            // Bake functions
            for (const auto& _function : parsed.functions)
            {
                if (_function.type == VarType::Unknown) source += _function.typeName;
                else source += getTypeName(_function.type);
                source += " " + _function.name + "(";
                bakeTokens(source, _function.arguments);
                source += ")\n{\n";
                bakeTokens(source, _function.body);
                source += "}\n\n";
            }

            // Bake main function
            source += "void main()\n{    vec4 oPosition;\n";
            semanticIndex = 0;
            for (auto& element : parsed.outputs)
            {
                switch (element.type)
                {
                case VarType::Float: source += "float "; break;
                case VarType::Float2: source += "vec2 "; break;
                case VarType::Float3: source += "vec3 "; break;
                case VarType::Float4: source += "vec4 "; break;
                default: assert(false);
                }
                source += element.name + ";\n";
                ++semanticIndex;
            }
            bakeTokens(source, parsed.mainFunction.body);
            semanticIndex = 0;
            for (auto& element : parsed.outputs)
            {
                source += "VARYING_ELEMENT" + std::to_string(semanticIndex) + " = " + element.name + ";\n";
                ++semanticIndex;
            }
            source += "    gl_Position = oPosition;\n}\n";

            // Now compile it
            auto pRet = createFromNativeSource(source, Type::Vertex, vertexElements);

            // Attributes
            int i = 0;
            for (auto& element : parsed.inputs)
            {
                ShaderGL::Attribute attribute;
                attribute.name = element.name;
                attribute.type = element.type;
                attribute.index = i++;
                ((ShaderGL*)(pRet.get()))->m_inputLayout.push_back(attribute);
            }
            ((ShaderGL*)(pRet.get()))->m_uniforms = uniforms;

#if defined(_DEBUG)
            ((ShaderGL*)(pRet.get()))->m_source = std::move(source);
#endif
            return pRet;
        }
        else if (in_type == OPixelShader)
        {
            auto parsed = parsePixelShader(source);
            std::string source;
            source.reserve(5000);

            source += "#version " SHADER_VERSION "\n\n";
            //source += "layout( location = 0 ) out vec4 oColor;\n\n";
            //source += "out vec4 oColor;\n\n";

            // Create textures
            int semanticIndex = 0;
            for (auto rit = parsed.textures.rbegin(); rit != parsed.textures.rend(); ++rit)
            {
                const auto& texture = *rit;
                //source += "layout(binding = " + std::to_string(texture.index) + ") uniform sampler2D sampler_" + texture.name + ";\n";
                source += "uniform sampler2D sampler_" + texture.name + ";\n";
                source += "vec4 " + texture.name + "(vec2 uv)\n";
                source += "{\n";
                source += "    return texture2D(sampler_" + texture.name + ", uv);\n";
                source += "}\n\n";
                ++semanticIndex;
            }

            // Inputs
            std::string elementStructsSource;
            semanticIndex = 0;
            for (auto& element : parsed.inputs)
            {
                switch (element.type)
                {
                //case VarType::Float: elementStructsSource += "layout(location = " + std::to_string(semanticIndex) + ") in float "; break;
                //case VarType::Float2: elementStructsSource += "layout(location = " + std::to_string(semanticIndex) + ") in vec2 "; break;
                //case VarType::Float3: elementStructsSource += "layout(location = " + std::to_string(semanticIndex) + ") in vec3 "; break;
                //case VarType::Float4: elementStructsSource += "layout(location = " + std::to_string(semanticIndex) + ") in vec4 "; break;
                case VarType::Float: elementStructsSource += "varying float "; break;
                case VarType::Float2: elementStructsSource += "varying vec2 "; break;
                case VarType::Float3: elementStructsSource += "varying vec3 "; break;
                case VarType::Float4: elementStructsSource += "varying vec4 "; break;
                default: assert(false);
                }
                elementStructsSource += "VARYING_ELEMENT" + std::to_string(semanticIndex) + ";\n";
                ++semanticIndex;
            }
            elementStructsSource += "\n";
            source += elementStructsSource;

            // Create uniforms
            ShaderGL::Uniforms uniforms;
            for (auto it = parsed.uniforms.begin(); it != parsed.uniforms.end(); ++it)
            {
                ShaderGL::Uniform uniform;
                uniform.name = it->name;
                uniform.type = it->type;
                source += "uniform " + getTypeName(it->type) + " " + uniform.name + "_PS;\n";
                source += getTypeName(it->type) + " " + uniform.name + " = " + uniform.name + "_PS;\n";
                uniforms.push_back(uniform);
            }

            // Bake structures
            for (const auto& _struct : parsed.structs)
            {
                source += "struct " + _struct.name + "\n";
                source += "{\n";
                for (const auto& member : _struct.members)
                {
                    if (member.type == VarType::Unknown) source += "    " + member.typeName;
                    else source += "    " + getTypeName(member.type);
                    source += " " + member.name + ";\n";
                }
                source += "};\n\n";
            }

            // Bake constants
            for (const auto& _const : parsed.consts)
            {
                source += "const " + getTypeName(_const.type) + " " + _const.name;
                if (_const.isArray)
                {
                    source += "[int(";
                    bakeTokens(source, _const.arrayArguments);
                    source += ")] = " + getTypeName(_const.type) + "[](";
                    bakeTokens(source, _const.arguments);
                    source += ")";
                }
                else
                {
                    source += " = ";
                    bakeTokens(source, _const.arguments);
                }
                source += ";\n\n";
            }

            // Bake functions
            for (const auto& _function : parsed.functions)
            {
                if (_function.type == VarType::Unknown) source += _function.typeName;
                else source += getTypeName(_function.type);
                source += " " + _function.name + "(";
                bakeTokens(source, _function.arguments);
                source += ")\n{\n";
                bakeTokens(source, _function.body);
                source += "}\n\n";
            }

            // Bake main function
            source += "void main()\n{    vec4 oColor;\n";
            semanticIndex = 0;
            for (auto& element : parsed.inputs)
            {
                switch (element.type)
                {
                case VarType::Float: source += "float "; break;
                case VarType::Float2: source += "vec2 "; break;
                case VarType::Float3: source += "vec3 "; break;
                case VarType::Float4: source += "vec4 "; break;
                default: assert(false);
                }
                source += element.name + " = VARYING_ELEMENT" + std::to_string(semanticIndex) + ";\n";
                ++semanticIndex;
            }
            bakeTokens(source, parsed.mainFunction.body);
            source += "    gl_FragColor = oColor;}\n";

            // Now compile it
            auto pRet = createFromNativeSource(source, Type::Pixel);
            ((ShaderGL*)(pRet.get()))->m_uniforms = uniforms;
            ((ShaderGL*)(pRet.get()))->m_textures = parsed.textures;
#if defined(_DEBUG)
            ((ShaderGL*)(pRet.get()))->m_source = std::move(source);
#endif
            return pRet;
        }
        else
        {
            assert(false);
        }

        return nullptr;
    }

    OShaderRef Shader::createFromNativeSource(const std::string& source, Type in_type, const VertexElements& vertexElements)
    {
        auto pRet = std::make_shared<ShaderGL>();
        pRet->m_type = in_type;
        if (in_type == OVertexShader)
        {
            pRet->m_shader = glCreateShader(GL_VERTEX_SHADER);

            auto szSource = source.c_str();
            glShaderSource(pRet->m_shader, 1, (const GLchar**)&szSource, NULL);
            glCompileShader(pRet->m_shader);
            GLint bResult;
            glGetShaderiv(pRet->m_shader, GL_COMPILE_STATUS, &bResult);
            if (bResult == GL_FALSE)
            {
                GLchar infoLog[1024];
                glGetShaderInfoLog(pRet->m_shader, 1023, NULL, infoLog);
#if defined(_DEBUG)
                auto split = onut::splitString(source, '\n', false);
                int i = 1;
                for (const auto& line : split)
                {
                    OLog(std::to_string(i++) + ": " + line);
                }
#endif
                OLogE("VS compile failed: " + std::string(infoLog));
                assert(false);
                return nullptr;
            }
            else
            {
                GLchar infoLog[1024];
                glGetShaderInfoLog(pRet->m_shader, 1023, NULL, infoLog);
                if (strlen(infoLog))
                {
                    OLogW("VS compiled: " + std::string(infoLog));
                }
            }
        }
        else if (in_type == OPixelShader)
        {
            pRet->m_shader = glCreateShader(GL_FRAGMENT_SHADER);

            auto szSource = source.c_str();
            glShaderSource(pRet->m_shader, 1, (const GLchar**)&szSource, NULL);
            glCompileShader(pRet->m_shader);
            GLint bResult;
            glGetShaderiv(pRet->m_shader, GL_COMPILE_STATUS, &bResult);
            if (bResult == GL_FALSE)
            {
                GLchar infoLog[1024];
                glGetShaderInfoLog(pRet->m_shader, 1023, NULL, infoLog);
#if defined(_DEBUG)
                auto split = onut::splitString(source, '\n', false);
                int i = 1;
                for (const auto& line : split)
                {
                    OLog(std::to_string(i++) + ": " + line);
                }
#endif
                OLogE("PS compile failed: " + std::string(infoLog));
                assert(false);
                return nullptr;
            }
            else
            {
                GLchar infoLog[1024];
                glGetShaderInfoLog(pRet->m_shader, 1023, NULL, infoLog);
                if (strlen(infoLog))
                {
                    OLogW("PS compiled: " + std::string(infoLog));
                }
            }
        }
        else
        {
            assert(false);
            return nullptr;
        }

        return pRet;
    }

    ShaderGL::~ShaderGL()
    {
        if (m_shader)
        {
            glDeleteShader(m_shader);
            m_shader = 0;
        }
    }

    ShaderGL::ShaderGL()
    {
    }

    int ShaderGL::getUniformId(const std::string& varName) const
    {
        for (int i = 0; i < (int)m_uniforms.size(); ++i)
        {
            if (m_uniforms[i].name == varName)
            {
                return i;
            }
        }
        assert(false);
        return -1;
    }

    void ShaderGL::setFloat(int varId, float value)
    {
        auto& uniform = m_uniforms[varId];
        uniform.dirty = true;
        uniform.value._11 = value;
    }

    void ShaderGL::setVector2(int varId, const Vector2& value)
    {
        auto& uniform = m_uniforms[varId];
        uniform.dirty = true;
        uniform.value._11 = value.x;
        uniform.value._12 = value.y;
    }

    void ShaderGL::setVector3(int varId, const Vector3& value)
    {
        auto& uniform = m_uniforms[varId];
        uniform.dirty = true;
        uniform.value._11 = value.x;
        uniform.value._12 = value.y;
        uniform.value._13 = value.z;
    }

    void ShaderGL::setVector4(int varId, const Vector4& value)
    {
        auto& uniform = m_uniforms[varId];
        uniform.dirty = true;
        uniform.value._11 = value.x;
        uniform.value._12 = value.y;
        uniform.value._13 = value.z;
        uniform.value._14 = value.w;
    }

    void ShaderGL::setMatrix(int varId, const Matrix& value)
    {
        auto& uniform = m_uniforms[varId];
        uniform.dirty = true;
        uniform.value = value;
    }

    void ShaderGL::setFloat(const std::string& varName, float value)
    {
        setFloat(getUniformId(varName), value);
    }

    void ShaderGL::setVector2(const std::string& varName, const Vector2& value)
    {
        setVector2(getUniformId(varName), value);
    }

    void ShaderGL::setVector3(const std::string& varName, const Vector3& value)
    {
        setVector3(getUniformId(varName), value);
    }

    void ShaderGL::setVector4(const std::string& varName, const Vector4& value)
    {
        setVector4(getUniformId(varName), value);
    }

    void ShaderGL::setMatrix(const std::string& varName, const Matrix& value)
    {
        setMatrix(getUniformId(varName), value);
    }

    ShaderGL::Uniforms& ShaderGL::getUniforms()
    {
        return m_uniforms;
    }

    const ShaderGL::ProgramRef& ShaderGL::getProgram(const OShaderGLRef& pVertexShader)
    {
        for (auto it = m_programs.begin(); it != m_programs.end();)
        {
            const auto& pProgram = *it;
            auto pVertexShaderRef = pProgram->pVertexShader.lock();
            if (!pVertexShaderRef)
            {
                // Hum remove that program
                it = m_programs.erase(it);
                continue;
            }
            if (pVertexShaderRef == pVertexShader)
            {
                return pProgram;
            }
            ++it;
        }

        GLenum err;

        // It doesnt exist, create a program
        auto program = glCreateProgram();
        if (!program)
        {
            err = glGetError();
            if (err != GL_NO_ERROR)
            {
                OLogE("glCreateProgram: " + std::to_string(err));
            }
        }

        glAttachShader(program, pVertexShader->m_shader);
        glAttachShader(program, m_shader);

        auto pProgram = OMake<Program>();
        auto pProgramRaw = pProgram.get();
        pProgramRaw->pVertexShader = pVertexShader;
        pProgramRaw->program = program;
        auto pVertexShaderRaw = (ShaderGL*)pVertexShader.get();

        // Find attributes
        pProgramRaw->attributes.resize(m_inputLayout.size());
        for (const auto& attribute : pVertexShaderRaw->m_inputLayout)
        {
            //pProgramRaw->attributes[attribute.index] = glGetAttribLocation(program, attribute.name.c_str());
            glBindAttribLocation(program, attribute.index, attribute.name.c_str());
        }

        glLinkProgram(program);

        // Now find all uniforms
        pProgram->oViewProjectionUniform = glGetUniformLocation(program, "oViewProjection");
        for (const auto& uniform : pVertexShaderRaw->m_uniforms)
        {
            pProgramRaw->uniformsVS.push_back(glGetUniformLocation(program, uniform.name.c_str()));
        }
        for (const auto& uniform : m_uniforms)
        {
            pProgramRaw->uniformsPS.push_back(glGetUniformLocation(program, (uniform.name + "_PS").c_str()));
        }

        // Find texture locations
        pProgramRaw->textures.resize(m_textures.size());
        for (const auto& texture : m_textures)
        {
            pProgramRaw->textures[texture.index] = glGetUniformLocation(program, ("sampler_" + texture.name).c_str());
        }

        m_programs.push_back(pProgram);

        return m_programs.back();
    }
};
