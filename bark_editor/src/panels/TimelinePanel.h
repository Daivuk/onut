#pragma once

#include "Panel.h"

class TimelinePanel final : public Panel
{
public:
    TimelinePanel();

    void render(GUIContext* ctx) override;
};
