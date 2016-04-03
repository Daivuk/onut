#pragma once
#if defined(WIN32)
// Onut
#include <onut/Shader.h>

// Third party
#include <d3d11.h>

// Forward
#include <onut/ForwardDeclaration.h>
OForwardDeclare(ShaderD3D11)

namespace onut
{
    class ShaderD3D11 final : public Shader
    {
    public:
        ShaderD3D11();
        ~ShaderD3D11();

        ID3D11VertexShader* getVertexShader() const;
        ID3D11PixelShader* getPixelShader() const;
        ID3D11InputLayout* getInputLayout() const;

    private:
        friend class Shader;

        ID3D11VertexShader* m_pVertexShader = nullptr;
        ID3D11PixelShader* m_pPixelShader = nullptr;
        ID3D11InputLayout* m_pInputLayout = nullptr;
    };
};

#endif
