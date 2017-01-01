#if defined(__unix__)
// Private
#include "IndexBufferGLES2.h"
#include "RendererGLES2.h"

// STL
#include <cassert>

namespace onut
{
    OIndexBufferRef IndexBuffer::createStatic(const void* pVertexData, uint32_t size)
    {
        auto pRet = OMake<IndexBufferGLES2>();
        pRet->setData(pVertexData, size);
        return pRet;
    }

    OIndexBufferRef IndexBuffer::createDynamic(uint32_t size)
    {
        auto pRet = OMake<IndexBufferGLES2>();
        
        GLuint handle;
        glGenBuffers(1, &handle);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, handle);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, NULL, GL_DYNAMIC_DRAW);
        
        pRet->m_handle = handle;
        pRet->m_size = size;
        pRet->m_pData = new uint8_t[size];
        pRet->m_isDynamic = true;
        
        oRenderer->renderStates.indexBuffer.forceDirty();
        
        return pRet;
    }

    IndexBufferGLES2::IndexBufferGLES2()
    {
    }

    IndexBufferGLES2::~IndexBufferGLES2()
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

    void IndexBufferGLES2::setData(const void* pIndexData, uint32_t size)
    {
        if (!m_isDynamic)
        {
            if (m_handle)
            {
                glDeleteBuffers(1, &m_handle);
            }
            
            glGenBuffers(1, &m_handle);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_handle);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, pIndexData, GL_STATIC_DRAW);

            m_size = size;
        }
        else
        {
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_handle);
            glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, size, pIndexData);
        }
        oRenderer->renderStates.indexBuffer.forceDirty();
    }

    void* IndexBufferGLES2::map()
    {
        assert(m_isDynamic);
        if (m_isDynamic)
        {
            return m_pData;
        }
        return nullptr;
    }

    void IndexBufferGLES2::unmap(uint32_t size)
    {
        assert(m_isDynamic);
        if (m_isDynamic)
        {
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_handle);
            glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, size, m_pData);
            oRenderer->renderStates.indexBuffer.forceDirty();
        }
    }

    uint32_t IndexBufferGLES2::size()
    {
        return m_size;
    }

    GLuint IndexBufferGLES2::getHandle() const
    {
        return m_handle;
    }
}

#endif
