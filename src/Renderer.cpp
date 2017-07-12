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
        depthEnabled = false;
        depthWrite = false;
        backFaceCull = false;
        scissorEnabled = false;
        primitiveMode = OPrimitiveTriangleList;
        vertexShader = nullptr;
        pixelShader = nullptr;
        vertexBuffer = nullptr;
        indexBuffer = nullptr;
        renderTarget = nullptr;
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
        depthEnabled.reset();
        depthWrite.reset();
        backFaceCull.reset();
        scissorEnabled.reset();
        primitiveMode.reset();
        vertexShader.reset();
        pixelShader.reset();
        vertexBuffer.reset();
        indexBuffer.reset();
        renderTarget.reset();
        clearColor.reset();
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

    void Renderer::loadShaders()
    {
        // Create 2D shaders
        {
            m_p2DVertexShader = OShader::createFromSource(SHADER_SRC_2D_VS, OVertexShader);
            m_p2DPixelShader = OShader::createFromSource(SHADER_SRC_2D_PS, OPixelShader);
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

    void Renderer::setupEffectRenderStates()
    {
        renderStates.depthEnabled = false;
        renderStates.scissorEnabled = false;
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
        auto& pRenderTarget = renderStates.renderTarget.get();
        if (pRenderTarget)
        {
            return pRenderTarget->getSize();
        }
        return getTrueResolution();
    }
}
