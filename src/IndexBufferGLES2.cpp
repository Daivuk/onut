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
        pRet->m_isDynamic = true;
        assert(false);
        return pRet;
    }

    IndexBufferGLES2::IndexBufferGLES2()
    {
    }

    IndexBufferGLES2::~IndexBufferGLES2()
    {
    }

    void IndexBufferGLES2::setData(const void* pVertexData, uint32_t size)
    {
        if (!m_isDynamic)
        {
            assert(false);
            m_size = size;
        }
        else
        {
            assert(false);
        }
    }

    void* IndexBufferGLES2::map()
    {
        assert(m_isDynamic);
        if (m_isDynamic)
        {
            assert(false);
        }
        return nullptr;
    }

    void IndexBufferGLES2::unmap(uint32_t size)
    {
        assert(m_isDynamic);
        if (m_isDynamic)
        {
            assert(false);
        }
    }

    uint32_t IndexBufferGLES2::size()
    {
        return m_size;
    }
}

#endif
