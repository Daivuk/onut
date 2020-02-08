#ifndef DEFERRED_H_INCLUDED
#define DEFERRED_H_INCLUDED

// Onut
#include <onut/BlendMode.h>
#include <onut/Maths.h>

// STL
#include <vector>

// Forward
#include <onut/ForwardDeclaration.h>
OForwardDeclare(Deferred)
OForwardDeclare(IndexBuffer)
OForwardDeclare(Model)
OForwardDeclare(Shader)
OForwardDeclare(Texture)
OForwardDeclare(VertexBuffer)

namespace onut
{
    class Deferred final
    {
    public:
        static ODeferredRef create();

        virtual ~Deferred();

    private:
        Deferred();
    };
};

extern ODeferredRef oDeferred;

#endif
