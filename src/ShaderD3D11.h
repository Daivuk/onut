#ifndef SHADERD3D11_H_INCLUDED
#define SHADERD3D11_H_INCLUDED

#if defined(WIN32)
// Onut
#include <onut/Shader.h>

// Third party
#include <d3d11.h>

// Forward
#include <onut/ForwardDeclaration.h>
OForwardDeclare(RendererD3D11)
OForwardDeclare(ShaderD3D11)

namespace onut
{
    class ShaderD3D11 final : public Shader
    {
    public:
        ShaderD3D11();
        ~ShaderD3D11();

        struct Uniform
        {
            bool dirty = true;
            ID3D11Buffer* pBuffer = nullptr;
            std::string name;
        };

        using Uniforms = std::vector<Uniform>;

        ID3D11VertexShader* getVertexShader() const;
        ID3D11PixelShader* getPixelShader() const;
        ID3D11InputLayout* getInputLayout() const;
        ID3D11SamplerState** getSamplerStates() const;
        int getSamplerStatesCount() const;
        Uniforms& getUniforms();

        int getUniformId(const std::string& varName) const override;
        void setFloat(int varId, float value) override;
        void setVector2(int varId, const Vector2& value) override;
        void setVector3(int varId, const Vector3& value) override;
        void setVector4(int varId, const Vector4& value) override;
        void setMatrix(int varId, const Matrix& value) override;
        void setFloat(const std::string& varName, float value) override;
        void setVector2(const std::string& varName, const Vector2& value) override;
        void setVector3(const std::string& varName, const Vector3& value) override;
        void setVector4(const std::string& varName, const Vector4& value) override;
        void setMatrix(const std::string& varName, const Matrix& value) override;

    private:
        friend class Shader;
        friend class RendererD3D11;

        ID3D11VertexShader* m_pVertexShader = nullptr;
        ID3D11PixelShader* m_pPixelShader = nullptr;
        ID3D11InputLayout* m_pInputLayout = nullptr;
        ID3D11SamplerState** m_ppSampleStates = nullptr;
        int m_samplerStatesCount = 0;

        Uniforms m_uniforms;
    };
};

#endif

#endif
