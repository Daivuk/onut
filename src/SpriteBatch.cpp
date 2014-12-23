#include "onut.h"
#include "SpriteBatch.h"

namespace onut {
    SpriteBatch::SpriteBatch() {
        auto pDevice = ORenderer->getDevice();
        auto pDeviceContext = ORenderer->getDeviceContext();

        // Create a white texture for rendering "without" texture
        unsigned char white[4] = { 255, 255, 255, 255 };
        m_pTexWhite = Texture::createFromData({ 1, 1 }, white, false);

        SVertexP2T2C4 vertices[MAX_SPRITE_COUNT * 4];
        unsigned short indices[MAX_SPRITE_COUNT * 6];
        for (unsigned int i = 0; i < MAX_SPRITE_COUNT; ++i) {
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

        assert(pDevice->CreateBuffer(&vertexBufferDesc, &vertexData, &m_pVertexBuffer) == S_OK);

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

        assert(pDevice->CreateBuffer(&indexBufferDesc, &indexData, &m_pIndexBuffer) == S_OK);
    }

    SpriteBatch::~SpriteBatch() {
        if (m_pVertexBuffer) m_pVertexBuffer->Release();
        if (m_pIndexBuffer) m_pIndexBuffer->Release();
    }

    void SpriteBatch::begin() {
        assert(!m_isDrawing); // Cannot call begin() twice without calling end()

        ORenderer->setupFor2D();
        m_isDrawing = true;
        auto pDeviceContext = ORenderer->getDeviceContext();
        pDeviceContext->Map(m_pVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &m_pMappedVertexBuffer);
    }

    void SpriteBatch::drawRectWithColors(Texture* pTexture, const Rect& rect, const std::vector<Color>& colors) {
        assert(m_isDrawing); // Should call begin() before calling draw()
        assert(colors.size() == 4); // Needs 4 colors

        if (!pTexture) pTexture = m_pTexWhite;
        if (pTexture != m_pTexture) {
            flush();
        }
        m_pTexture = pTexture;

        SVertexP2T2C4* pVerts = static_cast<SVertexP2T2C4*>(m_pMappedVertexBuffer.pData) + (m_spriteCount * 4);
        pVerts[0].position = { rect.x, rect.y };
        pVerts[0].texCoord = { 0, 0 };
        pVerts[0].color = colors[0];

        pVerts[1].position = { rect.x, rect.y + rect.w };
        pVerts[1].texCoord = { 0, 1 };
        pVerts[1].color = colors[1];

        pVerts[2].position = { rect.x + rect.z, rect.y + rect.w };
        pVerts[2].texCoord = { 1, 1 };
        pVerts[2].color = colors[2];

        pVerts[3].position = { rect.x + rect.z, rect.y };
        pVerts[3].texCoord = { 1, 0 };
        pVerts[3].color = colors[3];

        ++m_spriteCount;

        if (m_spriteCount == MAX_SPRITE_COUNT) {
            flush();
        }
    }

    void SpriteBatch::drawRect(Texture* pTexture, const Rect& rect, const Color& color) {
        assert(m_isDrawing); // Should call begin() before calling draw()

        if (color.x == 0 && color.y == 0 && color.z == 0 && color.w == 0) return;

        if (!pTexture) pTexture = m_pTexWhite;
        if (pTexture != m_pTexture) {
            flush();
        }
        m_pTexture = pTexture;

        SVertexP2T2C4* pVerts = static_cast<SVertexP2T2C4*>(m_pMappedVertexBuffer.pData) + (m_spriteCount * 4);
        pVerts[0].position = { rect.x, rect.y };
        pVerts[0].texCoord = { 0, 0 };
        pVerts[0].color = color;

        pVerts[1].position = { rect.x, rect.y + rect.w };
        pVerts[1].texCoord = { 0, 1 };
        pVerts[1].color = color;

        pVerts[2].position = { rect.x + rect.z, rect.y + rect.w };
        pVerts[2].texCoord = { 1, 1 };
        pVerts[2].color = color;

        pVerts[3].position = { rect.x + rect.z, rect.y };
        pVerts[3].texCoord = { 1, 0 };
        pVerts[3].color = color;

        ++m_spriteCount;

        if (m_spriteCount == MAX_SPRITE_COUNT) {
            flush();
        }
    }

    void SpriteBatch::drawRectWithUVs(Texture* pTexture, const Rect& rect, const Vector4& uvs, const Color& color) {
        assert(m_isDrawing); // Should call begin() before calling draw()

        if (color.x == 0 && color.y == 0 && color.z == 0 && color.w == 0) return;

        if (!pTexture) pTexture = m_pTexWhite;
        if (pTexture != m_pTexture) {
            flush();
        }
        m_pTexture = pTexture;

        SVertexP2T2C4* pVerts = static_cast<SVertexP2T2C4*>(m_pMappedVertexBuffer.pData) + (m_spriteCount * 4);
        pVerts[0].position = { rect.x, rect.y };
        pVerts[0].texCoord = { uvs.x, uvs.y };
        pVerts[0].color = color;

        pVerts[1].position = { rect.x, rect.y + rect.w };
        pVerts[1].texCoord = { uvs.x, uvs.w };
        pVerts[1].color = color;

        pVerts[2].position = { rect.x + rect.z, rect.y + rect.w };
        pVerts[2].texCoord = { uvs.z, uvs.w };
        pVerts[2].color = color;

        pVerts[3].position = { rect.x + rect.z, rect.y };
        pVerts[3].texCoord = { uvs.z, uvs.y };
        pVerts[3].color = color;

        ++m_spriteCount;

        if (m_spriteCount == MAX_SPRITE_COUNT) {
            flush();
        }
    }

    void SpriteBatch::drawRectWithUVsColors(Texture* pTexture, const Rect& rect, const Vector4& uvs, const std::vector<Color>& colors) {
        assert(m_isDrawing); // Should call begin() before calling draw()
        assert(colors.size() == 4); // Needs 4 colors

        if (!pTexture) pTexture = m_pTexWhite;
        if (pTexture != m_pTexture) {
            flush();
        }
        m_pTexture = pTexture;

        SVertexP2T2C4* pVerts = static_cast<SVertexP2T2C4*>(m_pMappedVertexBuffer.pData) + (m_spriteCount * 4);
        pVerts[0].position = { rect.x, rect.y };
        pVerts[0].texCoord = { uvs.x, uvs.y };
        pVerts[0].color = colors[0];

        pVerts[1].position = { rect.x, rect.y + rect.w };
        pVerts[1].texCoord = { uvs.x, uvs.w };
        pVerts[1].color = colors[1];

        pVerts[2].position = { rect.x + rect.z, rect.y + rect.w };
        pVerts[2].texCoord = { uvs.z, uvs.w };
        pVerts[2].color = colors[2];

        pVerts[3].position = { rect.x + rect.z, rect.y };
        pVerts[3].texCoord = { uvs.z, uvs.y };
        pVerts[3].color = colors[3];

        ++m_spriteCount;

        if (m_spriteCount == MAX_SPRITE_COUNT) {
            flush();
        }
    }

    void SpriteBatch::draw4Corner(Texture* pTexture, const Rect& rect, const Color& color) {
        assert(pTexture); // This call requires a texture to be bound

        if (pTexture != m_pTexture) {
            flush();
        }
        m_pTexture = pTexture;

        auto& textureSize = pTexture->getSize();
        Rect cornerRect{ 0, 0, static_cast<float>(textureSize.x) * .5f, static_cast<float>(textureSize.y) * .5f };
        drawRectWithUVs(pTexture, rect.TopLeft(cornerRect), { 0, 0, .5f, .5f }, color);
        drawRectWithUVs(pTexture, rect.TopRight(cornerRect), { .5f, 0, 1, .5f }, color);
        drawRectWithUVs(pTexture, rect.BottomLeft(cornerRect), { 0, .5f, .5f, 1 }, color);
        drawRectWithUVs(pTexture, rect.BottomRight(cornerRect), { .5f, .5f, 1, 1 }, color);
    }

    void SpriteBatch::end() {
        assert(m_isDrawing); // Should call begin() before calling end()

        m_isDrawing = false;
        flush();
    }

    void SpriteBatch::flush() {
        if (!m_spriteCount) {
            m_pTexture = nullptr;
            return; // Nothing to flush
        }

        auto pDeviceContext = ORenderer->getDeviceContext();

        pDeviceContext->Unmap(m_pVertexBuffer, 0);

        auto textureView = m_pTexture->getResource();
        pDeviceContext->PSSetShaderResources(0, 1, &textureView);

        pDeviceContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &m_stride, &m_offset);
        pDeviceContext->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R16_UINT, 0);
        pDeviceContext->DrawIndexed(6 * m_spriteCount, 0, 0);

        m_spriteCount = 0;
        m_pTexture = nullptr;

        if (m_isDrawing) {
            pDeviceContext->Map(m_pVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &m_pMappedVertexBuffer);
        }
    }
}
