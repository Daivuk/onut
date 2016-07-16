// Onut includes
#include <onut/Collider2DComponent.h>
#include <onut/Entity.h>
#include <onut/EntityManager.h>
#include <onut/Timing.h>

// Third parties
#include <Box2D/Box2D.h>

namespace onut
{
    Collider2DComponent::~Collider2DComponent()
    {
        if (m_pBody)
        {
            auto& pEntity = getEntity();
            if (pEntity)
            {
                auto& pEntityManager = pEntity->getEntityManager();
                if (pEntityManager)
                {
                    auto pPhysic = pEntityManager->getPhysic2DWorld();
                    if (pPhysic)
                    {
                        pPhysic->DestroyBody(m_pBody);
                    }
                }
            }
        }
    }

    const Vector2& Collider2DComponent::getSize() const
    {
        return m_size;
    }

    void Collider2DComponent::setSize(const Vector2& size)
    {
        m_size = size;
    }

    bool Collider2DComponent::getTrigger() const
    {
        return m_trigger;
    }

    void Collider2DComponent::setTrigger(bool trigger)
    {
        m_trigger = trigger;
    }

    float Collider2DComponent::getPhysicScale() const
    {
        return m_physicScale;
    }

    void Collider2DComponent::setPhysicScale(float scale)
    {
        m_physicScale = scale;
    }

    const Vector2& Collider2DComponent::getVelocity() const
    {
        return m_velocity;
    }

    void Collider2DComponent::setVelocity(const Vector2& velocity)
    {
        m_velocity = velocity;
    }

    void Collider2DComponent::onCreate()
    {
        auto& pEntity = getEntity();
        auto& pEntityManager = pEntity->getEntityManager();
        auto pPhysic = pEntityManager->getPhysic2DWorld();
        Vector2 pos = pEntity->getWorldTransform().Translation();

        b2BodyDef bodyDef;
        bodyDef.type = pEntity->isStatic() ? b2_staticBody : (m_trigger ? b2_kinematicBody : b2_dynamicBody);
        bodyDef.position.Set(pos.x / m_physicScale, pos.y / m_physicScale);

        m_pBody = pPhysic->CreateBody(&bodyDef);

        b2PolygonShape box;
        box.SetAsBox(m_size.x / 2.f / m_physicScale, m_size.y / 2.f / m_physicScale);

        if (pEntity->isStatic())
        {
            m_pBody->CreateFixture(&box, 0.0f);
        }
        else if (m_trigger)
        {
            b2FixtureDef fixtureDef;
            fixtureDef.shape = &box;
            fixtureDef.isSensor = true;
            m_pBody->CreateFixture(&fixtureDef);
        }
        else
        {
            b2FixtureDef fixtureDef;
            fixtureDef.shape = &box;
            fixtureDef.friction = 0.0f;
            fixtureDef.density = 1.0f;
            m_pBody->CreateFixture(&fixtureDef);
        }
    }

    void Collider2DComponent::onUpdate()
    {
        if (m_pBody)
        {
            if (m_trigger)
            {
                Vector2 position = getEntity()->getLocalTransform().Translation();
                m_pBody->SetTransform(b2Vec2(position.x / m_physicScale, position.y / m_physicScale), 0.0f);
            }
            else
            {
                auto b2Position = m_pBody->GetPosition();
                Vector2 currentPosition(b2Position.x * m_physicScale, b2Position.y * m_physicScale);
                getEntity()->setLocalTransform(Matrix::CreateTranslation(currentPosition));
                Vector2 vel = m_velocity / m_physicScale;

                m_pBody->SetTransform(b2Position, 0.0f);
                m_pBody->SetAngularVelocity(0.0f);
                m_pBody->SetLinearVelocity(b2Vec2(vel.x, vel.y));
            }
        }
    }
};
