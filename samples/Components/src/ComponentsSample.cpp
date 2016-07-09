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
public:
    void setSize(float size)
    {
        m_size = size;
    }

private:
    void render() override
    {
        auto& transform = getEntity()->getWorldTransform();
        oSpriteBatch->drawSprite(nullptr, transform, Vector2(m_size));
    }

    float m_size = 64.0f;
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
        getComponent<RotatingComponent>()->setEnabled(m_blink < 0.5f);
    }

    float m_blink = 0.0f;
};

// Main
int CALLBACK WinMain(HINSTANCE appInstance, HINSTANCE prevInstance, LPSTR cmdLine, int cmdCount)
{
    oSettings->setGameName("Components Sample");
    ORun(init, update, render);
}

void init()
{
    OEntityRef pSquare;

    // Static square
    pSquare = OEntity::create();
    pSquare->setStatic(true); // This square will not update and his rendering will potentially be batched.
    pSquare->addComponent<WhiteSquareComponent>();
    pSquare->setLocalTransform(Matrix::CreateTranslation(OScreenCenterf / 2.0f));

    // Rotating square
    pSquare = OEntity::create();
    pSquare->addComponent<WhiteSquareComponent>();
    pSquare->addComponent<RotatingComponent>();
    pSquare->setLocalTransform(Matrix::CreateTranslation(OScreenCenterf));
    // Smaller square orbiting this one
    {
        auto pSmallSquare = OEntity::create();
        auto pWhiteSquareComponent = pSmallSquare->addComponent<WhiteSquareComponent>();
        pWhiteSquareComponent->setSize(32.0f);
        pSmallSquare->setLocalTransform(Matrix::CreateTranslation(Vector2(64.0f, 0)));
        pSquare->add(pSmallSquare);
    }

    // Blinking square
    pSquare = OEntity::create();
    pSquare->addComponent<WhiteSquareComponent>();
    pSquare->addComponent<BlinkingComponent>();
    pSquare->setLocalTransform(Matrix::CreateTranslation(OScreenCenterf * 1.5f));

    // Intermittent rotating square
    pSquare = OEntity::create();
    pSquare->addComponent<WhiteSquareComponent>();
    pSquare->addComponent<RotatingComponent>();
    pSquare->addComponent<DeactivateComponent>();
    pSquare->setLocalTransform(Matrix::CreateTranslation(Vector2(OScreenCenterXf / 2.0f, OScreenCenterYf * 1.5f)));

    // All combined
    pSquare = OEntity::create();
    pSquare->addComponent<WhiteSquareComponent>();
    pSquare->addComponent<RotatingComponent>();
    pSquare->addComponent<BlinkingComponent>();
    pSquare->addComponent<DeactivateComponent>();
    pSquare->setLocalTransform(Matrix::CreateTranslation(Vector2(OScreenCenterXf * 1.5f, OScreenCenterYf / 2.0f)));
}

void update()
{
}

void render()
{
    // Clear
    oRenderer->clear(OColorHex(1d232d));
}
