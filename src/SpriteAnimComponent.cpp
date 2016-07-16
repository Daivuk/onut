// onut includes
#include <onut/Entity.h>
#include <onut/EntityManager.h>
#include <onut/SpriteAnim.h>
#include <onut/SpriteAnimComponent.h>
#include <onut/SpriteBatch.h>
#include <onut/Texture.h>
#include <onut/Updater.h>

namespace onut
{
    void SpriteAnimComponent::setSpriteAnim(const OSpriteAnimRef& pSpriteAnim)
    {
        m_pSpriteAnim = pSpriteAnim;
        m_pSpriteAnimInstance = OMake<OSpriteAnimInstance>(m_pSpriteAnim);
        m_pSpriteAnimInstance->setUpdater(getEntity()->getEntityManager()->getUpdater());
    }

    const OSpriteAnimRef& SpriteAnimComponent::getSpriteAnim() const
    {
        return m_pSpriteAnim;
    }

    void SpriteAnimComponent::setScale(const Vector2& scale)
    {
        m_scale = scale;
    }

    const Vector2& SpriteAnimComponent::getScale() const
    {
        return m_scale;
    }

    void SpriteAnimComponent::setColor(const Color& color)
    {
        m_color = color;
    }

    const Color& SpriteAnimComponent::getColor() const
    {
        return m_color;
    }

    void SpriteAnimComponent::setDefaultAnim(const std::string& anim)
    {
        m_defaultAnim = anim;
    }

    const std::string& SpriteAnimComponent::getDefaultAnim() const
    {
        return m_defaultAnim;
    }

    void SpriteAnimComponent::onCreate()
    {
        if (isEnabled())
        {
            m_currentlyPlaying = m_defaultAnim;
            m_pSpriteAnimInstance->play(m_defaultAnim);
        }
    }

    void SpriteAnimComponent::onRender2d()
    {
        if (!m_pSpriteAnimInstance) return;

        auto& pTexture = m_pSpriteAnimInstance->getTexture();
        if (!pTexture) return;
        auto& uvs = m_pSpriteAnimInstance->getUVs();
        auto& origin = m_pSpriteAnimInstance->getOrigin();

        auto& transform = getEntity()->getWorldTransform();
        oSpriteBatch->drawSpriteWithUVs(pTexture, transform, Vector2(m_scale), uvs, m_color, origin);
    }

    void SpriteAnimComponent::play(const std::string& animName, float framePerSecond)
    {
        if (m_pSpriteAnimInstance)
        {
            if (m_currentlyPlaying == animName) return;
            m_currentlyPlaying = animName;
            m_pSpriteAnimInstance->play(animName, framePerSecond);
        }
    }

    void SpriteAnimComponent::playBackward(const std::string& animName, float framePerSecond)
    {
        if (m_pSpriteAnimInstance)
        {
            if (m_currentlyPlaying == animName + "REVERSE") return;
            m_currentlyPlaying = animName + "REVERSE";
            m_pSpriteAnimInstance->playBackward(animName, framePerSecond);
        }
    }

    void SpriteAnimComponent::queueAnim(const std::string& animName)
    {
        if (m_pSpriteAnimInstance)
        {
            m_pSpriteAnimInstance->queueAnim(animName);
        }
    }

    void SpriteAnimComponent::stop(bool reset)
    {
        if (m_pSpriteAnimInstance)
        {
            m_currentlyPlaying = "";
            m_pSpriteAnimInstance->stop(reset);
        }
    }

    bool SpriteAnimComponent::isPlaying() const
    {
        if (m_pSpriteAnimInstance) return m_pSpriteAnimInstance->isPlaying();
        return false;
    }
};
