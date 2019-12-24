// Oak Nut include
#include <onut/Anim.h>
#include <onut/Model.h>
#include <onut/Renderer.h>
#include <onut/Settings.h>
#include <onut/Shader.h>
#include <onut/Texture.h>
#include <onut/Timing.h>

OAnimFloat angleAnim;
OModelRef pModels[4];

void initSettings()
{
    oSettings->setGameName("Models Sample");
    oSettings->setShowFPS(true);
}

void init()
{
    pModels[0] = OGetModel("teapot.model");
    pModels[1] = OGetModel("barn.model");
    pModels[2] = OGetModel("box.model");
    pModels[2]->getMesh(0)->pTexture = OGetTexture("diffuse.png"); // Swaping texture example
    pModels[3] = OGetModel("captainchair.model");

    angleAnim.play(0.0f, -360.0f, 8.0f, OTweenLinear, OLoop);
}

void update()
{
}

void render()
{
    oRenderer->setupFor3D({8, 8, 15}, {0, 0, 5}, Vector3::Up, 90);
    oRenderer->clear({0.5, 0.5, 0.5, 1});
    oRenderer->clearDepth();
    oRenderer->renderStates.blendMode = OBlendOpaque;

    oRenderer->setLight(0, Vector3(-15, -15, 0.5f), 30.0f, Color(0.5f, 1.0f, 1.5f));

    // Teapot
    pModels[0]->render(
        Matrix::CreateRotationZ(OConvertToRadians(angleAnim.get())) *
        Matrix::CreateTranslation({ -5, -30, 0 })
    );

    // Barn
    pModels[1]->render(
        Matrix::CreateRotationZ(OConvertToRadians(angleAnim.get())) *
        Matrix::CreateTranslation({-30, -30, 0})
    );

    // Box
    pModels[2]->render(
        Matrix::CreateTranslation({7.527910f, 19.933542f, 7.614748f}) * // Box model is off center
        Matrix::CreateRotationZ(OConvertToRadians(angleAnim.get())) *
        Matrix::CreateTranslation({-30, -5, 0})
    );

    // Use alpha blend to draw the captain's chair because of the fake shadow decal on it
    oRenderer->renderStates.blendMode = OBlendAlpha;
    oRenderer->setAlphaTestBias(0.0f);
    pModels[3]->render(Matrix::CreateRotationZ(OConvertToRadians(angleAnim.get())));
}

void postRender()
{
}

void renderUI()
{
}
