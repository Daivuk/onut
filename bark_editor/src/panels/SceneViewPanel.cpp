#include <onut/Input.h>
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

static const float  ZOOM_LEVELS[]       = {0.25f, 0.5f, 1.0f, 2.0f, 3.0f, 4.0f, 6.0f, 8.0f, 12.0f, 16.0f, 18.0f, 24.0f};
static const int    ZOOM_LEVEL_COUNT    = (int)(sizeof(ZOOM_LEVELS) / sizeof(float));

SceneViewPanel::SceneViewPanel(const std::string& in_filename)
{
    filename        = in_filename;
    name            = onut::getFilenameWithoutExtension(in_filename);
    game_content_rt = OTexture::createRenderTarget({128, 128}, false);
    closable        = true;

    scene_mgr.loadScene(in_filename);
}

void SceneViewPanel::render(GUIContext* ctx)
{
    auto game_content_rect = ctx->rect.Grow(-ctx->theme->border_size);
    Point res((int)game_content_rect.z, (int)game_content_rect.w);
    if (res.x <= 0 || res.y <= 0) return;
    Vector2 resf((float)res.x, (float)res.y);

    // Update mouse in view pos
    mouse_pos.x = ((ctx->mouse.x - game_content_rect.x) - game_content_rect.z / 2) / ZOOM_LEVELS[camera_2d_zoom] + camera_2d_pos.x;
    mouse_pos.y = ((ctx->mouse.y - game_content_rect.y) - game_content_rect.w / 2) / ZOOM_LEVELS[camera_2d_zoom] + camera_2d_pos.y;

    // Camera movement
    if (panning)
    {
        auto delta = pan_mouse_start - ctx->mouse;
        camera_2d_pos = camera_2d_pos_on_down + delta / ZOOM_LEVELS[camera_2d_zoom];

        if (OInputJustReleased(OMouse3)) panning = false;
    }
    else if (game_content_rect.Contains(ctx->mouse))
    {
        if (OInputJustPressed(OMouse3))
        {
            panning = true;
            pan_mouse_start = ctx->mouse;
            camera_2d_pos_on_down = camera_2d_pos;
        }
        else if (oInput->getStateValue(OMouseZ) < 0)
        {
            Vector2 diff_before = (mouse_pos - camera_2d_pos) * ZOOM_LEVELS[camera_2d_zoom];
            camera_2d_zoom--;
            if (camera_2d_zoom < 0) camera_2d_zoom = 0;
            Vector2 diff_after = (mouse_pos - camera_2d_pos) * ZOOM_LEVELS[camera_2d_zoom];
            camera_2d_pos += (diff_after - diff_before) / ZOOM_LEVELS[camera_2d_zoom];
        }
        else if (oInput->getStateValue(OMouseZ) > 0)
        {
            Vector2 diff_before = (mouse_pos - camera_2d_pos) * ZOOM_LEVELS[camera_2d_zoom];
            camera_2d_zoom++;
            if (camera_2d_zoom >= ZOOM_LEVEL_COUNT) camera_2d_zoom = ZOOM_LEVEL_COUNT - 1;
            Vector2 diff_after = (mouse_pos - camera_2d_pos) * ZOOM_LEVELS[camera_2d_zoom];
            camera_2d_pos += (diff_after - diff_before) / ZOOM_LEVELS[camera_2d_zoom];
        }
    }
    
    // Update mouse in view pos after
    mouse_pos.x = ((ctx->mouse.x - game_content_rect.x) - game_content_rect.z / 2) / ZOOM_LEVELS[camera_2d_zoom] + camera_2d_pos.x;
    mouse_pos.y = ((ctx->mouse.y - game_content_rect.y) - game_content_rect.w / 2) / ZOOM_LEVELS[camera_2d_zoom] + camera_2d_pos.y;

    Matrix camera_transform = Matrix::Create2DTranslationZoom(resf, camera_2d_pos, ZOOM_LEVELS[camera_2d_zoom]);

    // Draw content
    {
        game_content_rt->resizeTarget(res);

        ctx->rs->renderTargets[0].push(game_content_rt);
        ctx->rs->viewport.push({0, 0, res.x, res.y});

        scene_mgr.render(&camera_transform);

        ctx->rs->viewport.pop();
        ctx->rs->renderTargets[0].pop();
    }

    // Draw UI
    ctx->drawOutline(game_content_rect.Grow(ctx->theme->border_size), ctx->theme->border_size, ctx->theme->area_border_color);
    ctx->drawRect(game_content_rt, game_content_rect, Color::White);
}
