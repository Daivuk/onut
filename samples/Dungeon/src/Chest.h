#pragma once
#include <onut/Component.h>

#include <onut/ForwardDeclaration.h>
OForwardDeclare(Sound);

class Chest final : public OComponent
{
public:
    int getGold() const;
    void setGold(int gold);

    void open();

private:
    OSoundRef m_pOpenSound;
    int m_gold = 0;
};
