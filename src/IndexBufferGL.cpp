// Private
#include "IndexBufferGL.h"
#include "RendererGL.h"

// STL
#include <cassert>

namespace onut
{
    OIndexBufferRef IndexBuffer::createStatic(const void* pVertexData, uint32_t size, int typeSize)
    {
        auto pRet = OMake<IndexBufferGL>();
        pRet->setData(pVertexData, size, typeSize);
        return pRet;
    }

    OIndexBufferRef IndexBuffer::createDynamic(uint32_t size)
    {
        auto pRet = OMake<IndexBufferGL>();
        
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

    IndexBufferGL::IndexBufferGL()
    {
    }

    IndexBufferGL::~IndexBufferGL()
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

    void IndexBufferGL::setData(const void* pIndexData, uint32_t size, int typeSize)
    {
        m_typeSize = typeSize;
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

    void* IndexBufferGL::map()
    {
        assert(m_isDynamic);
        if (m_isDynamic)
        {
            return m_pData;
        }
        return nullptr;
    }

    void IndexBufferGL::unmap(uint32_t size)
    {
        assert(m_isDynamic);
        if (m_isDynamic)
        {
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_handle);
            glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, size, m_pData);
            oRenderer->renderStates.indexBuffer.forceDirty();
        }
    }

    uint32_t IndexBufferGL::size()
    {
        return m_size;
    }

    GLuint IndexBufferGL::getHandle() const
    {
        return m_handle;
    }

    int IndexBufferGL::getTypeSize() const
    {
        return m_typeSize;
    }
}
