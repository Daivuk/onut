#pragma once
#include "simplemath/SimpleMath.h"

using namespace DirectX::SimpleMath;

// Alignment
#define OTopLeft onut::Align::TopLeft
#define OTop onut::Align::Top
#define OTopRight onut::Align::TopRight
#define OLeft onut::Align::Left
#define OCenter onut::Align::Center
#define ORight onut::Align::Right
#define OBottomLeft onut::Align::BottomLeft
#define OBottom onut::Align::Bottom
#define OBottomRight onut::Align::BottomRight

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
