#ifndef TEXTCOMPONENT_H_INCLUDED
#define TEXTCOMPONENT_H_INCLUDED


// Onut includes
#include <onut/Component.h>
#include <onut/Maths.h>

// Forward declarations
#include <onut/ForwardDeclaration.h>
OForwardDeclare(Font);
OForwardDeclare(TextComponent);

namespace onut
{
    class TextComponent final : public Component
    {
    public:
        TextComponent();

        void setFont(const OFontRef& pFont);
        const OFontRef& getFont() const;

        void setText(const std::string& text);
        const std::string& getText() const;

        void setColor(const Color& color);
        const Color& getColor() const;

        void setOrigin(const Vector2& origin);
        const Vector2& getOrigin() const;

    private:
        void onRender2d() override;

        OFontRef m_pFont;
        std::string m_text;
        Color m_color = Color::White;
        Vector2 m_origin = OCenter;
    };
};

#endif
