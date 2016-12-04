#ifndef DAMAGER_H_INCLUDED
#define DAMAGER_H_INCLUDED

#include <onut/Component.h>

#include <vector>

class Damager final : public OComponent
{
public:
    struct Message
    {
        int damage;
        Vector2 direction;
    };

    int getDamage() const;
    void setDamage(int damage);

    void addExclusion(const OEntityRef& pEntity);

private:
    void onTriggerEnter(const OCollider2DComponentRef& pCollider) override;

    int m_damage = 0;
    std::vector<OEntityWeak> m_exclusions;
};

#endif
