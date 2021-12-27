#include <onut/Log.h>
#include <onut/Renderer.h>
#include <onut/SpriteBatch.h>
#include <onut/SpriteAnim.h>
#include <onut/Texture.h>
#include "SpriteAnimRendererComponent.h"
#include "Entity.h"
#include "TransformHelper.h"
#if BARK_EDITOR
#include "Gizmo2DContext.h"
#endif

#if BARK_EDITOR
void SpriteAnimRendererComponent::onEnable()
{
    _2DRendererComponent::onEnable();

    enableGizmo(entity);
}
#endif

#if BARK_EDITOR
void SpriteAnimRendererComponent::onDisable()
{
    _2DRendererComponent::onDisable();

    disableGizmo(entity);
}
#endif

#if BARK_EDITOR
void SpriteAnimRendererComponent::renderGizmo(Gizmo2DContext* ctx)
{
    if (!entity->selected) return;

    // In editor mode, we draw the first frame of the selected anim.
    // Or first frame of the first anim if no anim is selected
    OTextureRef texture;
    Vector2 origin;
    Vector2 sizef = Vector2::One;
    if (spriteAnim)
    {
        auto oanim = spriteAnim->getAnim(anim);
        if (!oanim)
        {
            const auto& anims = spriteAnim->getAnimNames();
            if (!anims.empty()) oanim = spriteAnim->getAnim(anims.front());
        }
        if (oanim && !oanim->frames.empty())
        {
            const auto& first_frame = oanim->frames.front();
            texture = first_frame.pTexture;
            origin = first_frame.origin;

            if (texture) sizef = texture->getSizef();
            sizef.x *= first_frame.UVs.z - first_frame.UVs.x;
            sizef.y *= first_frame.UVs.w - first_frame.UVs.y;
        }
    }

    const auto& transform = entity->getWorldTransform();

    ctx->drawDottedRect(transform, sizef, origin);
    ctx->drawOrigin(transform);
}
#endif

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
    if (spriteAnim)
    {
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

Rect SpriteAnimRendererComponent::getWorldRect()
{
#if !BARK_EDITOR
    if (!spriteAnimInstance) return Rect(Vector2(entity->getWorldTransform().Translation()), Vector2::Zero);
    // TODO
    const auto &origin = spriteAnimInstance->getOrigin();
    auto texture = spriteAnimInstance->getTexture();
    const auto &uvs = spriteAnimInstance->getUVs();

    Vector2 sizef = Vector2::One;
    if (texture) sizef = texture->getSizef();
    sizef.x *= uvs.z - uvs.x;
    sizef.y *= uvs.w - uvs.y;

    return TransformHelper::getWorldRect(entity->getWorldTransform(), sizef, origin);
#else
    // In editor mode, we draw the first frame of the selected anim.
    // Or first frame of the first anim if no anim is selected
    OTextureRef texture;
    Vector2 origin;
    Vector2 sizef = Vector2::One;

    if (spriteAnim)
    {
        auto oanim = spriteAnim->getAnim(anim);
        if (!oanim)
        {
            const auto& anims = spriteAnim->getAnimNames();
            if (!anims.empty()) oanim = spriteAnim->getAnim(anims.front());
        }
        if (oanim && !oanim->frames.empty())
        {
            const auto& first_frame = oanim->frames.front();
            texture = first_frame.pTexture;
            origin = first_frame.origin;

            if (texture) sizef = texture->getSizef();
            sizef.x *= first_frame.UVs.z - first_frame.UVs.x;
            sizef.y *= first_frame.UVs.w - first_frame.UVs.y;
        }
    }

    const auto& transform = entity->getWorldTransform();

    return TransformHelper::getWorldRect(transform, sizef, origin);
#endif
}
