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
        ID3D11SamplerState** getVSSamplerStates() const;
        int getSamplerStatesCount() const;
        int getVSSamplerStatesCount() const;
        Uniforms& getUniforms();

        int getUniformId(const std::string& varName) const override;

#define OVERRIDE_UNIFORM_SETTER(_type, _typename) \
        void set ## _typename(int varId, const _type& value) override; \
        void set ## _typename(const std::string& varName, const _type& value) override; \
        void set ## _typename ## Array(int varId, const _type* values, int count) override; \
        void set ## _typename ## Array(const std::string& varName, const _type* values, int count) override;

        OVERRIDE_UNIFORM_SETTER(float, Float);
        OVERRIDE_UNIFORM_SETTER(Vector2, Vector2);
        OVERRIDE_UNIFORM_SETTER(Vector3, Vector3);
        OVERRIDE_UNIFORM_SETTER(Vector4, Vector4);
        OVERRIDE_UNIFORM_SETTER(uint32_t, UInt);
        OVERRIDE_UNIFORM_SETTER(UInt4, UInt4);
        OVERRIDE_UNIFORM_SETTER(int32_t, Int);
        OVERRIDE_UNIFORM_SETTER(Int4, Int4);
        OVERRIDE_UNIFORM_SETTER(Matrix, Matrix);

    private:
        friend class Shader;
        friend class RendererD3D11;

        ID3D11VertexShader* m_pVertexShader = nullptr;
        ID3D11PixelShader* m_pPixelShader = nullptr;
        ID3D11InputLayout* m_pInputLayout = nullptr;
        ID3D11SamplerState** m_ppSampleStates = nullptr;
        ID3D11SamplerState** m_ppVSSampleStates = nullptr;
        int m_samplerStatesCount = 0;
        int m_vsSamplerStatesCount = 0;

        Uniforms m_uniforms;
    };
};

#endif

#endif
