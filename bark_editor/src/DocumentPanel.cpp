#include <onut/Texture.h>
#include <onut/Renderer.h>
#include <onut/SpriteBatch.h>
#include "DocumentPanel.h"
#include "GUIContext.h"
#include "Theme.h"

#include <onut/TiledMap.h>
#include <onut/ContentManager.h>
#include "globals.h"

DocumentPanel::DocumentPanel(const std::string& in_name)
{
    name = in_name;
    game_content_rt = OTexture::createRenderTarget({128, 128}, false);
}

void DocumentPanel::renderGameContent(GUIContext* ctx)
{
    auto game_content_rect = ctx->rect.Grow(-ctx->theme->border_size);
    Point res((int)game_content_rect.z, (int)game_content_rect.w);
    game_content_rt->resizeTarget(res);

    ctx->rs->renderTargets[0].push(game_content_rt);
    ctx->rs->viewport.push({0, 0, res.x, res.y});
    ctx->r->clear(ctx->theme->area_color);

    auto tiledMap = g_content_mgr->getResourceAs<OTiledMap>("dungeon.tmx");
    tiledMap->setTransform(Matrix::Create2DTranslationZoom(Vector2((float)res.x, (float)res.y), Vector2(400, 400), 3));
    tiledMap->render();

    ctx->rs->viewport.pop();
    ctx->rs->renderTargets[0].pop();
}

void DocumentPanel::render(GUIContext* ctx)
{
    ctx->drawArea();

    auto game_content_rect = ctx->rect.Grow(-ctx->theme->border_size);
    ctx->drawRect(game_content_rt, game_content_rect, Color::White);

    // Gizmos
}
