#ifndef INDEXBUFFERGL_H_INCLUDED
#define INDEXBUFFERGL_H_INCLUDED

// Onut
#include <onut/IndexBuffer.h>

// Third party
#include "gl_includes.h"

// Forward
#include <onut/ForwardDeclaration.h>
OForwardDeclare(IndexBufferGL)

namespace onut
{
    class IndexBufferGL final : public IndexBuffer
    {
    public:
        IndexBufferGL();
        ~IndexBufferGL();

        void setData(const void* pIndexData, uint32_t size, int typeSize = 16) override;
        void* map() override;
        void unmap(uint32_t size) override;
        uint32_t size() override;
        
        GLuint getHandle() const;
        int getTypeSize() const;

    private:
        friend class IndexBuffer;

        uint32_t m_size = 0;
        bool m_isDynamic = false;
        GLuint m_handle = 0;
        uint8_t* m_pData = nullptr;
        int m_typeSize = 16;
    };
};

#endif
