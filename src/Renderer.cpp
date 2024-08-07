// Onut
#include <onut/IndexBuffer.h>
#include <onut/Renderer.h>
#include <onut/Settings.h>
#include <onut/Shader.h>
#include <onut/Texture.h>
#include <onut/VertexBuffer.h>
#include <onut/Window.h>

// STL
#include <fstream>
#include <vector>

// Shaders
#include "default_shaders.h"

ORendererRef oRenderer;

namespace onut
{
    RenderStates::RenderStates()
    {
        for (auto& pTexture : textures)
        {
            pTexture = nullptr;
        }
        blendMode = OBlendOpaque;
        sampleFiltering = OFilterLinear;
        sampleAddressMode = OTextureWrap;
        scissor = viewport = {0, 0, 100, 100};
        projection = Matrix::Identity;
        view = Matrix::Identity;
        world = Matrix::Identity;
        wireframe = false;
        depthEnabled = false;
        depthWrite = false;
        backFaceCull = false;
        scissorEnabled = false;
        primitiveMode = OPrimitiveTriangleList;
        vertexShader = nullptr;
        pixelShader = nullptr;
        vertexBuffer = nullptr;
        indexBuffer = nullptr;
        for (auto& renderTarget : renderTargets)
        {
            renderTarget = nullptr;
        }
        clearColor = Color::fromHexRGB(0x1d232d);
    }

    RenderStates::RenderStates(const RenderStates& other)
    {
        for (int i = 0; i < MAX_TEXTURES; ++i)
        {
            textures[i] = other.textures[i];
        }
        blendMode = other.blendMode;
        sampleFiltering = other.sampleFiltering;
        sampleAddressMode = other.sampleAddressMode;
        viewport = other.viewport;
        scissor = other.scissor;
        projection = other.projection;
        view = other.view;
        world = other.world;
        wireframe = other.wireframe;
        depthEnabled = other.depthEnabled;
        depthWrite = other.depthWrite;
        backFaceCull = other.backFaceCull;
        scissorEnabled = other.scissorEnabled;
        primitiveMode = other.primitiveMode;
        vertexShader = other.vertexShader;
        pixelShader = other.pixelShader;
        vertexBuffer = other.vertexBuffer;
        indexBuffer = other.indexBuffer;
        clearColor = other.clearColor;
    }

    RenderStates& RenderStates::operator=(const RenderStates& other)
    {
        for (int i = 0; i < MAX_TEXTURES; ++i)
        {
            textures[i] = other.textures[i];
        }
        blendMode = other.blendMode;
        sampleFiltering = other.sampleFiltering;
        sampleAddressMode = other.sampleAddressMode;
        viewport = other.viewport;
        scissor = other.scissor;
        projection = other.projection;
        view = other.view;
        world = other.world;
        wireframe = other.wireframe;
        depthEnabled = other.depthEnabled;
        depthWrite = other.depthWrite;
        backFaceCull = other.backFaceCull;
        scissorEnabled = other.scissorEnabled;
        primitiveMode = other.primitiveMode;
        vertexShader = other.vertexShader;
        pixelShader = other.pixelShader;
        vertexBuffer = other.vertexBuffer;
        indexBuffer = other.indexBuffer;
        clearColor = other.clearColor;

        return *this;
    }

    void RenderStates::reset()
    {
        for (auto& pTexture : textures)
        {
            pTexture.reset();
        }
        blendMode.reset();
        sampleFiltering.reset();
        sampleAddressMode.reset();
        viewport.reset();
        scissor.reset();
        projection.reset();
        view.reset();
        world.reset();
        wireframe.reset();
        depthEnabled.reset();
        depthWrite.reset();
        backFaceCull.reset();
        scissorEnabled.reset();
        primitiveMode.reset();
        vertexShader.reset();
        pixelShader.reset();
        vertexBuffer.reset();
        indexBuffer.reset();
        for (auto& pRenderTarget : renderTargets)
        {
            pRenderTarget.reset();
        }
        clearColor.reset();
    }
    
    void RenderStates::pushAll()
    {
        for (int i = 0; i < MAX_TEXTURES; ++i) textures[i].push();
        blendMode.push();
        sampleFiltering.push();
        sampleAddressMode.push();
        viewport.push();
        scissor.push();
        projection.push();
        view.push();
        world.push();
        wireframe.push();
        depthEnabled.push();
        depthWrite.push();
        backFaceCull.push();
        scissorEnabled.push();
        primitiveMode.push();
        vertexShader.push();
        pixelShader.push();
        vertexBuffer.push();
        indexBuffer.push();
        for (int i = 0; i < MAX_RENDER_TARGETS; ++i) renderTargets[i].push();
        clearColor.push();
    }

    void RenderStates::popAll()
    {
        for (int i = 0; i < MAX_TEXTURES; ++i) textures[i].pop();
        blendMode.pop();
        sampleFiltering.pop();
        sampleAddressMode.pop();
        viewport.pop();
        scissor.pop();
        projection.pop();
        view.pop();
        world.pop();
        wireframe.pop();
        depthEnabled.pop();
        depthWrite.pop();
        backFaceCull.pop();
        scissorEnabled.pop();
        primitiveMode.pop();
        vertexShader.pop();
        pixelShader.pop();
        vertexBuffer.pop();
        indexBuffer.pop();
        for (int i = 0; i < MAX_RENDER_TARGETS; ++i) renderTargets[i].pop();
        clearColor.pop();
    }

    Renderer::Renderer()
    {
    }

    Renderer::~Renderer()
    {
    }

    void Renderer::init(const OWindowRef& pWindow)
    {
        loadShaders();
        const float vertices[] = {
            -1, -1,
            -1, 1,
            1, -1,
            1, -1,
            -1, 1,
            1, 1
        };
        m_pEffectsVertexBuffer = OVertexBuffer::createStatic(vertices, sizeof(vertices));
    }

    void Renderer::setupFor2D()
    {
        setupFor2D(Matrix::Identity);
    }

    void Renderer::setupFor2D(const Matrix& transform)
    {
        set2DCamera(Vector2::Zero);
        renderStates.world = transform;
        renderStates.vertexShader = m_p2DVertexShader;
        renderStates.pixelShader = m_p2DPixelShader;
        renderStates.depthEnabled = false;
        renderStates.blendMode = OBlendPreMultiplied;
    }

    void Renderer::setupFor3D(const Vector3& eye, const Vector3& target, const Vector3& up, float fov, float near_clip, float far_clip)
    {
        const auto& viewport = renderStates.viewport.get();
        renderStates.view = Matrix::CreateLookAt(eye, target, up);
        renderStates.projection = Matrix::CreatePerspectiveFieldOfView(OConvertToRadians(fov), static_cast<float>(viewport.right - viewport.left) / static_cast<float>(viewport.bottom - viewport.top), near_clip, far_clip);
        renderStates.vertexShader = m_p3DVertexShaderPNCT;
        renderStates.pixelShader = m_p3DPixelShaderCT;
        renderStates.depthEnabled = true;
        renderStates.depthWrite = true;
        renderStates.backFaceCull = true;
        renderStates.primitiveMode = OPrimitiveTriangleList;
        renderStates.blendMode = OBlendOpaque;
    }

    Renderer::CameraMatrices Renderer::build2DCamera(const Vector2& position, float zoom)
    {
        CameraMatrices ret;
        const auto& viewport = renderStates.viewport.get();
        ret.projection = Matrix::CreateOrthographicOffCenter(0, static_cast<float>(viewport.right - viewport.left), static_cast<float>(viewport.bottom - viewport.top), 0, -999, 999);
        ret.view = Matrix::CreateTranslation(-position) * Matrix::CreateScale(zoom);
        ret.view.Invert();
        return std::move(ret);
    }

    Renderer::CameraMatrices Renderer::build2DCameraOffCenter(const Vector2& position, float zoom)
    {
        CameraMatrices ret;
        const auto& viewport = renderStates.viewport.get();
        ret.projection = Matrix::CreateOrthographicOffCenter(0, static_cast<float>(viewport.right - viewport.left), static_cast<float>(viewport.bottom - viewport.top), 0, -999, 999);
        ret.view = Matrix::CreateTranslation(-position) *
            Matrix::CreateScale(zoom) *
            Matrix::CreateTranslation({static_cast<float>(viewport.right - viewport.left) * .5f, static_cast<float>(viewport.bottom - viewport.top) * .5f, 0.f});
        ret.view.Invert();
        return std::move(ret);
    }

    Renderer::CameraMatrices Renderer::set2DCamera(const Vector2& position, float zoom)
    {
        auto ret = build2DCamera(position, zoom);
        set2DCamera(ret);
        return std::move(ret);
    }

    Renderer::CameraMatrices Renderer::set2DCameraOffCenter(const Vector2& position, float zoom)
    {
        auto ret = build2DCameraOffCenter(position, zoom);
        set2DCamera(ret);
        return std::move(ret);
    }

    void Renderer::set2DCamera(const CameraMatrices& camera, const Matrix& transform)
    {
        renderStates.projection = camera.projection;
        renderStates.view = camera.view;
        renderStates.world = transform;
    }

    OShaderRef Renderer::get3DVSForInput(bool hasColor, bool hasTexture, bool hasWeights)
    {
        if (hasColor && hasTexture && !hasWeights)
        {
            return m_p3DVertexShaderPNCT;
        }
        else if (hasColor && !hasTexture && !hasWeights)
        {
            return m_p3DVertexShaderPNC;
        }
        else if (!hasColor && hasTexture && !hasWeights)
        {
            return m_p3DVertexShaderPNT;
        }
        else if (!hasColor && !hasTexture && !hasWeights)
        {
            return m_p3DVertexShaderPN;
        }
        if (hasColor && hasTexture && hasWeights)
        {
            return m_p3DVertexShaderPNCTW;
        }
        else if (hasColor && !hasTexture && hasWeights)
        {
            return m_p3DVertexShaderPNCW;
        }
        else if (!hasColor && hasTexture && hasWeights)
        {
            return m_p3DVertexShaderPNTW;
        }
        else if (!hasColor && !hasTexture && hasWeights)
        {
            return m_p3DVertexShaderPNW;
        }
        return nullptr; // Unreachable
    }

    OShaderRef Renderer::get3DPSForInput(bool hasTexture)
    {
        if (hasTexture)
        {
            return m_p3DPixelShaderCT;
        }
        else if (!hasTexture)
        {
            return m_p3DPixelShaderC;
        }
        return nullptr; // Unreachable
    }

    void Renderer::loadShaders()
    {
        // Create 2D shaders
        {
            m_p2DVertexShader = OShader::createFromSource(SHADER_SRC_2D_VS, OVertexShader);
            m_p2DPixelShader = OShader::createFromSource(SHADER_SRC_2D_PS, OPixelShader);
        }

        // Create 3D shaders
        {
            m_p3DVertexShaderPNCT = OShader::createFromSource(SHADER_SRC_3D_PNCT_VS, OVertexShader);
            m_p3DVertexShaderPNC = OShader::createFromSource(SHADER_SRC_3D_PNC_VS, OVertexShader);
            m_p3DVertexShaderPNT = OShader::createFromSource(SHADER_SRC_3D_PNT_VS, OVertexShader);
            m_p3DVertexShaderPN = OShader::createFromSource(SHADER_SRC_3D_PN_VS, OVertexShader);
#if !defined(ONUT_USE_OPENGL)
            m_p3DVertexShaderPNCTW = OShader::createFromSource(SHADER_SRC_3D_PNCTW_VS, OVertexShader);
            m_p3DVertexShaderPNCW = OShader::createFromSource(SHADER_SRC_3D_PNCW_VS, OVertexShader);
            m_p3DVertexShaderPNTW = OShader::createFromSource(SHADER_SRC_3D_PNTW_VS, OVertexShader);
            m_p3DVertexShaderPNW = OShader::createFromSource(SHADER_SRC_3D_PNW_VS, OVertexShader);
#endif
            m_p3DPixelShaderCT = OShader::createFromSource(SHADER_SRC_3D_CT_PS, OPixelShader);
            m_p3DPixelShaderC = OShader::createFromSource(SHADER_SRC_3D_C_PS, OPixelShader);

            // Setup default uniform values
            Vector3 sunDir(1, 1.5f, 2.0f);
            sunDir.Normalize();
            setSun(sunDir, Color(1.25f, 1.2f, 1.15f));
            setAlphaTestBias(0.3f);
            setAmbient(Color::Black);
            setLight(0, Vector3::Zero, 0.0f, Color::Black);
            setLight(1, Vector3::Zero, 0.0f, Color::Black);
            setLight(2, Vector3::Zero, 0.0f, Color::Black);
        }

        // Effects
        {
#if defined(__unix__) || defined(__APPLE__) || defined(ONUT_USE_OPENGL)
            m_pEffectsVertexShader = OShader::createFromSource(SHADER_SRC_EFFECTS_FLIP_Y_VS, OVertexShader);
#else
            m_pEffectsVertexShader = OShader::createFromSource(SHADER_SRC_EFFECTS_VS, OVertexShader);
#endif
            m_pBlurHPixelShader = OShader::createFromSource(SHADER_SRC_BLURH_PS, OPixelShader);
            m_pBlurVPixelShader = OShader::createFromSource(SHADER_SRC_BLURV_PS, OPixelShader);
            m_pSepiaPixelShader = OShader::createFromSource(SHADER_SRC_SEPIA_PS, OPixelShader);
            m_pCRTPixelShader = OShader::createFromSource(SHADER_SRC_CRT_PS, OPixelShader);
            m_pCartoonPixelShader = OShader::createFromSource(SHADER_SRC_CARTOON_PS, OPixelShader);
            m_pVignettePixelShader = OShader::createFromSource(SHADER_SRC_VIGNETTE_PS, OPixelShader);
        }
    }

    void Renderer::setAlphaTestBias(float bias)
    {
        m_p3DPixelShaderCT->setFloat(0, bias);
    }

    void Renderer::setSun(const Vector3& direction, const Color& color)
    {
        m_p3DVertexShaderPNCT->setVector3(0, direction);
        m_p3DVertexShaderPNCT->setVector3(1, color.ToVector3());
        m_p3DVertexShaderPNC->setVector3(0, direction);
        m_p3DVertexShaderPNC->setVector3(1, color.ToVector3());
        m_p3DVertexShaderPNT->setVector3(0, direction);
        m_p3DVertexShaderPNT->setVector3(1, color.ToVector3());
        m_p3DVertexShaderPN->setVector3(0, direction);
        m_p3DVertexShaderPN->setVector3(1, color.ToVector3());
#if !defined(ONUT_USE_OPENGL)
        m_p3DVertexShaderPNCTW->setVector3(0, direction);
        m_p3DVertexShaderPNCTW->setVector3(1, color.ToVector3());
        m_p3DVertexShaderPNCW->setVector3(0, direction);
        m_p3DVertexShaderPNCW->setVector3(1, color.ToVector3());
        m_p3DVertexShaderPNTW->setVector3(0, direction);
        m_p3DVertexShaderPNTW->setVector3(1, color.ToVector3());
        m_p3DVertexShaderPNW->setVector3(0, direction);
        m_p3DVertexShaderPNW->setVector3(1, color.ToVector3());
#endif
    }

    void Renderer::setLight(int index, const Vector3& position, float radius, const Color& color)
    {
        if (index < 0 || index > 2) return;
        m_p3DVertexShaderPNCT->setVector4(3 + index * 2 + 0, Vector4(position, radius));
        m_p3DVertexShaderPNCT->setVector3(3 + index * 2 + 1, color.ToVector3());
        m_p3DVertexShaderPNC->setVector4(3 + index * 2 + 0, Vector4(position, radius));
        m_p3DVertexShaderPNC->setVector3(3 + index * 2 + 1, color.ToVector3());
        m_p3DVertexShaderPNT->setVector4(3 + index * 2 + 0, Vector4(position, radius));
        m_p3DVertexShaderPNT->setVector3(3 + index * 2 + 1, color.ToVector3());
        m_p3DVertexShaderPN->setVector4(3 + index * 2 + 0, Vector4(position, radius));
        m_p3DVertexShaderPN->setVector3(3 + index * 2 + 1, color.ToVector3());
#if !defined(ONUT_USE_OPENGL)
        m_p3DVertexShaderPNCTW->setVector4(3 + index * 2 + 0, Vector4(position, radius));
        m_p3DVertexShaderPNCTW->setVector3(3 + index * 2 + 1, color.ToVector3());
        m_p3DVertexShaderPNCW->setVector4(3 + index * 2 + 0, Vector4(position, radius));
        m_p3DVertexShaderPNCW->setVector3(3 + index * 2 + 1, color.ToVector3());
        m_p3DVertexShaderPNTW->setVector4(3 + index * 2 + 0, Vector4(position, radius));
        m_p3DVertexShaderPNTW->setVector3(3 + index * 2 + 1, color.ToVector3());
        m_p3DVertexShaderPNW->setVector4(3 + index * 2 + 0, Vector4(position, radius));
        m_p3DVertexShaderPNW->setVector3(3 + index * 2 + 1, color.ToVector3());
#endif
    }

    void Renderer::setAmbient(const Color& color)
    {
        m_p3DVertexShaderPNCT->setVector3(2, color.ToVector3());
        m_p3DVertexShaderPNC->setVector3(2, color.ToVector3());
        m_p3DVertexShaderPNT->setVector3(2, color.ToVector3());
        m_p3DVertexShaderPN->setVector3(2, color.ToVector3());
#if !defined(ONUT_USE_OPENGL)
        m_p3DVertexShaderPNCTW->setVector3(2, color.ToVector3());
        m_p3DVertexShaderPNCW->setVector3(2, color.ToVector3());
        m_p3DVertexShaderPNTW->setVector3(2, color.ToVector3());
        m_p3DVertexShaderPNW->setVector3(2, color.ToVector3());
#endif
    }

    void Renderer::setupEffectRenderStates()
    {
        renderStates.depthEnabled = false;
        renderStates.scissorEnabled = false;
        renderStates.backFaceCull = false;
        renderStates.blendMode = BlendMode::Opaque;
        renderStates.sampleFiltering = OFilterLinear;
        renderStates.sampleAddressMode = OTextureClamp;
        renderStates.vertexShader = m_pEffectsVertexShader;
        renderStates.vertexBuffer = m_pEffectsVertexBuffer;
    }

    void Renderer::setSepia(const Vector3& tone, float saturation, float sepiaAmount)
    {
        m_pSepiaPixelShader->setVector3(0, tone);
        m_pSepiaPixelShader->setFloat(1, 1.f - saturation);
        m_pSepiaPixelShader->setFloat(2, sepiaAmount);
    }

    void Renderer::setVignette(const Vector2& kernelSize, float amount)
    {
        m_pVignettePixelShader->setVector2(0, kernelSize);
        m_pVignettePixelShader->setFloat(1, amount);
    }

    void Renderer::setCartoon(const Vector3& tone)
    {
        m_pCartoonPixelShader->setVector3(0, tone);
    }

    void Renderer::setCRT(const Vector2& resolution)
    {
        m_pCRTPixelShader->setVector2(0, { resolution.x, resolution.y });
        m_pCRTPixelShader->setVector2(1, { 1.f / resolution.x, 1.f / resolution.y });
    }

    void Renderer::setKernelSize(const Vector2& kernelSize)
    {
        m_pBlurHPixelShader->setVector2(0, kernelSize);
        m_pBlurVPixelShader->setVector2(0, kernelSize);
    }

    void Renderer::drawBlurH()
    {
        setupEffectRenderStates();
        renderStates.pixelShader = m_pBlurHPixelShader;
        draw(6);
    }

    void Renderer::drawBlurV()
    {
        setupEffectRenderStates();
        renderStates.pixelShader = m_pBlurVPixelShader;
        draw(6);
    }

    void Renderer::drawSepia()
    {
        setupEffectRenderStates();
        renderStates.pixelShader = m_pSepiaPixelShader;
        draw(6);
    }

    void Renderer::drawCRT()
    {
        setupEffectRenderStates();
        renderStates.pixelShader = m_pCRTPixelShader;
        draw(6);
    }

    void Renderer::drawCartoon()
    {
        setupEffectRenderStates();
        renderStates.pixelShader = m_pCartoonPixelShader;
        draw(6);
    }

    void Renderer::drawVignette()
    {
        setupEffectRenderStates();
        renderStates.pixelShader = m_pVignettePixelShader;
        draw(6);
    }

    Point Renderer::getResolution() const
    {
        auto& pRenderTarget = renderStates.renderTargets[0].get();
        if (pRenderTarget)
        {
            return pRenderTarget->getSize();
        }
        return getTrueResolution();
    }
}
