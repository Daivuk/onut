#include "PrimitiveBatch.h"
#include "onut.h"

namespace onut
{
    PrimitiveBatch::PrimitiveBatch()
    {
        // Create a white texture for rendering "without" texture
        unsigned char white[4] = {255, 255, 255, 255};
        m_pTexWhite = Texture::createFromData({1, 1}, white, false);

        auto pDevice = ORenderer->getDevice();
        auto pDeviceContext = ORenderer->getDeviceContext();

        SVertexP2T2C4 vertices[MAX_VERTEX_COUNT];

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
    }

    PrimitiveBatch::~PrimitiveBatch()
    {
        if (m_pVertexBuffer) m_pVertexBuffer->Release();
        delete m_pTexWhite;
    }

	void PrimitiveBatch::begin(ePrimitiveType primitiveType, Texture* pTexture, const Matrix& transform)
    {
        assert(!m_isDrawing); // Cannot call begin() twice without calling end()

        if (!pTexture) pTexture = m_pTexWhite;
        m_pTexture = pTexture;

        m_primitiveType = primitiveType;
		ORenderer->setupFor2D(transform);
        m_isDrawing = true;
        ORenderer->getDeviceContext()->Map(m_pVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &m_pMappedVertexBuffer);
    }

    void PrimitiveBatch::draw(const Vector2& position, const Color& color, const Vector2& texCoord)
    {
        SVertexP2T2C4* pVerts = static_cast<SVertexP2T2C4*>(m_pMappedVertexBuffer.pData) + m_vertexCount;
        pVerts->position = position;
        pVerts->texCoord = texCoord;
        pVerts->color = color;

        ++m_vertexCount;

        if (m_vertexCount == MAX_VERTEX_COUNT)
        {
            if (m_primitiveType == ePrimitiveType::LINE_STRIP)
            {
                auto lastVert = *pVerts;

                flush();

                auto pFirstVert = static_cast<SVertexP2T2C4*>(m_pMappedVertexBuffer.pData);
                *pFirstVert = lastVert;
                ++m_vertexCount;
            }
            else
            {
                flush();
            }
        }
    }

    void PrimitiveBatch::end()
    {
        assert(m_isDrawing); // Should call begin() before calling end()

        m_isDrawing = false;
        if (m_vertexCount)
        {
            flush();
        }
        ORenderer->getDeviceContext()->Unmap(m_pVertexBuffer, 0);
    }

    void PrimitiveBatch::flush()
    {
        if (!m_vertexCount)
        {
            return; // Nothing to flush
        }

        auto pDeviceContext = ORenderer->getDeviceContext();

        pDeviceContext->Unmap(m_pVertexBuffer, 0);

        auto textureView = m_pTexture->getResourceView();
        pDeviceContext->PSSetShaderResources(0, 1, &textureView);
        switch (m_primitiveType)
        {
            case ePrimitiveType::POINTS:
                pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
                break;
            case ePrimitiveType::LINES:
                pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
                break;
            case ePrimitiveType::LINE_STRIP:
                pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);
                break;
            case ePrimitiveType::TRIANGLES:
                pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
                break;
        }
        pDeviceContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &m_stride, &m_offset);
        pDeviceContext->Draw(m_vertexCount, 0);

        pDeviceContext->Map(m_pVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &m_pMappedVertexBuffer);

        m_vertexCount = 0;
    }
}
