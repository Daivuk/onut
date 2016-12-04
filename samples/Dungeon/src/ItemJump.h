#ifndef ITEMJUMP_H_INCLUDED
#define ITEMJUMP_H_INCLUDED

#include <onut/Anim.h>
#include <onut/Component.h>

class ItemJump final : public OComponent
{
public:
    ItemJump();

private:
    void onCreate() override;
    void onUpdate() override;

    OAnimVector2 m_anim;
};

#endif
