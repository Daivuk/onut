// Onut includes
#include <onut/Collider2DComponent.h>
#include <onut/ComponentFactory.h>
#include <onut/Entity.h>
#include <onut/SceneManager.h>
#include <onut/Log.h>
#include <onut/Strings.h>
#include <onut/TiledMap.h>
#include <onut/TiledMapComponent.h>

// Third parties
#include <Box2D/Box2D.h>

#include <onut/SpriteBatch.h>

namespace onut
{
    TiledMapComponent::TiledMapComponent()
        : Component(FLAG_RENDERABLE_2D | FLAG_UPDATABLE)
    {
    }

    TiledMapComponent::~TiledMapComponent()
    {
        destroyCollisions();
    }

    void TiledMapComponent::destroyCollisions()
    {
        if (m_pTiledMap)
        {
            auto pEntity = getEntity();
            auto pSceneManager = pEntity->getSceneManager();
            auto pPhysic = pSceneManager->getPhysic2DWorld();
            auto w = m_pTiledMap->getWidth();
            for (auto pCollisionTile : m_collisionTiles)
            {
                if (!pCollisionTile) continue;
                if (pCollisionTile->pBody)
                {
                    pPhysic->DestroyBody(pCollisionTile->pBody);
                }
                for (int y = pCollisionTile->mapPos.y; y < pCollisionTile->mapPos.y + pCollisionTile->size.y; ++y)
                {
                    for (int x = pCollisionTile->mapPos.x; x < pCollisionTile->mapPos.x + pCollisionTile->size.x; ++x)
                    {
                        m_collisionTiles[y * w + x] = nullptr;
                    }
                }
                delete pCollisionTile;
            }
            m_collisionTiles.clear();
        }
    }

    void TiledMapComponent::createCollisionTiles(const iRect& rect)
    {
        auto pCollisionsLayer = dynamic_cast<OTiledMap::TileLayer*>(m_pTiledMap->getLayer("collisions"));
        if (pCollisionsLayer)
        {
            auto pEntity = getEntity();
            auto pSceneManager = pEntity->getSceneManager();
            auto pPhysic = pSceneManager->getPhysic2DWorld();

            auto tileIds = pCollisionsLayer->tileIds;
            auto w = m_pTiledMap->getWidth();
            for (int y = rect.top; y < rect.bottom; ++y)
            {
                for (int x = rect.left; x < rect.right; ++x)
                {
                    if (!tileIds[y * w + x] || m_collisionTiles[y * w + x]) continue;
                    auto pCollisionTile = m_collisionTiles[y * w + x];
                    if (pCollisionTile) continue;
                    pCollisionTile = new CollisionTile();
                    pCollisionTile->mapPos = Point(x, y);
                    pCollisionTile->size = Point(1, 1);
                    int max = rect.right;
                    int y2;
                    for (y2 = y; y2 < rect.bottom; ++y2)
                    {
                        if (!tileIds[y2 * w + x] || m_collisionTiles[y2 * w + x])
                        {
                            break;
                        }
                        for (int x2 = x; x2 < max; ++x2)
                        {
                            if (!tileIds[y2 * w + x2] || m_collisionTiles[y2 * w + x2])
                            {
                                max = onut::min(x2, max);
                                break;
                            }
                        }
                    }
                    pCollisionTile->size = Point(max - x, y2 - y);
                    for (y2 = y; y2 < y + pCollisionTile->size.y; ++y2)
                    {
                        for (int x2 = x; x2 < x + pCollisionTile->size.x; ++x2)
                        {
                            m_collisionTiles[y2 * w + x2] = pCollisionTile;
                        }
                    }

                    // Create the body
                    b2BodyDef bodyDef;
                    bodyDef.type = b2_staticBody;
                    bodyDef.position.Set((float)pCollisionTile->mapPos.x + (float)pCollisionTile->size.x / 2.0f,
                                         (float)pCollisionTile->mapPos.y + (float)pCollisionTile->size.y / 2.0f);
                    pCollisionTile->pBody = pPhysic->CreateBody(&bodyDef);
                    b2PolygonShape box;
                    box.SetAsBox((float)pCollisionTile->size.x / 2.0f, (float)pCollisionTile->size.y / 2.0f);
                    pCollisionTile->pBody->CreateFixture(&box, 0.0f);
                }
            }
        }
    }

    void TiledMapComponent::setTiledMap(const OTiledMapRef& pTiledMap)
    {
        using ComponentMap = std::unordered_map<std::string, OComponentRef>;

        destroyCollisions();

        m_pTiledMap = pTiledMap;
        if (!m_pTiledMap) return;

        auto pEntity = getEntity();
        auto pSceneManager = pEntity->getSceneManager();
        auto pPhysic = pSceneManager->getPhysic2DWorld();

        // Create collision layer
        m_collisionTiles.assign(m_pTiledMap->getWidth() * m_pTiledMap->getHeight(), nullptr);
        createCollisionTiles({0, 0, m_pTiledMap->getWidth(), m_pTiledMap->getHeight()});

        // Populate with entities
        auto pEntitiesLayer = dynamic_cast<OTiledMap::ObjectLayer*>(pTiledMap->getLayer("entities"));
        if (pEntitiesLayer)
        {
            for (uint32_t i = 0; i < pEntitiesLayer->objectCount; ++i)
            {
                auto& object = pEntitiesLayer->pObjects[i];

                // Create the entity and set it's position
                auto pMapEntity = OEntity::create();
                auto position = object.position + Vector2(object.size / 2.0f);
                pMapEntity->setLocalTransform(Matrix::CreateTranslation(position));
                pMapEntity->setName(object.name);
                oComponentFactory->registerEntity(object.id, pMapEntity);
            }

            for (uint32_t i = 0; i < pEntitiesLayer->objectCount; ++i)
            {
                auto& object = pEntitiesLayer->pObjects[i];
                auto pMapEntity = oComponentFactory->getEntity(object.id);

                // Load his components
                ComponentMap componentMap;
                for (auto& kv : object.properties)
                {
                    if (kv.first == "Static")
                    {
                        pMapEntity->setStatic(kv.second == "true");
                        continue;
                    }
                    else if (kv.first == "Visible")
                    {
                        pMapEntity->setVisible(kv.second == "true");
                        continue;
                    }
                    else if (kv.first == "Enable")
                    {
                        pMapEntity->setEnabled(kv.second == "true");
                        continue;
                    }
                    auto split = onut::splitString(kv.first, ':');
                    if (split.size() == 0) continue;
                    auto& componentName = split[0];
                    auto it = componentMap.find(componentName);
                    OComponentRef pComponent;
                    if (it == componentMap.end())
                    {
                        pComponent = oComponentFactory->instantiate(componentName);
                        if (!pComponent)
                        {
                            OLogW("Component not registered \"" + componentName + "\"");
                            continue;
                        }
                        componentMap[componentName] = pComponent;
                    }
                    else
                    {
                        pComponent = it->second;
                    }
                    if (split.size() == 2)
                    {
                        auto& propertyName = split[1];
                        auto& value = kv.second;
                        oComponentFactory->setProperty(pComponent, componentName, propertyName, value);
                    }
                }
                for (auto& kv : componentMap)
                {
                    pMapEntity->addComponent(kv.second);
                }

                // Add the entity to the map
                pEntity->add(pMapEntity);
            }

            oComponentFactory->clearEntityRegistry();
        }
    }

    const OTiledMapRef& TiledMapComponent::getTiledMap() const
    {
        return m_pTiledMap;
    }

    void TiledMapComponent::onCreate()
    {
    }

    void TiledMapComponent::onUpdate()
    {
        auto& children = getEntity()->getChildren();
        for (auto& pEntityRef : children)
        {
            auto pEntity = pEntityRef.get();
            if (!pEntity->isVisible()) continue;
            int drawIndex = (int)pEntity->getLocalTransform().Translation().y + 1;
            pEntity->setDrawIndex(drawIndex);
        }
    }

    void TiledMapComponent::onRender2d()
    {
        auto& transform = getWorldTransform();
        m_pTiledMap->setTransform(transform);
        m_pTiledMap->render();
    }

    void TiledMapComponent::onAddChild(const OEntityRef& pChild)
    {
        if (m_pTiledMap)
        {
            auto pCollider2DComponent = pChild->getComponent<Collider2DComponent>();
            if (pCollider2DComponent)
            {
                pCollider2DComponent->setPhysicScale((float)m_pTiledMap->getTileSize());
            }
        }
    }

    bool TiledMapComponent::getPassable(const Point& mapPos) const
    {
        auto w = m_pTiledMap->getWidth();
        auto h = m_pTiledMap->getHeight();
        if (mapPos.x < 0 || mapPos.x >= w || mapPos.y < 0 || mapPos.y >= h) return false;
        return m_collisionTiles[mapPos.y * w + mapPos.x] == nullptr;
    }

    void TiledMapComponent::setPassable(const Point& mapPos, bool passable)
    {
        auto w = m_pTiledMap->getWidth();
        auto h = m_pTiledMap->getHeight();
        if (mapPos.x < 0 || mapPos.x >= w || mapPos.y < 0 || mapPos.y >= h) return;
        if (getPassable(mapPos) == passable) return;

        auto pEntity = getEntity();
        auto pSceneManager = pEntity->getSceneManager();
        auto pPhysic = pSceneManager->getPhysic2DWorld();

        auto pCollisionTile = m_collisionTiles[mapPos.y * w + mapPos.x];
        if (pCollisionTile)
        {
            auto pCollisionsLayer = dynamic_cast<OTiledMap::TileLayer*>(m_pTiledMap->getLayer("collisions"));
            if (pCollisionsLayer)
            {
                pCollisionsLayer->tileIds[mapPos.y * w + mapPos.x] = 0;
            }

            iRect rect{
                pCollisionTile->mapPos.x, 
                pCollisionTile->mapPos.y,
                pCollisionTile->mapPos.x + pCollisionTile->size.x, 
                pCollisionTile->mapPos.y + pCollisionTile->size.y
            };

            pPhysic->DestroyBody(pCollisionTile->pBody);
            for (int y = rect.top; y < rect.bottom; ++y)
            {
                for (int x = rect.left; x < rect.right; ++x)
                {
                    m_collisionTiles[y * w + x] = nullptr;
                }
            }
            delete pCollisionTile;

            createCollisionTiles(rect);
        }
        else
        {
            auto pCollisionsLayer = dynamic_cast<OTiledMap::TileLayer*>(m_pTiledMap->getLayer("collisions"));
            if (pCollisionsLayer)
            {
                pCollisionsLayer->tileIds[mapPos.y * w + mapPos.x] = 1;
            }

            // Just add a new one here
            pCollisionTile = new CollisionTile();
            pCollisionTile->mapPos = mapPos;
            pCollisionTile->size = Point(1, 1);
            m_collisionTiles[mapPos.y * w + mapPos.x] = pCollisionTile;

            // Create the body
            b2BodyDef bodyDef;
            bodyDef.type = b2_staticBody;
            bodyDef.position.Set((float)pCollisionTile->mapPos.x + (float)pCollisionTile->size.x / 2.0f,
                                 (float)pCollisionTile->mapPos.y + (float)pCollisionTile->size.y / 2.0f);
            pCollisionTile->pBody = pPhysic->CreateBody(&bodyDef);
            b2PolygonShape box;
            box.SetAsBox((float)pCollisionTile->size.x / 2.0f, (float)pCollisionTile->size.y / 2.0f);
            pCollisionTile->pBody->CreateFixture(&box, 0.0f);
        }
    }
};
