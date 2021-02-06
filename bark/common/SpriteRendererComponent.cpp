#include <onut/Renderer.h>
#include <onut/SpriteBatch.h>
#include "SpriteRendererComponent.h"
#include "Entity.h"

void SpriteRendererComponent::render(onut::RenderStates& rs, OSpriteBatch* sb)
{
    rs.sampleFiltering = smooth ? OFilterLinear : OFilterNearest;
    rs.blendMode = (onut::BlendMode)blendMode;

    sb->drawSprite(texture, entity->getWorldTransform(), color, origin);
}
