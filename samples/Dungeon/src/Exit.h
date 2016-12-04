#ifndef EXIT_H_INCLUDED
#define EXIT_H_INCLUDED

#include <onut/Component.h>

class Exit final : public OComponent
{
private:
    void onTriggerEnter(const OCollider2DComponentRef& pCollider) override;
};

#endif
