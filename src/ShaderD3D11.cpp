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
    //ID3D11PixelShader* RendererD3D11::create2DShader(const std::string& filename, const VertexElements& vertexElements)
    //{
    //    ID3D11PixelShader* pRet = nullptr;

    //    std::ifstream psFile(filename, std::ios::binary);
    //    std::vector<char> psData = {std::istreambuf_iterator<char>(psFile), std::istreambuf_iterator<char>()};

    //    auto ret = m_pDevice->CreatePixelShader(psData.data(), psData.size(), nullptr, &pRet);
    //    assert(ret == S_OK);

    //    return pRet;
    //}

    OShaderRef Shader::createFromBinaryFile(const std::string& filename, Type in_type, const VertexElements& vertexElements)
    {
        auto pRet = std::make_shared<OShaderD3D11>();
        pRet->m_type = in_type;
        assert(false);
        return pRet;
    }

    OShaderRef Shader::createFromSourceFile(const std::string& filename, Type in_type, const VertexElements& vertexElements)
    {
        auto pRet = std::make_shared<OShaderD3D11>();
        pRet->m_type = in_type;
        assert(false);
        return pRet;
    }

    OShaderRef Shader::createFromBinaryData(const uint8_t* pData, uint32_t size, Type in_type, const VertexElements& vertexElements)
    {
        auto pRet = std::make_shared<OShaderD3D11>();
        pRet->m_type = in_type;
        auto pRendererD3D11 = std::dynamic_pointer_cast<ORendererD3D11>(oRenderer);

        HRESULT ret = S_FALSE;
        switch (pRet->getType())
        {
            case Type::Vertex:
            {
                ret = pRendererD3D11->getDevice()->CreateVertexShader(pData, static_cast<SIZE_T>(size), nullptr, &pRet->m_pVertexShader);
                assert(ret == S_OK);

                // Create input layout
                assert(!vertexElements.empty());
                std::vector<D3D11_INPUT_ELEMENT_DESC> layout;
                std::unordered_map<std::string, UINT> semanticIndexes;
                for (auto& element : vertexElements)
                {
                    DXGI_FORMAT format;
                    switch (element.size)
                    {
                        case 1:
                            format = DXGI_FORMAT_R32_FLOAT;
                            break;
                        case 2:
                            format = DXGI_FORMAT_R32G32_FLOAT;
                            break;
                        case 3:
                            format = DXGI_FORMAT_R32G32B32_FLOAT;
                            break;
                        case 4:
                            format = DXGI_FORMAT_R32G32B32A32_FLOAT;
                            break;
                        default:
                            assert(false);
                    }

                    pRet->m_vertexSize += element.size * 4;
                    
                    D3D11_INPUT_ELEMENT_DESC inputElement = {
                        element.semanticName.c_str(), semanticIndexes[element.semanticName], 
                        format, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0
                    };
                    semanticIndexes[element.semanticName]++;
                    layout.push_back(inputElement);
                }
                auto ret = pRendererD3D11->getDevice()->CreateInputLayout(layout.data(), 
                                                                          static_cast<UINT>(vertexElements.size()),
                                                                          pData,
                                                                          static_cast<SIZE_T>(size),
                                                                          &pRet->m_pInputLayout);
                assert(ret == S_OK);
                break;
            }
            case Type::Pixel:
            {
                ret = pRendererD3D11->getDevice()->CreatePixelShader(pData, static_cast<SIZE_T>(size), nullptr, &pRet->m_pPixelShader);
                assert(ret == S_OK);
                break;
            }
        }

        return pRet;
    }

    OShaderRef Shader::createFromSource(const std::string& source, Type in_type, const VertexElements& vertexElements)
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
        if (m_pPixelShader)
        {
            m_pPixelShader->Release();
        }
        if (m_pInputLayout)
        {
            m_pInputLayout->Release();
        }
    }

    ShaderD3D11::ShaderD3D11()
    {
    }

    ID3D11VertexShader* ShaderD3D11::getVertexShader() const
    {
        return m_pVertexShader;
    }

    ID3D11PixelShader* ShaderD3D11::getPixelShader() const
    {
        return m_pPixelShader;
    }

    ID3D11InputLayout* ShaderD3D11::getInputLayout() const
    {
        return m_pInputLayout;
    }
};

#endif
