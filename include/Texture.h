#pragma once
#include <d3d11.h>
#include <memory>

namespace onut {
    class Texture {
    public:
        static Texture* createDynamic(const POINT& size);
        static Texture* createFromFile(const std::string& filename, bool generateMipmaps = true);
        static Texture* createFromData(const POINT& size, const unsigned char* in_pData, bool in_generateMipmaps = true);

        Texture() {}
        virtual ~Texture();

        void                        bind();
        ID3D11ShaderResourceView*   getResource() const { return m_pTextureView; }
        const POINT&                getSize() const { return m_size; }
        Vector4                     getRect() const {
            return std::move(Vector4{ 0, 0, static_cast<float>(m_size.x), static_cast<float>(m_size.y) });
        }

    private:
        ID3D11Texture2D*            m_pTexture = nullptr;
        ID3D11ShaderResourceView*   m_pTextureView = nullptr;
        POINT                       m_size;
    };
}

typedef onut::Texture OTexture;
