// Onut
#include <onut/ContentManager.h>
#include <onut/Files.h>
#include <onut/Log.h>
#include <onut/Renderer.h>
#include <onut/Strings.h>

// Private
#include "ShaderGL.h"
#include "RendererGL.h"

// Third parties
//#include <D3Dcompiler.h>

// STL
#include <cassert>
#include <regex>

namespace onut
{
    OShaderRef Shader::createFromBinaryFile(const std::string& filename, Type in_type, const VertexElements& vertexElements)
    {
        auto pRet = std::make_shared<OShaderGL>();
        pRet->m_type = in_type;
        assert(false);
        return pRet;
    }

    OShaderRef Shader::createFromBinaryData(const uint8_t* pData, uint32_t size, Type in_type, const VertexElements& vertexElements)
    {
        return nullptr;
    }

    OShaderRef Shader::createFromSource(const std::string& source, Type in_type, const VertexElements& vertexElements)
    {
        return nullptr;
    }

    ShaderGL::~ShaderGL()
    {
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
