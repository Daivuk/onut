#ifndef INDEXBUFFER_H_INCLUDED
#define INDEXBUFFER_H_INCLUDED

// Onut
#include <onut/Resource.h>

// Forward Declarations
#include <onut/ForwardDeclaration.h>
OForwardDeclare(IndexBuffer)

namespace onut
{
    class IndexBuffer : public Resource
    {
    public:
        static OIndexBufferRef createStatic(const void* pIndexData, uint32_t size, int typeSize = 16);
        static OIndexBufferRef createDynamic(uint32_t size);

        virtual ~IndexBuffer();

        virtual void setData(const void* pIndexData, uint32_t size, int typeSize = 16) = 0;
        virtual void* map() = 0;
        virtual void unmap(uint32_t size) = 0;
        virtual uint32_t size() = 0;

    protected:
        IndexBuffer();
    };
};

#endif
