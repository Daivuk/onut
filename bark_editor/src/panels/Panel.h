#pragma once

#include <string>

class GUIContext;

class Panel
{
public:
    std::string name;
    float       scroll      = 0.0f;
    float       min_width   = 200.0f;
    float       min_height  = 200.0f;
    bool        closable    = false;

    void renderFrame(GUIContext* ctx);

    virtual void render(GUIContext* ctx) = 0;
};
