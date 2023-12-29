#ifndef PRIMITIVEBATCH3D_H_INCLUDED
#define PRIMITIVEBATCH3D_H_INCLUDED

// Onut
#include <onut/Maths.h>
#include <onut/PrimitiveMode.h>

// Forward declares
#include <onut/ForwardDeclaration.h>
OForwardDeclare(PrimitiveBatch3D);
OForwardDeclare(Shader);
OForwardDeclare(Texture);
OForwardDeclare(VertexBuffer);

namespace onut
{
    class RenderStates;

    class PrimitiveBatch3D
    {
    public:
        static OPrimitiveBatch3DRef create();

        PrimitiveBatch3D();
        virtual ~PrimitiveBatch3D();

        void begin(PrimitiveMode primitiveType, const OTextureRef& pTexture = nullptr, const Matrix& transform = Matrix::Identity);
        void draw(const Vector3& position, const Color& color = Color::White, const Vector3& normal = Vector3::Up, const Vector2& texCoord = Vector2::Zero);

        // Only for line lists
        void drawPoint(const Vector3& position, float radius, const Color& color = Color::White);
        void drawCube(const Vector3& position, float extends, const Color& color = Color::White);
        void drawSphere(const Vector3& position, float radius, const Color& color = Color::White);
        void drawArrow(const Vector3& start, const Vector3& end, float headSize, const Color& color = Color::White);

        void end();

    private:
        struct SVertexP3T2C4
        {
            Vector3 position;
            Vector3 normal;
            Vector2 texCoord;
            Color   color;
        };

        static const int MAX_VERTEX_COUNT = 1200;
        static const int SPHERE_SIDE_COUNT = 24;

        void flush();

        OVertexBufferRef m_pVertexBuffer;
        SVertexP3T2C4* m_pMappedVertexBuffer = nullptr;

        unsigned int m_vertexCount = 0;

        bool m_isDrawing = false;

        OTextureRef m_pTexWhite;
        OTextureRef m_pTexture;
        OShaderRef m_pVS;
        OShaderRef m_pPS;

        PrimitiveMode m_primitiveType;
        RenderStates *m_pRenderStates = nullptr;

        float* m_sinTable = nullptr;
        float* m_cosTable = nullptr;
    };
}

extern OPrimitiveBatch3DRef oPrimitiveBatch3D;

#endif
