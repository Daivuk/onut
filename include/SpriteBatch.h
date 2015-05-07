#pragma once
#include "SimpleMath.h"
#include "Texture.h"
using namespace DirectX::SimpleMath;

namespace onut
{
    class SpriteBatch
    {
    public:
        SpriteBatch();
        virtual ~SpriteBatch();

        void begin();
        void drawRect(Texture* pTexture, const Rect& rect, const Color& color = Color::White);
        void drawInclinedRect(Texture* pTexture, const Rect& rect, float inclinedRatio = -1.f, const Color& color = Color::White);
        void drawRectWithColors(Texture* pTexture, const Rect& rect, const std::vector<Color>& colors);
        void drawRectWithUVs(Texture* pTexture, const Rect& rect, const Vector4& uvs, const Color& color = Color::White);
        void drawRectWithUVsColors(Texture* pTexture, const Rect& rect, const Vector4& uvs, const std::vector<Color>& colors);
        void drawRectScaled9(Texture* pTexture, const Rect& rect, const Vector4& padding, const Color& color = Color::White);
        void drawRectScaled9RepeatCenters(Texture* pTexture, const Rect& rect, const Vector4& padding, const Color& color = Color::White);
        void draw4Corner(Texture* pTexture, const Rect& rect, const Color& color = Color::White);
        void drawSprite(Texture* pTexture, const Vector2& position, const Color& color = Color::White);
        void drawSprite(Texture* pTexture, const Vector2& position, const Color& color, float rotation, float scale = 0.f);
        void end();

    private:
#ifndef EASY_GRAPHIX
        struct SVertexP2T2C4
        {
            Vector2 position;
            Vector2 texCoord;
            Color   color;
        };

        static const int MAX_SPRITE_COUNT = 300;


        ID3D11Buffer*               m_pVertexBuffer = nullptr;
        ID3D11Buffer*               m_pIndexBuffer = nullptr;
        D3D11_MAPPED_SUBRESOURCE    m_pMappedVertexBuffer;

        static const unsigned int   m_stride = sizeof(SVertexP2T2C4);
        static const unsigned int   m_offset = 0;

        bool                        m_isDrawing = false;

        Texture*                    m_pTexWhite = nullptr;
#endif /* !EASY_GRAPHIX */
        void flush();

        Texture*                    m_pTexture = nullptr;
        unsigned int                m_spriteCount = 0;
    };
}
