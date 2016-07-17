#pragma once
#include <onut/Component.h>

#include <string>

#include <onut/ForwardDeclaration.h>
OForwardDeclare(Collider2DComponent);
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

private:
    float m_speed = 1.0f;
    OSpriteAnimComponentRef m_pSpriteAnimComponent;
    OCollider2DComponentRef m_pCollider2DComponent;
    std::string m_dir = "s";
};
