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
            VarType type;
            Matrix value; // We just store everything in a matrix for easy copying
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

        struct Attribute
        {
            VarType type;
            std::string name;
        };
        using InputLayout = std::vector<Attribute>;

        struct Program
        {
            OShaderGLWeak pVertexShader;
            GLenum program = 0;
            std::vector<GLint> uniformsVS;
            std::vector<GLint> uniformsPS;
            GLint oViewProjectionUniform;
            std::vector<GLint> attributes;
            std::vector<GLint> textures;
        };
        using ProgramRef = std::shared_ptr<Program>;
        using Programs = std::vector<ProgramRef>;

        const ProgramRef& getProgram(const OShaderGLRef& pVertexShader);

        GLenum m_shader = 0;
        Uniforms m_uniforms;
        Programs m_programs;
        ParsedTextures m_textures;
        InputLayout m_inputLayout;
#if defined(_DEBUG)
        std::string m_source;
#endif
    };
};

#endif
