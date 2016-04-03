// Onut
#include <onut/IndexBuffer.h>
#include <onut/Renderer.h>
#include <onut/Shader.h>
#include <onut/Texture.h>
#include <onut/VertexBuffer.h>
#include <onut/Window.h>

// STL
#include <fstream>
#include <vector>

// Shaders
#include "_2dvs.cso.h"
#include "_2dps.cso.h"
#include "blurvs.cso.h"
#include "blurhps.cso.h"
#include "blurvps.cso.h"
#include "sepia.cso.h"
#include "crt.cso.h"
#include "cartoon.cso.h"
#include "vignette.cso.h"

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
        viewProjection = Matrix::Identity;
        world = Matrix::Identity;
        depthEnabled = false;
        scissorEnabled = false;
        primitiveMode = OPrimitiveTriangleList;
        vertexShader = nullptr;
        pixelShader = nullptr;
        vertexBuffer = nullptr;
        indexBuffer = nullptr;
        renderTarget = nullptr;
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
        viewProjection = other.viewProjection;
        world = other.world;
        depthEnabled = other.depthEnabled;
        scissorEnabled = other.scissorEnabled;
        primitiveMode = other.primitiveMode;
        vertexShader = other.vertexShader;
        pixelShader = other.pixelShader;
        vertexBuffer = other.vertexBuffer;
        indexBuffer = other.indexBuffer;
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
        viewProjection = other.viewProjection;
        world = other.world;
        depthEnabled = other.depthEnabled;
        scissorEnabled = other.scissorEnabled;
        primitiveMode = other.primitiveMode;
        vertexShader = other.vertexShader;
        pixelShader = other.pixelShader;
        vertexBuffer = other.vertexBuffer;
        indexBuffer = other.indexBuffer;

        return *this;
    }

    void RenderStates::reset()
    {
        for (auto& pTexture : textures)
        {
            pTexture.forceDirty();
        }
        blendMode.forceDirty();
        sampleFiltering.forceDirty();
        sampleAddressMode.forceDirty();
        viewport.forceDirty();
        scissor.forceDirty();
        viewProjection.forceDirty();
        world.forceDirty();
        depthEnabled.forceDirty();
        scissorEnabled.forceDirty();
        primitiveMode.forceDirty();
        vertexShader.forceDirty();
        pixelShader.forceDirty();
        vertexBuffer.forceDirty();
        indexBuffer.forceDirty();
        renderTarget.forceDirty();
    }

    void RenderStates::operator=(const OTextureRef& texture)
    {
        textures[0] = texture;
    }

    void RenderStates::operator=(BlendMode in_blendMode)
    {
        blendMode = in_blendMode;
    }

    void RenderStates::operator=(sample::Filtering in_sampleFiltering)
    {
        sampleFiltering = in_sampleFiltering;
    }

    void RenderStates::operator=(sample::AddressMode in_sampleAddressMode)
    {
        sampleAddressMode = in_sampleAddressMode;
    }

    void RenderStates::operator=(const Matrix& in_world)
    {
        world = in_world;
    }

    void RenderStates::operator=(PrimitiveMode in_primitiveMode)
    {
        primitiveMode = in_primitiveMode;
    }

    void RenderStates::operator=(const OVertexBufferRef& in_vertexBuffer)
    {
        vertexBuffer = in_vertexBuffer;
    }

    void RenderStates::operator=(const OIndexBufferRef& in_indexBuffer)
    {
        indexBuffer = in_indexBuffer;
    }

    void RenderStates::operator=(const OShaderRef& in_shader)
    {
        switch (in_shader->getType())
        {
            case Shader::Type::Vertex:
                vertexShader = in_shader;
                break;
            case Shader::Type::Pixel:
                pixelShader = in_shader;
                break;
        }
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

    void Renderer::operator=(const OTextureRef& texture)
    {
        renderStates = texture;
    }

    void Renderer::operator=(BlendMode blendMode)
    {
        renderStates = blendMode;
    }

    void Renderer::operator=(sample::Filtering sampleFiltering)
    {
        renderStates = sampleFiltering;
    }

    void Renderer::operator=(sample::AddressMode sampleAddressMode)
    {
        renderStates = sampleAddressMode;
    }

    void Renderer::operator=(const Matrix& in_world)
    {
        renderStates = in_world;
    }

    void Renderer::operator=(PrimitiveMode in_primitiveMode)
    {
        renderStates = in_primitiveMode;
    }

    void Renderer::operator=(const OVertexBufferRef& in_vertexBuffer)
    {
        renderStates = in_vertexBuffer;
    }

    void Renderer::operator=(const OIndexBufferRef& in_indexBuffer)
    {
        renderStates = in_indexBuffer;
    }

    void Renderer::operator=(const OShaderRef& in_shader)
    {
        renderStates = in_shader;
    }

    void Renderer::setupFor2D()
    {
        setupFor2D(Matrix::Identity);
    }

    void Renderer::setupFor2D(const Matrix& transform)
    {
        set2DCamera(Vector2::Zero);
        renderStates = transform;
        renderStates = m_p2DVertexShader;
        renderStates = m_p2DPixelShader;
    }

    Matrix Renderer::build2DCamera(const Vector2& position, float zoom)
    {
        auto proj = Matrix::CreateOrthographicOffCenter(0, static_cast<float>(getResolution().x), static_cast<float>(getResolution().y), 0, -999, 999);
        auto view = Matrix::CreateTranslation(-position) * Matrix::CreateScale(zoom);
        view.Invert();
        auto viewProj = view * proj;
        viewProj = viewProj.Transpose();

        return viewProj;
    }

    Matrix Renderer::build2DCameraOffCenter(const Vector2& position, float zoom)
    {
        auto proj = Matrix::CreateOrthographicOffCenter(0, static_cast<float>(getResolution().x), static_cast<float>(getResolution().y), 0, -999, 999);
        auto view = Matrix::CreateTranslation(-position) *
            Matrix::CreateScale(zoom) *
            Matrix::CreateTranslation({static_cast<float>(getResolution().x) * .5f, static_cast<float>(getResolution().y) * .5f, 0.f});
        view.Invert();
        auto viewProj = view * proj;
        viewProj = viewProj.Transpose();

        return viewProj;
    }

    Matrix Renderer::set2DCamera(const Vector2& position, float zoom)
    {
        auto viewProj = build2DCamera(position, zoom);
        set2DCamera(viewProj);
        return viewProj;
    }

    Matrix Renderer::set2DCameraOffCenter(const Vector2& position, float zoom)
    {
        auto viewProj = build2DCameraOffCenter(position, zoom);
        set2DCamera(viewProj);
        return viewProj;
    }

    void Renderer::set2DCamera(const Matrix& viewProj, const Matrix& transform)
    {
        renderStates.viewProjection = viewProj;
        renderStates.world = transform;
    }

    void Renderer::loadShaders()
    {
        // Create 2D shaders
        {
            m_p2DVertexShader = OShader::createFromBinaryData(_2dvs_cso, sizeof(_2dvs_cso), OVertexShader, {{2, "POSITION"}, {2, "TEXCOORD"}, {4, "COLOR"}});
            m_p2DPixelShader = OShader::createFromBinaryData(_2dps_cso, sizeof(_2dps_cso), OPixelShader);
        }

        // Effects
        {
            m_pEffectsVertexShader = OShader::createFromBinaryData(blurvs_cso, sizeof(blurvs_cso), OVertexShader, {{2, "POSITION"}});
            m_pBlurHPixelShader = OShader::createFromBinaryData(blurhps_cso, sizeof(blurhps_cso), OPixelShader);
            m_pBlurVPixelShader = OShader::createFromBinaryData(blurvps_cso, sizeof(blurvps_cso), OPixelShader);
            m_pSepiaPixelShader = OShader::createFromBinaryData(sepia_cso, sizeof(sepia_cso), OPixelShader);
            m_pCRTPixelShader = OShader::createFromBinaryData(crt_cso, sizeof(crt_cso), OPixelShader);
            m_pCartoonPixelShader = OShader::createFromBinaryData(cartoon_cso, sizeof(cartoon_cso), OPixelShader);
            m_pVignettePixelShader = OShader::createFromBinaryData(vignette_cso, sizeof(vignette_cso), OPixelShader);
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
}
