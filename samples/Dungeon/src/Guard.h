#pragma once
#include <onut/Component.h>

#include <onut/ForwardDeclaration.h>
OForwardDeclare(Entity);

class Guard final : public OComponent
{
public:
    int getLife() const;
    void setLife(int life);

private:
    void onCreate() override;
    void onMessage(int messageId, void* pData) override;

    int m_life = 1;
    OEntityRef m_pPlayer;
};
