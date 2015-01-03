#include "Anim.h"

namespace onut
{
    AnimManager AnimManager::s_globalInstance;

    void AnimManager::update()
    {
        m_isUpdating = true;
        for (m_updateIndex = 0; m_updateIndex < m_anims.size();)
        {
            auto pAnim = m_anims[m_updateIndex];
            ++m_updateIndex;
            if (pAnim)
            {
                pAnim->updateAnim();
            }
        }
        m_isUpdating = false;

        for (auto index : m_scheduledForRemoval)
        {
            m_anims.erase(m_anims.begin() + index);
        }
        m_scheduledForRemoval.clear();
    }

    void AnimManager::registerAnim(IAnim* pAnim)
    {
        m_anims.push_back(pAnim);
    }

    void AnimManager::unregisterAnim(IAnim* in_pAnim)
    {
        for (std::vector<IAnim*>::size_type i = 0; i < m_anims.size(); ++i)
        {
            auto pAnim = m_anims[i];
            if (pAnim == in_pAnim)
            {
                if (m_isUpdating)
                {
                    m_anims[i] = nullptr;
                    m_scheduledForRemoval.insert(i);
                }
                else
                {
                    m_anims.erase(m_anims.begin() + i);
                }
                return;
            }
        }
    }

    void AnimManager::stopAllAnims(bool goToEnd)
    {
        for (auto it = m_anims.begin(); it != m_anims.end(); ++it)
        {
            auto pAnim = *it;
            pAnim->stopButDontUnregister(goToEnd);
        }
        m_anims.clear();
        m_updateIndex = 0;
    }

    TweenType invertTween(TweenType tween)
    {
        switch (tween)
        {
            case TweenType::NONE:
                return TweenType::NONE;
            case TweenType::LINEAR:
                return TweenType::LINEAR;
            case TweenType::EASE_IN:
                return TweenType::EASE_OUT;
            case TweenType::EASE_OUT:
                return TweenType::EASE_IN;
            case TweenType::EASE_BOTH:
                return TweenType::EASE_BOTH;
            case TweenType::BOUNCE_IN:
                return TweenType::BOUNCE_OUT;
            case TweenType::BOUNCE_OUT:
                return TweenType::BOUNCE_IN;
            case TweenType::SPRING_IN:
                return TweenType::SPRING_OUT;
            case TweenType::SPRING_OUT:
                return TweenType::SPRING_IN;
        }
        return TweenType::NONE;
    }
}
