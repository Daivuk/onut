#ifndef GUARD_H_INCLUDED
#define GUARD_H_INCLUDED

#include <onut/Component.h>

#include <onut/ForwardDeclaration.h>
OForwardDeclare(Entity);
OForwardDeclare(SpriteAnimComponent);

class Guard final : public OComponent
{
public:
    Guard();

    const std::string& getDir() const;
    void setDir(const std::string& dir);

private:
    enum class State
    {
        Follow,
        PrepareAttack,
        Attacking
    };

    void onCreate() override;
    void onMessage(int messageId, void* pData) override;
    void onUpdate() override;
    void onDestroy() override;

    void attack();
    void followPlayer();
    void prepareAttack();

    State m_state = State::Follow;
    std::string m_dir;
    float m_prepareAttackDelay = 0.0f;
    OCollider2DComponentRef m_pCollider;
    OSpriteAnimComponentRef m_pSpriteAnimComponent;
    OEntityRef m_pSwordAttackEntity;
    OCollider2DComponentRef m_pSwordCollider;
    std::string m_resetDir;
};

#endif
