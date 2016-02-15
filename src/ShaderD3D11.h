#if defined(WIN32)
#pragma once
#include "onut/Shader.h"

#include <d3d11.h>

#include "onut/ForwardDeclaration.h"
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

    private:
        friend class Shader;

        ID3D11VertexShader* m_pVertexShader = nullptr;
        ID3D11PixelShader* m_pPixelShader = nullptr;
    };
};

#endif
