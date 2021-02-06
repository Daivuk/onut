#pragma once

#include "Panel.h"

class AssetsPanel final : public Panel
{
public:
    AssetsPanel();

    void render(GUIContext* ctx) override;
};
