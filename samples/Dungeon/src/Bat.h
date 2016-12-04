#ifndef BAT_H_INCLUDED
#define BAT_H_INCLUDED

#include <onut/Component.h>

#include <onut/ForwardDeclaration.h>
OForwardDeclare(SpriteAnimComponent);

class Bat final : public OComponent
{
public:
    Bat();

private:
    enum class State
    {
        Follow,
        Attacking,
        Dying
    };

    void onCreate() override;
    void onMessage(int messageId, void* pData) override;
    void onUpdate() override;
    void onDestroy() override;

    void followPlayer();

    State m_state = State::Follow;
    OCollider2DComponentRef m_pCollider;
    OSpriteAnimComponentRef m_pSpriteAnimComponent;
    float m_attackDelay = 0.0f;
};

#endif
