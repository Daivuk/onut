#ifndef DEFERRED_H_INCLUDED
#define DEFERRED_H_INCLUDED

// Onut
#include <onut/BlendMode.h>
#include <onut/Maths.h>
#include <onut/Model.h>

// STL
#include <functional>
#include <vector>

// Forward
#include <onut/ForwardDeclaration.h>
OForwardDeclare(Deferred)
OForwardDeclare(IndexBuffer)
OForwardDeclare(Shader)
OForwardDeclare(Texture)
OForwardDeclare(VertexBuffer)

namespace onut
{
    class Deferred final
    {
    public:
        enum class SSAOQuality
        {
            LOW,
            MEDIUM,
            HIGH
        };

        struct Mesh
        {
            OTextureRef pAlbedo;
            OTextureRef pNormal;
            OTextureRef pMaterial;
            OIndexBufferRef pIndexBuffer;
            OVertexBufferRef pVertexBuffer;
            uint32_t elementCount;
            Matrix transform;
        };

        using RenderCallback = std::function<void()>;

        static ODeferredRef create();

        virtual ~Deferred();


        // Deferred stuff
        void begin();

        void addSolid(const OModelRef& pModel, const Matrix& transform = Matrix::Identity);
        void addSolid(const OModel::Mesh* pMesh, const Matrix& transform = Matrix::Identity);
        void addSolid(const Mesh& mesh);
        void addSolid(const RenderCallback& drawCallback);

        void addAlphaTest(const OModelRef& pModel, const Matrix& transform = Matrix::Identity);
        void addAlphaTest(const OModel::Mesh* pMesh, const Matrix& transform = Matrix::Identity);
        void addAlphaTest(const Mesh& mesh);
        void addAlphaTest(const RenderCallback& drawCallback);

        void addTransparent(const OModelRef& pModel, const Matrix& transform = Matrix::Identity, onut::BlendMode blendMode = OBlendAlpha);
        void addTransparent(const OModel::Mesh* pMesh, const Matrix& transform = Matrix::Identity, onut::BlendMode blendMode = OBlendAlpha);
        void addTransparent(const Mesh& mesh, onut::BlendMode blendMode = OBlendAlpha);
        void addTransparent(const RenderCallback& drawCallback);

        void addSun(const Vector3& dir, const Color& color = Color::White, float intensity = 1.0f);
        void addOmni(const Vector3& pos, float radius = 1.0f, const Color& color = Color::White, float intensity = 1.0f);
        void addLight(const RenderCallback& drawCallback);

        void end(const Color& ambient = Color::Black,
                 bool ssaoEnabled = true, float ssaoRadius = 0.25f, float ssaoStrength = 1.8f, SSAOQuality ssaoQuality = SSAOQuality::MEDIUM);


        const OTextureRef& getAlbedo() const;
        const OTextureRef& getNormal() const;
        const OTextureRef& getDepth() const;
        const OTextureRef& getMaterial() const;

    private:
        using Solid = Mesh;
        using AlphaTest = Mesh;

        struct Transparent
        {
            onut::BlendMode blendMode;
            Mesh mesh;
        };

        struct Omni
        {
            Vector4 posAndRadius;
            Vector3 color;
        };

        struct Sun
        {
            Vector3 dir;
            Vector4 color;
        };

        Deferred();

        OTextureRef m_pAlbedo;
        OTextureRef m_pNormal;
        OTextureRef m_pDepth;
        OTextureRef m_pMaterial;

        OTextureRef m_defaultAlbedo;
        OTextureRef m_defaultNormal;
        OTextureRef m_defaultMaterial;

        OTextureRef m_pAO;

        OShaderRef m_pGBufferVS;
        OShaderRef m_pGBufferPS;
        OShaderRef m_pGBufferAlphaTestPS;
        OShaderRef m_pTransparentVS;
        OShaderRef m_pTransparentPS;
        OShaderRef m_pOmniPS;
        OShaderRef m_pSSAO[3];

        Vector3 m_viewPos;
        Matrix m_view;
        Matrix m_proj;
        Matrix m_viewProj;
        Matrix m_invViewProj;

        std::vector<Solid> m_solids;
        std::vector<float> m_solidDistances;
        std::vector<RenderCallback> m_solidCallbacks;
        std::vector<AlphaTest> m_alphaTests;
        std::vector<float> m_alphaTestDistances;
        std::vector<RenderCallback> m_alphaTestCallbacks;
        std::vector<Transparent> m_transparents;
        std::vector<float> m_transparentDistances;
        std::vector<RenderCallback> m_transparentCallbacks;

        std::vector<Omni> m_omnis;
    };
};

extern ODeferredRef oDeferred;

#endif
