// onut includes
#include <onut/Entity.h>
#include <onut/SpriteBatch.h>
#include <onut/SpriteComponent.h>
#include <onut/Texture.h>

namespace onut
{
    SpriteComponent::SpriteComponent()
        : Component(FLAG_RENDERABLE_2D)
    {
    }

    void SpriteComponent::setTexture(const OTextureRef& pTexture)
    {
        m_pTexture = pTexture;
    }

    const OTextureRef& SpriteComponent::getTexture() const
    {
        return m_pTexture;
    }

    void SpriteComponent::setScale(const Vector2& scale)
    {
        m_scale = scale;
    }

    const Vector2& SpriteComponent::getScale() const
    {
        return m_scale;
    }

    void SpriteComponent::setColor(const Color& color)
    {
        m_color = color;
    }

    const Color& SpriteComponent::getColor() const
    {
        return m_color;
    }

    void SpriteComponent::setOrigin(const Vector2& origin)
    {
        m_origin = origin;
    }

    const Vector2& SpriteComponent::getOrigin() const
    {
        return m_origin;
    }

    void SpriteComponent::onRender2d()
    {
        auto& transform = getEntity()->getWorldTransform();
        oSpriteBatch->drawSprite(m_pTexture, transform, Vector2(m_scale), m_color, m_origin);
    }
};
