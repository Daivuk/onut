#pragma once

#include "Panel.h"

class AssetPanel final : public Panel
{
public:
    AssetPanel();

    void render(GUIContext* ctx) override;
};
