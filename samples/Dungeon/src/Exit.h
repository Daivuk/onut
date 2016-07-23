#pragma once
#include <onut/Component.h>

class Exit final : public OComponent
{
private:
    void onTriggerEnter(const OCollider2DComponentRef& pCollider) override;
};
