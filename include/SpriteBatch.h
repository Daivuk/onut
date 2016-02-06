#pragma once
#include "SimpleMath.h"
#include "Texture.h"
using namespace DirectX::SimpleMath;

namespace onut
{
    class SpriteBatch
    {
    public:
        enum class eBlendMode
        {
            Opaque,
            Alpha,
            Add,
            PreMultiplied,
            Multiplied,
            ForceWrite,
            BlendModeCount
        };

        enum class eFiltering
        {
            Nearest,
            Linear,
            FilteringCount
        };

        SpriteBatch();
        virtual ~SpriteBatch();

        void begin(
            const Matrix& transform = Matrix::Identity,
            eBlendMode blendMode = eBlendMode::PreMultiplied);
        void begin(eBlendMode blendMode);
        void drawAbsoluteRect(Texture* pTexture, const Rect& rect, const Color& color = Color::White);
        void drawRect(Texture* pTexture, const Rect& rect, const Color& color = Color::White);
        void drawInclinedRect(Texture* pTexture, const Rect& rect, float inclinedRatio = -1.f, const Color& color = Color::White);
        void drawRectWithColors(Texture* pTexture, const Rect& rect, const std::vector<Color>& colors);
        void drawRectWithUVs(Texture* pTexture, const Rect& rect, const Vector4& uvs, const Color& color = Color::White);
        void drawRectWithUVsColors(Texture* pTexture, const Rect& rect, const Vector4& uvs, const std::vector<Color>& colors);
        void drawRectScaled9(Texture* pTexture, const Rect& rect, const Vector4& padding, const Color& color = Color::White);
        void drawRectScaled9RepeatCenters(Texture* pTexture, const Rect& rect, const Vector4& padding, const Color& color = Color::White);
        void draw4Corner(Texture* pTexture, const Rect& rect, const Color& color = Color::White);
        void drawSprite(Texture* pTexture, const Vector2& position, const Color& color = Color::White, const Vector2& origin = Vector2(0.5f, 0.5f));
        void drawSprite(Texture* pTexture, const Matrix& transform, const Color& color = Color::White, const Vector2& origin = Vector2(0.5f, 0.5f));
        void drawSprite(Texture* pTexture, const Vector2& position, const Color& color, float rotation, float scale = 1.f, const Vector2& origin = Vector2(0.5f, 0.5f));
        void drawSpriteWithUVs(Texture* pTexture, const Vector2& position, const Vector4& uvs, const Color& color, float rotation, float scale = 1.f, const Vector2& origin = Vector2(0.5f, 0.5f));
        void drawSpriteWithUVs(Texture* pTexture, const Matrix& transform, const Vector4& uvs, const Color& color, const Vector2& origin = Vector2(0.5f, 0.5f));
        void drawBeam(Texture* pTexture, const Vector2& from, const Vector2& to, float size, const Color& color, float uOffset = 0.f, float uScale = 1.f);
        void drawCross(const Vector2& position, float size, const Color& color = Color::White, float thickness = 2.f);
        void end();

        void changeBlendMode(eBlendMode blendMode);
        void changeFiltering(eFiltering filtering);

        const Matrix& getTransform() const { return m_currentTransform; }

        bool isInBatch() const { return m_isDrawing; };

    private:
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

        void flush();

        Texture*                    m_pTexture = nullptr;
        unsigned int                m_spriteCount = 0;
        eBlendMode                  m_curBlendMode = eBlendMode::PreMultiplied;
        ID3D11BlendState*           m_pBlendStates[static_cast<int>(eBlendMode::BlendModeCount)];
        eFiltering                  m_curFiltering = eFiltering::Linear;
        ID3D11SamplerState*         m_pSamplers[static_cast<int>(eFiltering::FilteringCount)];
        Matrix                      m_currentTransform;
    };
}
