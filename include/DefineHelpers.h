#pragma once
#include "Typedefs.h"

#define OAnim               onut::Anim

//--- Game pads
#define OABtn               onut::GamePad::eGamePad::A
#define OBBtn               onut::GamePad::eGamePad::B
#define OXBtn               onut::GamePad::eGamePad::X
#define OYBtn               onut::GamePad::eGamePad::Y
#define OUpBtn              onut::GamePad::eGamePad::DPAD_UP
#define ODownBtn            onut::GamePad::eGamePad::DPAD_DOWN
#define OLeftBtn            onut::GamePad::eGamePad::DPAD_LEFT
#define ORightBtn           onut::GamePad::eGamePad::DPAD_RIGHT
#define OLTBtn              onut::GamePad::eGamePad::LT
#define OLBBtn              onut::GamePad::eGamePad::LB
#define ORTBtn              onut::GamePad::eGamePad::RT
#define ORBBtn              onut::GamePad::eGamePad::RB
#define OLThumbBtn          onut::GamePad::eGamePad::LTHUMB
#define ORThumbBtn          onut::GamePad::eGamePad::RTHUMB
#define OStartBtn           onut::GamePad::eGamePad::START
#define OBackBtn            onut::GamePad::eGamePad::BACK
#define OLLeftBtn           onut::GamePad::eGamePad::LTHUMB_LEFT
#define OLRightBtn          onut::GamePad::eGamePad::LTHUMB_RIGHT
#define OLUpBtn             onut::GamePad::eGamePad::LTHUMB_UP
#define OLDownBtn           onut::GamePad::eGamePad::LTHUMB_DOWN
#define ORLeftBtn           onut::GamePad::eGamePad::RTHUMB_LEFT
#define ORRightBtn          onut::GamePad::eGamePad::RTHUMB_RIGHT
#define ORUpBtn             onut::GamePad::eGamePad::RTHUMB_UP
#define ORDownBtn           onut::GamePad::eGamePad::RTHUMB_DOWN

//--- Resolution helpers
#define OScreen             ORenderer->getResolution()
#define OScreenf            Vector2{static_cast<float>(ORenderer->getResolution().x), static_cast<float>(ORenderer->getResolution().y)}
#define OScreenW            ORenderer->getResolution().x
#define OScreenH            ORenderer->getResolution().y
#define OScreenWf           static_cast<float>(ORenderer->getResolution().x)
#define OScreenHf           static_cast<float>(ORenderer->getResolution().y)
#define OScreenCenter       (ORenderer->getResolution() / 2)
#define OScreenCenterf      Vector2{static_cast<float>(ORenderer->getResolution().x / 2), static_cast<float>(ORenderer->getResolution().y / 2)}
#define OScreenCenterX      (ORenderer->getResolution().x / 2)
#define OScreenCenterY      (ORenderer->getResolution().y / 2)
#define OScreenCenterXf     static_cast<float>(ORenderer->getResolution().x / 2)
#define OScreenCenterYf     static_cast<float>(ORenderer->getResolution().y / 2)

//--- Rect helpers for drawing
// Centered origin sprite
// cx,cy = Sprite center position
#define ORectCenteredOrigin(cx, cy, w, h)       Rect{(cx) - (w) * .5f, (cy) - (h) * .5f, w, h}

// Rectangle helpers
#define ORectFullScreen                         Rect{0, 0, OScreenWf, OScreenHf}
#define ORectGrid(cols, rows, ix, iy, px, py)   Rect{OScreenWf/(cols)*(ix)+((ix>0)?px*.5f:px),OScreenHf/(rows)*(iy)+((iy>0)?py*.5f:py),OScreenWf/(cols)-((ix<=0||ix>=cols-1)?px*1.5f:px),OScreenHf/(rows)-((iy<=0||iy>=rows-1)?py*1.5f:py)}
#define ORectLayout(left, top, right, bottom)   Rect{(left), (top), (right) - (left), (bottom) - (top)}
#define ORectAlign                              onut::alignedRect
template<typename TparentRect, typename Tsize>
inline Rect ORectFit(const TparentRect& parentRect, const Tsize& size)
{
    Rect ret;

    float scale = std::min<>((float)parentRect.z / (float)size.x, (float)parentRect.w / (float)size.y);

    ret.x = (float)parentRect.x + (float)parentRect.z * .5f - (float)size.x * scale * .5f;
    ret.y = (float)parentRect.y + (float)parentRect.w * .5f - (float)size.y * scale * .5f;
    ret.z = (float)size.x * scale;
    ret.w = (float)size.y * scale;

    return std::move(ret);
}

// Color stuff
#define OColorHex(hex)                          Color::fromHexRGB(0x ## hex)

// Gradient
#define OGradientH(left, right)                 {left, left, right, right}
#define OGradientV(top, bottom)                 {top, bottom, bottom, top}

// Alignment
#define OTopLeft                                onut::Align::TOP_LEFT
#define OTop                                    onut::Align::TOP
#define OTopRight                               onut::Align::TOP_RIGHT
#define OLeft                                   onut::Align::LEFT
#define OCenter                                 onut::Align::CENTER
#define ORight                                  onut::Align::RIGHT
#define OBottomLeft                             onut::Align::BOTTOM_LEFT
#define OBottom                                 onut::Align::BOTTOM
#define OBottomRight                            onut::Align::BOTTOM_RIGHT

// Loops
#define ODontLoop                               onut::LoopType::NONE
#define OLoop                                   onut::LoopType::LOOP
#define OPingPong                               onut::LoopType::PINGPONG_ONCE
#define OPingPongLoop                           onut::LoopType::PINGPONG_LOOP

// Tweens
#define OTeleport                               onut::TweenType::NONE
#define OLinear                                 onut::TweenType::LINEAR
#define OEaseIn                                 onut::TweenType::EASE_IN
#define OEaseOut                                onut::TweenType::EASE_OUT
#define OEaseBoth                               onut::TweenType::EASE_BOTH
#define OBounceIn                               onut::TweenType::BOUNCE_IN
#define OBounceOut                              onut::TweenType::BOUNCE_OUT
#define OSpringIn                               onut::TweenType::SPRING_IN
#define OSpringOut                              onut::TweenType::SPRING_OUT

// Anim helpers
#define OSequence(T, ...)                       std::vector<onut::Anim<T>::KeyFrame>(__VA_ARGS__)
#define OAnimWait(val, t)                       {val,t,OTeleport}
#define OAnimAppleStyleBounce(from, to)         {from,0.f,OTeleport},{to,.25f,OEaseOut},{from,.5f,OBounceOut}
   
// Alignement of 1D position
#define OPosAbsX(x, p)                          ((x) + (p))
#define OPosAbsY(y, p)                          ((y) + (p))
#define OPosRelX(x, p)                          (OScreenWf - (x) - (p))
#define OPosRelY(y, p)                          (OScreenHf - (y) - (p))
