#pragma once
#if defined(__unix__)
// Onut
#include <onut/VertexBuffer.h>

// Third party
//#include <d3d11.h>

// Forward
#include <onut/ForwardDeclaration.h>
OForwardDeclare(VertexBufferGLES2)

namespace onut
{
    class VertexBufferGLES2 final : public VertexBuffer
    {
    public:
        VertexBufferGLES2();
        ~VertexBufferGLES2();

        void setData(const void* pVertexData, uint32_t size) override;
        void* map() override;
        void unmap(uint32_t size) override;
        uint32_t size() override;

    private:
        friend class VertexBuffer;

        uint32_t m_size = 0;
        bool m_isDynamic = false;
    };
};

#endif
