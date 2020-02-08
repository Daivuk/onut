// Oak Nut include
#include <onut/Anim.h>
#include <onut/Deferred.h>
#include <onut/Font.h>
#include <onut/Input.h>
#include <onut/Model.h>
#include <onut/Renderer.h>
#include <onut/Settings.h>
#include <onut/Shader.h>
#include <onut/SpriteBatch.h>
#include <onut/Texture.h>
#include <onut/Timing.h>

OModelRef pScene;
bool showGBuffer = false;
bool aoEnabled = true;
OAnimFloat angleAnim(0.0f);

void initSettings()
{
    oSettings->setGameName("Deferred Sample");
    oSettings->setShowFPS(true);
    oSettings->setResolution({ 1280, 720 });
}

void init()
{
    angleAnim.playFromCurrent(O2PI, 5.0f, OTweenLinear, OLoop);
}

void update()
{
    if (oInput->isStateJustDown(OKeyG)) showGBuffer = !showGBuffer;
    if (oInput->isStateJustDown(OKeyA)) aoEnabled = !aoEnabled;
}

void render()
{
    // Clear buffers and setup a 3D view and 
    oRenderer->clear(Color::Black);
    oRenderer->clearDepth();
    oRenderer->setupFor3D({0, 0, 0.5f}, {1, 1, 0.5f}, Vector3::Up, 70);

    // Deferred
    // We could have batched the models here, but we are testing the deferred ordering.
    // See the ModelBatch sample for batching.
    oDeferred->begin(Color(0.05f, 0.075f, 0.1f));
    oDeferred->addSolid(OGetModel("floor.model"), Matrix::CreateTranslation({ 0, 0, 0 }));
    oDeferred->addSolid(OGetModel("floor.model"), Matrix::CreateTranslation({ 0, 0, 0 }));
    oDeferred->addSolid(OGetModel("floor.model"), Matrix::CreateTranslation({1, 0, 0}));
    oDeferred->addSolid(OGetModel("floor.model"), Matrix::CreateTranslation({2, 0, 0}));
    oDeferred->addSolid(OGetModel("floor.model"), Matrix::CreateTranslation({0, 1, 0}));
    oDeferred->addSolid(OGetModel("floor.model"), Matrix::CreateTranslation({1, 1, 0}));
    oDeferred->addSolid(OGetModel("floor.model"), Matrix::CreateTranslation({2, 1, 0}));
    oDeferred->addSolid(OGetModel("floor.model"), Matrix::CreateTranslation({0, 2, 0}));
    oDeferred->addSolid(OGetModel("floor.model"), Matrix::CreateTranslation({1, 2, 0}));
    oDeferred->addSolid(OGetModel("ceiling.model"), Matrix::CreateTranslation({0, 0, 0}));
    oDeferred->addSolid(OGetModel("lightWell.model"), Matrix::CreateTranslation({1, 0, 0}));
    oDeferred->addSolid(OGetModel("ceiling.model"), Matrix::CreateTranslation({2, 0, 0}));
    oDeferred->addSolid(OGetModel("ceiling.model"), Matrix::CreateTranslation({0, 1, 0}));
    oDeferred->addSolid(OGetModel("ceiling.model"), Matrix::CreateTranslation({1, 1, 0}));
    oDeferred->addSolid(OGetModel("ceiling.model"), Matrix::CreateTranslation({2, 1, 0}));
    oDeferred->addSolid(OGetModel("ceiling.model"), Matrix::CreateTranslation({0, 2, 0}));
    oDeferred->addSolid(OGetModel("ceiling.model"), Matrix::CreateTranslation({1, 2, 0}));
    oDeferred->addSolid(OGetModel("stairs.model"), Matrix::CreateTranslation({.5f, 2.5f, 0}));
    oDeferred->addSolid(OGetModel("wall.model"), Matrix::CreateTranslation({-.5f, 2.5f, 0}));
    oDeferred->addSolid(OGetModel("wall.model"), Matrix::CreateRotationZ(-OPIDIV2) * Matrix::CreateTranslation({1.5f, 2.5f, 0}));
    oDeferred->addSolid(OGetModel("wall.model"), Matrix::CreateTranslation({1.5f, 1.5f, 0}));
    oDeferred->addSolid(OGetModel("wall.model"), Matrix::CreateRotationZ(-OPIDIV2) * Matrix::CreateTranslation({2.5f, 1.5f, 0}));
    oDeferred->addSolid(OGetModel("wall.model"), Matrix::CreateRotationZ(-OPIDIV2) * Matrix::CreateTranslation({2.5f, 0.5f, 0}));
    oDeferred->addSolid(OGetModel("column.model"), Matrix::CreateTranslation({-.5f, 2.5f, 0}));
    oDeferred->addSolid(OGetModel("column.model"), Matrix::CreateTranslation({.5f, 2.5f, 0}));
    oDeferred->addSolid(OGetModel("column.model"), Matrix::CreateTranslation({1.5f, 2.5f, 0}));
    oDeferred->addSolid(OGetModel("column.model"), Matrix::CreateTranslation({1.5f, 1.5f, 0}));
    oDeferred->addSolid(OGetModel("column.model"), Matrix::CreateTranslation({2.5f, 1.5f, 0}));
    oDeferred->addSolid(OGetModel("column.model"), Matrix::CreateTranslation({2.5f, 0.5f, 0}));
    oDeferred->addSolid(OGetModel("column.model"), Matrix::CreateTranslation({2.5f, -.5f, 0}));

    oDeferred->addAlphaTest(OGetModel("alphaTest.model"), Matrix::CreateRotationZ(angleAnim.get()) * 
        Matrix::CreateTranslation({ 1.0f, 0.5f, 0.5f }));

    oDeferred->addTransparent(OGetModel("blend.model"), Matrix::CreateRotationZ(angleAnim.get()) *
        Matrix::CreateTranslation({ 2.0f, 0.5f, 0.5f }));
    oDeferred->addTransparent(OGetModel("blend.model"), Matrix::CreateRotationZ(angleAnim.get()) *
        Matrix::CreateTranslation({ 0.8f, 3.5f, 0.5f }));
    oDeferred->addTransparent(OGetModel("blend.model"), Matrix::CreateRotationZ(angleAnim.get()) *
        Matrix::CreateTranslation({ 0.8f / 2.0f, 3.5f / 2.0f, 0.35f }));
    oDeferred->addTransparent(OGetModel("blend.model"), Matrix::CreateRotationZ(angleAnim.get()) *
        Matrix::CreateTranslation({ 1.0f, 0.5f, 0.5f }));

    oDeferred->addOmni(Vector3(1, 5.0f, 1.5f), 3.0f, Color(0.5f, 1.0f, 1.5f));
    oDeferred->addOmni(Vector3(1, 0, 1.1f), 1.3f, Color(0.5f, 1.0f, 1.5f));
    oDeferred->addOmni(Vector3(1, 2, 0.5f), 1.5f, Color(1.5f, 1.0f, 0.5f));
    oDeferred->end(aoEnabled, 0.15f, 2.0f);

    // Debug stuff
    oSpriteBatch->begin();
    if (showGBuffer)
    {
        oSpriteBatch->drawRect(oDeferred->getAlbedo(), { 0, 0, OScreenWf / 2.0f, OScreenHf / 2.0f });
        oSpriteBatch->drawRect(oDeferred->getNormal(), { OScreenWf / 2.0f, 0, OScreenWf / 2.0f, OScreenHf / 2.0f });
        oSpriteBatch->drawRect(oDeferred->getDepth(), { 0, OScreenHf / 2.0f, OScreenWf / 2.0f, OScreenHf / 2.0f });
        oSpriteBatch->drawRect(oDeferred->getMaterial(), { OScreenWf / 2.0f, OScreenHf / 2.0f, OScreenWf / 2.0f, OScreenHf / 2.0f });
    }
    OGetFont("font.fnt")->draw("Press G to toggle gbuffer", { 0, 20 }, OTopLeft, Color(1, 1, 0));
    OGetFont("font.fnt")->draw("Press A to toggle SSAO", { 0, 35 }, OTopLeft, Color(1, 1, 0));
    oSpriteBatch->end();
}

void postRender()
{
}

void renderUI()
{
}
