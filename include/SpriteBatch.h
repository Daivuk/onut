#pragma once
#include "Typedefs.h"
#include "Texture.h"

#include <memory>

namespace onut {
    class SpriteBatch {
    public:
        SpriteBatch();
        virtual ~SpriteBatch();

        void begin();
        void drawRect(std::shared_ptr<Texture> pTexture, const Rect& rect, const Color& color = Color::White);
        void drawRectWithColors(std::shared_ptr<Texture> pTexture, const Rect& rect, const std::vector<Color>& colors);
        void drawRectWithUVs(std::shared_ptr<Texture> pTexture, const Rect& rect, const Vector4& uvs, const Color& color = Color::White);
        void drawRectWithUVsColors(std::shared_ptr<Texture> pTexture, const Rect& rect, const Vector4& uvs, const std::vector<Color>& colors);
        void end();

    private:
        struct SVertexP2T2C4 {
            Vector2 position;
            Vector2 texCoord;
            Color    color;
        };

        static const int MAX_SPRITE_COUNT = 300;

        void flush();

        ID3D11Buffer*                m_pVertexBuffer = nullptr;
        ID3D11Buffer*                m_pIndexBuffer = nullptr;
        D3D11_MAPPED_SUBRESOURCE    m_pMappedVertexBuffer;

        std::shared_ptr<Texture>    m_pTexWhite = nullptr;
        std::shared_ptr<Texture>    m_pTexture = nullptr;

        static const unsigned int    m_stride = sizeof(SVertexP2T2C4);
        static const unsigned int    m_offset = 0;
        unsigned int                m_spriteCount = 0;

        bool                        m_isDrawing = false;
    };
}
