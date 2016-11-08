// Onut
#include <onut/PrimitiveBatch.h>
#include <onut/Renderer.h>
#include <onut/Texture.h>
#include <onut/VertexBuffer.h>

// STL
#include <cassert>

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

        // Vertex buffer.
        m_pVertexBuffer = OVertexBuffer::createDynamic(sizeof(SVertexP2T2C4) * MAX_VERTEX_COUNT);
    }

    PrimitiveBatch::~PrimitiveBatch()
    {
    }

    void PrimitiveBatch::begin(PrimitiveMode primitiveType, const OTextureRef& pTexture, const Matrix& transform)
    {
        assert(!m_isDrawing); // Cannot call begin() twice without calling end()

        if (!pTexture) m_pTexture = m_pTexWhite;
        else m_pTexture = pTexture;

        m_primitiveType = primitiveType;
		oRenderer->setupFor2D(transform);
        m_isDrawing = true;

        m_pMappedVertexBuffer = reinterpret_cast<SVertexP2T2C4*>(m_pVertexBuffer->map());
    }

    void PrimitiveBatch::draw(const Vector2& position, const Color& color, const Vector2& texCoord)
    {
        SVertexP2T2C4* pVerts = m_pMappedVertexBuffer + m_vertexCount;
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

                auto pFirstVert = m_pMappedVertexBuffer;
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
        m_pVertexBuffer->unmap(sizeof(SVertexP2T2C4) * m_vertexCount);
    }

    void PrimitiveBatch::flush()
    {
        if (!m_vertexCount)
        {
            return; // Nothing to flush
        }

        m_pVertexBuffer->unmap(sizeof(SVertexP2T2C4) * m_vertexCount);

        oRenderer->renderStates.textures[0] = m_pTexture;
        oRenderer->renderStates.primitiveMode = m_primitiveType;
        oRenderer->renderStates.vertexBuffer = m_pVertexBuffer;
        oRenderer->draw(m_vertexCount);

        m_pMappedVertexBuffer = reinterpret_cast<SVertexP2T2C4*>(m_pVertexBuffer->map());

        m_vertexCount = 0;
    }
}
