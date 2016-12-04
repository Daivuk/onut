#ifndef PUSHBACK_H_INCLUDED
#define PUSHBACK_H_INCLUDED

#include <onut/Component.h>

class PushBack final : public OComponent
{
public:
    PushBack();

    float getStrength() const;
    void setStrength(float strength);

private:
    void onCreate() override;
    void onMessage(int messageId, void* pData) override;
    void onUpdate() override;

    float m_strength = 16.0f;
    float m_time = 0.0;
    Vector2 m_direction;
    OCollider2DComponentRef m_pCollider;
};

#endif
