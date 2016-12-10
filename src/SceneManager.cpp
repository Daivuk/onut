// onut includes
#include <onut/Collider2DComponent.h>
#include <onut/Camera2DComponent.h>
#include <onut/Component.h>
#include <onut/Entity.h>
#include <onut/Font.h>
#include <onut/SceneManager.h>
#include <onut/Renderer.h>
#include <onut/SpriteBatch.h>
#include <onut/Timing.h>
#include <onut/Updater.h>

// Third parties
#include <Box2D/Box2D.h>

// STL
#include <atomic>

OSceneManagerRef oSceneManager;

namespace onut
{
#if defined(_DEBUG)
    extern std::atomic<int> g_componentCount;
    extern std::atomic<int> g_entityCount;
#endif

    class Physic2DContactListener : public b2ContactListener
    {
    public:
        Physic2DContactListener(SceneManager* pSceneManager)
            : m_pSceneManager(pSceneManager)
        {
        }

    private:
        void BeginContact(b2Contact* contact) override
        {
            m_pSceneManager->begin2DContact(contact);
        }

        void EndContact(b2Contact* contact) override
        {
            m_pSceneManager->end2DContact(contact);
        }

        SceneManager* m_pSceneManager;
    };

    OSceneManagerRef SceneManager::create()
    {
        return std::shared_ptr<SceneManager>(new SceneManager());
    }

    SceneManager::SceneManager()
    {
        m_pUpdater = OUpdater::create();
        m_pPhysic2DWorld = new b2World(b2Vec2(0, 0));
        m_pPhysic2DContactListener = new Physic2DContactListener(this);
        m_pPhysic2DWorld->SetContactListener(m_pPhysic2DContactListener);
        m_pComponentUpdates = new TList<Component>(offsetOf(&Component::m_updateLink));
        m_pComponentRenders = new TList<Component>(offsetOf(&Component::m_renderLink));
        m_pComponentRender2Ds = new TList<Component>(offsetOf(&Component::m_render2DLink));
#if defined(_DEBUG)
        g_componentCount = 0;
        g_entityCount = 0;
#endif
    }

    SceneManager::~SceneManager()
    {
        delete m_pPhysic2DWorld;
        delete m_pPhysic2DContactListener;
    }

    void SceneManager::addEntity(const OEntityRef& pEntity)
    {
        if (pEntity->m_pSceneManager)
        {
            auto pEntityRef = pEntity;
            pEntityRef->m_pSceneManager->removeEntity(pEntityRef);
            pEntityRef->m_pSceneManager = OThis;
            m_entities.insert(pEntityRef);
        }
        else
        {
            pEntity->m_pSceneManager = OThis;
            m_entities.insert(pEntity);
        }
    }

    void SceneManager::removeEntity(const OEntityRef& pEntity)
    {
        auto pRawEntity = pEntity.get();

        // Remove components
        if (pRawEntity->isEnabled() && !pRawEntity->isStatic())
        {
            for (auto& pComponent : pRawEntity->m_components)
            {
                if (pComponent->m_isEnabled && pComponent->m_flags & Component::FLAG_UPDATABLE)
                {
                    addComponentAction(pComponent, ComponentAction::Action::RemoveUpdate);
                }
            }
        }
        if (pRawEntity->isVisible())
        {
            for (auto& pComponent : pRawEntity->m_components)
            {
                if (pComponent->m_isEnabled && pComponent->m_flags & Component::FLAG_RENDERABLE)
                {
                    addComponentAction(pComponent, ComponentAction::Action::RemoveRender);
                }
                if (pComponent->m_isEnabled && pComponent->m_flags & Component::FLAG_RENDERABLE_2D)
                {
                    addComponentAction(pComponent, ComponentAction::Action::RemoveRender2D);
                }
            }
        }

        // Remove entity
        m_entitiesToRemove.push_back(pEntity);
    }

    void SceneManager::addComponentAction(const OComponentRef& pComponent, ComponentAction::Action action)
    {
        m_componentActions.push_back({action, pComponent});
    }

    void SceneManager::performComponentActions()
    {
        for (size_t i = 0; i < m_componentJustCreated.size(); ++i)
        {
            m_componentJustCreated[i]->onCreate();
        }
        m_componentJustCreated.clear();
        for (auto& componentAction : m_componentActions)
        {
            switch (componentAction.action)
            {
                case ComponentAction::Action::AddUpdate:
                    m_pComponentUpdates->InsertTail(componentAction.pComponent.get());
                    break;
                case ComponentAction::Action::RemoveUpdate:
                    componentAction.pComponent->m_updateLink.Unlink();
                    break;
                case ComponentAction::Action::AddRender:
                    m_pComponentRenders->InsertTail(componentAction.pComponent.get());
                    break;
                case ComponentAction::Action::RemoveRender:
                    componentAction.pComponent->m_renderLink.Unlink();
                    break;
                case ComponentAction::Action::AddRender2D:
                {
                    auto drawIndex = componentAction.pComponent->getEntity()->getDrawIndex();
                    Component* pComponent;
                    for (pComponent = m_pComponentRender2Ds->Head(); pComponent; pComponent = pComponent->m_render2DLink.Next())
                    {
                        auto otherDrawIndex = pComponent->getEntity()->getDrawIndex();
                        if (drawIndex < otherDrawIndex)
                        {
                            m_pComponentRender2Ds->InsertBefore(componentAction.pComponent.get(), pComponent);
                            break;
                        }
                    }
                    if (!pComponent) m_pComponentRender2Ds->InsertTail(componentAction.pComponent.get());
                    break;
                }
                case ComponentAction::Action::RemoveRender2D:
                    componentAction.pComponent->m_render2DLink.Unlink();
                    break;
            }
        }
        m_componentActions.clear();
    }

    void SceneManager::setActiveCamera2D(const OCamera2DComponentRef& pActiveCamera2D)
    {
        m_pActiveCamera2D = pActiveCamera2D;
    }

    void SceneManager::performContacts()
    {
        auto contacts = m_contact2Ds;
        m_contact2Ds.clear();
        for (auto& contact2D : contacts)
        {
            switch (contact2D.type)
            {
                case Contact2D::Type::Begin:
                    contact2D.pColliderA->getEntity()->onTriggerEnter(contact2D.pColliderB);
                    contact2D.pColliderB->getEntity()->onTriggerEnter(contact2D.pColliderA);
                    break;
                case Contact2D::Type::End:
                    contact2D.pColliderA->getEntity()->onTriggerLeave(contact2D.pColliderB);
                    contact2D.pColliderB->getEntity()->onTriggerLeave(contact2D.pColliderA);
                    break;
            }
        }
    }

    void SceneManager::performEntityActions()
    {
        for (auto& pEntity : m_entitiesToRemove)
        {
            for (auto& pComponent : pEntity->m_components)
            {
                pComponent->onDestroy();
            }
        }
        for (auto& pEntity : m_entitiesToRemove)
        {
            auto pParent = pEntity->getParent();
            if (pParent)
            {
                pParent->remove(pEntity);
            }
            pEntity->m_components.clear();
            m_entities.erase(pEntity);
        }
        m_entitiesToRemove.clear();
    }

    void SceneManager::update()
    {
        // Update scene updater that managers sprite animations and such
        if (!m_pause) m_pUpdater->update();
        
        // Perform add/remove of entities
        performEntityActions();

        // Perform add/remove of components to various lists
        performComponentActions();

        if (!m_pause)
        {
            // Update physics
            m_pPhysic2DWorld->Step(ODT, 6, 2);

            // Send physic contact messages
            performContacts();

            // Update updatables
            for (auto pComponent = m_pComponentUpdates->Head(); pComponent; pComponent = pComponent->m_updateLink.Next())
            {
                pComponent->onUpdate();
            }

            // Redo the entity/component add/remove actions because they might have changed while updating.
            performComponentActions();
            performEntityActions();
        }
    }

    void SceneManager::render()
    {
        if (m_entities.empty()) return;

#if defined(_DEBUG)
        m_renderCount = 0;
#endif
        for (auto pComponent = m_pComponentRenders->Head(); pComponent; pComponent = pComponent->m_renderLink.Next())
        {
#if defined(_DEBUG)
            ++m_renderCount;
#endif
            pComponent->onRender();
        }

        Matrix transform;
        if (m_pActiveCamera2D)
        {
            if (m_pActiveCamera2D->getClear())
            {
                oRenderer->clear(m_pActiveCamera2D->getClearColor());
            }
            transform = m_pActiveCamera2D->getEntity()->getWorldTransform().Invert();
            transform *= Matrix::CreateScale(m_pActiveCamera2D->getZoom());
            transform *= Matrix::CreateTranslation(OScreenCenterf);
        }
        transform._41 = std::roundf(transform._41);
        transform._42 = std::roundf(transform._42);
        oSpriteBatch->begin(transform);
#if defined(_DEBUG)
        m_render2DCount = 0;
#endif
        for (auto pComponent = m_pComponentRender2Ds->Head(); pComponent; pComponent = pComponent->m_render2DLink.Next())
        {
#if defined(_DEBUG)
            ++m_render2DCount;
#endif
            pComponent->onRender2d();
        }

#if defined(_DEBUG)
        auto pPhysic = getPhysic2DWorld();
        if (pPhysic)
        {
            auto pBody = pPhysic->GetBodyList();
            while (pBody)
            {
                b2AABB aabb;
                aabb.lowerBound = b2Vec2(FLT_MAX, FLT_MAX);
                aabb.upperBound = b2Vec2(-FLT_MAX, -FLT_MAX);
                b2Fixture *pFixture = pBody->GetFixtureList();
                bool isTrigger = false;
                while (pFixture)
                {
                    isTrigger |= pFixture->IsSensor();
                    aabb.Combine(aabb, pFixture->GetAABB(0));
                    pFixture = pFixture->GetNext();
                }

                Color color = Color(.35f, 0, 0, .35f);
                switch (pBody->GetType())
                {
                    case b2BodyType::b2_staticBody:
                        color = Color(.35f, 0, 0, .35f);
                        break;
                    case b2BodyType::b2_dynamicBody:
                    case b2BodyType::b2_kinematicBody:
                        color = Color(0, .35f, 0, .35f);
                        break;
                }
                if (isTrigger) color.b = .35f;
                
                oSpriteBatch->drawOutterOutlineRect(Rect(
                    aabb.lowerBound.x * 16.0f + 1,
                    aabb.lowerBound.y * 16.0f + 1,
                    (aabb.upperBound.x - aabb.lowerBound.x) * 16.0f - 1,
                    (aabb.upperBound.y - aabb.lowerBound.y) * 16.0f - 1),
                    1, color);

                pBody = pBody->GetNext();
            }
        }
#endif

        oSpriteBatch->end();
    }

    void SceneManager::DrawDebugInfo()
    {
#if defined(_DEBUG)
        int updateCount = 0;
        for (auto pComponent = m_pComponentUpdates->Head(); pComponent; pComponent = pComponent->m_updateLink.Next())
        {
            ++updateCount;
        }
        auto pFont = OGetFont("font.fnt");
        if (pFont)
        {
            oSpriteBatch->begin();
            oSpriteBatch->drawRect(nullptr, {0, 16, 200, 100}, Color(0, 0, 0, .75f));
            pFont->draw("Updatables: " + std::to_string(updateCount), {0, 20});
            pFont->draw("Renderables: " + std::to_string(m_renderCount), {0, 40});
            pFont->draw("Renderables 2D: " + std::to_string(m_render2DCount), {0, 60});
            pFont->draw("Components: " + std::to_string(g_componentCount), {0, 80});
            pFont->draw("Entities: " + std::to_string(g_entityCount), {0, 100});
            oSpriteBatch->end();
        }
#endif
    }

    OEntityRef SceneManager::findEntity(const std::string& name) const
    {
        for (auto& pEntity : m_entities)
        {
            if (pEntity->getName() == name) return pEntity;
        }
        return nullptr;
    }

    b2World* SceneManager::getPhysic2DWorld() const
    {
        return m_pPhysic2DWorld;
    }

    void SceneManager::begin2DContact(b2Contact* pContact)
    {
        b2Fixture* pFixtureA = pContact->GetFixtureA();
        b2Fixture* pFixtureB = pContact->GetFixtureB();
        if (pFixtureA == pFixtureB) return;

        // Make sure only one of the fixtures was a sensor
        bool sensorA = pFixtureA->IsSensor();
        bool sensorB = pFixtureB->IsSensor();
        if (!(sensorA ^ sensorB)) return;

        auto* pColliderA = static_cast<Collider2DComponent*>(pFixtureA->GetBody()->GetUserData());
        auto* pColliderB = static_cast<Collider2DComponent*>(pFixtureB->GetBody()->GetUserData());
        if (!pColliderA || !pColliderB) return;

        if (sensorA)
        {
            for (auto& contact2D : m_contact2Ds)
            {
                if (contact2D.pColliderA.get() == pColliderA &&
                    contact2D.pColliderB.get() == pColliderB &&
                    contact2D.type == Contact2D::Type::Begin) return;
            }
            m_contact2Ds.push_back({Contact2D::Type::Begin,
                                    OStaticCast<Collider2DComponent>(pColliderA->shared_from_this()),
                                    OStaticCast<Collider2DComponent>(pColliderB->shared_from_this())});
        }
        else
        {
            for (auto& contact2D : m_contact2Ds)
            {
                if (contact2D.pColliderA.get() == pColliderB &&
                    contact2D.pColliderB.get() == pColliderA &&
                    contact2D.type == Contact2D::Type::Begin) return;
            }
            m_contact2Ds.push_back({Contact2D::Type::Begin,
                                   OStaticCast<Collider2DComponent>(pColliderB->shared_from_this()),
                                   OStaticCast<Collider2DComponent>(pColliderA->shared_from_this())});
        }
    }

    void SceneManager::end2DContact(b2Contact* pContact)
    {
        b2Fixture* pFixtureA = pContact->GetFixtureA();
        b2Fixture* pFixtureB = pContact->GetFixtureB();
        if (pFixtureA == pFixtureB) return;

        // Make sure only one of the fixtures was a sensor
        bool sensorA = pFixtureA->IsSensor();
        bool sensorB = pFixtureB->IsSensor();
        if (!(sensorA ^ sensorB)) return;

        auto* pColliderA = static_cast<Collider2DComponent*>(pFixtureA->GetBody()->GetUserData());
        auto* pColliderB = static_cast<Collider2DComponent*>(pFixtureB->GetBody()->GetUserData());
        if (!pColliderA || !pColliderB) return;

        if (sensorA)
        {
            for (auto& contact2D : m_contact2Ds)
            {
                if (contact2D.pColliderA.get() == pColliderA &&
                    contact2D.pColliderB.get() == pColliderB &&
                    contact2D.type == Contact2D::Type::End) return;
            }
            m_contact2Ds.push_back({Contact2D::Type::End,
                                   OStaticCast<Collider2DComponent>(pColliderA->shared_from_this()),
                                   OStaticCast<Collider2DComponent>(pColliderB->shared_from_this())});
        }
        else
        {
            for (auto& contact2D : m_contact2Ds)
            {
                if (contact2D.pColliderA.get() == pColliderB &&
                    contact2D.pColliderB.get() == pColliderA &&
                    contact2D.type == Contact2D::Type::End) return;
            }
            m_contact2Ds.push_back({Contact2D::Type::End,
                                   OStaticCast<Collider2DComponent>(pColliderB->shared_from_this()),
                                   OStaticCast<Collider2DComponent>(pColliderA->shared_from_this())});
        }
    }

    bool SceneManager::getPause() const
    {
        return m_pause;
    }

    void SceneManager::setPause(bool pause)
    {
        m_pause = pause;
    }

    OUpdaterRef SceneManager::getUpdater() const
    {
        return m_pUpdater;
    }

    void SceneManager::boardcastMessage(int messageId, void* pData)
    {
        for (auto& pEntity : m_entities)
        {
            pEntity->sendMessage(messageId, pData);
        }
    }
};
