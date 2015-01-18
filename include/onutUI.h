#pragma once
#include "UI.h"
#include "SimpleMath.h"
using namespace DirectX::SimpleMath;

namespace onut
{
    inline Rect UI2Onut(const sUIRect& uiRect)
    {
        Rect ret;
        ret.x = uiRect.position.x;
        ret.y = uiRect.position.y;
        ret.z = uiRect.size.x;
        ret.w = uiRect.size.y;
        return std::move(ret);
    }
};
