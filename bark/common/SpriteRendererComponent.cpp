#include <onut/Renderer.h>
#include <onut/SpriteBatch.h>
#include "SpriteRendererComponent.h"
#include "Entity.h"
#if BARK_EDITOR
#include <onut/Texture.h>
#include "Gizmo2DContext.h"
#endif

#if BARK_EDITOR
void SpriteRendererComponent::onEnable()
{
    _2DRendererComponent::onEnable();

    enableGizmo(entity);
}
#endif

#if BARK_EDITOR
void SpriteRendererComponent::onDisable()
{
    _2DRendererComponent::onDisable();

    disableGizmo(entity);
}
#endif

#if BARK_EDITOR
void SpriteRendererComponent::renderGizmo(Gizmo2DContext* ctx)
{
    if (!entity->selected) return;

    const auto& transform = entity->getWorldTransform();
    Vector2 sizef = texture ? texture->getSizef() : Vector2::One;

    ctx->drawDottedRect(transform, sizef, origin);
    ctx->drawOrigin(transform);
}
#endif

void SpriteRendererComponent::render(onut::RenderStates& rs, OSpriteBatch* sb)
{
    rs.sampleFiltering = smooth ? OFilterLinear : OFilterNearest;
    rs.blendMode = (onut::BlendMode)blendMode;

    sb->drawSprite(texture, entity->getWorldTransform(), color, origin);
}
