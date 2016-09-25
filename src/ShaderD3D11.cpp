#if defined(WIN32)
// Onut
#include <onut/ContentManager.h>
#include <onut/Files.h>
#include <onut/Log.h>
#include <onut/Renderer.h>
#include <onut/Strings.h>

// Private
#include "ShaderD3D11.h"
#include "RendererD3D11.h"

// Third parties
#include <D3Dcompiler.h>

// STL
#include <cassert>
#include <regex>

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

    OShaderRef Shader::createFromFile(const std::string& filename, const OContentManagerRef& pContentManager)
    {
        auto pRendererD3D11 = std::dynamic_pointer_cast<ORendererD3D11>(oRenderer);
        auto pDevice = pRendererD3D11->getDevice();

        // Set the shader type
        auto ext = onut::getExtension(filename);
        if (ext == "VS")
        {
            auto parsed = parseVertexShader(filename);
            std::smatch match;

            // Add engine default constant buffers
            if (std::regex_search(parsed.source, std::regex("\\boViewProjection\\b")))
            {
                parsed.source.insert(0, "cbuffer OViewProjection : register(b0)\n{\n    matrix oViewProjection;\n}\n\n");
            }

            // Replace main
            if (std::regex_search(parsed.source, match, std::regex("void\\s+main\\s*\\(\\s*\\)\\s*\\{")))
            {
                parsed.source[match.position()] = '/';
                parsed.source[match.position() + 1] = '*';
                parsed.source[match.position() + match.length() - 2] = '*';
                parsed.source[match.position() + match.length() - 1] = '/';
                parsed.source.insert(match.position(), "oVSOutput main(oVSInput oInput)\n{\n    oVSOutput oOutput;\n");

                size_t offset = match.position() + match.length();
                if (std::regex_search(parsed.source.cbegin() + offset, parsed.source.cend(), match, std::regex("\\}")))
                {
                    parsed.source.insert(offset + match.position(), "    return oOutput;\n");
                }
                else assert(false); // No matching closing } to main function
            }
            else assert(false); // Shader should include "void main()"

            // Replace element variables with structs
            for (auto& element : parsed.inputs)
            {
                onut::replace(parsed.source, "\\b" + element.name + "\\b", "oInput." + element.name);
            }
            for (auto& element : parsed.outputs)
            {
                onut::replace(parsed.source, "\\b" + element.name + "\\b", "oOutput." + element.name);
            }
            onut::replace(parsed.source, "\\boPosition\\b", "oOutput.position");

            // Add vertex elements
            VertexElements vertexElements;
            std::string elementStructsSource;
            elementStructsSource += "struct oVSInput\n{\n";
            int semanticIndex = 0;
            for (auto& element : parsed.inputs)
            {
                std::string semanticName = "INPUT_ELEMENT" + std::to_string(semanticIndex++);
                vertexElements.push_back({element.size, "INPUT_ELEMENT"});
                switch (element.size)
                {
                    case 1: elementStructsSource += "    float "; break;
                    case 2: elementStructsSource += "    float2 "; break;
                    case 3: elementStructsSource += "    float3 "; break;
                    case 4: elementStructsSource += "    float4 "; break;
                    default: assert(false);
                }
                elementStructsSource += element.name + " : " + semanticName + ";\n";
            }
            elementStructsSource += "};\n\n";

            // Outputs
            elementStructsSource += "struct oVSOutput\n{\n";
            elementStructsSource += "    float4 position : SV_POSITION;\n";
            semanticIndex = 0;
            for (auto& element : parsed.outputs)
            {
                std::string semanticName = "OUTPUT_ELEMENT" + std::to_string(semanticIndex++);
                switch (element.size)
                {
                    case 1: elementStructsSource += "    float "; break;
                    case 2: elementStructsSource += "    float2 "; break;
                    case 3: elementStructsSource += "    float3 "; break;
                    case 4: elementStructsSource += "    float4 "; break;
                    default: assert(false);
                }
                elementStructsSource += element.name + " : " + semanticName + ";\n";
            }
            elementStructsSource += "};\n\n";
            parsed.source.insert(0, elementStructsSource);

            // Create uniforms
            ShaderD3D11::Uniforms uniforms;
            int uniformId = 4;
            for (auto& parsedUniform : parsed.uniforms)
            {
                ShaderD3D11::Uniform uniform;

                uniform.name = parsedUniform.name;

                UINT uniformSize = 4;
                if (parsedUniform.type == "matrix") uniformSize = 16;

                D3D11_BUFFER_DESC cbDesc = CD3D11_BUFFER_DESC(uniformSize * 4, D3D11_BIND_CONSTANT_BUFFER, D3D11_USAGE_DYNAMIC, D3D11_CPU_ACCESS_WRITE);
                auto ret = pDevice->CreateBuffer(&cbDesc, NULL, &(uniform.pBuffer));
                assert(ret == S_OK);

                parsed.source.insert(0, "cbuffer cb_" + uniform.name + " : register(b" + std::to_string(uniformId) + ")\n{\n    " + parsedUniform.type + " " + uniform.name + ";\n}\n\n");

                uniforms.push_back(uniform);
                ++uniformId;
            }

            // Now compile it
            auto pRet = createFromSource(parsed.source, Type::Vertex, vertexElements);
            auto pRetD3D11 = ODynamicCast<ShaderD3D11>(pRet);
            if (pRetD3D11)
            {
                pRetD3D11->m_uniforms = uniforms;
            }

            return pRet;
        }
        else if (ext == "PS")
        {
            auto parsed = parsePixelShader(filename);
            std::smatch match;

            // Add engine default constant buffers

            // Replace main
            if (std::regex_search(parsed.source, match, std::regex("void\\s+main\\s*\\(\\s*\\)\\s*\\{")))
            {
                parsed.source[match.position()] = '/';
                parsed.source[match.position() + 1] = '*';
                parsed.source[match.position() + match.length() - 2] = '*';
                parsed.source[match.position() + match.length() - 1] = '/';
                parsed.source.insert(match.position(), "float4 main(oPSInput oInput) : SV_TARGET\n{\n    float4 oColor;\n");

                size_t offset = match.position() + match.length();
                if (std::regex_search(parsed.source.cbegin() + offset, parsed.source.cend(), match, std::regex("\\}")))
                {
                    parsed.source.insert(offset + match.position(), "    return oColor;\n");
                }
                else assert(false); // No matching closing } to main function
            }
            else assert(false); // Shader should include "void main()"

            // Replace element variables with structs
            for (auto& element : parsed.inputs)
            {
                onut::replace(parsed.source, "\\b" + element.name + "\\b", "oInput." + element.name);
            }

            // Replace texture sampling
            for (auto& texture : parsed.textures)
            {
                onut::replace(parsed.source, "(\\b" + texture.name + "\\b)\\s*\\(", "$1.Sample(sampler_" + texture.name + ", ");
            }

            // Inputs
            std::string elementStructsSource;
            elementStructsSource += "struct oPSInput\n{\n";
            elementStructsSource += "    float4 position : SV_POSITION;\n";
            int semanticIndex = 0;
            for (auto& element : parsed.inputs)
            {
                std::string semanticName = "OUTPUT_ELEMENT" + std::to_string(semanticIndex++);
                switch (element.size)
                {
                    case 1: elementStructsSource += "    float "; break;
                    case 2: elementStructsSource += "    float2 "; break;
                    case 3: elementStructsSource += "    float3 "; break;
                    case 4: elementStructsSource += "    float4 "; break;
                    default: assert(false);
                }
                elementStructsSource += element.name + " : " + semanticName + ";\n";
            }
            elementStructsSource += "};\n\n";
            parsed.source.insert(0, elementStructsSource);

            // Create uniforms
            ShaderD3D11::Uniforms uniforms;
            int uniformId = 4;
            for (auto& parsedUniform : parsed.uniforms)
            {
                ShaderD3D11::Uniform uniform;

                uniform.name = parsedUniform.name;

                UINT uniformSize = 4;
                if (parsedUniform.type == "matrix") uniformSize = 16;

                D3D11_BUFFER_DESC cbDesc = CD3D11_BUFFER_DESC(uniformSize * 4, D3D11_BIND_CONSTANT_BUFFER, D3D11_USAGE_DYNAMIC, D3D11_CPU_ACCESS_WRITE);
                auto ret = pDevice->CreateBuffer(&cbDesc, NULL, &(uniform.pBuffer));
                assert(ret == S_OK);

                parsed.source.insert(0, "cbuffer cb_" + uniform.name + " : register(b" + std::to_string(uniformId) + ")\n{\n    " + parsedUniform.type + " " + uniform.name + ";\n}\n\n");

                uniforms.push_back(uniform);
                ++uniformId;
            }

            // Create textures samplers
            for (auto& texture : parsed.textures)
            {
                std::string samplerStr = "Texture2D " + texture.name + " : register(t" + std::to_string(texture.index) + ");\n";
                samplerStr += "SamplerState sampler_" + texture.name + "\n{\n";
                switch (texture.filter)
                {
                    case ParsedTexture::Filter::Nearest:
                        samplerStr += "    Filter = MIN_MAG_MIP_POINT;\n";
                        break;
                    case ParsedTexture::Filter::Linear:
                        samplerStr += "    Filter = MIN_POINT_MAG_LINEAR_MIP_POINT;\n";
                        break;
                    case ParsedTexture::Filter::Bilinear:
                        samplerStr += "    Filter = MIN_MAG_LINEAR_MIP_POINT;\n";
                        break;
                    case ParsedTexture::Filter::Trilinear:
                        samplerStr += "    Filter = MIN_MAG_MIP_LINEAR;\n";
                        break;
                    case ParsedTexture::Filter::Anisotropic:
                        samplerStr += "    Filter = ANISOTROPIC;\n";
                        break;
                }
                switch (texture.repeatX)
                {
                    case ParsedTexture::Repeat::Clamp:
                        samplerStr += "    AddressU = Clamp;\n";
                        break;
                    case ParsedTexture::Repeat::Wrap:
                        samplerStr += "    AddressU = Wrap;\n";
                        break;
                }
                switch (texture.repeatY)
                {
                    case ParsedTexture::Repeat::Clamp:
                        samplerStr += "    AddressV = Clamp;\n";
                        break;
                    case ParsedTexture::Repeat::Wrap:
                        samplerStr += "    AddressV = Wrap;\n";
                        break;
                }
                samplerStr += "};\n\n";
                parsed.source.insert(0, samplerStr);
            }

            // Now compile it
            auto pRet = createFromSource(parsed.source, Type::Pixel);
            auto pRetD3D11 = ODynamicCast<ShaderD3D11>(pRet);
            if (pRetD3D11)
            {
                pRetD3D11->m_uniforms = uniforms;
            }

            return pRet;
        }
        else
        {
            assert(false);
        }

        return nullptr;
    }

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

    ID3DBlob* compileShader(const char *szSource, const char *szProfile)
    {
        ID3DBlob* shaderBlob = nullptr;
        ID3DBlob* errorBlob = nullptr;

        HRESULT result = D3DCompile(szSource, (SIZE_T)strlen(szSource), nullptr, nullptr, nullptr, "main", szProfile,
                                    D3DCOMPILE_ENABLE_STRICTNESS
#ifdef _DEBUG
                                    | D3DCOMPILE_DEBUG
#endif
                                    , 0, &shaderBlob, &errorBlob);

#ifdef _DEBUG
        if (errorBlob)
        {
            char* pError = (char*)errorBlob->GetBufferPointer();
            OLog(pError);
            return nullptr;
        }
#endif

        return shaderBlob;
    }

    OShaderRef Shader::createFromSource(const std::string& source, Type in_type, const VertexElements& vertexElements)
    {
        ID3DBlob* pBlob = nullptr;
        switch (in_type)
        {
            case Type::Vertex:
                pBlob = compileShader(source.c_str(), "vs_4_0");
                break;
            case Type::Pixel:
                pBlob = compileShader(source.c_str(), "ps_4_0");
                break;
        }
        if (!pBlob) return nullptr;
        return createFromBinaryData(reinterpret_cast<uint8_t*>(pBlob->GetBufferPointer()), static_cast<uint32_t>(pBlob->GetBufferSize()), in_type, vertexElements);
    }

    ShaderD3D11::~ShaderD3D11()
    {
        for (auto& pUniform : m_uniforms)
        {
            pUniform.pBuffer->Release();
        }
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

    int ShaderD3D11::getUniformId(const std::string& varName) const
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

    void ShaderD3D11::setFloat(int varId, float value)
    {
        auto& uniform = m_uniforms[varId];
        uniform.dirty = true;
        auto pBuffer = uniform.pBuffer;
        auto pRendererD3D11 = std::dynamic_pointer_cast<ORendererD3D11>(oRenderer);
        auto pDeviceContext = pRendererD3D11->getDeviceContext();

        D3D11_MAPPED_SUBRESOURCE map;
        pDeviceContext->Map(pBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &map);
        float values[4] = {value, 0, 0, 0};
        memcpy(map.pData, values, 16);
        pDeviceContext->Unmap(pBuffer, 0);
    }

    void ShaderD3D11::setVector2(int varId, const Vector2& value)
    {
        auto& uniform = m_uniforms[varId];
        uniform.dirty = true;
        auto pBuffer = uniform.pBuffer;
        auto pRendererD3D11 = std::dynamic_pointer_cast<ORendererD3D11>(oRenderer);
        auto pDeviceContext = pRendererD3D11->getDeviceContext();

        D3D11_MAPPED_SUBRESOURCE map;
        pDeviceContext->Map(pBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &map);
        float values[4] = {value.x, value.y, 0, 0};
        memcpy(map.pData, values, 16);
        pDeviceContext->Unmap(pBuffer, 0);
    }

    void ShaderD3D11::setVector3(int varId, const Vector3& value)
    {
        auto& uniform = m_uniforms[varId];
        uniform.dirty = true;
        auto pBuffer = uniform.pBuffer;
        auto pRendererD3D11 = std::dynamic_pointer_cast<ORendererD3D11>(oRenderer);
        auto pDeviceContext = pRendererD3D11->getDeviceContext();

        D3D11_MAPPED_SUBRESOURCE map;
        pDeviceContext->Map(pBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &map);
        float values[4] = {value.x, value.y, value.z, 0};
        memcpy(map.pData, values, 16);
        pDeviceContext->Unmap(pBuffer, 0);
    }

    void ShaderD3D11::setVector4(int varId, const Vector4& value)
    {
        auto& uniform = m_uniforms[varId];
        uniform.dirty = true;
        auto pBuffer = uniform.pBuffer;
        auto pRendererD3D11 = std::dynamic_pointer_cast<ORendererD3D11>(oRenderer);
        auto pDeviceContext = pRendererD3D11->getDeviceContext();

        D3D11_MAPPED_SUBRESOURCE map;
        pDeviceContext->Map(pBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &map);
        memcpy(map.pData, &value.x, 16);
        pDeviceContext->Unmap(pBuffer, 0);
    }

    void ShaderD3D11::setMatrix(int varId, const Matrix& value)
    {
        auto& uniform = m_uniforms[varId];
        uniform.dirty = true;
        auto pBuffer = uniform.pBuffer;
        auto pRendererD3D11 = std::dynamic_pointer_cast<ORendererD3D11>(oRenderer);
        auto pDeviceContext = pRendererD3D11->getDeviceContext();

        D3D11_MAPPED_SUBRESOURCE map;
        pDeviceContext->Map(pBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &map);
        memcpy(map.pData, &value._11, 64);
        pDeviceContext->Unmap(pBuffer, 0);
    }

    void ShaderD3D11::setFloat(const std::string& varName, float value)
    {
        setFloat(getUniformId(varName), value);
    }

    void ShaderD3D11::setVector2(const std::string& varName, const Vector2& value)
    {
        setVector2(getUniformId(varName), value);
    }

    void ShaderD3D11::setVector3(const std::string& varName, const Vector3& value)
    {
        setVector3(getUniformId(varName), value);
    }

    void ShaderD3D11::setVector4(const std::string& varName, const Vector4& value)
    {
        setVector4(getUniformId(varName), value);
    }

    void ShaderD3D11::setMatrix(const std::string& varName, const Matrix& value)
    {
        setMatrix(getUniformId(varName), value);
    }

    ShaderD3D11::Uniforms& ShaderD3D11::getUniforms()
    {
        return m_uniforms;
    }
};

#endif
