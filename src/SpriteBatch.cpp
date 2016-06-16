// Onut
#include <onut/IndexBuffer.h>
#include <onut/PrimitiveMode.h>
#include <onut/Renderer.h>
#include <onut/SpriteBatch.h>
#include <onut/Texture.h>
#include <onut/VertexBuffer.h>

// STL
#include <cmath>

OSpriteBatchRef oSpriteBatch;

namespace onut
{
    OSpriteBatchRef SpriteBatch::create()
    {
        return OMake<SpriteBatch>();
    }

    SpriteBatch::SpriteBatch()
    {
        // Create a white texture for rendering "without" texture
        unsigned char white[4] = {255, 255, 255, 255};
        m_pTexWhite = Texture::createFromData(white, {1, 1}, false);

        // Create dynamic vertex buffer
        m_pVertexBuffer = OVertexBuffer::createDynamic(sizeof(SVertexP2T2C4) * MAX_SPRITE_COUNT * 4);

        // Create index buffer
        unsigned short indices[MAX_SPRITE_COUNT * 6];
        for (unsigned int i = 0; i < MAX_SPRITE_COUNT; ++i)
        {
            indices[i * 6 + 0] = i * 4 + 0;
            indices[i * 6 + 1] = i * 4 + 1;
            indices[i * 6 + 2] = i * 4 + 2;
            indices[i * 6 + 3] = i * 4 + 2;
            indices[i * 6 + 4] = i * 4 + 3;
            indices[i * 6 + 5] = i * 4 + 0;
        }
        m_pIndexBuffer = OIndexBuffer::createStatic(indices, sizeof(indices));
    }

    SpriteBatch::~SpriteBatch()
    {
    }

    void SpriteBatch::begin(BlendMode blendMode)
    {
        begin(Matrix::Identity, blendMode);
    }

    void SpriteBatch::begin(const Matrix& transform, BlendMode blendMode)
    {
        assert(!m_isDrawing); // Cannot call begin() twice without calling end()

        oRenderer->setupFor2D(transform);

        m_currentTransform = transform;
        m_curBlendMode = blendMode;
        m_pTexture = nullptr;
        m_isDrawing = true;

        m_pMappedVertexBuffer = reinterpret_cast<SVertexP2T2C4*>(m_pVertexBuffer->map());
    }

    void SpriteBatch::changeBlendMode(BlendMode blendMode)
    {
        if (!isInBatch()) return;
        if (m_curBlendMode == blendMode) return;
        end();
        begin(m_currentTransform, blendMode);
    }

    void SpriteBatch::changeFiltering(sample::Filtering filtering)
    {
        if (m_curFiltering == filtering) return;
        auto bManageBatch = isInBatch();
        if (bManageBatch) end();
        m_curFiltering = filtering;
        if (bManageBatch) begin(m_currentTransform, m_curBlendMode);
    }

    void SpriteBatch::drawRectWithColors(const OTextureRef& pTexture, const Rect& rect, const std::vector<Color>& colors)
    {
        assert(m_isDrawing); // Should call begin() before calling draw()
        assert(colors.size() == 4); // Needs 4 colors

        changeTexture(pTexture);

        SVertexP2T2C4* pVerts = m_pMappedVertexBuffer + (m_spriteCount * 4);
        pVerts[0].position = {rect.x, rect.y};
        pVerts[0].texCoord = {0, 0};
        pVerts[0].color = colors[0];

        pVerts[1].position = {rect.x, rect.y + rect.w};
        pVerts[1].texCoord = {0, 1};
        pVerts[1].color = colors[1];

        pVerts[2].position = {rect.x + rect.z, rect.y + rect.w};
        pVerts[2].texCoord = {1, 1};
        pVerts[2].color = colors[2];

        pVerts[3].position = {rect.x + rect.z, rect.y};
        pVerts[3].texCoord = {1, 0};
        pVerts[3].color = colors[3];

        ++m_spriteCount;

        if (m_spriteCount == MAX_SPRITE_COUNT)
        {
            flush();
        }
    }

    void SpriteBatch::drawAbsoluteRect(const OTextureRef& pTexture, const Rect& rect, const Color& color)
    {
        drawRect(pTexture, {rect.x, rect.y, rect.z - rect.x, rect.w - rect.y}, color);
    }

    void SpriteBatch::drawOutterOutlineRect(const Rect& rect, float thickness, const Color& color)
    {
        assert(m_isDrawing); // Should call begin() before calling draw()

        drawRect(m_pTexWhite, {rect.x - thickness, rect.y - thickness, rect.z + thickness * 2, thickness}, color);
        drawRect(m_pTexWhite, {rect.x - thickness, rect.y + rect.w, rect.z + thickness * 2, thickness}, color);
        drawRect(m_pTexWhite, {rect.x - thickness, rect.y, thickness, rect.w}, color);
        drawRect(m_pTexWhite, {rect.x + rect.z, rect.y, thickness, rect.w}, color);
    }

    void SpriteBatch::drawRect(const OTextureRef& pTexture, const Rect& rect, const Color& color)
    {
        assert(m_isDrawing); // Should call begin() before calling draw()

        changeTexture(pTexture);

        SVertexP2T2C4* pVerts = m_pMappedVertexBuffer + (m_spriteCount * 4);
        pVerts[0].position = {rect.x, rect.y};
        pVerts[0].texCoord = {0, 0};
        pVerts[0].color = color;

        pVerts[1].position = {rect.x, rect.y + rect.w};
        pVerts[1].texCoord = {0, 1};
        pVerts[1].color = color;

        pVerts[2].position = {rect.x + rect.z, rect.y + rect.w};
        pVerts[2].texCoord = {1, 1};
        pVerts[2].color = color;

        pVerts[3].position = {rect.x + rect.z, rect.y};
        pVerts[3].texCoord = {1, 0};
        pVerts[3].color = color;

        ++m_spriteCount;

        if (m_spriteCount == MAX_SPRITE_COUNT)
        {
            flush();
        }
    }

    void SpriteBatch::drawRectScaled9(const OTextureRef& pTexture, const Rect& rect, const Vector4& padding, const Color& color)
    {
        assert(m_isDrawing); // Should call begin() before calling draw()

        changeTexture(pTexture);

        auto textureSize = m_pTexture->getSize();
        auto sizexf = static_cast<float>(textureSize.x);
        auto sizeyf = static_cast<float>(textureSize.y);
        const Vector4 paddingUVs{padding.x / sizexf, padding.y / sizeyf, padding.z / sizexf, padding.w / sizeyf};

        drawRectWithUVs(m_pTexture,
            {rect.x, rect.y, padding.x, padding.y},
            {0.f, 0.f, paddingUVs.x, paddingUVs.y}, color);
        drawRectWithUVs(m_pTexture,
            {rect.x + padding.x, rect.y, rect.z - padding.x - padding.z, padding.y},
            {paddingUVs.x, 0.f, 1.f - paddingUVs.z, paddingUVs.y}, color);
        drawRectWithUVs(m_pTexture,
            {rect.x + rect.z - padding.z, rect.y, padding.z, padding.y},
            {1.f - paddingUVs.z, 0.f, 1.f, paddingUVs.y}, color);

        drawRectWithUVs(m_pTexture,
            {rect.x, rect.y + padding.y, padding.x, rect.w - padding.y - padding.w},
            {0.f, paddingUVs.y, paddingUVs.x, 1.f - paddingUVs.w}, color);
        drawRectWithUVs(m_pTexture,
            {rect.x + padding.x, rect.y + padding.y, rect.z - padding.x - padding.z, rect.w - padding.y - padding.w},
            {paddingUVs.x, paddingUVs.y, 1.f - paddingUVs.z, 1.f - paddingUVs.w}, color);
        drawRectWithUVs(m_pTexture,
            {rect.x + rect.z - padding.z, rect.y + padding.y, padding.z, rect.w - padding.y - padding.w},
            {1.f - paddingUVs.z, paddingUVs.y, 1.f, 1.f - paddingUVs.w}, color);

        drawRectWithUVs(m_pTexture,
            {rect.x, rect.y + rect.w - padding.w, padding.x, padding.w},
            {0.f, 1.f - paddingUVs.w, paddingUVs.x, 1.f}, color);
        drawRectWithUVs(m_pTexture,
            {rect.x + padding.x, rect.y + rect.w - padding.w, rect.z - padding.x - padding.z, padding.w},
            {paddingUVs.x, 1.f - paddingUVs.w, 1.f - paddingUVs.z, 1.f}, color);
        drawRectWithUVs(m_pTexture,
            {rect.x + rect.z - padding.z, rect.y + rect.w - padding.z, padding.w, padding.w},
            {1.f - paddingUVs.z, 1.f - paddingUVs.w, 1.f, 1.f}, color);
    }

    void SpriteBatch::drawRectScaled9RepeatCenters(const OTextureRef& pTexture, const Rect& rect, const Vector4& padding, const Color& color)
    {
        assert(m_isDrawing); // Should call begin() before calling draw()

        changeTexture(pTexture);

        auto textureSize = m_pTexture->getSize();
        auto sizexf = static_cast<float>(textureSize.x);
        auto sizeyf = static_cast<float>(textureSize.y);
        const Vector4 paddingUVs{padding.x / sizexf, padding.y / sizeyf, padding.z / sizexf, padding.w / sizeyf};

        // Corners
        drawRectWithUVs(m_pTexture,
            {rect.x, rect.y, padding.x, padding.y},
            {0.f, 0.f, paddingUVs.x, paddingUVs.y}, color);
        drawRectWithUVs(m_pTexture,
            {rect.x + rect.z - padding.z, rect.y, padding.z, padding.y},
            {1.f - paddingUVs.z, 0.f, 1.f, paddingUVs.y}, color);
        drawRectWithUVs(m_pTexture,
            {rect.x, rect.y + rect.w - padding.w, padding.x, padding.w},
            {0.f, 1.f - paddingUVs.w, paddingUVs.x, 1.f}, color);
        drawRectWithUVs(m_pTexture,
            {rect.x + rect.z - padding.z, rect.y + rect.w - padding.w, padding.z, padding.w},
            {1.f - paddingUVs.z, 1.f - paddingUVs.w, 1.f, 1.f}, color);

        // Edges
        auto middleXLen = rect.z - padding.x - padding.z;
        auto middleYLen = rect.w - padding.y - padding.w;
        auto textureMiddleXLen = sizexf - padding.x - padding.z;
        auto textureMiddleYLen = sizeyf - padding.y - padding.w;
        auto repeatX = middleXLen / textureMiddleXLen;
        auto repeatY = middleYLen / textureMiddleYLen;
        auto xCount = std::max<>(1, static_cast<int>(round(repeatX)));
        auto yCount = std::max<>(1, static_cast<int>(round(repeatY)));
        auto xPartLen = middleXLen / static_cast<float>(xCount);
        auto yPartLen = middleYLen / static_cast<float>(yCount);

        for (int x = 0; x < xCount; ++x)
        {
            drawRectWithUVs(m_pTexture,
                {rect.x + padding.x + static_cast<float>(x)* xPartLen,
                rect.y, xPartLen, padding.y},
                {paddingUVs.x, 0.f, 1.f - paddingUVs.z, paddingUVs.y}, color);
            drawRectWithUVs(m_pTexture,
                {rect.x + padding.x + static_cast<float>(x)* xPartLen, 
                rect.y + rect.w - padding.w, xPartLen, padding.w},
                {paddingUVs.x, 1.f - paddingUVs.w, 1.f - paddingUVs.z, 1.f}, color);
        }

        for (int y = 0; y < yCount; ++y)
        {
            drawRectWithUVs(m_pTexture,
                {rect.x, 
                rect.y + padding.y + static_cast<float>(y)* yPartLen,
                padding.x, yPartLen},
                {0.f, paddingUVs.y, paddingUVs.x, 1.f - paddingUVs.w}, color);
            drawRectWithUVs(m_pTexture,
                {rect.x + rect.z - padding.z, 
                rect.y + padding.y + static_cast<float>(y)* yPartLen,
                padding.z, yPartLen},
                {1.f - paddingUVs.z, paddingUVs.y, 1.f, 1.f - paddingUVs.w}, color);
        }

        for (int y = 0; y < yCount; ++y)
        {
            for (int x = 0; x < xCount; ++x)
            {
                // The middle part
                drawRectWithUVs(m_pTexture,
                {rect.x + padding.x + static_cast<float>(x)* xPartLen,
                rect.y + padding.y + static_cast<float>(y)* yPartLen,
                xPartLen,
                yPartLen},
                {paddingUVs.x, paddingUVs.y, 1.f - paddingUVs.z, 1.f - paddingUVs.w}, color);
            }
        }
    }

    void SpriteBatch::drawInclinedRect(const OTextureRef& pTexture, const Rect& rect, float inclinedRatio, const Color& color)
    {
        assert(m_isDrawing); // Should call begin() before calling draw()

        changeTexture(pTexture);

        SVertexP2T2C4* pVerts = m_pMappedVertexBuffer + (m_spriteCount * 4);
        pVerts[0].position = {rect.x, rect.y};
        pVerts[0].texCoord = {0, 0};
        pVerts[0].color = color;

        pVerts[1].position = {rect.x + inclinedRatio * rect.w, rect.y + rect.w};
        pVerts[1].texCoord = {0, 1};
        pVerts[1].color = color;

        pVerts[2].position = {rect.x + rect.z + inclinedRatio * rect.w, rect.y + rect.w};
        pVerts[2].texCoord = {1, 1};
        pVerts[2].color = color;

        pVerts[3].position = {rect.x + rect.z, rect.y};
        pVerts[3].texCoord = {1, 0};
        pVerts[3].color = color;

        ++m_spriteCount;

        if (m_spriteCount == MAX_SPRITE_COUNT)
        {
            flush();
        }
    }

    void SpriteBatch::drawRectWithUVs(const OTextureRef& pTexture, const Rect& rect, const Vector4& uvs, const Color& color)
    {
        assert(m_isDrawing); // Should call begin() before calling draw()

        changeTexture(pTexture);

        SVertexP2T2C4* pVerts = m_pMappedVertexBuffer + (m_spriteCount * 4);
        pVerts[0].position = {rect.x, rect.y};
        pVerts[0].texCoord = {uvs.x, uvs.y};
        pVerts[0].color = color;

        pVerts[1].position = {rect.x, rect.y + rect.w};
        pVerts[1].texCoord = {uvs.x, uvs.w};
        pVerts[1].color = color;

        pVerts[2].position = {rect.x + rect.z, rect.y + rect.w};
        pVerts[2].texCoord = {uvs.z, uvs.w};
        pVerts[2].color = color;

        pVerts[3].position = {rect.x + rect.z, rect.y};
        pVerts[3].texCoord = {uvs.z, uvs.y};
        pVerts[3].color = color;

        ++m_spriteCount;

        if (m_spriteCount == MAX_SPRITE_COUNT)
        {
            flush();
        }
    }

    void SpriteBatch::drawRectWithUVsColors(const OTextureRef& pTexture, const Rect& rect, const Vector4& uvs, const std::vector<Color>& colors)
    {
        assert(m_isDrawing); // Should call begin() before calling draw()
        assert(colors.size() == 4); // Needs 4 colors

        changeTexture(pTexture);

        SVertexP2T2C4* pVerts = m_pMappedVertexBuffer + (m_spriteCount * 4);
        pVerts[0].position = {rect.x, rect.y};
        pVerts[0].texCoord = {uvs.x, uvs.y};
        pVerts[0].color = colors[0];

        pVerts[1].position = {rect.x, rect.y + rect.w};
        pVerts[1].texCoord = {uvs.x, uvs.w};
        pVerts[1].color = colors[1];

        pVerts[2].position = {rect.x + rect.z, rect.y + rect.w};
        pVerts[2].texCoord = {uvs.z, uvs.w};
        pVerts[2].color = colors[2];

        pVerts[3].position = {rect.x + rect.z, rect.y};
        pVerts[3].texCoord = {uvs.z, uvs.y};
        pVerts[3].color = colors[3];

        ++m_spriteCount;

        if (m_spriteCount == MAX_SPRITE_COUNT)
        {
            flush();
        }
    }

    void SpriteBatch::changeTexture(const OTextureRef& pTexture)
    {
        if (!pTexture && m_pTexture == m_pTexWhite) return;
        if (pTexture != m_pTexture)
        {
            flush();
            if (!pTexture) m_pTexture = m_pTexWhite;
            else m_pTexture = pTexture;
        }
        if (!m_pTexture) m_pTexture = m_pTexWhite;
    }

    void SpriteBatch::draw4Corner(const OTextureRef& pTexture, const Rect& rect, const Color& color)
    {
        changeTexture(pTexture);

        auto textureSize = m_pTexture->getSize();
        auto sizexf = static_cast<float>(textureSize.x);
        auto sizeyf = static_cast<float>(textureSize.y);
        Rect cornerRect{0, 0, sizexf * .5f, sizeyf * .5f};
        drawRectWithUVs(m_pTexture, rect.TopLeft(cornerRect), {0, 0, .5f, .5f}, color);
        drawRectWithUVs(m_pTexture, rect.TopRight(cornerRect), {.5f, 0, 1, .5f}, color);
        drawRectWithUVs(m_pTexture, rect.BottomLeft(cornerRect), {0, .5f, .5f, 1}, color);
        drawRectWithUVs(m_pTexture, rect.BottomRight(cornerRect), {.5f, .5f, 1, 1}, color);
    }

    void SpriteBatch::drawSprite(const OTextureRef& pTexture, const Vector2& position, const Color& color, const Vector2& origin)
    {
        changeTexture(pTexture);

        auto& textureSize = m_pTexture->getSize();
        auto sizexf = static_cast<float>(textureSize.x);
        auto sizeyf = static_cast<float>(textureSize.y);
        drawRect(m_pTexture, {position.x - sizexf * origin.x, position.y - sizeyf * origin.y, sizexf, sizeyf}, color);
    }

    void SpriteBatch::drawSprite(const OTextureRef& pTexture, const Matrix& transform, const Color& color, const Vector2& origin)
    {
        changeTexture(pTexture);
        auto sizef = m_pTexture->getSizef();

        auto invOrigin = Vector2(1.f - origin.x, 1.f - origin.y);

        SVertexP2T2C4* pVerts = m_pMappedVertexBuffer + (m_spriteCount * 4);
        pVerts[0].position = Vector2::Transform(Vector2(-sizef.x * origin.x, -sizef.y * origin.y), transform);
        pVerts[0].texCoord = {0, 0};
        pVerts[0].color = color;

        pVerts[1].position = Vector2::Transform(Vector2(-sizef.x * origin.x, sizef.y * invOrigin.y), transform);
        pVerts[1].texCoord = {0, 1};
        pVerts[1].color = color;

        pVerts[2].position = Vector2::Transform(Vector2(sizef.x * invOrigin.x, sizef.y * invOrigin.y), transform);
        pVerts[2].texCoord = {1, 1};
        pVerts[2].color = color;

        pVerts[3].position = Vector2::Transform(Vector2(sizef.x * invOrigin.x, -sizef.y * origin.y), transform);
        pVerts[3].texCoord = {1, 0};
        pVerts[3].color = color;

        ++m_spriteCount;

        if (m_spriteCount == MAX_SPRITE_COUNT)
        {
            flush();
        }
    }

    void SpriteBatch::drawSpriteWithUVs(const OTextureRef& pTexture, const Matrix& transform, const Vector4& uvs, const Color& color, const Vector2& origin)
    {
        changeTexture(pTexture);
        auto sizef = m_pTexture->getSizef();
        sizef.x *= std::abs(uvs.z - uvs.x);
        sizef.y *= std::abs(uvs.w - uvs.y);

        auto invOrigin = Vector2(1.f - origin.x, 1.f - origin.y);

        SVertexP2T2C4* pVerts = m_pMappedVertexBuffer + (m_spriteCount * 4);
        pVerts[0].position = Vector2::Transform(Vector2(-sizef.x * origin.x, -sizef.y * origin.y), transform);
        pVerts[0].texCoord = {uvs.x, uvs.y};
        pVerts[0].color = color;

        pVerts[1].position = Vector2::Transform(Vector2(-sizef.x * origin.x, sizef.y * invOrigin.y), transform);
        pVerts[1].texCoord = {uvs.x, uvs.w};
        pVerts[1].color = color;

        pVerts[2].position = Vector2::Transform(Vector2(sizef.x * invOrigin.x, sizef.y * invOrigin.y), transform);
        pVerts[2].texCoord = {uvs.z, uvs.w};
        pVerts[2].color = color;

        pVerts[3].position = Vector2::Transform(Vector2(sizef.x * invOrigin.x, -sizef.y * origin.y), transform);
        pVerts[3].texCoord = {uvs.z, uvs.y};
        pVerts[3].color = color;

        ++m_spriteCount;

        if (m_spriteCount == MAX_SPRITE_COUNT)
        {
            flush();
        }
    }

    void SpriteBatch::drawSpriteWithUVs(const OTextureRef& pTexture, const Vector2& position, const Vector4& uvs, const Color& color, float rotation, float scale, const Vector2& origin)
    {
        changeTexture(pTexture);
        auto textureSize = m_pTexture->getSize();
        auto sizexf = static_cast<float>(textureSize.x);
        auto sizeyf = static_cast<float>(textureSize.y);
        sizexf *= (uvs.z - uvs.x);
        sizeyf *= (uvs.w - uvs.y);
        auto hSize = Vector2(sizexf * .5f * scale, sizeyf * .5f * scale);
        auto radTheta = DirectX::XMConvertToRadians(rotation);
        auto sinTheta = std::sin(radTheta);
        auto cosTheta = std::cos(radTheta);
        auto invOrigin = Vector2(1.f - origin.x, 1.f - origin.y) * 2.f;

        Vector2 right{cosTheta * hSize.x, sinTheta * hSize.x};
        Vector2 down{-sinTheta * hSize.y, cosTheta * hSize.y};

        SVertexP2T2C4* pVerts = m_pMappedVertexBuffer + (m_spriteCount * 4);
        pVerts[0].position = position;
        pVerts[0].position -= right * origin.x * 2.f;
        pVerts[0].position -= down * origin.y * 2.f;
        pVerts[0].texCoord = {uvs.x, uvs.y};
        pVerts[0].color = color;

        pVerts[1].position = position;
        pVerts[1].position -= right * origin.x * 2.f;
        pVerts[1].position += down * invOrigin.y;
        pVerts[1].texCoord = {uvs.x, uvs.w};
        pVerts[1].color = color;

        pVerts[2].position = position;
        pVerts[2].position += right * invOrigin.x;
        pVerts[2].position += down * invOrigin.y;
        pVerts[2].texCoord = {uvs.z, uvs.w};
        pVerts[2].color = color;

        pVerts[3].position = position;
        pVerts[3].position += right * invOrigin.x;
        pVerts[3].position -= down * origin.y * 2.f;
        pVerts[3].texCoord = {uvs.z, uvs.y};
        pVerts[3].color = color;

        ++m_spriteCount;

        if (m_spriteCount == MAX_SPRITE_COUNT)
        {
            flush();
        }
    }

    void SpriteBatch::drawBeam(const OTextureRef& pTexture, const Vector2& from, const Vector2& to, float size, const Color& color, float uOffset, float uScale)
    {
        changeTexture(pTexture);

        auto texSize = m_pTexture->getSizef();
        Vector2 dir = to - from;
        float len = dir.Length();
        if (len == 0) return;
        dir /= len;
        Vector2 right{-dir.y, dir.x};
        right *= size * .5f;

        SVertexP2T2C4* pVerts = m_pMappedVertexBuffer + (m_spriteCount * 4);
        pVerts[0].position = Vector2(from.x - right.x, from.y - right.y);
        pVerts[0].texCoord = {uOffset, 0};
        pVerts[0].color = color;

        pVerts[1].position = Vector2(from.x + right.x, from.y + right.y);
        pVerts[1].texCoord = {uOffset, 1};
        pVerts[1].color = color;

        pVerts[2].position = Vector2(to.x + right.x, to.y + right.y);
        pVerts[2].texCoord = {uOffset + len * uScale / texSize.x, 1};
        pVerts[2].color = color;

        pVerts[3].position = Vector2(to.x - right.x, to.y - right.y);
        pVerts[3].texCoord = {uOffset + len * uScale / texSize.x, 0};
        pVerts[3].color = color;

        ++m_spriteCount;

        if (m_spriteCount == MAX_SPRITE_COUNT)
        {
            flush();
        }
    }

    void SpriteBatch::drawCross(const Vector2& position, float size, const Color& color, float thickness)
    {
        drawRect(nullptr, {position.x - thickness * .5f, position.y - size, thickness, size * 2.f}, color);
        drawRect(nullptr, {position.x - size, position.y - thickness * .5f, size * 2.f, thickness}, color);
    }

    void SpriteBatch::drawSprite(const OTextureRef& pTexture, const Vector2& position, const Color& color, float rotation, float scale, const Vector2& origin)
    {
        changeTexture(pTexture);

        auto textureSize = m_pTexture->getSize();
        auto sizexf = static_cast<float>(textureSize.x);
        auto sizeyf = static_cast<float>(textureSize.y);
        auto hSize = Vector2(sizexf * .5f * scale, sizeyf * .5f * scale);
        auto radTheta = DirectX::XMConvertToRadians(rotation);
        auto sinTheta = std::sin(radTheta);
        auto cosTheta = std::cos(radTheta);
        auto invOrigin = Vector2(1.f - origin.x, 1.f - origin.y) * 2.f;

        Vector2 right{cosTheta * hSize.x, sinTheta * hSize.x};
        Vector2 down{-sinTheta * hSize.y, cosTheta * hSize.y};

        SVertexP2T2C4* pVerts = m_pMappedVertexBuffer + (m_spriteCount * 4);
        pVerts[0].position = position;
        pVerts[0].position -= right * origin.x * 2.f;
        pVerts[0].position -= down * origin.y * 2.f;
        pVerts[0].texCoord = {0, 0};
        pVerts[0].color = color;

        pVerts[1].position = position;
        pVerts[1].position -= right * origin.x * 2.f;
        pVerts[1].position += down * invOrigin.y;
        pVerts[1].texCoord = {0, 1};
        pVerts[1].color = color;

        pVerts[2].position = position;
        pVerts[2].position += right * invOrigin.x;
        pVerts[2].position += down * invOrigin.y;
        pVerts[2].texCoord = {1, 1};
        pVerts[2].color = color;

        pVerts[3].position = position;
        pVerts[3].position += right * invOrigin.x;
        pVerts[3].position -= down * origin.y * 2.f;
        pVerts[3].texCoord = {1, 0};
        pVerts[3].color = color;

        ++m_spriteCount;

        if (m_spriteCount == MAX_SPRITE_COUNT)
        {
            flush();
        }
    }

    void SpriteBatch::end()
    {
        assert(m_isDrawing); // Should call begin() before calling end()

        m_isDrawing = false;
        if (m_spriteCount)
        {
            flush();
        }

        m_pVertexBuffer->unmap(sizeof(SVertexP2T2C4) * m_spriteCount * 4);
    }

    void SpriteBatch::flush()
    {
        if (!m_spriteCount)
        {
            return; // Nothing to flush
        }
        m_pVertexBuffer->unmap(sizeof(SVertexP2T2C4) * m_spriteCount * 4);

        oRenderer->renderStates.textures[0] = m_pTexture;
        oRenderer->renderStates.blendMode = m_curBlendMode;
        oRenderer->renderStates.sampleFiltering = m_curFiltering;
        oRenderer->renderStates.primitiveMode = OPrimitiveTriangleList;
        oRenderer->renderStates.indexBuffer = m_pIndexBuffer;
        oRenderer->renderStates.vertexBuffer = m_pVertexBuffer;
        oRenderer->drawIndexed(6 * m_spriteCount);

        m_pMappedVertexBuffer = reinterpret_cast<SVertexP2T2C4*>(m_pVertexBuffer->map());

        m_spriteCount = 0;
        m_pTexture = nullptr;
    }
}
