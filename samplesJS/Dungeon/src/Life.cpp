#include "Defines.h"
#include "Life.h"

#include <onut/Sound.h>

int Life::getAmount() const
{
    return m_amount;
}

void Life::setAmount(int amount)
{
    m_amount = amount;
}

OSoundRef Life::getDamageSound() const
{
    return m_pDamageSound;
}

void Life::setDamageSound(const OSoundRef& pDamageSound)
{
    m_pDamageSound = pDamageSound;
}

void Life::onMessage(int messageId, void* pData)
{
    if (messageId == Messages::Damage)
    {
        int damageAmount = *(int*)pData;
        m_amount -= damageAmount;
        if (m_pDamageSound) m_pDamageSound->play(0.5f);
        if (m_amount <= 0)
        {
            m_amount = 0;
            destroy();
        }
    }
}
