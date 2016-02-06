#pragma once
#include "SimpleMath.h"
#include "onut/Texture.h"
using namespace DirectX::SimpleMath;

#include "onut/forward_declaration.h"
OForwardDeclare(Texture);

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

        void begin(const Matrix& transform = Matrix::Identity, eBlendMode blendMode = eBlendMode::PreMultiplied);
        void begin(eBlendMode blendMode);
        void drawAbsoluteRect(const OTextureRef& pTexture, const Rect& rect, const Color& color = Color::White);
        void drawRect(const OTextureRef& pTexture, const Rect& rect, const Color& color = Color::White);
        void drawInclinedRect(const OTextureRef& pTexture, const Rect& rect, float inclinedRatio = -1.f, const Color& color = Color::White);
        void drawRectWithColors(const OTextureRef& pTexture, const Rect& rect, const std::vector<Color>& colors);
        void drawRectWithUVs(const OTextureRef& pTexture, const Rect& rect, const Vector4& uvs, const Color& color = Color::White);
        void drawRectWithUVsColors(const OTextureRef& pTexture, const Rect& rect, const Vector4& uvs, const std::vector<Color>& colors);
        void drawRectScaled9(const OTextureRef& pTexture, const Rect& rect, const Vector4& padding, const Color& color = Color::White);
        void drawRectScaled9RepeatCenters(const OTextureRef& pTexture, const Rect& rect, const Vector4& padding, const Color& color = Color::White);
        void draw4Corner(const OTextureRef& pTexture, const Rect& rect, const Color& color = Color::White);
        void drawSprite(const OTextureRef& pTexture, const Vector2& position, const Color& color = Color::White, const Vector2& origin = Vector2(0.5f, 0.5f));
        void drawSprite(const OTextureRef& pTexture, const Matrix& transform, const Color& color = Color::White, const Vector2& origin = Vector2(0.5f, 0.5f));
        void drawSprite(const OTextureRef& pTexture, const Vector2& position, const Color& color, float rotation, float scale = 1.f, const Vector2& origin = Vector2(0.5f, 0.5f));
        void drawSpriteWithUVs(const OTextureRef& pTexture, const Vector2& position, const Vector4& uvs, const Color& color, float rotation, float scale = 1.f, const Vector2& origin = Vector2(0.5f, 0.5f));
        void drawSpriteWithUVs(const OTextureRef& pTexture, const Matrix& transform, const Vector4& uvs, const Color& color, const Vector2& origin = Vector2(0.5f, 0.5f));
        void drawBeam(const OTextureRef& pTexture, const Vector2& from, const Vector2& to, float size, const Color& color, float uOffset = 0.f, float uScale = 1.f);
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

        OTextureRef                 m_pTexWhite = nullptr;

        void flush();
        void changeTexture(const OTextureRef& pTexture);

        OTextureRef                 m_pTexture = nullptr;
        unsigned int                m_spriteCount = 0;
        eBlendMode                  m_curBlendMode = eBlendMode::PreMultiplied;
        ID3D11BlendState*           m_pBlendStates[static_cast<int>(eBlendMode::BlendModeCount)];
        eFiltering                  m_curFiltering = eFiltering::Linear;
        ID3D11SamplerState*         m_pSamplers[static_cast<int>(eFiltering::FilteringCount)];
        Matrix                      m_currentTransform;
    };
}
