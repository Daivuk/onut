#include <onut/Component.h>

#include <onut/ForwardDeclaration.h>
OForwardDeclare(Entity);

class Player final : public OComponent
{
public:
private:
    void onCreate() override;

    OEntityRef m_pSwordAttack;
};
