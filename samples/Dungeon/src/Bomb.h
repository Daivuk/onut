#ifndef BOMB_H_INCLUDED
#define BOMB_H_INCLUDED

#include <onut/Component.h>

#include <onut/ForwardDeclaration.h>
OForwardDeclare(SpriteAnim);

class Bomb final : public OComponent
{
public:
    Bomb();

private:
    void onCreate() override;
    void onUpdate() override;

    OSpriteAnimRef m_pSpriteAnim;
    float m_progress = 0.0f;
    bool m_dieNextFrame = false;
};

#endif
