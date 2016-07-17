// onut includes
#include <onut/Entity.h>
#include <onut/Font.h>
#include <onut/SpriteBatch.h>
#include <onut/TextComponent.h>

namespace onut
{
    TextComponent::TextComponent()
        : Component(FLAG_RENDERABLE_2D)
    {
    }

    void TextComponent::setFont(const OFontRef& pFont)
    {
        m_pFont = pFont;
    }

    const OFontRef& TextComponent::getFont() const
    {
        return m_pFont;
    }

    void TextComponent::setText(const std::string& text)
    {
        m_text = text;
    }

    const std::string& TextComponent::getText() const
    {
        return m_text;
    }

    void TextComponent::setColor(const Color& color)
    {
        m_color = color;
    }

    const Color& TextComponent::getColor() const
    {
        return m_color;
    }

    void TextComponent::setOrigin(const Vector2& origin)
    {
        m_origin = origin;
    }

    const Vector2& TextComponent::getOrigin() const
    {
        return m_origin;
    }

    void TextComponent::onRender2d()
    {
        auto& transform = getEntity()->getWorldTransform();
        oSpriteBatch->end();
        oSpriteBatch->begin(transform);
        m_pFont->draw(m_text, Vector2::Zero, m_origin, m_color);
        oSpriteBatch->end();
        oSpriteBatch->begin();
    }
};
