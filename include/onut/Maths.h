#pragma once
// Third party
#include <simplemath/SimpleMath.h>

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
#define OColorRGB(r, g, b)                      Color(static_cast<float>(r) / 255.0f, static_cast<float>(g) / 255.0f, static_cast<float>(b) / 255.0f)
#define OColorRGBA(r, g, b, a)                  Color(static_cast<float>(r) / 255.0f, static_cast<float>(g) / 255.0f, static_cast<float>(b) / 255.0f, static_cast<float>(a) / 255.0f)
#define OColorBGR(b, g, r)                      Color(static_cast<float>(r) / 255.0f, static_cast<float>(g) / 255.0f, static_cast<float>(b) / 255.0f)
#define OColorBGRA(b, g, r, a)                  Color(static_cast<float>(r) / 255.0f, static_cast<float>(g) / 255.0f, static_cast<float>(b) / 255.0f, static_cast<float>(a) / 255.0f)

// Gradient
#define OGradientH(left, right)                 {left, left, right, right}
#define OGradientV(top, bottom)                 {top, bottom, bottom, top}

#ifdef max
#undef max
#endif
#ifdef min
#undef min
#endif

namespace onut
{
    template<typename Tsize>
    Tsize max(Tsize a, Tsize b)
    {
        return std::max<Tsize>(a, b);
    }

    template<typename Tsize, typename ... Targs>
    Tsize max(Tsize a, Tsize b, Targs ... args)
    {
        return std::max<Tsize>(a, max(b, args...));
    }

    template<typename Tsize>
    Tsize min(Tsize a, Tsize b)
    {
        return std::min<Tsize>(a, b);
    }

    template<typename Tsize, typename ... Targs>
    Tsize min(Tsize a, Tsize b, Targs ... args)
    {
        return std::min<Tsize>(a, min(b, args...));
    }

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

inline Vector4 ORectLocalToWorld(const Vector4& local, const Vector4& parent)
{
    auto ret = local;
    ret.x *= parent.z;
    ret.y *= parent.w;
    ret.x += parent.x;
    ret.y += parent.y;
    ret.z *= parent.z;
    ret.w *= parent.w;
    return std::move(ret);
}

inline Vector4 ORectWorldToLocal(const Vector4& world, const Vector4& parent)
{
    auto ret = world;
    ret.x -= parent.x;
    ret.y -= parent.y;
    ret.x /= parent.z;
    ret.y /= parent.w;
    ret.z /= parent.z;
    ret.w /= parent.w;
    return std::move(ret);
}
