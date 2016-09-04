#include "ItemJump.h"

#include <onut/Random.h>
#include <onut/SceneManager.h>

ItemJump::ItemJump()
    : OComponent(FLAG_UPDATABLE)
{
}

void ItemJump::onUpdate()
{
    setLocalTransform(Matrix::CreateTranslation(m_anim.get()));
}

void ItemJump::onCreate()
{
    Vector2 position = getLocalTransform().Translation();
    Vector2 target = onut::randCircle(position, 16.0f);

    m_anim.setUpdater(getSceneManager()->getUpdater());

    m_anim.play(position, Vector2((position + target) / 2.0f) + Vector2{0, -24.0f}, .35f, OTweenEaseOut);
    m_anim.queue(target, .35f, OTweenEaseIn);
    m_anim.queue(target + Vector2{0, -16.0f}, .25f, OTweenEaseOut);
    m_anim.queue(target, .5f, OTweenBounceOut, [this]{ destroy(); });
}
