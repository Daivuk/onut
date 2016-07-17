#pragma once
#include <onut/Component.h>

class Vase final : public OComponent
{
public:
    int getGold() const;
    void setGold(int gold);

private:
    void onMessage(int messageId, void* pData) override;

    int m_gold = 0;
};
