#pragma once
#include "SimpleMath.h"
#include "Texture.h"
using namespace DirectX::SimpleMath;

namespace onut
{
    enum class ePrimitiveType
    {
        POINTS,
        LINES,
        LINE_STRIP,
        TRIANGLES
    };

    class PrimitiveBatch
    {
    public:
        PrimitiveBatch();
        virtual ~PrimitiveBatch();

        void begin(ePrimitiveType primitiveType, Texture* pTexture = nullptr, const Matrix& transform = Matrix::Identity);
        void draw(const Vector2& position, const Color& color = Color::White, const Vector2& texCoord = Vector2::Zero);
        void end();

    private:
        struct SVertexP2T2C4
        {
            Vector2 position;
            Vector2 texCoord;
            Color   color;
        };

        static const int MAX_VERTEX_COUNT = 1200;

        void flush();

        ID3D11Buffer*               m_pVertexBuffer = nullptr;
        D3D11_MAPPED_SUBRESOURCE    m_pMappedVertexBuffer;

        static const unsigned int   m_stride = sizeof(SVertexP2T2C4);
        static const unsigned int   m_offset = 0;
        unsigned int                m_vertexCount = 0;

        bool                        m_isDrawing = false;

        Texture*                    m_pTexWhite = nullptr;
        Texture*                    m_pTexture = nullptr;

        ePrimitiveType              m_primitiveType;
    };
}
