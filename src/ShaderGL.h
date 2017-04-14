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

        GLenum m_shader = 0;

        Uniforms m_uniforms;

        struct Program
        {
            OShaderWeak pVertexShader;
            GLenum m_program = 0;
        };

        using Programs = std::vector<Program>;

        Programs m_programs;
    };
};

#endif
