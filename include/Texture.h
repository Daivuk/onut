#pragma once
#ifdef EASY_GRAPHIX
#include "eg.h"
#else
#include <d3d11.h>
#endif
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

        static Texture* createRenderTarget(const sSize& size);
        static Texture* createDynamic(const sSize& size);
        template<typename TcontentManagerType>
        static Texture* createFromFile(const std::string& filename, TcontentManagerType* pContentManager, bool generateMipmaps = true)
        {
            return Texture::createFromFile(filename, generateMipmaps);
        }
        static Texture* createFromFile(const std::string& filename, bool generateMipmaps = true);
        static Texture* createFromData(const sSize& size, const unsigned char* in_pData, bool in_generateMipmaps = true);

        void setData(const uint8_t *in_pData);

        Texture() {}
        virtual ~Texture();

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

#ifdef EASY_GRAPHIX
        EGTexture                   getResource() const { return m_pTextureView; }
#else
        ID3D11ShaderResourceView*   getResource() const { return m_pTextureView; }
#endif

    private:
#ifdef EASY_GRAPHIX
        EGTexture                   m_pTextureView = 0;
#else
        ID3D11Texture2D*            m_pTexture = nullptr;
        ID3D11ShaderResourceView*   m_pTextureView = nullptr;
        ID3D11RenderTargetView*     m_pRenderTargetView = nullptr;
#endif
        sSize                       m_size;
    };
}

typedef onut::Texture OTexture;
