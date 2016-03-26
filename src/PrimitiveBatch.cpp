// Onut
#include <onut/PrimitiveBatch.h>
#include <onut/Texture.h>

// Private
#include "RendererD3D11.h"

OPrimitiveBatchRef oPrimitiveBatch;

namespace onut
{
    OPrimitiveBatchRef PrimitiveBatch::create()
    {
        return OMake<PrimitiveBatch>();
    }

    PrimitiveBatch::PrimitiveBatch()
    {
        // Create a white texture for rendering "without" texture
        unsigned char white[4] = {255, 255, 255, 255};
        m_pTexWhite = OTexture::createFromData(white, {1, 1}, false);

        auto pRendererD3D11 = ODynamicCast<ORendererD3D11>(oRenderer);
        auto pDevice = pRendererD3D11->getDevice();
        auto pDeviceContext = pRendererD3D11->getDeviceContext();

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
    }

    void PrimitiveBatch::begin(PrimitiveMode primitiveType, const OTextureRef& pTexture, const Matrix& transform)
    {
        assert(!m_isDrawing); // Cannot call begin() twice without calling end()

        if (!pTexture) m_pTexture = m_pTexWhite;
        else m_pTexture = pTexture;

        m_primitiveType = primitiveType;
		oRenderer->setupFor2D(transform);
        m_isDrawing = true;

        auto pRendererD3D11 = std::dynamic_pointer_cast<ORendererD3D11>(oRenderer);
        pRendererD3D11->getDeviceContext()->Map(m_pVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &m_pMappedVertexBuffer);
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
            if (m_primitiveType == OPrimitiveLineStrip)
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
        auto pRendererD3D11 = std::dynamic_pointer_cast<ORendererD3D11>(oRenderer);
        pRendererD3D11->getDeviceContext()->Unmap(m_pVertexBuffer, 0);
    }

    void PrimitiveBatch::flush()
    {
        if (!m_vertexCount)
        {
            return; // Nothing to flush
        }

        auto pRendererD3D11 = std::dynamic_pointer_cast<ORendererD3D11>(oRenderer);
        auto pDeviceContext = pRendererD3D11->getDeviceContext();

        pDeviceContext->Unmap(m_pVertexBuffer, 0);

        *oRenderer = m_pTexture;
        *oRenderer = m_primitiveType;
        oRenderer->applyRenderStates();

        pDeviceContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &m_stride, &m_offset);
        pDeviceContext->Draw(m_vertexCount, 0);

        pDeviceContext->Map(m_pVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &m_pMappedVertexBuffer);

        m_vertexCount = 0;
    }
}
