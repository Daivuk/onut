// Required for WinMain
#include <Windows.h>

// Oak Nut include
#include <onut/Component.h>
#include <onut/ContentManager.h>
#include <onut/Entity.h>
#include <onut/onut.h>
#include <onut/Renderer.h>
#include <onut/Settings.h>
#include <onut/SpriteBatch.h>
#include <onut/Timing.h>

void init();
void update();
void render();

//--- User defined components

// This component draws a white square
class WhiteSquareComponent : public onut::Component
{
private:
    void render() override
    {
        auto& transform = getEntity()->getWorldTransform();
        oSpriteBatch->drawSprite(nullptr, transform);
    }
};

// If this component is attached to an entity, it will rotate on the Z axis
class RotatingComponent : public onut::Component
{
private:
    void update() override
    {
        auto transform = getEntity()->getLocalTransform();
        transform = Matrix::CreateRotationZ(ODT) * transform;
        getEntity()->setLocalTransform(transform);
    }
};

// Blinks the entity by toggling visible state on and off
class BlinkingComponent : public onut::Component
{
private:
    void update() override
    {
        m_blink += ODT * 2.0f;
        if (m_blink >= 1.0f) m_blink -= 1.0f;
        getEntity()->setVisible(m_blink < 0.5f);
    }

    float m_blink = 0.0f;
};

// Pause the rotation by toggling enable state on and off
class DeactivateComponent : public onut::Component
{
private:
    void update() override
    {
        m_blink += ODT;
        if (m_blink >= 1.0f) m_blink -= 1.0f;

        auto pRotationComponent = getEntity()->getComponent<RotatingComponent>();
        if (pRotationComponent)
        {
            pRotationComponent->setEnabled(m_blink < 0.5f);
        }
    }

    float m_blink = 0.0f;
};

// Main
int CALLBACK WinMain(HINSTANCE appInstance, HINSTANCE prevInstance, LPSTR cmdLine, int cmdCount)
{
    oSettings->setGameName("Entities Sample");
    ORun(init, update, render);
}

void init()
{
    OEntityRef pSquare;

    // Static square
    pSquare = OEntity::create();
    pSquare->setStatic(true); // This square will not update and his rendering will potentially be batched.
    pSquare->addComponent<WhiteSquareComponent>();
    pSquare->setLocalTransform(Matrix::CreateScale(128) * Matrix::CreateTranslation(OScreenCenterf / 2.0f));

    // Rotating square
    pSquare = OEntity::create();
    pSquare->addComponent<WhiteSquareComponent>();
    pSquare->addComponent<RotatingComponent>();
    pSquare->setLocalTransform(Matrix::CreateScale(128) * Matrix::CreateTranslation(OScreenCenterf));

    // Blinking square
    pSquare = OEntity::create();
    pSquare->addComponent<WhiteSquareComponent>();
    pSquare->addComponent<BlinkingComponent>();
    pSquare->setLocalTransform(Matrix::CreateScale(128) * Matrix::CreateTranslation(OScreenCenterf * 1.5f));

    // Intermittent rotating square
    pSquare = OEntity::create();
    pSquare->addComponent<WhiteSquareComponent>();
    pSquare->addComponent<RotatingComponent>();
    pSquare->addComponent<DeactivateComponent>();
    pSquare->setLocalTransform(Matrix::CreateScale(128) * Matrix::CreateTranslation(Vector2(OScreenCenterXf / 2.0f, OScreenCenterYf * 1.5f)));

    // All combined
    pSquare = OEntity::create();
    pSquare->addComponent<WhiteSquareComponent>();
    pSquare->addComponent<RotatingComponent>();
    pSquare->addComponent<BlinkingComponent>();
    pSquare->addComponent<DeactivateComponent>();
    pSquare->setLocalTransform(Matrix::CreateScale(128) * Matrix::CreateTranslation(Vector2(OScreenCenterXf * 1.5f, OScreenCenterYf / 2.0f)));
}

void update()
{
}

void render()
{
    // Clear
    oRenderer->clear(OColorHex(1d232d));
}
