#pragma once
#include <onut/Component.h>

#include <onut/ForwardDeclaration.h>
OForwardDeclare(Entity);
OForwardDeclare(SpriteAnimComponent);

class Guard final : public OComponent
{
public:
    Guard();

    const std::string& getDir() const;
    void setDir(const std::string& dir);

private:
    void onCreate() override;
    void onMessage(int messageId, void* pData) override;
    void onUpdate() override;

    std::string m_dir;
    OEntityWeak m_pPlayer;
    OCollider2DComponentRef m_pCollider;
    OSpriteAnimComponentRef m_pSpriteAnimComponent;
    std::string m_resetDir;
};
