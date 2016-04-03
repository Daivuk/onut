// Private
#include "VertexBufferD3D11.h"
#include "RendererD3D11.h"

namespace onut
{
    OVertexBufferRef VertexBuffer::createStatic(const void* pVertexData, uint32_t size)
    {
        auto pRet = OMake<VertexBufferD3D11>();
        pRet->setData(pVertexData, size);
        return pRet;
    }

    OVertexBufferRef VertexBuffer::createDynamic(uint32_t size)
    {
        auto pRet = OMake<VertexBufferD3D11>();
        auto pRendererD3D11 = std::dynamic_pointer_cast<ORendererD3D11>(oRenderer);
        auto pDevice = pRendererD3D11->getDevice();

        pRet->m_isDynamic = true;

        // Set up the description of the static vertex buffer.
        D3D11_BUFFER_DESC vertexBufferDesc;
        vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
        vertexBufferDesc.ByteWidth = size;
        vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        vertexBufferDesc.MiscFlags = 0;
        vertexBufferDesc.StructureByteStride = 0;

        auto ret = pDevice->CreateBuffer(&vertexBufferDesc, nullptr, &pRet->m_pBuffer);
        assert(ret == S_OK);

        return pRet;
    }

    VertexBufferD3D11::VertexBufferD3D11()
    {
    }

    VertexBufferD3D11::~VertexBufferD3D11()
    {
        if (m_pBuffer)
        {
            m_pBuffer->Release();
        }
    }

    ID3D11Buffer* VertexBufferD3D11::getBuffer() const
    {
        return m_pBuffer;
    }

    void VertexBufferD3D11::setData(const void* pVertexData, uint32_t size)
    {
        if (!m_isDynamic)
        {
            if (m_pBuffer)
            {
                m_pBuffer->Release();
            }

            auto pRendererD3D11 = std::dynamic_pointer_cast<ORendererD3D11>(oRenderer);
            auto pDevice = pRendererD3D11->getDevice();

            // Set up the description of the static vertex buffer.
            D3D11_BUFFER_DESC vertexBufferDesc;
            vertexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
            vertexBufferDesc.ByteWidth = static_cast<UINT>(size);
            vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
            vertexBufferDesc.CPUAccessFlags = 0;
            vertexBufferDesc.MiscFlags = 0;
            vertexBufferDesc.StructureByteStride = 0;

            // Give the subresource structure a pointer to the vertex data.
            D3D11_SUBRESOURCE_DATA vertexData;
            vertexData.pSysMem = pVertexData;
            vertexData.SysMemPitch = 0;
            vertexData.SysMemSlicePitch = 0;

            auto ret = pDevice->CreateBuffer(&vertexBufferDesc, &vertexData, &m_pBuffer);
            assert(ret == S_OK);

            m_size = size;
        }
        else
        {
            assert(false);
        }
    }

    void* VertexBufferD3D11::map()
    {
        assert(m_isDynamic);
        if (m_isDynamic)
        {
            auto pRendererD3D11 = std::dynamic_pointer_cast<ORendererD3D11>(oRenderer);
            pRendererD3D11->getDeviceContext()->Map(m_pBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &m_mappedVertexBuffer);
            return m_mappedVertexBuffer.pData;
        }
        return nullptr;
    }

    void VertexBufferD3D11::unmap(uint32_t size)
    {
        assert(m_isDynamic);
        if (m_isDynamic)
        {
            auto pRendererD3D11 = std::dynamic_pointer_cast<ORendererD3D11>(oRenderer);
            pRendererD3D11->getDeviceContext()->Unmap(m_pBuffer, 0);
        }
    }

    uint32_t VertexBufferD3D11::size()
    {
        return m_size;
    }
}
