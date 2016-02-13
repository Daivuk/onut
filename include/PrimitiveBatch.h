#pragma once
#include "onut/Maths.h"
#include "onut/Texture.h"

#include <memory>

// Forward declares
namespace onut
{
    class Texture;
};
using OTextureRef = std::shared_ptr<onut::Texture>;

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

        void begin(ePrimitiveType primitiveType, const OTextureRef& pTexture = nullptr, const Matrix& transform = Matrix::Identity);
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

        OTextureRef m_pTexWhite;
        OTextureRef m_pTexture;

        ePrimitiveType              m_primitiveType;
    };
}
