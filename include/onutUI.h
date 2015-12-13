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

    inline Color UI2Onut(const sUIColor& uiColor)
    {
        Color ret(uiColor.r * uiColor.a, uiColor.g * uiColor.a, uiColor.b * uiColor.a, uiColor.a);
        return std::move(ret);
    }

    inline Vector4 UI2Onut(const sUIPadding& uiPadding)
    {
        Vector4 ret{&uiPadding.left};
        return std::move(ret);
    }

    inline Align UI2Onut(eUIAlign uiAlign)
    {
        return static_cast<Align>(uiAlign);
    }

    inline Vector2 UI2Onut(sUIVector2 uiPos)
    {
        return{uiPos.x, uiPos.y};
    }
};

#define OToOnut onut::UI2Onut
