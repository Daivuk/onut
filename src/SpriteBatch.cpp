#include <cmath>
#include "onut.h"
#include "SpriteBatch.h"

namespace onut
{
    SpriteBatch::SpriteBatch()
    {
        auto pDevice = ORenderer->getDevice();
        auto pDeviceContext = ORenderer->getDeviceContext();

        // Create a white texture for rendering "without" texture
        unsigned char white[4] = {255, 255, 255, 255};
        m_pTexWhite = Texture::createFromData({1, 1}, white, false);

        SVertexP2T2C4 vertices[MAX_SPRITE_COUNT * 4];
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

        // Set up the description of the static vertex buffer.
        D3D11_BUFFER_DESC vertexBufferDesc;
        vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
        vertexBufferDesc.ByteWidth = sizeof(vertices);
        vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        vertexBufferDesc.MiscFlags = 0;
        vertexBufferDesc.StructureByteStride = 0;

        // Give the subresource structure a pointer to the vertex data.
        D3D11_SUBRESOURCE_DATA vertexData;
        vertexData.pSysMem = vertices;
        vertexData.SysMemPitch = 0;
        vertexData.SysMemSlicePitch = 0;

        auto ret = pDevice->CreateBuffer(&vertexBufferDesc, &vertexData, &m_pVertexBuffer);
        assert(ret == S_OK);

        // Set up the description of the static index buffer.
        D3D11_BUFFER_DESC indexBufferDesc;
        indexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
        indexBufferDesc.ByteWidth = sizeof(unsigned short) * 6 * MAX_SPRITE_COUNT;
        indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
        indexBufferDesc.CPUAccessFlags = 0;
        indexBufferDesc.MiscFlags = 0;
        indexBufferDesc.StructureByteStride = 0;

        // Give the subresource structure a pointer to the index data.
        D3D11_SUBRESOURCE_DATA indexData;
        indexData.pSysMem = indices;
        indexData.SysMemPitch = 0;
        indexData.SysMemSlicePitch = 0;

        ret = pDevice->CreateBuffer(&indexBufferDesc, &indexData, &m_pIndexBuffer);
        assert(ret == S_OK);

        // Blend modes
        D3D11_BLEND_DESC blendDesc;
        blendDesc = D3D11_BLEND_DESC
        {
            FALSE,
            FALSE,
            {{
                    FALSE,
                    D3D11_BLEND_ONE,
                    D3D11_BLEND_ZERO,
                    D3D11_BLEND_OP_ADD,
                    D3D11_BLEND_ONE,
                    D3D11_BLEND_ZERO,
                    D3D11_BLEND_OP_ADD,
                    D3D10_COLOR_WRITE_ENABLE_ALL
                }, {0}, {0}, {0}, {0}, {0}, {0}, {0}}
        };
        ret = pDevice->CreateBlendState(&blendDesc, &m_pBlendStates[static_cast<int>(eBlendMode::Opaque)]);
        assert(ret == S_OK);
        blendDesc = D3D11_BLEND_DESC
        {
            FALSE,
            FALSE,
            {{
                    TRUE,
                    D3D11_BLEND_SRC_ALPHA,
                    D3D11_BLEND_INV_SRC_ALPHA,
                    D3D11_BLEND_OP_ADD,
                    D3D11_BLEND_ONE,
                    D3D11_BLEND_ONE,
                    D3D11_BLEND_OP_ADD,
                    D3D10_COLOR_WRITE_ENABLE_ALL
                }, {0}, {0}, {0}, {0}, {0}, {0}, {0}}
        };
        ret = pDevice->CreateBlendState(&blendDesc, &m_pBlendStates[static_cast<int>(eBlendMode::Alpha)]);
        assert(ret == S_OK);
        blendDesc = D3D11_BLEND_DESC
        {
            FALSE,
            FALSE,
            {{
                    TRUE,
                    D3D11_BLEND_SRC_ALPHA,
                    D3D11_BLEND_ONE,
                    D3D11_BLEND_OP_ADD,
                    D3D11_BLEND_ONE,
                    D3D11_BLEND_ONE,
                    D3D11_BLEND_OP_ADD,
                    D3D10_COLOR_WRITE_ENABLE_ALL
                }, {0}, {0}, {0}, {0}, {0}, {0}, {0}}
        };
        ret = pDevice->CreateBlendState(&blendDesc, &m_pBlendStates[static_cast<int>(eBlendMode::Add)]);
        assert(ret == S_OK);
        blendDesc = D3D11_BLEND_DESC
        {
            FALSE,
            FALSE,
            {{
                    TRUE,
                    D3D11_BLEND_ONE,
                    D3D11_BLEND_INV_SRC_ALPHA,
                    D3D11_BLEND_OP_ADD,
                    D3D11_BLEND_ONE,
                    D3D11_BLEND_ONE,
                    D3D11_BLEND_OP_ADD,
                    D3D10_COLOR_WRITE_ENABLE_ALL
                }, {0}, {0}, {0}, {0}, {0}, {0}, {0}}
        };
        ret = pDevice->CreateBlendState(&blendDesc, &m_pBlendStates[static_cast<int>(eBlendMode::PreMultiplied)]);
        assert(ret == S_OK);
        blendDesc = D3D11_BLEND_DESC
        {
            FALSE,
            FALSE,
            {{
                    TRUE,
                    D3D11_BLEND_DEST_COLOR,
                    D3D11_BLEND_INV_SRC_ALPHA,
                    D3D11_BLEND_OP_ADD,
                    D3D11_BLEND_ONE,
                    D3D11_BLEND_ONE,
                    D3D11_BLEND_OP_ADD,
                    D3D10_COLOR_WRITE_ENABLE_ALL
                }, {0}, {0}, {0}, {0}, {0}, {0}, {0}}
        };
        ret = pDevice->CreateBlendState(&blendDesc, &m_pBlendStates[static_cast<int>(eBlendMode::Multiplied)]);
        assert(ret == S_OK);
        blendDesc = D3D11_BLEND_DESC
        {
            FALSE,
            FALSE,
            {{
                    TRUE,
                    D3D11_BLEND_ONE,
                    D3D11_BLEND_ZERO,
                    D3D11_BLEND_OP_ADD,
                    D3D11_BLEND_ONE,
                    D3D11_BLEND_ZERO,
                    D3D11_BLEND_OP_ADD,
                    D3D10_COLOR_WRITE_ENABLE_ALL
                }, {0}, {0}, {0}, {0}, {0}, {0}, {0}}
        };
        ret = pDevice->CreateBlendState(&blendDesc, &m_pBlendStates[static_cast<int>(eBlendMode::ForceWrite)]);
        assert(ret == S_OK);

        // Samplers
        D3D11_SAMPLER_DESC samplerDesc;
        samplerDesc = D3D11_SAMPLER_DESC
        {
            D3D11_FILTER_MIN_MAG_MIP_POINT,
            D3D11_TEXTURE_ADDRESS_WRAP,
            D3D11_TEXTURE_ADDRESS_WRAP,
            D3D11_TEXTURE_ADDRESS_WRAP,
            0.f,
            1,
            D3D11_COMPARISON_ALWAYS,
            {0, 0, 0, 0},
            0,
            D3D11_FLOAT32_MAX
        };
        ret = pDevice->CreateSamplerState(&samplerDesc, &m_pSamplers[static_cast<int>(eFiltering::Nearest)]);
        assert(ret == S_OK);
        samplerDesc = D3D11_SAMPLER_DESC
        {
            D3D11_FILTER_MIN_MAG_MIP_LINEAR,
            D3D11_TEXTURE_ADDRESS_WRAP,
            D3D11_TEXTURE_ADDRESS_WRAP,
            D3D11_TEXTURE_ADDRESS_WRAP,
            0.f,
            1,
            D3D11_COMPARISON_ALWAYS,
            {0, 0, 0, 0},
            0,
            D3D11_FLOAT32_MAX
        };
        ret = pDevice->CreateSamplerState(&samplerDesc, &m_pSamplers[static_cast<int>(eFiltering::Linear)]);
        assert(ret == S_OK);
    }

    SpriteBatch::~SpriteBatch()
    {
        for (int i = 0; i < static_cast<int>(eBlendMode::BlendModeCount); ++i)
        {
            m_pBlendStates[i]->Release();
        }
        for (int i = 0; i < static_cast<int>(eFiltering::FilteringCount); ++i)
        {
            m_pSamplers[i]->Release();
        }
        if (m_pVertexBuffer) m_pVertexBuffer->Release();
        if (m_pIndexBuffer) m_pIndexBuffer->Release();
        delete m_pTexWhite;
    }

    void SpriteBatch::begin(eBlendMode blendMode)
    {
        begin(Matrix::Identity, blendMode);
    }

    void SpriteBatch::begin(const Matrix& transform, eBlendMode blendMode)
    {
        assert(!m_isDrawing); // Cannot call begin() twice without calling end()

        m_currentTransform = transform;
        ORenderer->setupFor2D(transform);
        m_curBlendMode = blendMode;
        ORenderer->getDeviceContext()->OMSetBlendState(m_pBlendStates[static_cast<int>(blendMode)], NULL, 0xffffffff);
        ORenderer->getDeviceContext()->PSSetSamplers(0, 1, &m_pSamplers[static_cast<int>(m_curFiltering)]);
        m_pTexture = nullptr;
        m_isDrawing = true;
        ORenderer->getDeviceContext()->Map(m_pVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &m_pMappedVertexBuffer);
    }

    void SpriteBatch::changeBlendMode(eBlendMode blendMode)
    {
        if (!isInBatch()) return;
        if (m_curBlendMode == blendMode) return;
        end();
        begin(m_currentTransform, blendMode);
    }

    void SpriteBatch::changeFiltering(eFiltering filtering)
    {
        if (m_curFiltering == filtering) return;
        auto bManageBatch = isInBatch();
        if (bManageBatch) end();
        m_curFiltering = filtering;
        if (bManageBatch) begin(m_currentTransform, m_curBlendMode);
    }

    void SpriteBatch::drawRectWithColors(Texture* pTexture, const Rect& rect, const std::vector<Color>& colors)
    {
        assert(m_isDrawing); // Should call begin() before calling draw()
        assert(colors.size() == 4); // Needs 4 colors

        if (!pTexture) pTexture = m_pTexWhite;
        if (pTexture != m_pTexture)
        {
            flush();
        }
        m_pTexture = pTexture;

        SVertexP2T2C4* pVerts = static_cast<SVertexP2T2C4*>(m_pMappedVertexBuffer.pData) + (m_spriteCount * 4);
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

    void SpriteBatch::drawAbsoluteRect(Texture* pTexture, const Rect& rect, const Color& color)
    {
        drawRect(pTexture, {rect.x, rect.y, rect.z - rect.x, rect.w - rect.y}, color);
    }

    void SpriteBatch::drawRect(Texture* pTexture, const Rect& rect, const Color& color)
    {
        assert(m_isDrawing); // Should call begin() before calling draw()

        if (!pTexture) pTexture = m_pTexWhite;
        if (pTexture != m_pTexture)
        {
            flush();
        }
        m_pTexture = pTexture;

        SVertexP2T2C4* pVerts = static_cast<SVertexP2T2C4*>(m_pMappedVertexBuffer.pData) + (m_spriteCount * 4);
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

    void SpriteBatch::drawRectScaled9(Texture* pTexture, const Rect& rect, const Vector4& padding, const Color& color)
    {
        assert(m_isDrawing); // Should call begin() before calling draw()

        if (!pTexture) pTexture = m_pTexWhite;

        auto textureSize = pTexture->getSize();
        auto sizexf = static_cast<float>(textureSize.x);
        auto sizeyf = static_cast<float>(textureSize.y);
        const Vector4 paddingUVs{padding.x / sizexf, padding.y / sizeyf, padding.z / sizexf, padding.w / sizeyf};

        drawRectWithUVs(pTexture,
            {rect.x, rect.y, padding.x, padding.y},
            {0.f, 0.f, paddingUVs.x, paddingUVs.y}, color);
        drawRectWithUVs(pTexture,
            {rect.x + padding.x, rect.y, rect.z - padding.x - padding.z, padding.y},
            {paddingUVs.x, 0.f, 1.f - paddingUVs.z, paddingUVs.y}, color);
        drawRectWithUVs(pTexture,
            {rect.x + rect.z - padding.z, rect.y, padding.z, padding.y},
            {1.f - paddingUVs.z, 0.f, 1.f, paddingUVs.y}, color);

        drawRectWithUVs(pTexture,
            {rect.x, rect.y + padding.y, padding.x, rect.w - padding.y - padding.w},
            {0.f, paddingUVs.y, paddingUVs.x, 1.f - paddingUVs.w}, color);
        drawRectWithUVs(pTexture,
            {rect.x + padding.x, rect.y + padding.y, rect.z - padding.x - padding.z, rect.w - padding.y - padding.w},
            {paddingUVs.x, paddingUVs.y, 1.f - paddingUVs.z, 1.f - paddingUVs.w}, color);
        drawRectWithUVs(pTexture,
            {rect.x + rect.z - padding.z, rect.y + padding.y, padding.z, rect.w - padding.y - padding.w},
            {1.f - paddingUVs.z, paddingUVs.y, 1.f, 1.f - paddingUVs.w}, color);

        drawRectWithUVs(pTexture,
            {rect.x, rect.y + rect.w - padding.w, padding.x, padding.w},
            {0.f, 1.f - paddingUVs.w, paddingUVs.x, 1.f}, color);
        drawRectWithUVs(pTexture,
            {rect.x + padding.x, rect.y + rect.w - padding.w, rect.z - padding.x - padding.z, padding.w},
            {paddingUVs.x, 1.f - paddingUVs.w, 1.f - paddingUVs.z, 1.f}, color);
        drawRectWithUVs(pTexture,
            {rect.x + rect.z - padding.z, rect.y + rect.w - padding.z, padding.w, padding.w},
            {1.f - paddingUVs.z, 1.f - paddingUVs.w, 1.f, 1.f}, color);
    }

    void SpriteBatch::drawRectScaled9RepeatCenters(Texture* pTexture, const Rect& rect, const Vector4& padding, const Color& color)
    {
        assert(m_isDrawing); // Should call begin() before calling draw()

        if (!pTexture) pTexture = m_pTexWhite;

        auto textureSize = pTexture->getSize();
        auto sizexf = static_cast<float>(textureSize.x);
        auto sizeyf = static_cast<float>(textureSize.y);
        const Vector4 paddingUVs{padding.x / sizexf, padding.y / sizeyf, padding.z / sizexf, padding.w / sizeyf};

        // Corners
        drawRectWithUVs(pTexture,
            {rect.x, rect.y, padding.x, padding.y},
            {0.f, 0.f, paddingUVs.x, paddingUVs.y}, color);
        drawRectWithUVs(pTexture,
            {rect.x + rect.z - padding.z, rect.y, padding.z, padding.y},
            {1.f - paddingUVs.z, 0.f, 1.f, paddingUVs.y}, color);
        drawRectWithUVs(pTexture,
            {rect.x, rect.y + rect.w - padding.w, padding.x, padding.w},
            {0.f, 1.f - paddingUVs.w, paddingUVs.x, 1.f}, color);
        drawRectWithUVs(pTexture,
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
            drawRectWithUVs(pTexture,
                {rect.x + padding.x + static_cast<float>(x)* xPartLen,
                rect.y, xPartLen, padding.y},
                {paddingUVs.x, 0.f, 1.f - paddingUVs.z, paddingUVs.y}, color);
            drawRectWithUVs(pTexture,
                {rect.x + padding.x + static_cast<float>(x)* xPartLen, 
                rect.y + rect.w - padding.w, xPartLen, padding.w},
                {paddingUVs.x, 1.f - paddingUVs.w, 1.f - paddingUVs.z, 1.f}, color);
        }

        for (int y = 0; y < yCount; ++y)
        {
            drawRectWithUVs(pTexture,
                {rect.x, 
                rect.y + padding.y + static_cast<float>(y)* yPartLen,
                padding.x, yPartLen},
                {0.f, paddingUVs.y, paddingUVs.x, 1.f - paddingUVs.w}, color);
            drawRectWithUVs(pTexture,
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
                drawRectWithUVs(pTexture,
                {rect.x + padding.x + static_cast<float>(x)* xPartLen,
                rect.y + padding.y + static_cast<float>(y)* yPartLen,
                xPartLen,
                yPartLen},
                {paddingUVs.x, paddingUVs.y, 1.f - paddingUVs.z, 1.f - paddingUVs.w}, color);
            }
        }
    }

    void SpriteBatch::drawInclinedRect(Texture* pTexture, const Rect& rect, float inclinedRatio, const Color& color)
    {
        assert(m_isDrawing); // Should call begin() before calling draw()

        if (!pTexture) pTexture = m_pTexWhite;
        if (pTexture != m_pTexture)
        {
            flush();
        }
        m_pTexture = pTexture;

        SVertexP2T2C4* pVerts = static_cast<SVertexP2T2C4*>(m_pMappedVertexBuffer.pData) + (m_spriteCount * 4);
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

    void SpriteBatch::drawRectWithUVs(Texture* pTexture, const Rect& rect, const Vector4& uvs, const Color& color)
    {
        assert(m_isDrawing); // Should call begin() before calling draw()

        if (!pTexture) pTexture = m_pTexWhite;
        if (pTexture != m_pTexture)
        {
            flush();
        }
        m_pTexture = pTexture;

        SVertexP2T2C4* pVerts = static_cast<SVertexP2T2C4*>(m_pMappedVertexBuffer.pData) + (m_spriteCount * 4);
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

    void SpriteBatch::drawRectWithUVsColors(Texture* pTexture, const Rect& rect, const Vector4& uvs, const std::vector<Color>& colors)
    {
        assert(m_isDrawing); // Should call begin() before calling draw()
        assert(colors.size() == 4); // Needs 4 colors

        if (!pTexture) pTexture = m_pTexWhite;
        if (pTexture != m_pTexture)
        {
            flush();
        }
        m_pTexture = pTexture;

        SVertexP2T2C4* pVerts = static_cast<SVertexP2T2C4*>(m_pMappedVertexBuffer.pData) + (m_spriteCount * 4);
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

    void SpriteBatch::draw4Corner(Texture* pTexture, const Rect& rect, const Color& color)
    {
        if (!pTexture) pTexture = m_pTexWhite;

        auto textureSize = pTexture->getSize();
        auto sizexf = static_cast<float>(textureSize.x);
        auto sizeyf = static_cast<float>(textureSize.y);
        Rect cornerRect{0, 0, sizexf * .5f, sizeyf * .5f};
        drawRectWithUVs(pTexture, rect.TopLeft(cornerRect), {0, 0, .5f, .5f}, color);
        drawRectWithUVs(pTexture, rect.TopRight(cornerRect), {.5f, 0, 1, .5f}, color);
        drawRectWithUVs(pTexture, rect.BottomLeft(cornerRect), {0, .5f, .5f, 1}, color);
        drawRectWithUVs(pTexture, rect.BottomRight(cornerRect), {.5f, .5f, 1, 1}, color);
    }

    void SpriteBatch::drawSprite(Texture* pTexture, const Vector2& position, const Color& color, const Vector2& origin)
    {
        if (!pTexture) pTexture = m_pTexWhite;

        auto& textureSize = pTexture->getSize();
        auto sizexf = static_cast<float>(textureSize.x);
        auto sizeyf = static_cast<float>(textureSize.y);
        drawRect(pTexture, {position.x - sizexf * origin.x, position.y - sizeyf * origin.y, sizexf, sizeyf}, color);
    }

    void SpriteBatch::drawSprite(Texture* pTexture, const Matrix& transform, const Color& color, const Vector2& origin)
    {
        if (!pTexture) pTexture = m_pTexWhite;
        auto sizef = pTexture->getSizef();

        if (!pTexture) pTexture = m_pTexWhite;
        if (pTexture != m_pTexture)
        {
            flush();
        }
        m_pTexture = pTexture;

        auto invOrigin = Vector2(1.f - origin.x, 1.f - origin.y);

        SVertexP2T2C4* pVerts = static_cast<SVertexP2T2C4*>(m_pMappedVertexBuffer.pData) + (m_spriteCount * 4);
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

    void SpriteBatch::drawSpriteWithUVs(Texture* pTexture, const Matrix& transform, const Vector4& uvs, const Color& color, const Vector2& origin)
    {
        if (!pTexture) pTexture = m_pTexWhite;
        auto sizef = pTexture->getSizef();
        sizef.x *= std::abs(uvs.z - uvs.x);
        sizef.y *= std::abs(uvs.w - uvs.y);

        if (!pTexture) pTexture = m_pTexWhite;
        if (pTexture != m_pTexture)
        {
            flush();
        }
        m_pTexture = pTexture;

        auto invOrigin = Vector2(1.f - origin.x, 1.f - origin.y);

        SVertexP2T2C4* pVerts = static_cast<SVertexP2T2C4*>(m_pMappedVertexBuffer.pData) + (m_spriteCount * 4);
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

    void SpriteBatch::drawSpriteWithUVs(Texture* pTexture, const Vector2& position, const Vector4& uvs, const Color& color, float rotation, float scale, const Vector2& origin)
    {
        if (!pTexture) pTexture = m_pTexWhite;
        auto textureSize = pTexture->getSize();
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

        if (!pTexture) pTexture = m_pTexWhite;
        if (pTexture != m_pTexture)
        {
            flush();
        }
        m_pTexture = pTexture;

        SVertexP2T2C4* pVerts = static_cast<SVertexP2T2C4*>(m_pMappedVertexBuffer.pData) + (m_spriteCount * 4);
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

    void SpriteBatch::drawBeam(Texture* pTexture, const Vector2& from, const Vector2& to, float size, const Color& color, float uOffset, float uScale)
    {
        if (!pTexture) pTexture = m_pTexWhite;
        if (pTexture != m_pTexture)
        {
            flush();
        }
        m_pTexture = pTexture;

        auto texSize = m_pTexture->getSizef();
        Vector2 dir = to - from;
        float len = dir.Length();
        if (len == 0) return;
        dir /= len;
        Vector2 right{-dir.y, dir.x};
        right *= size * .5f;

        SVertexP2T2C4* pVerts = static_cast<SVertexP2T2C4*>(m_pMappedVertexBuffer.pData) + (m_spriteCount * 4);
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

    void SpriteBatch::drawSprite(Texture* pTexture, const Vector2& position, const Color& color, float rotation, float scale, const Vector2& origin)
    {
        if (!pTexture) pTexture = m_pTexWhite;
        auto textureSize = pTexture->getSize();
        auto sizexf = static_cast<float>(textureSize.x);
        auto sizeyf = static_cast<float>(textureSize.y);
        auto hSize = Vector2(sizexf * .5f * scale, sizeyf * .5f * scale);
        auto radTheta = DirectX::XMConvertToRadians(rotation);
        auto sinTheta = std::sin(radTheta);
        auto cosTheta = std::cos(radTheta);
        auto invOrigin = Vector2(1.f - origin.x, 1.f - origin.y) * 2.f;

        Vector2 right{cosTheta * hSize.x, sinTheta * hSize.x};
        Vector2 down{-sinTheta * hSize.y, cosTheta * hSize.y};

        if (!pTexture) pTexture = m_pTexWhite;
        if (pTexture != m_pTexture)
        {
            flush();
        }
        m_pTexture = pTexture;

        SVertexP2T2C4* pVerts = static_cast<SVertexP2T2C4*>(m_pMappedVertexBuffer.pData) + (m_spriteCount * 4);
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
        ORenderer->getDeviceContext()->Unmap(m_pVertexBuffer, 0);
    }

    void SpriteBatch::flush()
    {
        if (!m_spriteCount)
        {
            return; // Nothing to flush
        }
        auto pDeviceContext = ORenderer->getDeviceContext();

        pDeviceContext->Unmap(m_pVertexBuffer, 0);

        auto textureView = m_pTexture->getResourceView();
        pDeviceContext->PSSetShaderResources(0, 1, &textureView);
        pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
        pDeviceContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &m_stride, &m_offset);
        pDeviceContext->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R16_UINT, 0);
        pDeviceContext->DrawIndexed(6 * m_spriteCount, 0, 0);

        pDeviceContext->Map(m_pVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &m_pMappedVertexBuffer);

        m_spriteCount = 0;
        m_pTexture = nullptr;
    }
}
