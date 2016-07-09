// Onut includes
#include <onut/Entity.h>
#include <onut/TiledMap.h>
#include <onut/TiledMapComponent.h>

namespace onut
{
    void TiledMapComponent::setTiledMap(const OTiledMapRef& pTiledMap)
    {
        m_pTiledMap = pTiledMap;
    }

    const OTiledMapRef& TiledMapComponent::getTiledMap() const
    {
        return m_pTiledMap;
    }

    void TiledMapComponent::render2d()
    {
        auto& transform = getEntity()->getWorldTransform();
        m_pTiledMap->setTransform(transform);
        m_pTiledMap->render();
    }
};
