#include <onut/Component.h>

#include <string>

#include <onut/ForwardDeclaration.h>
OForwardDeclare(SpriteAnimComponent);
ForwardDeclare(Controllable);

class Controllable final : public OComponent
{
public:
    void onCreate() override;
    void onUpdate() override;

    float getSpeed() const;
    void setSpeed(float speed);

private:
    float m_speed = 1.0f;
    OSpriteAnimComponentRef m_pSpriteAnimComponent;
    std::string m_dir = "s";
};
