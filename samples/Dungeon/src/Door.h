#pragma once
#include <onut/Component.h>
#include <onut/Point.h>

class Door final : public OComponent
{
public:
    bool getOpen() const;
    void setOpen(bool open);

protected:
    void onCreate() override;
    void onMessage(int messageId, void *pData) override;

private:
    Point m_mapPos;
    bool m_open = false;
};
