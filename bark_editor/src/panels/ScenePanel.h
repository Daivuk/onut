#pragma once

#include "Panel.h"

class ScenePanel final : public Panel
{
public:
    ScenePanel();

    void render(GUIContext* ctx) override;
};
