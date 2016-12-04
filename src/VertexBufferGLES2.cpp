#if defined(__unix__)
// Private
#include "VertexBufferGLES2.h"
#include "RendererGLES2.h"

// STL
#include <cassert>

namespace onut
{
    OVertexBufferRef VertexBuffer::createStatic(const void* pVertexData, uint32_t size)
    {
        auto pRet = OMake<VertexBufferGLES2>();
        pRet->setData(pVertexData, size);
        return pRet;
    }

    OVertexBufferRef VertexBuffer::createDynamic(uint32_t size)
    {
        auto pRet = OMake<VertexBufferGLES2>();
        
        GLuint handle;
        glGenBuffers(1, &handle);
        glBindBuffer(GL_ARRAY_BUFFER, handle);
        glBufferData(GL_ARRAY_BUFFER, size, NULL, GL_DYNAMIC_DRAW);
        
        pRet->m_handle = handle;
        pRet->m_size = size;
        pRet->m_pData = new uint8_t[size];
        pRet->m_isDynamic = true;
        
        oRenderer->renderStates.vertexBuffer.forceDirty();
        
        return pRet;
    }

    VertexBufferGLES2::VertexBufferGLES2()
    {
    }

    VertexBufferGLES2::~VertexBufferGLES2()
    {
        if (m_handle)
        {
            glDeleteBuffers(1, &m_handle);
        }
        if (m_pData)
        {
            delete [] m_pData;
        }
    }

    void VertexBufferGLES2::setData(const void* pVertexData, uint32_t size)
    {
        if (!m_isDynamic)
        {
            if (m_handle)
            {
                glDeleteBuffers(1, &m_handle);
            }
            
            glGenBuffers(1, &m_handle);
            glBindBuffer(GL_ARRAY_BUFFER, m_handle);
            glBufferData(GL_ARRAY_BUFFER, size, pVertexData, GL_STATIC_DRAW);

            m_size = size;
        }
        else
        {
            glBindBuffer(GL_ARRAY_BUFFER, m_handle);
            glBufferSubData(GL_ARRAY_BUFFER, 0, size, pVertexData);
        }
        oRenderer->renderStates.vertexBuffer.forceDirty();
    }

    void* VertexBufferGLES2::map()
    {
        assert(m_isDynamic);
        return m_pData;
    }

    void VertexBufferGLES2::unmap(uint32_t size)
    {
        assert(m_isDynamic);
        if (m_isDynamic)
        {
            glBindBuffer(GL_ARRAY_BUFFER, m_handle);
            glBufferSubData(GL_ARRAY_BUFFER, 0, size, m_pData);
            oRenderer->renderStates.vertexBuffer.forceDirty();
        }
    }

    uint32_t VertexBufferGLES2::size()
    {
        return m_size;
    }

    GLuint VertexBufferGLES2::getHandle() const
    {
        return m_handle;
    }
}

#endif
