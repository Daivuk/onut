#pragma once

#include "Panel.h"

class EntityPanel final : public Panel
{
public:
    EntityPanel();

    void render(GUIContext* ctx) override;
};
