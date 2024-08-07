#ifndef TEXTURED3D11_H_INCLUDED
#define TEXTURED3D11_H_INCLUDED

#if defined(WIN32)
// Onut
#include <onut/Texture.h>

// Third party
#include <d3d11.h>

// Forward
#include <onut/ForwardDeclaration.h>
OForwardDeclare(TextureD3D11);

extern bool oGenerateMipmaps;

namespace onut
{
    class TextureD3D11 final : public Texture
    {
    public:
        ~TextureD3D11();

        ID3D11Texture2D* getD3DTexture() const { return m_pTexture; }
        ID3D11ShaderResourceView* getD3DResourceView();
        ID3D11RenderTargetView* getD3DRenderTargetView();

        void clearRenderTarget(const Color& color) override;

        // Apply effects. It will only work if the texture is a render target
        void blur(float amountX = 16.f, float amountY = -1.0f) override; // Blur radius
        void sepia(const Vector3& tone = Vector3(1.40f, 1.10f, 0.90f), // 0 - 2.55
                           float saturation = 0, // 0 - 1
                           float sepiaAmount = .75f) override; // 0 - 1
        void crt() override;
        void cartoon(const Vector3& tone = Vector3(2, 5, 1)) override; // Values range from 1 to infinite
        void vignette(float amount = .5f) override; // 0 - 1

        void setData(const uint8_t* pData) override;
        void resizeTarget(const Point& size) override;

    protected:
        TextureD3D11() {}

    private:
        friend Texture;
        friend RendererD3D11;

        void createRenderTargetViews(ID3D11Texture2D*& pTexture, ID3D11ShaderResourceView*& pTextureView, ID3D11RenderTargetView*& pRenderTargetView);
        DXGI_FORMAT getDXGIFormat();
        UINT getPitch();
        void resolve();

        ID3D11Texture2D* m_pResolvedTexture = nullptr; // Used for Multisample only
        ID3D11ShaderResourceView* m_pResolvedTextureView = nullptr;

        ID3D11Texture2D* m_pTexture = nullptr;
        ID3D11ShaderResourceView* m_pTextureView = nullptr;
        ID3D11RenderTargetView* m_pRenderTargetView = nullptr;

        ID3D11Texture2D* m_pTextureFX = nullptr;
        ID3D11ShaderResourceView* m_pTextureViewFX = nullptr;
        ID3D11RenderTargetView* m_pRenderTargetViewFX = nullptr;

        bool needResolve = false;
        bool multiSampled = false;
    };
}

#endif

#endif
