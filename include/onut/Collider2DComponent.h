#ifndef COLLIDER2DCOMPONENT_H_INCLUDED
#define COLLIDER2DCOMPONENT_H_INCLUDED


// Onut includes
#include <onut/Component.h>
#include <onut/Maths.h>

// Forward declarations
#include <onut/ForwardDeclaration.h>
OForwardDeclare(Collider2DComponent);
class b2Body;

namespace onut
{
    class Collider2DComponent final : public Component
    {
    public:
        Collider2DComponent();
        ~Collider2DComponent();

        const Vector2& getSize() const;
        void setSize(const Vector2& size);

        bool getTrigger() const;
        void setTrigger(bool trigger);

        float getPhysicScale() const;
        void setPhysicScale(float scale);

        const Vector2& getVelocity() const;
        void setVelocity(const Vector2& velocity);

        void teleport(const Vector2& position);

    private:
        void onCreate() override;
        void onUpdate() override;
        void onEnable() override;
        void onDisable() override;

        void createBody();
        void destroyBody();

        bool m_trigger = false;
        Vector2 m_size = Vector2::One;
        float m_physicScale = 1.0f;
        b2Body* m_pBody = nullptr;
        Vector2 m_velocity;
    };
};

#endif
