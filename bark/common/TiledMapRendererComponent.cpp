#include <onut/Renderer.h>
#include <onut/SpriteBatch.h>
#include <onut/TiledMap.h>
#include "TiledMapRendererComponent.h"
#include "SceneManager.h"
#include "Entity.h"

void TiledMapRendererComponent::render(onut::RenderStates& rs, OSpriteBatch* sb)
{
    if (!tiledMap) return;

    rs.sampleFiltering = smooth ? OFilterLinear : OFilterNearest;
    rs.blendMode = (onut::BlendMode)blendMode;

    tiledMap->setTransform(entity->getWorldTransform());
    tiledMap->render();
}
