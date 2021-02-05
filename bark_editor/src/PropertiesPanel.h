#pragma once

#include "Panel.h"

class PropertiesPanel final : public Panel
{
public:
    PropertiesPanel();

    void render(GUIContext* ctx) override;
};
