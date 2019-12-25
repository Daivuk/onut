// Oak Nut include
#include <onut/Anim.h>
#include <onut/Model.h>
#include <onut/Renderer.h>
#include <onut/Settings.h>
#include <onut/Shader.h>
#include <onut/Texture.h>
#include <onut/Timing.h>

OModelRef pScene;

void initSettings()
{
    oSettings->setGameName("Model Batch Sample");
    oSettings->setShowFPS(true);
}

void init()
{
    oRenderer->setSun(Vector3::UnitZ, Color::Black);
    oRenderer->setAmbient(Color(0.05f, 0.075f, 0.1f));
    oRenderer->setLight(0, Vector3(1, 5.0f, 1.5f), 3.0f, Color(0.5f, 1.0f, 1.5f)); // Stairs
    oRenderer->setLight(1, Vector3(1, 0, 1.1f), 1.3f, Color(0.5f, 1.0f, 1.5f)); // Light well
    oRenderer->setLight(2, Vector3(1, 2, 0.5f), 1.5f, Color(1.5f, 1.0f, 0.5f)); // Torch

    pScene = OModel::createFromBatch({
        { OGetModel("floor.model"), Matrix::CreateTranslation({0, 0, 0}) },
        { OGetModel("floor.model"), Matrix::CreateTranslation({1, 0, 0}) },
        { OGetModel("floor.model"), Matrix::CreateTranslation({2, 0, 0}) },
        { OGetModel("floor.model"), Matrix::CreateTranslation({0, 1, 0}) },
        { OGetModel("floor.model"), Matrix::CreateTranslation({1, 1, 0}) },
        { OGetModel("floor.model"), Matrix::CreateTranslation({2, 1, 0}) },
        { OGetModel("floor.model"), Matrix::CreateTranslation({0, 2, 0}) },
        { OGetModel("floor.model"), Matrix::CreateTranslation({1, 2, 0}) },
        { OGetModel("ceiling.model"), Matrix::CreateTranslation({0, 0, 0}) },
        { OGetModel("lightWell.model"), Matrix::CreateTranslation({1, 0, 0}) },
        { OGetModel("ceiling.model"), Matrix::CreateTranslation({2, 0, 0}) },
        { OGetModel("ceiling.model"), Matrix::CreateTranslation({0, 1, 0}) },
        { OGetModel("ceiling.model"), Matrix::CreateTranslation({1, 1, 0}) },
        { OGetModel("ceiling.model"), Matrix::CreateTranslation({2, 1, 0}) },
        { OGetModel("ceiling.model"), Matrix::CreateTranslation({0, 2, 0}) },
        { OGetModel("ceiling.model"), Matrix::CreateTranslation({1, 2, 0}) },
        { OGetModel("stairs.model"), Matrix::CreateTranslation({.5f, 2.5f, 0}) },
        { OGetModel("wall.model"), Matrix::CreateTranslation({-.5f, 2.5f, 0}) },
        { OGetModel("wall.model"), Matrix::CreateRotationZ(-OPIDIV2) * Matrix::CreateTranslation({1.5f, 2.5f, 0}) },
        { OGetModel("wall.model"), Matrix::CreateTranslation({1.5f, 1.5f, 0}) },
        { OGetModel("wall.model"), Matrix::CreateRotationZ(-OPIDIV2) * Matrix::CreateTranslation({2.5f, 1.5f, 0}) },
        { OGetModel("wall.model"), Matrix::CreateRotationZ(-OPIDIV2) * Matrix::CreateTranslation({2.5f, 0.5f, 0}) },
        { OGetModel("column.model"), Matrix::CreateTranslation({-.5f, 2.5f, 0}) },
        { OGetModel("column.model"), Matrix::CreateTranslation({.5f, 2.5f, 0}) },
        { OGetModel("column.model"), Matrix::CreateTranslation({1.5f, 2.5f, 0}) },
        { OGetModel("column.model"), Matrix::CreateTranslation({1.5f, 1.5f, 0}) },
        { OGetModel("column.model"), Matrix::CreateTranslation({2.5f, 1.5f, 0}) },
        { OGetModel("column.model"), Matrix::CreateTranslation({2.5f, 0.5f, 0}) },
        { OGetModel("column.model"), Matrix::CreateTranslation({2.5f, -.5f, 0}) }
    });
}

void update()
{
}

void render()
{
    oRenderer->setupFor3D({0, 0, 0.5f}, {1, 1, 0.5f}, Vector3::Up, 90);
    oRenderer->clear({0, 0, 0, 1});
    oRenderer->clearDepth();

    pScene->render(Matrix::Identity);
}

void postRender()
{
}

void renderUI()
{
}
