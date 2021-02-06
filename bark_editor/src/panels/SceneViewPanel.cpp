#include <onut/Files.h>
#include <onut/Texture.h>
#include <onut/Renderer.h>
#include <onut/SpriteBatch.h>
#include "SceneViewPanel.h"
#include "GUIContext.h"
#include "Theme.h"

#include <onut/TiledMap.h>
#include <onut/ContentManager.h>
#include "globals.h"

SceneViewPanel::SceneViewPanel(const std::string& filename)
{
    name            = onut::getFilenameWithoutExtension(filename);
    game_content_rt = OTexture::createRenderTarget({128, 128}, false);
    closable        = true;

    scene_mgr.loadScene(filename);
}

void SceneViewPanel::render(GUIContext* ctx)
{
    // Draw content
    auto game_content_rect = ctx->rect.Grow(-ctx->theme->border_size);
    Point res((int)game_content_rect.z, (int)game_content_rect.w);
    if (res.x <= 0 || res.y <= 0) return;

    game_content_rt->resizeTarget(res);

    ctx->rs->renderTargets[0].push(game_content_rt);
    ctx->rs->viewport.push({0, 0, res.x, res.y});

    scene_mgr.render();

    ctx->rs->viewport.pop();
    ctx->rs->renderTargets[0].pop();

    // Gizmos

    // Draw UI frame
    ctx->drawOutline(game_content_rect.Grow(ctx->theme->border_size), ctx->theme->border_size, ctx->theme->area_border_color);
    ctx->drawRect(game_content_rt, game_content_rect, Color::White);
}
