#pragma once
#include <onut/Component.h>

class TreasureHunter final : public OComponent
{
private:
    void onTriggerEnter(const OCollider2DComponentRef& pCollider);
};
