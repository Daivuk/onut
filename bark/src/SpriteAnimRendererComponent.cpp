#include <onut/Log.h>
#include <onut/Renderer.h>
#include <onut/SpriteBatch.h>
#include <onut/SpriteAnim.h>
#include "SpriteAnimRendererComponent.h"
#include "Entity.h"

void SpriteAnimRendererComponent::onCreate()
{
    if (!spriteAnim) return;

    spriteAnimInstance = std::make_shared<OSpriteAnimInstance>(spriteAnim);
    spriteAnimInstance->play(anim);
}

void SpriteAnimRendererComponent::render(onut::RenderStates& rs, OSpriteBatch* sb)
{
    if (!spriteAnimInstance) return;

    rs.sampleFiltering = smooth ? OFilterLinear : OFilterNearest;
    rs.blendMode = (onut::BlendMode)blendMode;

    auto s = spriteAnimInstance.get();
    const auto& texture = s->getTexture();
    const auto& origin = s->getOrigin();
    const auto& uvs = s->getUVs();

    sb->drawSpriteWithUVs(texture, entity->getWorldTransform(), uvs, color, origin);
}

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
