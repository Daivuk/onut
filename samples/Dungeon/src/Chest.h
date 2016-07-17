#pragma once
#include <onut/Component.h>

class Chest final : public OComponent
{
public:
    int getGold() const;
    void setGold(int gold);

    void open();

private:
    int m_gold = 0;
};
