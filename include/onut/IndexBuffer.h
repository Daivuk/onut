#pragma once
// Onut
#include <onut/Resource.h>

// Third party
#include <onut/ForwardDeclaration.h>
OForwardDeclare(IndexBuffer)

namespace onut
{
    class IndexBuffer : public Resource
    {
    public:
        static OIndexBufferRef createStatic(const void* pIndexData, uint32_t size);
        static OIndexBufferRef createDynamic(uint32_t size);

        virtual ~IndexBuffer();

        virtual void setData(const void* pIndexData, uint32_t size) = 0;
        virtual void* map() = 0;
        virtual void unmap(uint32_t size) = 0;
        virtual uint32_t size() = 0;

    protected:
        IndexBuffer();
    };
};
