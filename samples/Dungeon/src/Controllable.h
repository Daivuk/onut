#pragma once
#include <onut/Component.h>

#include <string>

#include <onut/ForwardDeclaration.h>
OForwardDeclare(Collider2DComponent);
OForwardDeclare(Sound);
OForwardDeclare(SpriteAnimComponent);
ForwardDeclare(Controllable);

class Controllable final : public OComponent
{
public:
    Controllable();

    void onCreate() override;
    void onUpdate() override;

    float getSpeed() const;
    void setSpeed(float speed);

    const OSoundRef& getStepSound() const;
    void setStepSound(const OSoundRef& pSound);

private:
    float m_speed = 1.0f;
    OSoundRef m_pStepSound;
    OSpriteAnimComponentRef m_pSpriteAnimComponent;
    OCollider2DComponentRef m_pCollider2DComponent;
    float m_stepDelay = 0.0f;
    std::string m_dir = "s";
};
