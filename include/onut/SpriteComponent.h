#ifndef SPRITECOMPONENT_H_INCLUDED
#define SPRITECOMPONENT_H_INCLUDED


// Onut includes
#include <onut/Component.h>
#include <onut/Maths.h>

// Forward declarations
#include <onut/ForwardDeclaration.h>
OForwardDeclare(SpriteComponent);
OForwardDeclare(Texture);

namespace onut
{
    class SpriteComponent final : public Component
    {
    public:
        SpriteComponent();

        void setTexture(const OTextureRef& pTexture);
        const OTextureRef& getTexture() const;

        void setScale(const Vector2& scale);
        const Vector2& getScale() const;

        void setColor(const Color& color);
        const Color& getColor() const;

        void setOrigin(const Vector2& origin);
        const Vector2& getOrigin() const;

    private:
        void onRender2d() override;

        OTextureRef m_pTexture;
        Vector2 m_scale = Vector2(1);
        Color m_color = Color::White;
        Vector2 m_origin = OCenter;
    };
};

#endif
