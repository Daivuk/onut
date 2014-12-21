#pragma once
#include "SimpleMath.h"
using namespace DirectX::SimpleMath;

namespace onut {
    /**
        Rect alignement helper
    */
    template<Align Talign = Align::TOP_LEFT>
    Rect alignedRect(float xOffset, float yOffset, float width, float height, float padding = 0, Align align = Talign) {
        switch (align) {
        case Align::TOP_LEFT:
            xOffset += padding;
            yOffset += padding;
            width -= padding * 1.5f;
            height -= padding * 1.5f;
            return std::move(Rect{ xOffset, yOffset, width, height });
        case Align::TOP:
            yOffset += padding;
            width -= padding;
            height -= padding * 1.5f;
            return std::move(Rect{ OScreenCenterXf - width * .5f + xOffset, yOffset, width, height });
        case Align::TOP_RIGHT:
            xOffset += padding;
            yOffset += padding;
            width -= padding * 1.5f;
            height -= padding * 1.5f;
            return std::move(Rect{ OScreenWf - xOffset - width, yOffset, width, height });
        case Align::LEFT:
            xOffset += padding;
            width -= padding * 1.5f;
            height -= padding;
            return std::move(Rect{ xOffset, OScreenCenterYf - height * .5f + yOffset, width, height });
        case Align::CENTER:
            width -= padding;
            height -= padding;
            return std::move(Rect{ OScreenCenterXf - width * .5f + xOffset, OScreenCenterYf - height * .5f + yOffset, width, height });
        case Align::RIGHT:
            xOffset += padding;
            width -= padding * 1.5f;
            height -= padding;
            return std::move(Rect{ OScreenWf - xOffset - width, OScreenCenterYf - height * .5f + yOffset, width, height });
        case Align::BOTTOM_LEFT:
            xOffset += padding;
            yOffset += padding;
            width -= padding * 1.5f;
            height -= padding * 1.5f;
            return std::move(Rect{ xOffset, OScreenHf - yOffset - height, width, height });
        case Align::BOTTOM:
            yOffset += padding;
            width -= padding;
            height -= padding * 1.5f;
            return std::move(Rect{ OScreenCenterXf - width * .5f + xOffset, OScreenHf - yOffset - height, width, height });
        case Align::BOTTOM_RIGHT:
            xOffset += padding;
            yOffset += padding;
            width -= padding * 1.5f;
            height -= padding * 1.5f;
            return std::move(Rect{ OScreenWf - xOffset - width, OScreenHf - yOffset - height, width, height });
        }
        return std::move(Rect());
    }

    template<Align Talign = Align::TOP_LEFT>
    Rect alignedRect(const Rect& rect, float padding = 0, Align align = Talign) {
        alignedRect(rect.x, rect.y, rect.z, rect.w, padding, align);
    }
}
