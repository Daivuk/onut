// Private
#include "IndexBufferD3D11.h"
#include "RendererD3D11.h"

// STL
#include <cassert>

namespace onut
{
    OIndexBufferRef IndexBuffer::createStatic(const void* pVertexData, uint32_t size, int typeSize)
    {
        auto pRet = OMake<IndexBufferD3D11>();
        pRet->setData(pVertexData, size, typeSize);
        return pRet;
    }

    OIndexBufferRef IndexBuffer::createDynamic(uint32_t size)
    {
        auto pRet = OMake<IndexBufferD3D11>();
        auto pRendererD3D11 = std::dynamic_pointer_cast<ORendererD3D11>(oRenderer);
        auto pDevice = pRendererD3D11->getDevice();

        pRet->m_isDynamic = true;

        // Set up the description of the static vertex buffer.
        D3D11_BUFFER_DESC vertexBufferDesc;
        vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
        vertexBufferDesc.ByteWidth = size;
        vertexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
        vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        vertexBufferDesc.MiscFlags = 0;
        vertexBufferDesc.StructureByteStride = 0;

        auto ret = pDevice->CreateBuffer(&vertexBufferDesc, nullptr, &pRet->m_pBuffer);
        assert(ret == S_OK);

        pRet->m_size = size;

        return pRet;
    }

    IndexBufferD3D11::IndexBufferD3D11()
    {
    }

    IndexBufferD3D11::~IndexBufferD3D11()
    {
        if (m_pBuffer)
        {
            m_pBuffer->Release();
        }
    }

    ID3D11Buffer* IndexBufferD3D11::getBuffer() const
    {
        return m_pBuffer;
    }

    void IndexBufferD3D11::setData(const void* pVertexData, uint32_t size, int typeSize)
    {
        m_typeSize = typeSize;
        if (!m_isDynamic)
        {
            if (m_pBuffer)
            {
                m_pBuffer->Release();
            }

            auto pRendererD3D11 = std::dynamic_pointer_cast<ORendererD3D11>(oRenderer);
            auto pDevice = pRendererD3D11->getDevice();

            // Set up the description of the static vertex buffer.
            D3D11_BUFFER_DESC indexBufferDesc;
            indexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
            indexBufferDesc.ByteWidth = static_cast<UINT>(size);
            indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
            indexBufferDesc.CPUAccessFlags = 0;
            indexBufferDesc.MiscFlags = 0;
            indexBufferDesc.StructureByteStride = 0;

            // Give the subresource structure a pointer to the vertex data.
            D3D11_SUBRESOURCE_DATA indexData;
            indexData.pSysMem = pVertexData;
            indexData.SysMemPitch = 0;
            indexData.SysMemSlicePitch = 0;

            auto ret = pDevice->CreateBuffer(&indexBufferDesc, &indexData, &m_pBuffer);
            assert(ret == S_OK);

            m_size = size;
        }
        else
        {
            auto pData = map();
            memcpy(pData, pVertexData, size);
            unmap(size);
        }
    }

    void* IndexBufferD3D11::map()
    {
        assert(m_isDynamic);
        if (m_isDynamic)
        {
            auto pRendererD3D11 = std::dynamic_pointer_cast<ORendererD3D11>(oRenderer);
            pRendererD3D11->getDeviceContext()->Map(m_pBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &m_mappedIndexBuffer);
            return m_mappedIndexBuffer.pData;
        }
        return nullptr;
    }

    void IndexBufferD3D11::unmap(uint32_t size)
    {
        assert(m_isDynamic);
        if (m_isDynamic)
        {
            auto pRendererD3D11 = std::dynamic_pointer_cast<ORendererD3D11>(oRenderer);
            pRendererD3D11->getDeviceContext()->Unmap(m_pBuffer, 0);
        }
    }

    uint32_t IndexBufferD3D11::size()
    {
        return m_size;
    }

    int IndexBufferD3D11::GetTypeSize() const
    {
        return m_typeSize;
    }
}
