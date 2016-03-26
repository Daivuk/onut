#if defined(WIN32)
// Onut
#include <onut/ContentManager.h>
#include <onut/Renderer.h>

// Private
#include "ShaderD3D11.h"
#include "RendererD3D11.h"

// STL
#include <cassert>

namespace onut
{
    //ID3D11PixelShader* RendererD3D11::create2DShader(const std::string& filename)
    //{
    //    ID3D11PixelShader* pRet = nullptr;

    //    std::ifstream psFile(filename, std::ios::binary);
    //    std::vector<char> psData = {std::istreambuf_iterator<char>(psFile), std::istreambuf_iterator<char>()};

    //    auto ret = m_pDevice->CreatePixelShader(psData.data(), psData.size(), nullptr, &pRet);
    //    assert(ret == S_OK);

    //    return pRet;
    //}

    OShaderRef Shader::createFromBinaryFile(const std::string& filename, Type in_type)
    {
        auto pRet = std::make_shared<OShaderD3D11>();
        pRet->m_type = in_type;
        assert(false);
        return pRet;
    }

    OShaderRef Shader::createFromSourceFile(const std::string& filename, Type in_type)
    {
        auto pRet = std::make_shared<OShaderD3D11>();
        pRet->m_type = in_type;
        assert(false);
        return pRet;
    }

    OShaderRef Shader::createFromBinaryData(const uint8_t* pData, uint32_t size, Type in_type)
    {
        auto pRet = std::make_shared<OShaderD3D11>();
        pRet->m_type = in_type;
        auto pRendererD3D11 = std::dynamic_pointer_cast<ORendererD3D11>(oRenderer);

        HRESULT ret = S_FALSE;
        switch (pRet->getType())
        {
            case Type::Vertex:
                ret = pRendererD3D11->getDevice()->CreateVertexShader(pData, static_cast<SIZE_T>(size), nullptr, &pRet->m_pVertexShader);
                break;
            case Type::Pixel:
                ret = pRendererD3D11->getDevice()->CreatePixelShader(pData, static_cast<SIZE_T>(size), nullptr, &pRet->m_pPixelShader);
                break;
        }
        assert(ret == S_OK);

        return pRet;
    }

    OShaderRef Shader::createFromSource(const std::string& source, Type in_type)
    {
        auto pRet = std::make_shared<OShaderD3D11>();
        pRet->m_type = in_type;
        assert(false);
        return pRet;
    }

    ShaderD3D11::~ShaderD3D11()
    {
        if (m_pVertexShader)
        {
            m_pVertexShader->Release();
        }
    }

    ShaderD3D11::ShaderD3D11()
    {
        if (m_pPixelShader)
        {
            m_pPixelShader->Release();
        }
    }

    ID3D11VertexShader* ShaderD3D11::getVertexShader() const
    {
        return m_pVertexShader;
    }

    ID3D11PixelShader* ShaderD3D11::getPixelShader() const
    {
        return m_pPixelShader;
    }
};

#endif
