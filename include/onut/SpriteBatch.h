#pragma once
// Onut
#include <onut/BlendMode.h>
#include <onut/Maths.h>
#include <onut/SampleMode.h>

// STL
#include <vector>

// Forward
#include <onut/ForwardDeclaration.h>
OForwardDeclare(IndexBuffer);
OForwardDeclare(SpriteBatch);
OForwardDeclare(Texture);
OForwardDeclare(VertexBuffer);

namespace onut
{
    class SpriteBatch
    {
    public:
        static OSpriteBatchRef create();

        SpriteBatch();
        virtual ~SpriteBatch();

        void begin(const Matrix& transform = Matrix::Identity, BlendMode blendMode = BlendMode::PreMultiplied);
        void begin(BlendMode blendMode);
        void drawAbsoluteRect(const OTextureRef& pTexture, const Rect& rect, const Color& color = Color::White);
        void drawRect(const OTextureRef& pTexture, const Rect& rect, const Color& color = Color::White);
        void drawInclinedRect(const OTextureRef& pTexture, const Rect& rect, float inclinedRatio = -1.f, const Color& color = Color::White);
        void drawRectWithColors(const OTextureRef& pTexture, const Rect& rect, const std::vector<Color>& colors);
        void drawRectWithUVs(const OTextureRef& pTexture, const Rect& rect, const Vector4& uvs, const Color& color = Color::White);
        void drawRectWithUVsColors(const OTextureRef& pTexture, const Rect& rect, const Vector4& uvs, const std::vector<Color>& colors);
        void drawRectScaled9(const OTextureRef& pTexture, const Rect& rect, const Vector4& padding, const Color& color = Color::White);
        void drawRectScaled9RepeatCenters(const OTextureRef& pTexture, const Rect& rect, const Vector4& padding, const Color& color = Color::White);
        void draw4Corner(const OTextureRef& pTexture, const Rect& rect, const Color& color = Color::White);
        void drawSprite(const OTextureRef& pTexture, const Vector2& position, const Color& color = Color::White, const Vector2& origin = OCenter);
        void drawSprite(const OTextureRef& pTexture, const Matrix& transform, const Color& color = Color::White, const Vector2& origin = OCenter);
        void drawSprite(const OTextureRef& pTexture, const Vector2& position, const Color& color, float rotation, float scale = 1.f, const Vector2& origin = OCenter);
        void drawSpriteWithUVs(const OTextureRef& pTexture, const Vector2& position, const Vector4& uvs, const Color& color, float rotation, float scale = 1.f, const Vector2& origin = OCenter);
        void drawSpriteWithUVs(const OTextureRef& pTexture, const Matrix& transform, const Vector4& uvs, const Color& color, const Vector2& origin = OCenter);
        void drawBeam(const OTextureRef& pTexture, const Vector2& from, const Vector2& to, float size, const Color& color, float uOffset = 0.f, float uScale = 1.f);
        void drawCross(const Vector2& position, float size, const Color& color = Color::White, float thickness = 2.f);
        void drawOutterOutlineRect(const Rect& rect, float thickness, const Color& color = Color::White);
        void end();

        void changeBlendMode(BlendMode blendMode);
        void changeFiltering(sample::Filtering filtering);

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

        OVertexBufferRef m_pVertexBuffer;
        OIndexBufferRef m_pIndexBuffer;
        SVertexP2T2C4* m_pMappedVertexBuffer = nullptr;

        bool m_isDrawing = false;

        OTextureRef m_pTexWhite = nullptr;

        void flush();
        void changeTexture(const OTextureRef& pTexture);

        OTextureRef m_pTexture = nullptr;
        unsigned int m_spriteCount = 0;
        BlendMode m_curBlendMode = BlendMode::PreMultiplied;
        sample::Filtering m_curFiltering = sample::Filtering::Linear;
        Matrix m_currentTransform;
    };
}

extern OSpriteBatchRef oSpriteBatch;
