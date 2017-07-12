#ifndef VERTEXBUFFERGL_H_INCLUDED
#define VERTEXBUFFERGL_H_INCLUDED

// Onut
#include <onut/VertexBuffer.h>

// Third partys
#include "gl_includes.h"

// Forward
#include <onut/ForwardDeclaration.h>
OForwardDeclare(VertexBufferGL)

namespace onut
{
    class VertexBufferGL final : public VertexBuffer
    {
    public:
        VertexBufferGL();
        ~VertexBufferGL();

        void setData(const void* pVertexData, uint32_t size) override;
        void* map() override;
        void unmap(uint32_t size) override;
        uint32_t size() override;
        
        GLuint getHandle() const;

    private:
        friend class VertexBuffer;

        uint32_t m_size = 0;
        bool m_isDynamic = false;
        GLuint m_vao = 0;
        GLuint m_handle = 0;
        uint8_t* m_pData = nullptr;
    };
};

#endif
