#pragma once

// Onut includes
#include <onut/Component.h>
#include <onut/Maths.h>

// Forward declarations
#include <onut/ForwardDeclaration.h>
OForwardDeclare(SpriteAnim);
OForwardDeclare(SpriteAnimComponent);
OForwardDeclare(SpriteAnimInstance);

namespace onut
{
    class SpriteAnimComponent final : public Component
    {
    public:
        void setSpriteAnim(const OSpriteAnimRef& pSpriteAnim);
        const OSpriteAnimRef& getSpriteAnim() const;

        void setScale(const Vector2& scale);
        const Vector2& getScale() const;

        void setColor(const Color& color);
        const Color& getColor() const;

        void setDefaultAnim(const std::string& anim);
        const std::string& getDefaultAnim() const;

    private:
        void onCreate() override;
        void onRender2d() override;

        OSpriteAnimInstanceRef m_pSpriteAnimInstance;
        Vector2 m_scale = Vector2(1);
        Color m_color = Color::White;
        OSpriteAnimRef m_pSpriteAnim;
        std::string m_defaultAnim;
    };
};
