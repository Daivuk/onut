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

namespace onut
{
    OShaderRef Shader::createFromBinaryFile(const std::string& filename, Type in_type, const VertexElements& vertexElements)
    {
        return nullptr;
    }

    OShaderRef Shader::createFromBinaryData(const uint8_t* pData, uint32_t size, Type in_type, const VertexElements& vertexElements)
    {
        return nullptr;
    }

    static void replaceTypes(std::string& source)
    {
        onut::replace(source, "\\bfloat2\\b", "vec2");
        onut::replace(source, "\\bfloat3\\b", "vec3");
        onut::replace(source, "\\bfloat4\\b", "vec4");
        onut::replace(source, "\\bmatrix\\b", "mat4");
    }

    static void replaceIntrisics(std::string& source)
    {
        onut::replace(source, "\\bmul\\b\\s*\\((.*)\\s*,\\s*(.*)\\)", "($2 * $1)");
        onut::replace(source, "\\bsaturate\\b\\s*\\((.*)\\)", "clamp($1, 0.0, 1.0)");
        onut::replace(source, "\\blerp\\b", "mix");
        onut::replace(source, "\\bfrac\\b", "fract");
        onut::replace(source, "\\brsqrt\\b", "inversesqrt");
    }

    OShaderRef Shader::createFromSource(const std::string& source, Type in_type, const VertexElements& vertexElements)
    {
        // Set the shader type
        if (in_type == OVertexShader)
        {
            auto parsed = parseVertexShader(source);
            std::smatch match;

            // Add engine default constant buffers
            if (std::regex_search(parsed.source, std::regex("\\boViewProjection\\b")))
            {
                parsed.source.insert(0, "uniform mat4 oViewProjection;\n\n");
            }

            // Replace element variables with structs
            onut::replace(parsed.source, "\\boPosition\\b", "gl_Position");

            // Add vertex elements
            VertexElements vertexElements;
            std::string elementStructsSource;
            for (auto& element : parsed.inputs)
            {
                vertexElements.push_back({ element.size, "INPUT_ELEMENT" });
                switch (element.size)
                {
                case 1: elementStructsSource += "attribute float "; break;
                case 2: elementStructsSource += "attribute vec2 "; break;
                case 3: elementStructsSource += "attribute vec3 "; break;
                case 4: elementStructsSource += "attribute vec4 "; break;
                default: assert(false);
                }
                elementStructsSource += element.name + ";\n";
            }
            elementStructsSource += "\n";

            // Outputs
            for (auto& element : parsed.outputs)
            {
                switch (element.size)
                {
                case 1: elementStructsSource += "varying float "; break;
                case 2: elementStructsSource += "varying vec2 "; break;
                case 3: elementStructsSource += "varying vec3 "; break;
                case 4: elementStructsSource += "varying vec4 "; break;
                default: assert(false);
                }
                elementStructsSource += element.name + ";\n";
            }
            elementStructsSource += "\n";

            parsed.source.insert(0, elementStructsSource);

            // Create uniforms
            ShaderGL::Uniforms uniforms;
            for (auto it = parsed.uniforms.rbegin(); it != parsed.uniforms.rend(); ++it)
            {
                ShaderGL::Uniform uniform;
                uniform.name = it->name;
                parsed.source.insert(0, "uniform " + it->type + " " + uniform.name + ";\n");
                uniforms.push_back(uniform);
            }

            // Replace types and intrinsics
            replaceTypes(parsed.source);
            replaceIntrisics(parsed.source);

            // Now compile it
            auto pRet = createFromNativeSource(parsed.source, Type::Vertex, vertexElements);

            return pRet;
        }
        else if (in_type == OPixelShader)
        {
            auto parsed = parsePixelShader(source);
            std::smatch match;

            // Replace element variables with structs
            onut::replace(parsed.source, "\\boColor\\b", "gl_FragColor");

            // Replace texture sampling
            for (auto& texture : parsed.textures)
            {
                onut::replace(parsed.source, "(\\b" + texture.name + "\\b)\\s*\\(", "texture2D(" + texture.name + ", ");
            }

            // Inputs
            std::string elementStructsSource;
            for (auto& element : parsed.inputs)
            {
                switch (element.size)
                {
                case 1: elementStructsSource += "varying float "; break;
                case 2: elementStructsSource += "varying float2 "; break;
                case 3: elementStructsSource += "varying float3 "; break;
                case 4: elementStructsSource += "varying float4 "; break;
                default: assert(false);
                }
                elementStructsSource += element.name + ";\n";
            }
            elementStructsSource += "\n";
            parsed.source.insert(0, elementStructsSource);

            // Create uniforms
            ShaderGL::Uniforms uniforms;
            for (auto it = parsed.uniforms.rbegin(); it != parsed.uniforms.rend(); ++it)
            {
                ShaderGL::Uniform uniform;
                uniform.name = it->name;
                parsed.source.insert(0, "uniform " + it->type + " " + uniform.name + ";\n");
                uniforms.push_back(uniform);
            }

            // Create textures
            for (auto rit = parsed.textures.rbegin(); rit != parsed.textures.rend(); ++rit)
            {
                const auto& texture = *rit;

                std::string textureStr = "uniform sampler2D " + texture.name + ";\n";
                parsed.source.insert(0, textureStr);
            }

            // Replace types and intrinsics
            replaceTypes(parsed.source);
            replaceIntrisics(parsed.source);

            // Now compile it
            auto pRet = createFromNativeSource(parsed.source, Type::Pixel);

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
                glGetShaderInfoLog(pRet->m_shader, 1024, NULL, infoLog);
                OLog("VS compile failed: " + std::string(infoLog));
                assert(false);
                return nullptr;
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
                glGetShaderInfoLog(pRet->m_shader, 1024, NULL, infoLog);
                OLog("PS compile failed: " + std::string(infoLog));
                assert(false);
                return nullptr;
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
        return -1;
    }

    void ShaderGL::setFloat(int varId, float value)
    {
    /*    auto& uniform = m_uniforms[varId];
        uniform.dirty = true;
        auto pBuffer = uniform.pBuffer;
        auto pRendererD3D11 = std::dynamic_pointer_cast<ORendererD3D11>(oRenderer);
        auto pDeviceContext = pRendererD3D11->getDeviceContext();

        D3D11_MAPPED_SUBRESOURCE map;
        pDeviceContext->Map(pBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &map);
        float values[4] = {value, 0, 0, 0};
        memcpy(map.pData, values, 16);
        pDeviceContext->Unmap(pBuffer, 0);*/
    }

    void ShaderGL::setVector2(int varId, const Vector2& value)
    {
  /*      auto& uniform = m_uniforms[varId];
        uniform.dirty = true;
        auto pBuffer = uniform.pBuffer;
        auto pRendererD3D11 = std::dynamic_pointer_cast<ORendererD3D11>(oRenderer);
        auto pDeviceContext = pRendererD3D11->getDeviceContext();

        D3D11_MAPPED_SUBRESOURCE map;
        pDeviceContext->Map(pBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &map);
        float values[4] = {value.x, value.y, 0, 0};
        memcpy(map.pData, values, 16);
        pDeviceContext->Unmap(pBuffer, 0);*/
    }

    void ShaderGL::setVector3(int varId, const Vector3& value)
    {
    /*    auto& uniform = m_uniforms[varId];
        uniform.dirty = true;
        auto pBuffer = uniform.pBuffer;
        auto pRendererD3D11 = std::dynamic_pointer_cast<ORendererD3D11>(oRenderer);
        auto pDeviceContext = pRendererD3D11->getDeviceContext();

        D3D11_MAPPED_SUBRESOURCE map;
        pDeviceContext->Map(pBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &map);
        float values[4] = {value.x, value.y, value.z, 0};
        memcpy(map.pData, values, 16);
        pDeviceContext->Unmap(pBuffer, 0);*/
    }

    void ShaderGL::setVector4(int varId, const Vector4& value)
    {
     /*   auto& uniform = m_uniforms[varId];
        uniform.dirty = true;
        auto pBuffer = uniform.pBuffer;
        auto pRendererD3D11 = std::dynamic_pointer_cast<ORendererD3D11>(oRenderer);
        auto pDeviceContext = pRendererD3D11->getDeviceContext();

        D3D11_MAPPED_SUBRESOURCE map;
        pDeviceContext->Map(pBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &map);
        memcpy(map.pData, &value.x, 16);
        pDeviceContext->Unmap(pBuffer, 0);*/
    }

    void ShaderGL::setMatrix(int varId, const Matrix& value)
    {
    /*    auto& uniform = m_uniforms[varId];
        uniform.dirty = true;
        auto pBuffer = uniform.pBuffer;
        auto pRendererD3D11 = std::dynamic_pointer_cast<ORendererD3D11>(oRenderer);
        auto pDeviceContext = pRendererD3D11->getDeviceContext();

        D3D11_MAPPED_SUBRESOURCE map;
        pDeviceContext->Map(pBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &map);
        memcpy(map.pData, &value._11, 64);
        pDeviceContext->Unmap(pBuffer, 0);*/
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
};
