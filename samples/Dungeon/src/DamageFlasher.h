#pragma once
#include <onut/Component.h>

#include <onut/ForwardDeclaration.h>
OForwardDeclare(SpriteAnimComponent);
OForwardDeclare(SpriteComponent);

class DamageFlasher final : public OComponent
{
public:
    DamageFlasher();

private:
    void onCreate() override;
    void onMessage(int messageId, void* pData) override;
    void onUpdate() override;

    float m_time = 0.0;

    OSpriteAnimComponentRef m_pSpriteAnimComponent;
    OSpriteComponentRef m_pSpriteComponent;
};
