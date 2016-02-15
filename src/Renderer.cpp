#include "onut/IndexBuffer.h"
#include "onut/Renderer.h"
#include "onut/Shader.h"
#include "onut/Texture.h"
#include "onut/VertexBuffer.h"

#include "Window.h"

#include <fstream>
#include <vector>

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
}
