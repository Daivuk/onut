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

OSceneManagerRef oSceneManager;

namespace onut
{
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
            }
        }

        // Remove entity
        pRawEntity->m_pSceneManager = nullptr;
        m_entities.erase(pEntity);
    }

    void SceneManager::addComponentAction(const OComponentRef& pComponent, ComponentAction::Action action)
    {
        m_componentActions.push_back({action, pComponent});
    }

    void SceneManager::performComponentActions()
    {
        for (auto& pComponent : m_componentJustCreated)
        {
            pComponent->onCreate();
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
            }
        }
        m_componentActions.clear();
    }

    void SceneManager::addComponent(const OComponentRef& pComponent, Components& to)
    {
        to.push_back(pComponent);
    }

    void SceneManager::removeComponent(const OComponentRef& pComponent, Components& from)
    {
        for (auto it = from.begin(); it != from.end(); ++it)
        {
            if (*it == pComponent)
            {
                from.erase(it);
                return;
            }
        }
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

    void SceneManager::update()
    {
        if (m_pause) return;
        m_pUpdater->update();
        performComponentActions();
        m_pPhysic2DWorld->Step(ODT, 6, 2);
        performContacts();
        for (auto pComponent = m_pComponentUpdates->Head(); pComponent; pComponent = pComponent->m_updateLink.Next())
        {
            pComponent->onUpdate();
        }
        performComponentActions();
    }

    void SceneManager::render()
    {
        int renderCount = 0;
        for (auto pComponent = m_pComponentRenders->Head(); pComponent; pComponent = pComponent->m_renderLink.Next())
        {
            ++renderCount;
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
        oSpriteBatch->begin(transform);
        for (auto& pEntity : m_entities)
        {
            if (!pEntity->getParent() && pEntity->isVisible())
            {
                pEntity->render2d();
            }
        }
        oSpriteBatch->end();

        auto pFont = OGetFont("font.fnt");
        int updateCount = 0;
        for (auto pComponent = m_pComponentUpdates->Head(); pComponent; pComponent = pComponent->m_updateLink.Next())
        {
            ++updateCount;
        }
        pFont->draw("Updatables: " + std::to_string(updateCount), {0, 20});
        pFont->draw("Renderables: " + std::to_string(renderCount), {0, 40});
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

        m_contact2Ds.push_back({Contact2D::Type::Begin,
                               OStaticCast<Collider2DComponent>(pColliderA->shared_from_this()),
                               OStaticCast<Collider2DComponent>(pColliderB->shared_from_this())});
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

        m_contact2Ds.push_back({Contact2D::Type::End,
                               OStaticCast<Collider2DComponent>(pColliderA->shared_from_this()),
                               OStaticCast<Collider2DComponent>(pColliderB->shared_from_this())});
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
};
