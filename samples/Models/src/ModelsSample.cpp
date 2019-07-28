// Oak Nut include
#include <onut/Anim.h>
#include <onut/Model.h>
#include <onut/Renderer.h>
#include <onut/Settings.h>
#include <onut/Texture.h>
#include <onut/Timing.h>

OAnimFloat angleAnim;
OModelRef pModels[3];

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
    pModels[0]->render(
        Matrix::CreateRotationZ(OConvertToRadians(angleAnim.get()))
    );
    pModels[1]->render(
        Matrix::CreateRotationZ(OConvertToRadians(angleAnim.get())) *
        Matrix::CreateTranslation({-5, -30, 0})
    );
    pModels[2]->render(
        Matrix::CreateTranslation({7.527910, 19.933542, 7.614748}) * // Box model is off center
        Matrix::CreateRotationZ(OConvertToRadians(angleAnim.get())) *
        Matrix::CreateTranslation({-30, -5, 0})
    );
}

void postRender()
{
}

void renderUI()
{
}
