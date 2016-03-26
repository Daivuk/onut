#pragma once
// Onut
#include <onut/Maths.h>
#include <onut/Point.h>
#include <onut/Resource.h>

// Third party
#if defined(WIN32)
#include <d3d11.h>
#endif

// Forward
#include <onut/ForwardDeclaration.h>
OForwardDeclare(Texture);
OForwardDeclare(ContentManager);

extern bool oGenerateMipmaps;

namespace onut
{
    class Texture final : public Resource, public std::enable_shared_from_this<Texture>
    {
    public:
        static OTextureRef createFromFile(const std::string& filename, const OContentManagerRef& pContentManager = nullptr, bool generateMipmaps = oGenerateMipmaps);
        static OTextureRef createFromFileData(const uint8_t* pData, uint32_t size, bool generateMipmaps = oGenerateMipmaps);
        static OTextureRef createFromData(const uint8_t* pData, const Point& size, bool generateMipmaps = oGenerateMipmaps);
        static OTextureRef createRenderTarget(const Point& size, bool willBeUsedInEffects = false);
        static OTextureRef createScreenRenderTarget(bool willBeUsedInEffects = false);
        static OTextureRef createDynamic(const Point& size);

        ~Texture();

        const Point& getSize() const;
        Vector2 getSizef() const;
        void bind(int slot = 0);
        void setData(const uint8_t* pData);
        bool isRenderTarget() const;
        bool isDynamic() const;

        void bindRenderTarget();
        void unbindRenderTarget();
        void resizeTarget(const Point& size);
        void clearRenderTarget(const Color& color);

#if defined(WIN32)
        ID3D11Texture2D* getD3DTexture() const { return m_pTexture; }
        ID3D11ShaderResourceView* getD3DResourceView() const { return m_pTextureView; }
        ID3D11RenderTargetView* getD3DRenderTargetView() const { return m_pRenderTargetView; }
#endif

        // Apply effects. It will only work if the texture is a render target
        void blur(float amount = 16.f); // Blur radius
        void sepia(const Vector3& tone = Vector3(1.40f, 1.10f, 0.90f), // 0 - 2.55
                   float saturation = 0, // 0 - 1
                   float sepiaAmount = .75f); // 0 - 1
        void crt();
        void cartoon(const Vector3& tone = Vector3(2, 5, 1)); // Values range from 1 to infinite
        void vignette(float amount = .5f); // 0 - 1

    private:
        enum class Type
        {
            Static,
            Dynamic,
            RenderTarget,
            ScreenRenderTarget
        };

#if defined(WIN32)
        void createRenderTargetViews(ID3D11Texture2D*& pTexture, ID3D11ShaderResourceView*& pTextureView, ID3D11RenderTargetView*& pRenderTargetView);
#endif

        Point m_size;
        Type m_type;
#if defined(WIN32)
        ID3D11Texture2D* m_pTexture = nullptr;
        ID3D11ShaderResourceView* m_pTextureView = nullptr;
        ID3D11RenderTargetView* m_pRenderTargetView = nullptr;
        ID3D11Texture2D* m_pTextureFX = nullptr;
        ID3D11ShaderResourceView* m_pTextureViewFX = nullptr;
        ID3D11RenderTargetView* m_pRenderTargetViewFX = nullptr;
#endif
        bool m_isScreenRenderTarget = false;
    };
}

OTextureRef OGetTexture(const std::string& name);
