// Onut
#include <onut/Deferred.h>
#include <onut/Renderer.h>
#include <onut/Shader.h>
#include <onut/SpriteBatch.h>
#include <onut/Texture.h>

// Private
#include "default_shaders.h"

ODeferredRef oDeferred;

template<typename T>
static int binSearch(const std::vector<T>& v, const T& key)
{
    int start = 0;
    int end = (int)v.size() - 1;
    int mid = 0;
    while (start <= end)
    {
        mid = (start + end) / 2;
        if (key == v[mid])
            return mid;
        else if (key < v[mid])
            end = mid - 1;
        else start = mid + 1;
    }
    return mid;
}

template<typename T>
static int invBinSearch(const std::vector<T>& v, const T& key)
{
    if (v.empty()) return 0;
    int start = 0;
    int len = (int)v.size() - 1;
    int end = len;
    int mid = 0;
    while (start <= end)
    {
        mid = (start + end) / 2;
        if (key == v[mid])
            return mid;
        else if (key > v[mid])
            end = mid - 1;
        else start = mid + 1;
    }
    return start;
}

namespace onut
{
    ODeferredRef Deferred::create()
    {
        return ODeferredRef(new Deferred());
    }

    Deferred::~Deferred()
    {
    }

    Deferred::Deferred()
    {
        m_pAlbedo = OTexture::createScreenRenderTarget(false, OFormatRGBA8);
        m_pNormal = OTexture::createScreenRenderTarget(false, OFormatRGBA8);
        m_pDepth = OTexture::createScreenRenderTarget(false, OFormatR32);
        m_pMaterial = OTexture::createScreenRenderTarget(false, OFormatRGBA8);
        m_pAO = OTexture::createScreenRenderTarget();

        {
            unsigned char data[4] = { 255, 255, 255, 255 };
            m_defaultAlbedo = Texture::createFromData(data, { 1, 1 }, false);
        }
        {
            unsigned char data[4] = { 128, 128, 128, 255 };
            m_defaultNormal = Texture::createFromData(data, { 1, 1 }, false);
        }
        {
            unsigned char data[4] = { 0, 128, 0, 255 };
            m_defaultMaterial = Texture::createFromData(data, { 1, 1 }, false);
        }

        m_pGBufferVS = OShader::createFromSource(SHADER_GBUFFER_VS, OVertexShader);
        m_pGBufferPS = OShader::createFromSource(SHADER_GBUFFER_PS, OPixelShader);
        m_pTransparentVS = OShader::createFromSource(SHADER_TRANSPARENT_VS, OVertexShader);
        m_pTransparentPS = OShader::createFromSource(SHADER_TRANSPARENT_PS, OPixelShader);
        m_pGBufferAlphaTestPS = OShader::createFromSource(SHADER_GBUFFER_ALPHA_TEST_PS, OPixelShader);
        m_pOmniPS = OShader::createFromSource(SHADER_DEFERRED_OMNI_PS, OPixelShader);
        m_pSSAO[0] = OShader::createFromSource(SHADER_SSAO_LOW_PS, OPixelShader);
        m_pSSAO[1] = OShader::createFromSource(SHADER_SSAO_MEDIUM_PS, OPixelShader);
        m_pSSAO[2] = OShader::createFromSource(SHADER_SSAO_HIGH_PS, OPixelShader);
    }

    const OTextureRef& Deferred::getAlbedo() const
    {
        return m_pAlbedo;
    }

    const OTextureRef& Deferred::getNormal() const
    {
        return m_pNormal;
    }

    const OTextureRef& Deferred::getDepth() const
    {
        return m_pDepth;
    }

    const OTextureRef& Deferred::getMaterial() const
    {
        return m_pMaterial;
    }

    void Deferred::begin()
    {
        auto pRenderer = oRenderer.get();

        m_view = pRenderer->renderStates.view.get();
        m_proj = pRenderer->renderStates.projection.get();
        m_viewProj = m_view * m_proj;
        m_invViewProj = m_viewProj.Invert();
        m_viewPos = m_view.Invert().Translation();

        m_solids.clear();
        m_solidDistances.clear();
        m_solidCallbacks.clear();
        m_alphaTests.clear();
        m_alphaTestDistances.clear();
        m_alphaTestCallbacks.clear();
        m_transparents.clear();
        m_transparentDistances.clear();
        m_transparentCallbacks.clear();

        m_omnis.clear();
    }


    void Deferred::addSolid(const OModelRef& pModel, const Matrix& transform)
    {
        float dis = Vector3::DistanceSquared(transform.Translation(), m_viewPos);
        auto index = binSearch(m_solidDistances, dis);
        m_solidDistances.insert(m_solidDistances.begin() + index, dis);

        auto ppModel = pModel.get();
        for (int i = 0, len = ppModel->getMeshCount(); i < len; ++i)
        {
            auto pMesh = ppModel->getMesh(i);
            m_solids.insert(m_solids.begin() + index, {
               pMesh->pTexture,
               m_defaultNormal,
               m_defaultMaterial,
               pMesh->pIndexBuffer,
               pMesh->pVertexBuffer,
               pMesh->elementCount,
               transform
            });
        }
    }

    void Deferred::addSolid(const OModel::Mesh* pMesh, const Matrix& transform)
    {
        float dis = Vector3::DistanceSquared(transform.Translation(), m_viewPos);
        auto index = binSearch(m_solidDistances, dis);
        m_solidDistances.insert(m_solidDistances.begin() + index, dis);

        m_solids.insert(m_solids.begin() + index, {
            pMesh->pTexture,
            m_defaultNormal,
            m_defaultMaterial,
            pMesh->pIndexBuffer,
            pMesh->pVertexBuffer,
            pMesh->elementCount,
            transform
        });
    }

    void Deferred::addSolid(const Mesh& mesh)
    {
        float dis = Vector3::DistanceSquared(mesh.transform.Translation(), m_viewPos);
        auto index = binSearch(m_solidDistances, dis);
        m_solidDistances.insert(m_solidDistances.begin() + index, dis);

        m_solids.insert(m_solids.begin() + index, mesh);
    }

    void Deferred::addSolid(const RenderCallback& drawCallback)
    {
        m_solidCallbacks.push_back(drawCallback);
    }


    void Deferred::addAlphaTest(const OModelRef& pModel, const Matrix& transform)
    {
        float dis = Vector3::DistanceSquared(transform.Translation(), m_viewPos);
        auto index = binSearch(m_alphaTestDistances, dis);
        m_alphaTestDistances.insert(m_alphaTestDistances.begin() + index, dis);

        auto ppModel = pModel.get();
        for (int i = 0, len = ppModel->getMeshCount(); i < len; ++i)
        {
            auto pMesh = ppModel->getMesh(i);
            m_alphaTests.insert(m_alphaTests.begin() + index, {
               pMesh->pTexture,
               m_defaultNormal,
               m_defaultMaterial,
               pMesh->pIndexBuffer,
               pMesh->pVertexBuffer,
               pMesh->elementCount,
               transform
            });
        }
    }

    void Deferred::addAlphaTest(const OModel::Mesh* pMesh, const Matrix& transform)
    {
        float dis = Vector3::DistanceSquared(transform.Translation(), m_viewPos);
        auto index = binSearch(m_alphaTestDistances, dis);
        m_alphaTestDistances.insert(m_alphaTestDistances.begin() + index, dis);

        m_alphaTests.insert(m_alphaTests.begin() + index, {
            pMesh->pTexture,
            m_defaultNormal,
            m_defaultMaterial,
            pMesh->pIndexBuffer,
            pMesh->pVertexBuffer,
            pMesh->elementCount,
            transform
        });
    }

    void Deferred::addAlphaTest(const Mesh& mesh)
    {
        float dis = Vector3::DistanceSquared(mesh.transform.Translation(), m_viewPos);
        auto index = binSearch(m_alphaTestDistances, dis);
        m_alphaTestDistances.insert(m_alphaTestDistances.begin() + index, dis);

        m_alphaTests.insert(m_alphaTests.begin() + index, mesh);
    }

    void Deferred::addAlphaTest(const RenderCallback& drawCallback)
    {
        m_alphaTestCallbacks.push_back(drawCallback);
    }


    void Deferred::addTransparent(const OModelRef& pModel, const Matrix& transform, onut::BlendMode blendMode)
    {
        float dis = Vector3::DistanceSquared(transform.Translation(), m_viewPos);
        auto index = invBinSearch(m_transparentDistances, dis);
        m_transparentDistances.insert(m_transparentDistances.begin() + index, dis);

        auto ppModel = pModel.get();
        for (int i = 0, len = ppModel->getMeshCount(); i < len; ++i)
        {
            auto pMesh = ppModel->getMesh(i);
            m_transparents.insert(m_transparents.begin() + index, {
                blendMode, {
                   pMesh->pTexture,
                   m_defaultNormal,
                   m_defaultMaterial,
                   pMesh->pIndexBuffer,
                   pMesh->pVertexBuffer,
                   pMesh->elementCount,
                   transform
                }
            });
        }
    }

    void Deferred::addTransparent(const OModel::Mesh* pMesh, const Matrix& transform, onut::BlendMode blendMode)
    {
        float dis = Vector3::DistanceSquared(transform.Translation(), m_viewPos);
        auto index = invBinSearch(m_transparentDistances, dis);
        m_transparentDistances.insert(m_transparentDistances.begin() + index, dis);

        m_transparents.insert(m_transparents.begin() + index, {
            blendMode, {
                pMesh->pTexture,
                m_defaultNormal,
                m_defaultMaterial,
                pMesh->pIndexBuffer,
                pMesh->pVertexBuffer,
                pMesh->elementCount,
                transform
            }
        });
    }

    void Deferred::addTransparent(const Mesh& mesh, onut::BlendMode blendMode)
    {
        float dis = Vector3::DistanceSquared(mesh.transform.Translation(), m_viewPos);
        auto index = invBinSearch(m_transparentDistances, dis);
        m_transparentDistances.insert(m_transparentDistances.begin() + index, dis);

        m_transparents.insert(m_transparents.begin() + index, {
            blendMode, mesh
        });
    }

    void Deferred::addTransparent(const RenderCallback& drawCallback)
    {
        m_transparentCallbacks.push_back(drawCallback);
    }


    void Deferred::addSun(const Vector3& dir, const Color& color, float intensity)
    {

    }

    void Deferred::addOmni(const Vector3& pos, float radius, const Color& color, float intensity)
    {
        m_omnis.push_back({
            Vector4(pos.x, pos.y, pos.z, radius),
            Vector3(color.r * intensity, color.g * intensity, color.b * intensity)
        });
    }

    void Deferred::addLight(const RenderCallback& drawCallback)
    {

    }

    void Deferred::end(const Color& ambient, 
                       bool ssaoEnabled, float ssaoRadius, float ssaoStrength, SSAOQuality ssaoQuality)
    {
        auto pRenderer = oRenderer.get();
        auto pSpriteBatch = oSpriteBatch.get();
        auto& rs = pRenderer->renderStates;
        auto screenRect = OScreenRectf;

        // Set render states
        for (int i = 0; i < RenderStates::MAX_TEXTURES; ++i) rs.textures[i].push(nullptr);
        rs.blendMode.push(OBlendOpaque);
        rs.depthEnabled.push(true);
        rs.depthWrite.push(true);
        rs.backFaceCull.push(true);
        rs.primitiveMode.push(OPrimitiveTriangleList);
        for (int i = 4; i < RenderStates::MAX_RENDER_TARGETS; ++i) rs.renderTargets[i].push(nullptr);

        // Prepare our gbuffer
        rs.renderTargets[0].push(m_pAlbedo);
        oRenderer->clear(Color(0, 0, 0, 0));
        rs.renderTargets[1].push(m_pNormal);
        rs.renderTargets[2].push(m_pDepth);
        rs.renderTargets[3].push(m_pMaterial);

        // Draw solids
        rs.vertexShader = m_pGBufferVS;
        rs.pixelShader = m_pGBufferPS;
        for (const auto& solid : m_solids)
        {
            rs.textures[0] = solid.pAlbedo;
            rs.textures[1] = solid.pNormal;
            rs.textures[2] = solid.pMaterial;
            rs.vertexBuffer = solid.pVertexBuffer;
            rs.indexBuffer = solid.pIndexBuffer;
            rs.world = solid.transform;
            pRenderer->drawIndexed(solid.elementCount);
        }
        for (const auto& callback : m_solidCallbacks) callback();

        // Draw alpha tested
        rs.pixelShader = m_pGBufferAlphaTestPS;
        for (const auto& alphaTest : m_alphaTests)
        {
            rs.textures[0] = alphaTest.pAlbedo;
            rs.textures[1] = alphaTest.pNormal;
            rs.textures[2] = alphaTest.pMaterial;
            rs.vertexBuffer = alphaTest.pVertexBuffer;
            rs.indexBuffer = alphaTest.pIndexBuffer;
            rs.world = alphaTest.transform;
            pRenderer->drawIndexed(alphaTest.elementCount);
        }
        for (const auto& callback : m_alphaTestCallbacks) callback();

        // Reset MRT
        rs.renderTargets[0].pop();
        rs.renderTargets[1].pop();
        rs.renderTargets[2].pop();
        rs.renderTargets[3].pop();

        // Render Ambient
        rs.depthEnabled = false;
        rs.depthWrite = false;
        rs.backFaceCull = false;
        rs.textures[1] = nullptr;
        rs.textures[2] = nullptr;
        rs.textures[3] = nullptr;
        pSpriteBatch->begin();
        pSpriteBatch->drawRect(m_pAlbedo, screenRect, ambient);
        pSpriteBatch->end();

        // Render omnis
        rs.textures[1] = m_pNormal;
        rs.textures[2] = m_pDepth;
        rs.textures[3] = m_pMaterial;
        m_pOmniPS->setMatrix(0, m_invViewProj);
        for (const auto& omni : m_omnis)
        {
            pSpriteBatch->begin();
            rs.blendMode = OBlendAdd;
            rs.pixelShader = m_pOmniPS;
            m_pOmniPS->setVector4(1, omni.posAndRadius);
            m_pOmniPS->setVector3(2, omni.color);
            pSpriteBatch->drawRect(m_pAlbedo, screenRect);
            pSpriteBatch->end();
        }

        // Ambient Occlusion
        if (ssaoEnabled)
        {
            // Generate
            rs.renderTargets[0].push(m_pAO);
            rs.textures[1] = m_pNormal;
            rs.textures[2] = m_pDepth;
            pSpriteBatch->begin();
            rs.blendMode = OBlendOpaque;
            auto pSSAO_PS = m_pSSAO[(int)ssaoQuality];
            rs.pixelShader = pSSAO_PS;
            pSSAO_PS->setMatrix(0, m_invViewProj);
            pSSAO_PS->setVector4(1, Vector4((0.01f / 0.08f) * ssaoRadius, ssaoRadius, (1.0f / 0.08f) * ssaoRadius, ssaoStrength));
            pSpriteBatch->drawRect(m_pAlbedo, screenRect);
            pSpriteBatch->end();
            rs.renderTargets[0].pop();

            // Draw on top
            pSpriteBatch->begin();
            rs.blendMode = OBlendAlpha;
            pSpriteBatch->drawRect(m_pAO, screenRect);
            pSpriteBatch->end();
        }

        // Render transparents
        rs.vertexShader = m_pTransparentVS;
        rs.pixelShader = m_pTransparentPS;
        rs.depthEnabled = true;
        rs.depthWrite = false;
        rs.backFaceCull = true;
        rs.view = m_view;
        rs.projection = m_proj;
        for (const auto& transparent : m_transparents)
        {
            rs.blendMode = transparent.blendMode;
            rs.textures[0] = transparent.mesh.pAlbedo;
            rs.textures[1] = transparent.mesh.pNormal;
            rs.textures[2] = transparent.mesh.pMaterial;
            rs.vertexBuffer = transparent.mesh.pVertexBuffer;
            rs.indexBuffer = transparent.mesh.pIndexBuffer;
            rs.world = transparent.mesh.transform;
            pRenderer->drawIndexed(transparent.mesh.elementCount);
        }
        for (const auto& callback : m_transparentCallbacks) callback();

        // Pop states
        for (int i = 0; i < RenderStates::MAX_TEXTURES; ++i) rs.textures[i].pop();
        rs.blendMode.pop();
        rs.depthEnabled.pop();
        rs.depthWrite.pop();
        rs.backFaceCull.pop();
        rs.primitiveMode.pop();
        for (int i = 0; i < RenderStates::MAX_RENDER_TARGETS; ++i) rs.renderTargets[i].pop();
    }
};
