// Oak Nut include
#include <onut/Anim.h>
#include <onut/Model.h>
#include <onut/Renderer.h>
#include <onut/Settings.h>
#include <onut/SpriteBatch.h>
#include <onut/Texture.h>
#include <onut/Timing.h>

OAnimFloat angleAnim;
OModelRef pTeapot;
OTextureRef rt;

void initSettings()
{
    oSettings->setGameName("Models Sample");
    oSettings->setShowFPS(true);
}

void init()
{
    rt = OTexture::createScreenRenderTarget();
    pTeapot = OGetModel("barn.model");
    angleAnim.play(0.0f, -360.0f, 8.0f, OTweenLinear, OLoop);
}

void update()
{
}

void render()
{
    oRenderer->renderStates.renderTarget.push(rt);
    // Clear to black
    oRenderer->setupFor3D({-8, -15, 10}, {0, 0, 0}, Vector3::Up, 90);
    oRenderer->clear({0.5, 0.5, 0.5, 1});
    oRenderer->clearDepth();
    pTeapot->render(Matrix::Identity);
    pTeapot->render(Matrix::CreateRotationZ(OConvertToRadians(angleAnim.get())));
    oRenderer->renderStates.renderTarget.pop();
    oSpriteBatch->begin();
    oSpriteBatch->drawRect(rt, OScreenRectf);
    oSpriteBatch->end();
}

void postRender()
{
}

void renderUI()
{
}
