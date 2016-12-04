#ifndef VERTEXBUFFERGLES2_H_INCLUDED
#define VERTEXBUFFERGLES2_H_INCLUDED

#if defined(__unix__)
// Onut
#include <onut/VertexBuffer.h>

// Third party
#include <GLES/gl.h>
#include <EGL/egl.h>
#include <EGL/eglext.h>

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
        
        GLuint getHandle() const;

    private:
        friend class VertexBuffer;

        uint32_t m_size = 0;
        bool m_isDynamic = false;
        GLuint m_handle = 0;
        uint8_t* m_pData = nullptr;
    };
};

#endif

#endif
