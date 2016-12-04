#ifndef TREASUREHUNTER_H_INCLUDED
#define TREASUREHUNTER_H_INCLUDED

#include <onut/Component.h>

class TreasureHunter final : public OComponent
{
private:
    void onTriggerEnter(const OCollider2DComponentRef& pCollider);
};

#endif
