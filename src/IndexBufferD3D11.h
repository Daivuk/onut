#ifndef INDEXBUFFERD3D11_H_INCLUDED
#define INDEXBUFFERD3D11_H_INCLUDED

#if defined(WIN32)
// Onut
#include <onut/IndexBuffer.h>

// Third party
#include <d3d11.h>

// Forward
#include <onut/ForwardDeclaration.h>
OForwardDeclare(IndexBufferD3D11)

namespace onut
{
    class IndexBufferD3D11 final : public IndexBuffer
    {
    public:
        IndexBufferD3D11();
        ~IndexBufferD3D11();

        ID3D11Buffer* getBuffer() const;

        void setData(const void* pIndexData, uint32_t size, int typeSize = 16) override;
        void* map() override;
        void unmap(uint32_t size) override;
        uint32_t size() override;
        int GetTypeSize() const;

    private:
        friend class IndexBuffer;

        ID3D11Buffer* m_pBuffer = nullptr;
        uint32_t m_size = 0;
        bool m_isDynamic = false;
        D3D11_MAPPED_SUBRESOURCE m_mappedIndexBuffer;
        int m_typeSize = 16;
    };
};

#endif

#endif
