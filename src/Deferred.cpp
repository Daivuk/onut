// Onut
#include <onut/Deferred.h>

ODeferredRef oDeferred;

namespace onut
{
    ODeferredRef Deferred::create()
    {
        return ODeferredRef(new Deferred());
    }

    Deferred::~Deferred()
    {
    }

    Deferred::Deferred()
    {
    }
};
