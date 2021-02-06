#include <onut/Log.h>
#include <onut/Renderer.h>
#include <onut/SpriteBatch.h>
#include <onut/SpriteAnim.h>
#include "SpriteAnimRendererComponent.h"
#include "Entity.h"

void SpriteAnimRendererComponent::onCreate()
{
    if (!spriteAnim) return;

#if !BARK_EDITOR
    spriteAnimInstance = std::make_shared<OSpriteAnimInstance>(spriteAnim);
    spriteAnimInstance->play(anim);
#endif
}

void SpriteAnimRendererComponent::render(onut::RenderStates& rs, OSpriteBatch* sb)
{
#if !BARK_EDITOR
    if (!spriteAnimInstance) return;
#endif

    rs.sampleFiltering = smooth ? OFilterLinear : OFilterNearest;
    rs.blendMode = (onut::BlendMode)blendMode;

#if BARK_EDITOR
    // In editor mode, we draw the first frame of the selected anim.
    // Or first frame of the first anim if no anim is selected
    auto oanim = spriteAnim->getAnim(anim);
    if (!oanim)
    {
        const auto& anims = spriteAnim->getAnimNames();
        if (!anims.empty()) oanim = spriteAnim->getAnim(anims.front());
    }
    if (oanim && !oanim->frames.empty())
    {
        const auto& first_frame = oanim->frames.front();
        sb->drawSpriteWithUVs(first_frame.pTexture, 
                              entity->getWorldTransform(), 
                              first_frame.UVs, 
                              color,
                              first_frame.origin);
    }
#else // !BARK_EDITOR
    auto s = spriteAnimInstance.get();
    const auto& texture = s->getTexture();
    const auto& origin = s->getOrigin();
    const auto& uvs = s->getUVs();

    sb->drawSpriteWithUVs(texture, entity->getWorldTransform(), uvs, color, origin);
#endif
}

#if !BARK_EDITOR
void SpriteAnimRendererComponent::set_spriteAnim(const OSpriteAnimRef& value)
{
    if (spriteAnim == value) return;

    spriteAnim = value;

    spriteAnimInstance = std::make_shared<OSpriteAnimInstance>(spriteAnim);
    spriteAnimInstance->play(anim);
}

void SpriteAnimRendererComponent::set_anim(const std::string& value)
{
    if (anim == value) return;

    anim = value;

    if (!spriteAnimInstance) return;
    spriteAnimInstance->play(anim);
}
#endif
