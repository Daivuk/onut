#ifndef SPRITEBATCH_H_INCLUDED
#define SPRITEBATCH_H_INCLUDED

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
    class RenderStates;

    class SpriteBatch
    {
    public:
        struct SVertexP2T2C4
        {
            Vector2 position;
            Vector2 texCoord;
            Color   color;
        };

        static OSpriteBatchRef create();

        SpriteBatch();
        virtual ~SpriteBatch();

        void begin();
        void begin(const Matrix& transform);
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
        void drawSprite(const OTextureRef& pTexture, const Matrix& transform, const Vector2& scale, const Color& color = Color::White, const Vector2& origin = OCenter);
        void drawSprite(const OTextureRef& pTexture, const Vector2& position, const Color& color, float rotation, float scale = 1.f, const Vector2& origin = OCenter);
        void drawSpriteWithUVs(const OTextureRef& pTexture, const Vector2& position, const Vector4& uvs, const Color& color, float rotation, float scale = 1.f, const Vector2& origin = OCenter);
        void drawSpriteWithUVs(const OTextureRef& pTexture, const Matrix& transform, const Vector4& uvs, const Color& color, const Vector2& origin = OCenter);
        void drawSpriteWithUVs(const OTextureRef& pTexture, const Matrix& transform, const Vector2& scale, const Vector4& uvs, const Color& color, const Vector2& origin = OCenter);
        void drawBeam(const OTextureRef& pTexture, const Vector2& from, const Vector2& to, float size, const Color& color, float uOffset = 0.f, float uScale = 1.f);
        void drawCross(const Vector2& position, float size, const Color& color = Color::White, float thickness = 2.f);
        void drawOutterOutlineRect(const Rect& rect, float thickness, const Color& color = Color::White);
        void end();

        const Matrix& getTransform() const { return m_currentTransform; }

        bool isInBatch() const { return m_isDrawing; };

        void flush();

    private:
        static const int MAX_SPRITE_COUNT = 300;

        OVertexBufferRef m_pVertexBuffer;
        OIndexBufferRef m_pIndexBuffer;
        SVertexP2T2C4* m_pMappedVertexBuffer = nullptr;

        bool m_isDrawing = false;
        bool m_snapToPixel = false;

        OTextureRef m_pTexWhite = nullptr;

        void changeTexture(const OTextureRef& pTexture);

        OTextureRef m_pTexture = nullptr;
        unsigned int m_spriteCount = 0;
        Matrix m_currentTransform;
        RenderStates *m_pRenderStates = nullptr;
    };
}

extern OSpriteBatchRef oSpriteBatch;

#endif
