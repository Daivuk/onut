#ifndef CHEST_H_INCLUDED
#define CHEST_H_INCLUDED

#include <onut/Component.h>

#include <onut/ForwardDeclaration.h>
OForwardDeclare(Sound);

class Chest final : public OComponent
{
public:
    int getGold() const;
    void setGold(int gold);

    int getBomb() const;
    void setBomb(int bomb);

    void open();

private:
    OSoundRef m_pOpenSound;
    int m_gold = 0;
    int m_bomb = 0;
};

#endif
