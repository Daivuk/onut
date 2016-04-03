#pragma once
#if defined(WIN32)
// Onut
#include <onut/Renderer.h>

// Third party
#include <d3d11.h>

// Forward
#include <onut/ForwardDeclaration.h>
OForwardDeclare(RendererD3D11);

namespace onut
{
    class RendererD3D11 final : public Renderer
    {
    public:
        RendererD3D11(const OWindowRef& pWindow);
        ~RendererD3D11();

        void clear(const Color& color = {.25f, .5f, 1, 1});

        void beginFrame();
        void endFrame();

        void draw(uint32_t vertexCount) override;
        void drawIndexed(uint32_t indexCount) override;

        Point getResolution() const override;
        void onResize(const Point& newSize);

        ID3D11Device* getDevice() const;
        ID3D11DeviceContext* getDeviceContext() const;

        // For effects
        void setKernelSize(const Vector2& kernelSize);
        void setSepia(const Vector3& tone = Vector3(1.40f, 1.10f, 0.90f), // 0 - 2.55
                      float saturation = .25f, // 0 - 1
                      float sepiaAmount = .75f); // 0 - 1
        void setCRT(const Vector2& kernelSize);
        void setCartoon(const Vector3& tone);
        void setVignette(const Vector2& kernelSize, float amount = .5f);

        void applyRenderStates() override;
        void init(const OWindowRef& pWindow) override;

    private:
        void createDevice(const OWindowRef& pWindow);
        void createRenderTarget();
        void createRenderStates();
        void createUniforms();

        // Device stuff
        IDXGISwapChain* m_pSwapChain = nullptr;
        ID3D11Device* m_pDevice = nullptr;
        ID3D11DeviceContext* m_pDeviceContext = nullptr;

        // Render target
        ID3D11RenderTargetView* m_pRenderTargetView = nullptr;
        D3D11_TEXTURE2D_DESC m_backBufferDesc;

        // Render states
        ID3D11BlendState* m_pBlendStates[static_cast<int>(BlendMode::COUNT)];
        ID3D11SamplerState* m_pSamplerStates[static_cast<int>(sample::Filtering::COUNT) * static_cast<int>(sample::AddressMode::COUNT)];
        ID3D11RasterizerState* m_pRasterizerStates[2];
        ID3D11DepthStencilState* m_pDepthStencilStates[1];

        // Constant buffers
        ID3D11Buffer* m_pViewProj2dBuffer = nullptr;
        ID3D11Buffer* m_pKernelSizeBuffer = nullptr;
        ID3D11Buffer* m_pSepiaBuffer = nullptr;

        OTextureRef m_boundTextures[RenderStates::MAX_TEXTURES];
    };
};

#endif
