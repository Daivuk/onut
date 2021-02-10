#pragma once

#include "Panel.h"
#include "JsonHelper.h"

class ProjectPanel final : public Panel
{
public:
    float scroll = 0.0f;
    Json::Value original_json; // Before editing

    ProjectPanel();

    void render(GUIContext* ctx) override;
};
