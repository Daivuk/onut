#pragma once
#if defined(WIN32)
// Onut
#include <onut/VertexBuffer.h>

// Third party
#include <d3d11.h>

// Forward
#include <onut/ForwardDeclaration.h>
OForwardDeclare(VertexBufferD3D11)

namespace onut
{
    class VertexBufferD3D11 final : public VertexBuffer
    {
    public:
        VertexBufferD3D11();
        ~VertexBufferD3D11();

        ID3D11Buffer* getBuffer() const;

        void setData(const void* pVertexData, uint32_t size) override;
        void* map() override;
        void unmap(uint32_t size) override;
        uint32_t size() override;

    private:
        friend class VertexBuffer;

        ID3D11Buffer* m_pBuffer = nullptr;
        uint32_t m_size = 0;
        bool m_isDynamic = false;
        D3D11_MAPPED_SUBRESOURCE m_mappedVertexBuffer;
    };
};

#endif
