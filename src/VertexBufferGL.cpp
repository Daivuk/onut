// Private
#include "VertexBufferGL.h"
#include "RendererGL.h"

// STL
#include <cassert>

namespace onut
{
    OVertexBufferRef VertexBuffer::createStatic(const void* pVertexData, uint32_t size)
    {
        auto pRet = OMake<VertexBufferGL>();
        pRet->setData(pVertexData, size);
        return pRet;
    }

    OVertexBufferRef VertexBuffer::createDynamic(uint32_t size)
    {
        auto pRet = OMake<VertexBufferGL>();
        
        GLuint handle; 
        glGenVertexArrays(1, &pRet->m_vao);
        glBindVertexArray(pRet->m_vao);
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

    VertexBufferGL::VertexBufferGL()
    {
    }

    VertexBufferGL::~VertexBufferGL()
    {
        if (m_handle)
        {
            glDeleteBuffers(1, &m_handle);
        }
        if (m_vao)
        {
            glDeleteVertexArrays(1, &m_vao);
        }
        if (m_pData)
        {
            delete [] m_pData;
        }
    }

    void VertexBufferGL::setData(const void* pVertexData, uint32_t size)
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

    void* VertexBufferGL::map()
    {
        assert(m_isDynamic);
        return m_pData;
    }

    void VertexBufferGL::unmap(uint32_t size)
    {
        assert(m_isDynamic);
        if (m_isDynamic)
        {
            glBindBuffer(GL_ARRAY_BUFFER, m_handle);
            glBufferSubData(GL_ARRAY_BUFFER, 0, size, m_pData);
            oRenderer->renderStates.vertexBuffer.forceDirty();
        }
    }

    uint32_t VertexBufferGL::size()
    {
        return m_size;
    }

    GLuint VertexBufferGL::getHandle() const
    {
        return m_handle;
    }
}
