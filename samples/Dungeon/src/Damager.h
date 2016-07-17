#pragma once
#include <onut/Component.h>

class Damager final : public OComponent
{
public:
    int getDamage() const;
    void setDamage(int damage);

private:
    void onTriggerEnter(const OCollider2DComponentRef& pCollider) override;

    int m_damage = 0;
};
