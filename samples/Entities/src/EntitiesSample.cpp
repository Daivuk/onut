// Required for WinMain
#include <Windows.h>

// Oak Nut include
#include <onut/Component.h>
#include <onut/ContentManager.h>
#include <onut/Entity.h>
#include <onut/EntityManager.h>
#include <onut/onut.h>
#include <onut/Renderer.h>
#include <onut/Settings.h>
#include <onut/SpriteBatch.h>
#include <onut/Timing.h>

void init();
void update();
void render();

// User defined component that rotates and draw a rectangle
class RotatingSquareComponent : public onut::Component
{
private:
    void update() override
    {
        auto transform = getEntity()->getLocalTransform();
        transform = Matrix::CreateRotationZ(ODT) * transform;
        getEntity()->setLocalTransform(transform);
    }

    void render() override
    {
        auto& transform = getEntity()->getWorldTransform();
        oSpriteBatch->drawSprite(nullptr, transform);
    }
};

// Main
int CALLBACK WinMain(HINSTANCE appInstance, HINSTANCE prevInstance, LPSTR cmdLine, int cmdCount)
{
    oSettings->setGameName("Entities Sample");
    ORun(init, update, render);
}

void init()
{
    auto pEntity = OEntity::create();
    pEntity->addComponent<RotatingSquareComponent>();
    pEntity->setLocalTransform(Matrix::CreateScale(128) * Matrix::CreateTranslation(OScreenCenterf));

    pEntity = OEntity::create();
    pEntity->addComponent<RotatingSquareComponent>();
    pEntity->setLocalTransform(Matrix::CreateScale(128) * Matrix::CreateTranslation(OScreenCenterf / 2));

    pEntity = OEntity::create();
    pEntity->addComponent<RotatingSquareComponent>();
    pEntity->setLocalTransform(Matrix::CreateScale(128) * Matrix::CreateTranslation(OScreenCenterf * 1.5f));
}

void update()
{
}

void render()
{
    // Clear
    oRenderer->clear(OColorHex(1d232d));
}
