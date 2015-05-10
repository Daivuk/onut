#include "object.h"

namespace onut
{
    void Object::retain()
    {
        ++m_refCount;
    }

    void Object::release()
    {
        --m_refCount;
        if (m_refCount <= 0)
        {
            delete this;
        }
    }
};
