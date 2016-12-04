#ifndef PLAYER_H_INCLUDED
#define PLAYER_H_INCLUDED

#include <onut/Component.h>

#include <string>

#include <onut/ForwardDeclaration.h>
OForwardDeclare(Collider2DComponent);
OForwardDeclare(Sound);
OForwardDeclare(SpriteAnimComponent);
ForwardDeclare(Player);

class Player final : public OComponent
{
public:
    Player();

    void onCreate() override;
    void onUpdate() override;

    float getSpeed() const;
    void setSpeed(float speed);

    const OSoundRef& getStepSound() const;
    void setStepSound(const OSoundRef& pSound);

    int getBombCount() const;
    void setBombCount(int count);

    int getCoinCount() const;
    void setCoinCount(int count);

private:
    enum class State
    {
        Idle,
        Moving,
        Attacking
    };

    void onDestroy() override;

    void attack();
    void placeBomb();

    State m_state = State::Idle;
    float m_speed = 1.0f;
    OSoundRef m_pStepSound;
    OSpriteAnimComponentRef m_pSpriteAnimComponent;
    OCollider2DComponentRef m_pCollider2DComponent;
    float m_stepDelay = 0.0f;
    OEntityRef m_pSwordAttackEntity;
    OCollider2DComponentRef m_pSwordCollider;
    int m_bombs = 0;
    int m_coins = 0;
    std::string m_dir = "s";
};

#endif
