#ifndef MATHS_H_INCLUDED
#define MATHS_H_INCLUDED

#include <onut/Color.h>
#include <onut/Curve.h>
#include <onut/iRect.h>
#include <onut/Matrix.h>
#include <onut/Point.h>
#include <onut/Tween.h>
#include <onut/Vector2.h>
#include <onut/Vector3.h>
#include <onut/Vector4.h>

const float GOLDEN_RATIO = 1.6180339887498948482f;
const float GOLDEN_SECOND = 1.0f / 1.6180339887498948482f;
const float GOLDEN_FIRST = 1.0f - GOLDEN_SECOND;

const float OPI = 3.141592654f;
const float O2PI = 6.283185307f;
const float O1DIVPI = 0.318309886f;
const float O1DIV2PI = 0.159154943f;
const float OPIDIV2 = 1.570796327f;
const float OPIDIV4 = 0.785398163f;

inline float OConvertToRadians(float fDegrees) { return fDegrees * (OPI / 180.0f); }
inline float OConvertToDegrees(float fRadians) { return fRadians * (180.0f / OPI); }

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

#endif
