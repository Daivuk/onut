// Onut
#include <onut/PrimitiveBatch3D.h>
#include <onut/Renderer.h>
#include <onut/Shader.h>
#include <onut/Texture.h>
#include <onut/VertexBuffer.h>

// Internal
#include "default_shaders.h"

// STL
#include <cassert>

OPrimitiveBatch3DRef oPrimitiveBatch3D;

namespace onut
{
    OPrimitiveBatch3DRef PrimitiveBatch3D::create()
    {
        return OMake<PrimitiveBatch3D>();
    }

    PrimitiveBatch3D::PrimitiveBatch3D()
    {
        // Create a white texture for rendering "without" texture
        unsigned char white[4] = {255, 255, 255, 255};
        m_pTexWhite = OTexture::createFromData(white, {1, 1}, false);

        // Vertex buffer.
        m_pVertexBuffer = OVertexBuffer::createDynamic(sizeof(SVertexP3T2C4) * MAX_VERTEX_COUNT);

        // Shaders
        m_pVS = OShader::createFromSource(SHADER_SRC_PRIMITIVE_BATCH_3D_VS, OVertexShader);
        m_pPS = OShader::createFromSource(SHADER_SRC_PRIMITIVE_BATCH_3D_PS, OPixelShader);

        m_sinTable = new float[SPHERE_SIDE_COUNT + 1];
        m_cosTable = new float[SPHERE_SIDE_COUNT + 1];
        float inv_r_SPHERE_SIDE_COUNT = (1.0f / SPHERE_SIDE_COUNT) * O2PI;
        for (int i = 0; i < SPHERE_SIDE_COUNT; ++i)
        {
            auto a = (float)i * inv_r_SPHERE_SIDE_COUNT;
            m_sinTable[i] = std::sin(a);
            m_cosTable[i] = std::cos(a);
        }
        m_sinTable[SPHERE_SIDE_COUNT] = m_sinTable[0];
        m_cosTable[SPHERE_SIDE_COUNT] = m_cosTable[0];
    }

    PrimitiveBatch3D::~PrimitiveBatch3D()
    {
        delete[] m_sinTable;
        delete[] m_cosTable;
    }

    void PrimitiveBatch3D::begin(PrimitiveMode primitiveType, const OTextureRef& pTexture, const Matrix& transform)
    {
        assert(!m_isDrawing); // Cannot call begin() twice without calling end()

        if (!pTexture) m_pTexture = m_pTexWhite;
        else m_pTexture = pTexture;

        auto& rs = oRenderer->renderStates;

        m_primitiveType = primitiveType;
        rs.world = transform;
		rs.vertexShader = m_pVS;
        rs.pixelShader = m_pPS;
        m_isDrawing = true;
        m_pRenderStates = &oRenderer->renderStates;

        m_pMappedVertexBuffer = reinterpret_cast<SVertexP3T2C4*>(m_pVertexBuffer->map());
    }

    void PrimitiveBatch3D::draw(const Vector3& position, const Color& color, const Vector3& normal, const Vector2& texCoord)
    {
        SVertexP3T2C4* pVerts = m_pMappedVertexBuffer + m_vertexCount;
        pVerts->position = position;
        pVerts->normal = normal;
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

    void PrimitiveBatch3D::drawPoint(const Vector3& position, float radius, const Color& color)
    {
        if (m_primitiveType != OPrimitiveLineList) return;

        draw(Vector3(position.x - radius, position.y, position.z), color);
        draw(Vector3(position.x + radius, position.y, position.z), color);
        draw(Vector3(position.x, position.y - radius, position.z), color);
        draw(Vector3(position.x, position.y + radius, position.z), color);
        draw(Vector3(position.x, position.y, position.z - radius), color);
        draw(Vector3(position.x, position.y, position.z + radius), color);
    }

    void PrimitiveBatch3D::drawCube(const Vector3& position, float radius, const Color& color)
    {
        if (m_primitiveType != OPrimitiveLineList) return;

        // X axis edges
        draw(Vector3(position.x - radius, position.y - radius, position.z - radius), color);
        draw(Vector3(position.x + radius, position.y - radius, position.z - radius), color);
        draw(Vector3(position.x - radius, position.y + radius, position.z - radius), color);
        draw(Vector3(position.x + radius, position.y + radius, position.z - radius), color);
        draw(Vector3(position.x - radius, position.y + radius, position.z + radius), color);
        draw(Vector3(position.x + radius, position.y + radius, position.z + radius), color);
        draw(Vector3(position.x - radius, position.y - radius, position.z + radius), color);
        draw(Vector3(position.x + radius, position.y - radius, position.z + radius), color);

        // Y axis edges
        draw(Vector3(position.x - radius, position.y - radius, position.z - radius), color);
        draw(Vector3(position.x - radius, position.y + radius, position.z - radius), color);
        draw(Vector3(position.x + radius, position.y - radius, position.z - radius), color);
        draw(Vector3(position.x + radius, position.y + radius, position.z - radius), color);
        draw(Vector3(position.x + radius, position.y - radius, position.z + radius), color);
        draw(Vector3(position.x + radius, position.y + radius, position.z + radius), color);
        draw(Vector3(position.x - radius, position.y - radius, position.z + radius), color);
        draw(Vector3(position.x - radius, position.y + radius, position.z + radius), color);

        // Z axis edges
        draw(Vector3(position.x - radius, position.y - radius, position.z - radius), color);
        draw(Vector3(position.x - radius, position.y - radius, position.z + radius), color);
        draw(Vector3(position.x - radius, position.y + radius, position.z - radius), color);
        draw(Vector3(position.x - radius, position.y + radius, position.z + radius), color);
        draw(Vector3(position.x + radius, position.y + radius, position.z - radius), color);
        draw(Vector3(position.x + radius, position.y + radius, position.z + radius), color);
        draw(Vector3(position.x + radius, position.y - radius, position.z - radius), color);
        draw(Vector3(position.x + radius, position.y - radius, position.z + radius), color);
    }

    void PrimitiveBatch3D::drawSphere(const Vector3& position, float radius, const Color& color)
    {
        if (m_primitiveType != OPrimitiveLineList) return;

        for (int i = 0; i < SPHERE_SIDE_COUNT; ++i)
        {
            draw(Vector3(position.x, position.y + m_cosTable[i] * radius, position.z + m_sinTable[i] * radius), color);
            draw(Vector3(position.x, position.y + m_cosTable[i + 1] * radius, position.z + m_sinTable[i + 1] * radius), color);
            draw(Vector3(position.x + m_cosTable[i] * radius, position.y, position.z + m_sinTable[i] * radius), color);
            draw(Vector3(position.x + m_cosTable[i + 1] * radius, position.y, position.z + m_sinTable[i + 1] * radius), color);
            draw(Vector3(position.x + m_cosTable[i] * radius, position.y + m_sinTable[i] * radius, position.z), color);
            draw(Vector3(position.x + m_cosTable[i + 1] * radius, position.y + m_sinTable[i + 1] * radius, position.z), color);
        }
    }

    void PrimitiveBatch3D::drawArrow(const Vector3& start, const Vector3& end, float headSize, const Color& color)
    {
        if (m_primitiveType != OPrimitiveLineList) return;

        auto dir = end - start;
        dir.Normalize();

        auto headPos = end - dir * headSize;

        draw(start, color);
        draw(headPos, color);

        Vector3 right;
        if (dir.z >= 0.7f)
            right = dir.Cross(Vector3::Right);
        else
            right = dir.Cross(Vector3::Up);

        right.Normalize();
        auto front = right.Cross(dir);

        auto radius = headSize * 0.5f;
        for (int i = 0; i < SPHERE_SIDE_COUNT; i += 2)
        {
            draw(headPos + right * m_sinTable[i] * radius + front * m_cosTable[i] * radius, color);
            draw(headPos + right * m_sinTable[i + 2] * radius + front * m_cosTable[i + 2] * radius, color);
        }

        draw(headPos + right * m_sinTable[0] * radius + front * m_cosTable[0] * radius, color);
        draw(end, color);
        draw(headPos + right * m_sinTable[SPHERE_SIDE_COUNT / 4] * radius + front * m_cosTable[SPHERE_SIDE_COUNT / 4] * radius, color);
        draw(end, color);
        draw(headPos + right * m_sinTable[SPHERE_SIDE_COUNT / 2] * radius + front * m_cosTable[SPHERE_SIDE_COUNT / 2] * radius, color);
        draw(end, color);
        draw(headPos + right * m_sinTable[SPHERE_SIDE_COUNT * 3 / 4] * radius + front * m_cosTable[SPHERE_SIDE_COUNT * 3 / 4] * radius, color);
        draw(end, color);
    }

    void PrimitiveBatch3D::end()
    {
        assert(m_isDrawing); // Should call begin() before calling end()

        m_isDrawing = false;
        if (m_vertexCount)
        {
            flush();
        }
        m_pVertexBuffer->unmap(sizeof(SVertexP3T2C4) * m_vertexCount);
    }

    void PrimitiveBatch3D::flush()
    {
        if (!m_vertexCount)
        {
            return; // Nothing to flush
        }

        m_pVertexBuffer->unmap(sizeof(SVertexP3T2C4) * m_vertexCount);

        oRenderer->renderStates.textures[0] = m_pTexture;
        oRenderer->renderStates.primitiveMode = m_primitiveType;
        oRenderer->renderStates.vertexBuffer = m_pVertexBuffer;
        oRenderer->draw(m_vertexCount);

        m_pMappedVertexBuffer = reinterpret_cast<SVertexP3T2C4*>(m_pVertexBuffer->map());

        m_vertexCount = 0;
    }
}
