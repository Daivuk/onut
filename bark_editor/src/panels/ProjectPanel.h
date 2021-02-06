#pragma once

#include "Panel.h"

class ProjectPanel final : public Panel
{
public:
    ProjectPanel();

    void render(GUIContext* ctx) override;
};
