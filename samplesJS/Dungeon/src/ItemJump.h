#pragma once
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
