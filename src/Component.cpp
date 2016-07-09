// Onut includes
#include <onut/Component.h>
#include <onut/Entity.h>

namespace onut
{
    Component::Component()
    {
    }

    Component::~Component()
    {
    }

    const OEntityRef& Component::getEntity() const
    {
        return m_pEntity;
    }
};
