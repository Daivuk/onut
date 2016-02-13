#pragma once
#include "onut/Maths.h"

namespace onut
{
    /**
    Rect alignement helper
    */
    template<Align Talign = Align::TopLeft>
    Rect alignedRect(float xOffset, float yOffset, float width, float height, float padding = 0, Align align = Talign)
    {
        switch (align)
        {
            case Align::TopLeft:
                xOffset += padding;
                yOffset += padding;
                width -= padding * 1.5f;
                height -= padding * 1.5f;
                return std::move(Rect{xOffset, yOffset, width, height});
            case Align::Top:
                yOffset += padding;
                width -= padding;
                height -= padding * 1.5f;
                return std::move(Rect{OScreenCenterXf - width * .5f + xOffset, yOffset, width, height});
            case Align::TopRight:
                xOffset += padding;
                yOffset += padding;
                width -= padding * 1.5f;
                height -= padding * 1.5f;
                return std::move(Rect{OScreenWf - xOffset - width, yOffset, width, height});
            case Align::Left:
                xOffset += padding;
                width -= padding * 1.5f;
                height -= padding;
                return std::move(Rect{xOffset, OScreenCenterYf - height * .5f + yOffset, width, height});
            case Align::Center:
                width -= padding;
                height -= padding;
                return std::move(Rect{OScreenCenterXf - width * .5f + xOffset, OScreenCenterYf - height * .5f + yOffset, width, height});
            case Align::Right:
                xOffset += padding;
                width -= padding * 1.5f;
                height -= padding;
                return std::move(Rect{OScreenWf - xOffset - width, OScreenCenterYf - height * .5f + yOffset, width, height});
            case Align::BottomLeft:
                xOffset += padding;
                yOffset += padding;
                width -= padding * 1.5f;
                height -= padding * 1.5f;
                return std::move(Rect{xOffset, OScreenHf - yOffset - height, width, height});
            case Align::Bottom:
                yOffset += padding;
                width -= padding;
                height -= padding * 1.5f;
                return std::move(Rect{OScreenCenterXf - width * .5f + xOffset, OScreenHf - yOffset - height, width, height});
            case Align::BottomRight:
                xOffset += padding;
                yOffset += padding;
                width -= padding * 1.5f;
                height -= padding * 1.5f;
                return std::move(Rect{OScreenWf - xOffset - width, OScreenHf - yOffset - height, width, height});
        }
        return std::move(Rect());
    }

    template<Align Talign = Align::TopLeft>
    Vector2 alignedRect(const Rect& rect, Align align = Talign)
    {
        Vector2 ret;
        switch (align)
        {
            case Align::TopLeft:
                ret.x = rect.x;
                ret.y = rect.y;
                break;
            case Align::Top:
                ret.x = rect.x + rect.z * .5f;
                ret.y = rect.y;
                break;
            case Align::TopRight:
                ret.x = rect.x + rect.z;
                ret.y = rect.y;
                break;
            case Align::Left:
                ret.x = rect.x;
                ret.y = rect.y + rect.w * .5f;
                break;
            case Align::Center:
                ret.x = rect.x + rect.z * .5f;
                ret.y = rect.y + rect.w * .5f;
                break;
            case Align::Right:
                ret.x = rect.x + rect.z;
                ret.y = rect.y + rect.w * .5f;
                break;
            case Align::BottomLeft:
                ret.x = rect.x;
                ret.y = rect.y + rect.w;
                break;
            case Align::Bottom:
                ret.x = rect.x + rect.z * .5f;
                ret.y = rect.y + rect.w;
                break;
            case Align::BottomRight:
                ret.x = rect.x + rect.z;
                ret.y = rect.y + rect.w;
                break;
        }
        return std::move(ret);
    }

    template<Align Talign = Align::TopLeft>
    Rect alignedRect(const Rect& rect, float padding = 0, Align align = Talign)
    {
        return alignedRect(rect.x, rect.y, rect.z, rect.w, padding, align);
    }

    template<Align Talign = Align::TopLeft>
    Rect alignedRect(float width, float height, float padding = 0, Align align = Talign)
    {
        return alignedRect(0, 0, width, height, padding, align);
    }

    template<Align Talign = Align::TopLeft>
    Rect alignedRect(float size, float padding = 0, Align align = Talign)
    {
        return alignedRect(0, 0, size, size, padding, align);
    }
}
