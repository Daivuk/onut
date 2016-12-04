#ifndef LIFE_H_INCLUDED
#define LIFE_H_INCLUDED

#include <onut/Component.h>

#include <onut/ForwardDeclaration.h>
OForwardDeclare(Sound);

class Life final : public OComponent
{
public:
    int getAmount() const;
    void setAmount(int amount);

    OSoundRef getDamageSound() const;
    void setDamageSound(const OSoundRef& pDamageSound);

private:
    void onMessage(int messageId, void* pData) override;

    int m_amount = 1;
    OSoundRef m_pDamageSound;
};

#endif
