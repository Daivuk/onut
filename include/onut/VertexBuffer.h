#pragma once
// Onut
#include <onut/Resource.h>

// Third party
#include <onut/ForwardDeclaration.h>
OForwardDeclare(VertexBuffer)

namespace onut
{
    class VertexBuffer : public Resource
    {
    public:
        static OVertexBufferRef createStatic(const void* pVertexData, uint32_t size);
        static OVertexBufferRef createDynamic(uint32_t size);

        virtual ~VertexBuffer();

        virtual void setData(const void* pVertexData, uint32_t size) = 0;
        virtual void* map() = 0;
        virtual void unmap(uint32_t size) = 0;
        virtual uint32_t size() = 0;

    protected:
        VertexBuffer();
    };
};
