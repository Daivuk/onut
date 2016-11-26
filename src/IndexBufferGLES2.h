#pragma once
#if defined(__unix__)
// Onut
#include <onut/IndexBuffer.h>

// Third party
//#include <d3d11.h>

// Forward
#include <onut/ForwardDeclaration.h>
OForwardDeclare(IndexBufferGLES2)

namespace onut
{
    class IndexBufferGLES2 final : public IndexBuffer
    {
    public:
        IndexBufferGLES2();
        ~IndexBufferGLES2();

        void setData(const void* pIndexData, uint32_t size) override;
        void* map() override;
        void unmap(uint32_t size) override;
        uint32_t size() override;

    private:
        friend class IndexBuffer;

        uint32_t m_size = 0;
        bool m_isDynamic = false;
    };
};

#endif
