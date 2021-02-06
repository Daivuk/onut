#pragma once

#include <onut/ForwardDeclaration.h>
#include "Component.h"

OForwardDeclare(SpriteBatch);
namespace onut
{
    class RenderStates;
};

class _2DRendererComponent : public Component
{
public:
    virtual ~_2DRendererComponent() {}

    virtual void onEnable() override;
    virtual void onDisable() override;

    virtual void render(onut::RenderStates& rs, OSpriteBatch* sb) = 0;
};
