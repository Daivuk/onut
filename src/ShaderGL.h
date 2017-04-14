#ifndef SHADERGL_H_INCLUDED
#define SHADERGL_H_INCLUDED

// Onut
#include <onut/Shader.h>

// Third party
#include "gl_includes.h"

// Forward
#include <onut/ForwardDeclaration.h>
OForwardDeclare(RendererGL)
OForwardDeclare(ShaderGL)

namespace onut
{
    class ShaderGL final : public Shader
    {
    public:
        ShaderGL();
        ~ShaderGL();

        struct Uniform
        {
            bool dirty = true;
        //    ID3D11Buffer* pBuffer = nullptr;
            std::string name;
        };

        using Uniforms = std::vector<Uniform>;

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
        friend class RendererGL;

      //  ID3D11VertexShader* m_pVertexShader = nullptr;
      //  ID3D11PixelShader* m_pPixelShader = nullptr;
      //  ID3D11InputLayout* m_pInputLayout = nullptr;

        Uniforms m_uniforms;
    };
};

#endif
