#pragma once
#include <onut/Component.h>

#include <onut/ForwardDeclaration.h>
OForwardDeclare(Entity);

class Guard final : public OComponent
{
public:
    const std::string& getDir() const;
    void setDir(const std::string& dir);

private:
    void onCreate() override;
    void onMessage(int messageId, void* pData) override;
    void onUpdate() override;

    OEntityWeak m_pPlayer;
    std::string m_dir;
    std::string m_resetDir;
};
