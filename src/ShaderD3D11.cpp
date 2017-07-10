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
    static std::string getTypeName(Shader::VarType type)
    {
        switch (type)
        {
        case Shader::VarType::Float: return "float";
        case Shader::VarType::Float2: return "float2";
        case Shader::VarType::Float3: return "float3";
        case Shader::VarType::Float4: return "float4";
        case Shader::VarType::Matrix: return "matrix";
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
                    INTRINSICT_IMPL_2(ATan2, atan2, 0, 1);
                    INTRINSICT_IMPL_2(Pow, pow, 0, 1);
                    INTRINSICT_IMPL_1(Exp, exp, 0);
                    INTRINSICT_IMPL_1(Log, log, 0);
                    INTRINSICT_IMPL_1(Exp2, exp2, 0);
                    INTRINSICT_IMPL_1(Log2, log2, 0);
                    INTRINSICT_IMPL_1(Sqrt, sqrt, 0);
                    INTRINSICT_IMPL_1(RSqrt, rsqrt, 0);
                    INTRINSICT_IMPL_1(Abs, abs, 0);
                    INTRINSICT_IMPL_1(Sign, sign, 0);
                    INTRINSICT_IMPL_1(Floor, floor, 0);
                    INTRINSICT_IMPL_1(Ceil, ceil, 0);
                    INTRINSICT_IMPL_1(Frac, frac, 0);
                    INTRINSICT_IMPL_2(Mod, fmod, 0, 1);
                    INTRINSICT_IMPL_2(Min, min, 0, 1);
                    INTRINSICT_IMPL_2(Max, max, 0, 1);
                    INTRINSICT_IMPL_3(Clamp, clamp, 0, 1, 2);
                    INTRINSICT_IMPL_1(Saturate, saturate, 0);
                    INTRINSICT_IMPL_3(Lerp, lerp, 0, 1, 2);
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
                    INTRINSICT_IMPL_2(Mul, mul, 0, 1);
                    default: assert(false);
                }
            }
        }
    }

    OShaderRef Shader::createFromSource(const std::string& in_source, Type in_type, const VertexElements& vertexElements)
    {
        auto pRendererD3D11 = std::dynamic_pointer_cast<ORendererD3D11>(oRenderer);
        auto pDevice = pRendererD3D11->getDevice();

        // Set the shader type
        if (in_type == OVertexShader)
        {
            auto parsed = parseVertexShader(in_source);
            std::string source;
            source.reserve(5000);

            // Add engine default constant buffers
            source += "cbuffer OViewProjection : register(b0)\n{\n    matrix oViewProjection;\n}\n\n";

            // Put inputs
            VertexElements vertexElements;
            std::string elementStructsSource;
            elementStructsSource += "struct oVSInput\n{\n";
            int semanticIndex = 0;
            for (auto& element : parsed.inputs)
            {
                std::string semanticName = "INPUT_ELEMENT" + std::to_string(semanticIndex++);
                vertexElements.push_back({ element.type, "INPUT_ELEMENT" });
                elementStructsSource += "    " + getTypeName(element.type) + " ";
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
                elementStructsSource += "    " + getTypeName(element.type) + " ";
                elementStructsSource += element.name + " : " + semanticName + ";\n";
            }
            elementStructsSource += "};\n\n";
            source += elementStructsSource;

            // Create uniforms
            ShaderD3D11::Uniforms uniforms;
            int uniformId = 4;
            for (auto& parsedUniform : parsed.uniforms)
            {
                ShaderD3D11::Uniform uniform;

                uniform.name = parsedUniform.name;

                UINT uniformSize = 4;
                if (parsedUniform.type == VarType::Matrix) uniformSize = 16;

                D3D11_BUFFER_DESC cbDesc = CD3D11_BUFFER_DESC(uniformSize * 4, D3D11_BIND_CONSTANT_BUFFER, D3D11_USAGE_DYNAMIC, D3D11_CPU_ACCESS_WRITE);
                auto ret = pDevice->CreateBuffer(&cbDesc, NULL, &(uniform.pBuffer));
                assert(ret == S_OK);

                source += "cbuffer cb_" + uniform.name + " : register(b" + std::to_string(uniformId) + ")\n{\n    " + getTypeName(parsedUniform.type) + " " + uniform.name + ";\n}\n\n";

                uniforms.push_back(uniform);
                ++uniformId;
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
                source += "static";
                bakeTokens(source, _const.line);
            }
            source += ";\n\n";

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
            source += "oVSOutput main(oVSInput oInput)\n{\n    oVSOutput oOutput;\n";
            for (auto& element : parsed.outputs)
            {
                source += "    " + getTypeName(element.type) + " " + element.name + ";\n";
            }
            source += "    float4 oPosition;\n";
            for (auto& element : parsed.inputs)
            {
                source += "    " + getTypeName(element.type) + " " + element.name + " = oInput." + element.name + ";\n";
            }
            bakeTokens(source, parsed.mainFunction.body);
            source += "    oOutput.position = oPosition;\n";
            for (auto& element : parsed.outputs)
            {
                source += "    oOutput." + element.name + " = " + element.name + ";\n";
            }
            source += "    return oOutput;\n}\n";

            // Now compile it
            auto pRet = createFromNativeSource(source, Type::Vertex, vertexElements);
            auto pRetD3D11 = ODynamicCast<ShaderD3D11>(pRet);
            if (pRetD3D11)
            {
                pRetD3D11->m_uniforms = uniforms;
            }

            return pRet;
        }
        else if (in_type == OPixelShader)
        {
            auto parsed = parsePixelShader(in_source);
            std::string source;
            source.reserve(5000);

            // Create textures
            for (auto rit = parsed.textures.rbegin(); rit != parsed.textures.rend(); ++rit)
            {
                const auto& texture = *rit;
                source += "SamplerState sampler_" + texture.name + " : register(s" + std::to_string(texture.index) + ");\n";
                source += "Texture2D texture_" + texture.name + " : register(t" + std::to_string(texture.index) + ");\n";
                source += "float4 " + texture.name + "(float2 uv)\n";
                source += "{\n";
                source += "    return texture_" + texture.name + ".Sample(sampler_" + texture.name + ", uv);\n";
                source += "}\n\n";
            }

            // Put inputs
            std::string elementStructsSource;
            elementStructsSource += "struct oPSInput\n{\n";
            int semanticIndex = 0;
            elementStructsSource += "    float4 position : SV_POSITION;\n";
            for (auto& element : parsed.inputs)
            {
                std::string semanticName = "OUTPUT_ELEMENT" + std::to_string(semanticIndex++);
                elementStructsSource += "    " + getTypeName(element.type) + " ";
                elementStructsSource += element.name + " : " + semanticName + ";\n";
            }
            elementStructsSource += "};\n\n";
            source += elementStructsSource;

            // Create uniforms
            ShaderD3D11::Uniforms uniforms;
            int uniformId = 4;
            for (auto& parsedUniform : parsed.uniforms)
            {
                ShaderD3D11::Uniform uniform;

                uniform.name = parsedUniform.name;

                UINT uniformSize = 4;
                if (parsedUniform.type == VarType::Matrix) uniformSize = 16;

                D3D11_BUFFER_DESC cbDesc = CD3D11_BUFFER_DESC(uniformSize * 4, D3D11_BIND_CONSTANT_BUFFER, D3D11_USAGE_DYNAMIC, D3D11_CPU_ACCESS_WRITE);
                auto ret = pDevice->CreateBuffer(&cbDesc, NULL, &(uniform.pBuffer));
                assert(ret == S_OK);

                source += "cbuffer cb_" + uniform.name + " : register(b" + std::to_string(uniformId) + ")\n{\n    " + getTypeName(parsedUniform.type) + " " + uniform.name + ";\n}\n\n";

                uniforms.push_back(uniform);
                ++uniformId;
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
                source += "static";
                bakeTokens(source, _const.line);
            }
            source += ";\n\n";

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
            source += "float4 main(oPSInput oInput) : SV_TARGET\n{\n    float4 oColor;\n";
            for (auto& element : parsed.inputs)
            {
                source += "    " + getTypeName(element.type) + " " + element.name + " = oInput." + element.name + ";\n";
            }
            bakeTokens(source, parsed.mainFunction.body);
            source += "    return oColor;\n}\n";

            // Now compile it
            auto pRet = createFromNativeSource(source, Type::Pixel);
            auto pRetD3D11 = ODynamicCast<ShaderD3D11>(pRet);
            if (pRetD3D11)
            {
                pRetD3D11->m_uniforms = uniforms;

                // Create textures samplers
                if (!parsed.textures.empty())
                {
                    pRetD3D11->m_samplerStatesCount = (int)parsed.textures.size();
                    pRetD3D11->m_ppSampleStates = new ID3D11SamplerState*[pRetD3D11->m_samplerStatesCount];
                    int i = 0;
                    for (auto& texture : parsed.textures)
                    {
                        D3D11_SAMPLER_DESC samplerState;
                        memset(&samplerState, 0, sizeof(samplerState));
                        samplerState.MaxAnisotropy = 1;
                        samplerState.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
                        samplerState.MaxLOD = D3D11_FLOAT32_MAX;

                        switch (texture.filter)
                        {
                        case ParsedTexture::Filter::Nearest:
                            samplerState.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
                            break;
                        case ParsedTexture::Filter::Linear:
                            samplerState.Filter = D3D11_FILTER_MIN_POINT_MAG_LINEAR_MIP_POINT;
                            break;
                        case ParsedTexture::Filter::Bilinear:
                            samplerState.Filter = D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT;
                            break;
                        case ParsedTexture::Filter::Trilinear:
                            samplerState.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
                            break;
                        case ParsedTexture::Filter::Anisotropic:
                            samplerState.Filter = D3D11_FILTER_ANISOTROPIC;
                            break;
                        }
                        switch (texture.repeatX)
                        {
                        case ParsedTexture::Repeat::Clamp:
                            samplerState.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
                            break;
                        case ParsedTexture::Repeat::Wrap:
                            samplerState.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
                            break;
                        }
                        switch (texture.repeatY)
                        {
                        case ParsedTexture::Repeat::Clamp:
                            samplerState.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
                            break;
                        case ParsedTexture::Repeat::Wrap:
                            samplerState.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
                            break;
                        }
                        samplerState.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
                        pDevice->CreateSamplerState(&samplerState, &pRetD3D11->m_ppSampleStates[i]);
                        ++i;
                    }
                }
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
                    switch (element.type)
                    {
                        case VarType::Float:
                            format = DXGI_FORMAT_R32_FLOAT;
                            break;
                        case VarType::Float2:
                            format = DXGI_FORMAT_R32G32_FLOAT;
                            break;
                        case VarType::Float3:
                            format = DXGI_FORMAT_R32G32B32_FLOAT;
                            break;
                        case VarType::Float4:
                            format = DXGI_FORMAT_R32G32B32A32_FLOAT;
                            break;
                        default:
                            assert(false);
                    }

                    pRet->m_vertexSize += (int)element.type * 4;
                    
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
#else
        if (errorBlob)
        {
            char* pError = (char*)errorBlob->GetBufferPointer();
            MessageBoxA(nullptr, pError, "Shader compilation error", MB_OK);
            return nullptr;
        }
#endif

        return shaderBlob;
    }

    OShaderRef Shader::createFromNativeSource(const std::string& source, Type in_type, const VertexElements& vertexElements)
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
        for (int i = 0; i < m_samplerStatesCount; ++i)
        {
            m_ppSampleStates[i]->Release();
        }
        if (m_ppSampleStates) delete[] m_ppSampleStates;
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

    ID3D11SamplerState** ShaderD3D11::getSamplerStates() const
    {
        return m_ppSampleStates;
    }

    int ShaderD3D11::getSamplerStatesCount() const
    {
        return m_samplerStatesCount;
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
