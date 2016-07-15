// Onut includes
#include <onut/TiledMap.h>
#include <onut/TiledMapColliderComponent.h>

namespace onut
{
    bool TiledMapColliderComponent::getCollideWithTiles() const
    {
        return m_collideWithTiles;
    }

    void TiledMapColliderComponent::setCollideWithTiles(bool collideWithTiles)
    {
        m_collideWithTiles = collideWithTiles;
    }

    const Vector2& TiledMapColliderComponent::getSize() const
    {
        return m_size;
    }

    void TiledMapColliderComponent::setSize(const Vector2& size)
    {
        m_size = size;
    }

    void TiledMapColliderComponent::setTiledMap(const OTiledMapRef& pTiledMap)
    {
        m_pTiledMap = pTiledMap;
    }

    const OTiledMapRef& TiledMapColliderComponent::getTiledMap() const
    {
        return m_pTiledMap;
    }
};
