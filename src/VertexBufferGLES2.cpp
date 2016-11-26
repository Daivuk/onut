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
        assert(false);
        return pRet;
    }

    VertexBufferGLES2::VertexBufferGLES2()
    {
    }

    VertexBufferGLES2::~VertexBufferGLES2()
    {
    }

    void VertexBufferGLES2::setData(const void* pVertexData, uint32_t size)
    {
        if (!m_isDynamic)
        {
            assert(false);

            m_size = size;
        }
        else
        {
            auto pData = map();
            memcpy(pData, pVertexData, size);
            unmap(size);
        }
    }

    void* VertexBufferGLES2::map()
    {
        assert(m_isDynamic);
        if (m_isDynamic)
        {
            assert(false);
        }
        return nullptr;
    }

    void VertexBufferGLES2::unmap(uint32_t size)
    {
        assert(m_isDynamic);
        if (m_isDynamic)
        {
            assert(false);
        }
    }

    uint32_t VertexBufferGLES2::size()
    {
        return m_size;
    }
}

#endif
