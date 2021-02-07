#pragma once

#include "Panel.h"
#include <vector>

struct AssetDir;
struct Asset;

class AssetsPanel final : public Panel
{
public:
    float   scroll = 0.0f;
    Asset*  asset_to_open = nullptr;

    AssetsPanel();

    void render(GUIContext* ctx) override;

    void renderSubDir(GUIContext* ctx, AssetDir* dir, int indent);
};
