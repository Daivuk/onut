#pragma once
#include <d3d11.h>
#include <vector>

#include "SimpleMath.h"
using namespace DirectX::SimpleMath;

namespace onut
{
    class Texture
    {
    public:
        struct sSize
        {
            uint32_t x;
            uint32_t y;
        };

        static Texture* createRenderTarget(const sSize& size = {1, 1}, bool willBeUsedInEffects = false);
        static Texture* createScreenRenderTarget(bool willBeUsedInEffects = false);
        static Texture* createDynamic(const sSize& size);
        template<typename TcontentManagerType>
        static Texture* createFromFile(const std::string& filename, TcontentManagerType* pContentManager, bool generateMipmaps = true)
        {
            return Texture::createFromFile(filename, generateMipmaps);
        }
        static Texture* createFromFile(const std::string& filename, bool generateMipmaps = true);
        static Texture* createFromFileData(const std::vector<uint8_t>& data, bool in_generateMipmaps = true);
        static Texture* createFromFileData(const unsigned char* in_pData, uint32_t in_size, bool in_generateMipmaps = true);
        static Texture* createFromData(const sSize& size, const unsigned char* in_pData, bool in_generateMipmaps = true);

        Texture() {}
        virtual ~Texture();

        void setData(const uint8_t *in_pData);

        const std::string&          getName() const { return m_name; }
        void                        resizeTarget(const sSize& size);
        void                        bind(int slot = 0);
        void                        bindRenderTarget();
        void                        unbindRenderTarget();
        void                        clearRenderTarget(const Color& color);
        const sSize&                getSize() const { return m_size; }
        Vector2                     getSizef() const
        {
            return std::move(Vector2(static_cast<float>(m_size.x), static_cast<float>(m_size.y))); 
        }
        Vector4                     getRect() const
        {
            return std::move(Vector4{0, 0, static_cast<float>(m_size.x), static_cast<float>(m_size.y)});
        }

        ID3D11Texture2D*            getResource() const { return m_pTexture; }
        ID3D11ShaderResourceView*   getResourceView() const { return m_pTextureView; }
        ID3D11RenderTargetView*     getRenderTargetResource() const { return m_pRenderTargetView; }

        // Apply effects. It will only work if the texture is a render target
        void blur(float amount = 16.f); // Blur radius
        void sepia(const Vector3& tone = Vector3(1.40f, 1.10f, 0.90f), // 0 - 2.55
                   float saturation = 0, // 0 - 1
                   float sepiaAmount = .75f); // 0 - 1
        void crt();
        void cartoon(const Vector3& tone = Vector3(2, 5, 1)); // Values range from 1 to infinite
        void vignette(float amount = .5f); // 0 - 1

    private:
        void createRenderTargetViews(ID3D11Texture2D*& pTexture, ID3D11ShaderResourceView*& pTextureView, ID3D11RenderTargetView*& pRenderTargetView);

        sSize                       m_size;
        ID3D11Texture2D*            m_pTexture = nullptr;
        ID3D11ShaderResourceView*   m_pTextureView = nullptr;
        ID3D11RenderTargetView*     m_pRenderTargetView = nullptr;
        ID3D11Texture2D*            m_pTextureFX = nullptr;
        ID3D11ShaderResourceView*   m_pTextureViewFX = nullptr;
        ID3D11RenderTargetView*     m_pRenderTargetViewFX = nullptr;
        bool                        m_isScreenRenderTarget = false;
        std::string                 m_name;
    };
}

typedef onut::Texture OTexture;
