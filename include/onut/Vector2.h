#ifndef VECTOR2_H_INCLUDED
#define VECTOR2_H_INCLUDED

// Onut
#include <onut/Align.h>

#ifdef max
#undef max
#endif
#ifdef min
#undef min
#endif

// STL
#include <algorithm>
#include <cmath>

// Forward Declarations
struct Matrix;
struct Vector2;
struct Vector3;
struct Vector4;

Vector2 operator+ (const Vector2& V1, const Vector2& V2);
Vector2 operator- (const Vector2& V1, const Vector2& V2);
Vector2 operator* (const Vector2& V1, const Vector2& V2);
Vector2 operator* (const Vector2& V, float S);
Vector2 operator/ (const Vector2& V1, const Vector2& V2);
Vector2 operator/ (const Vector2& V1, float S);
Vector2 operator* (float S, const Vector2& V);

struct Vector2
{
    float x, y;

    Vector2() : x(0.f), y(0.f) {}
    explicit Vector2(float _x) : x(_x), y(_x) {}
    Vector2(float _x, float _y) : x(_x), y(_y) {}
    Vector2(const Vector3& v3);
    explicit Vector2(const float *pArray) : x(pArray[0]), y(pArray[1]) {}
    Vector2(onut::Align align)
    {
        switch (align)
        {
            case onut::Align::TopLeft:
                x = 0; y = 0;
                break;
            case onut::Align::Top:
                x = .5f; y = 0;
                break;
            case onut::Align::TopRight:
                x = 1; y = 0;
                break;
            case onut::Align::Left:
                x = 0; y = .5f;
                break;
            case onut::Align::Right:
                x = 1; y = .5f;
                break;
            case onut::Align::BottomLeft:
                x = 0; y = 1;
                break;
            case onut::Align::Bottom:
                x = .5f; y = 1;
                break;
            case onut::Align::BottomRight:
                x = 1; y = 1;
                break;
            case onut::Align::Center:
            default:
                x = .5f; y = .5f;
                break;
        }
    }

    // Comparison operators
    inline bool operator==(const Vector2& V) const
    {
        return x == V.x && y == V.y;
    }
    inline bool operator!=(const Vector2& V) const
    {
        return x != V.x || y != V.y;
    }

    // Assignment operators
    Vector2& operator= (const Vector2& V) { x = V.x; y = V.y; return *this; }
    inline Vector2& operator+= (const Vector2& V)
    {
        x += V.x;
        y += V.y;
        return *this;
    }

    inline Vector2& operator-= (const Vector2& V)
    {
        x -= V.x;
        y -= V.y;
        return *this;
    }

    inline Vector2& operator*= (const Vector2& V)
    {
        x *= V.x;
        y *= V.y;
        return *this;
    }

    inline Vector2& operator*= (float S)
    {
        x *= S;
        y *= S;
        return *this;
    }

    inline Vector2& operator/= (float S)
    {
        x /= S;
        y /= S;
        return *this;
    }

    // Unary operators
    Vector2 operator+ () const { return *this; }
    Vector2 operator- () const { return Vector2(-x, -y); }

    // Vector operations
    inline bool InBounds(const Vector2& Bounds) const
    {
        return (((x <= Bounds.x && x >= -Bounds.x) &&
            (y <= Bounds.y && y >= -Bounds.y)) != 0);
    }

    inline float Length() const
    {
        return std::sqrt(x * x + y * y);
    }

    inline float LengthSquared() const
    {
        return x * x + y * y;
    }

    inline float Dot(const Vector2& V) const
    {
        return x * V.x + y * V.y;
    }

    void Cross(const Vector2& V, Vector3& result) const;
    Vector3 Cross(const Vector2& V) const;

    inline void Normalize()
    {
        auto len = std::sqrt(x * x + y * y);
        if (len > 0.0f)
        {
            len = 1.0f / len;
        }
        x *= len;
        y *= len;
    }

    inline void Normalize(Vector2& result) const
    {
        auto len = std::sqrt(x * x + y * y);
        if (len > 0.0f)
        {
            len = 1.0f / len;
        }
        result.x = x * len;
        result.y = y * len;
    }

    inline void Clamp(const Vector2& vmin, const Vector2& vmax)
    {
        x = std::max(vmin.x, x);
        x = std::min(vmax.x, x);
        y = std::max(vmin.y, y);
        y = std::min(vmax.y, y);
    }

    inline void Clamp(const Vector2& vmin, const Vector2& vmax, Vector2& result) const
    {
        result.x = std::max(vmin.x, x);
        result.x = std::min(vmax.x, result.x);
        result.y = std::max(vmin.y, y);
        result.y = std::min(vmax.y, result.y);
    }

    // Static functions
    static inline float Distance(const Vector2& v1, const Vector2& v2)
    {
        auto diff = v2 - v1;
        return diff.Length();
    }

    static inline float DistanceSquared(const Vector2& v1, const Vector2& v2)
    {
        auto diff = v2 - v1;
        return diff.LengthSquared();
    }

    static inline void Min(const Vector2& v1, const Vector2& v2, Vector2& result)
    {
        result.x = std::min(v1.x, v2.x);
        result.y = std::min(v1.y, v2.y);
    }

    static inline Vector2 Min(const Vector2& v1, const Vector2& v2)
    {
        Vector2 result;
        result.x = std::min(v1.x, v2.x);
        result.y = std::min(v1.y, v2.y);
        return result;
    }

    static inline void Max(const Vector2& v1, const Vector2& v2, Vector2& result)
    {
        result.x = std::max(v1.x, v2.x);
        result.y = std::max(v1.y, v2.y);
    }

    static inline Vector2 Max(const Vector2& v1, const Vector2& v2)
    {
        Vector2 result;
        result.x = std::max(v1.x, v2.x);
        result.y = std::max(v1.y, v2.y);
        return result;
    }

    static inline void Lerp(const Vector2& v1, const Vector2& v2, float t, Vector2& result)
    {
        result = v1 + (v2 - v1) * t;
    }

    static inline Vector2 Lerp(const Vector2& v1, const Vector2& v2, float t)
    {
        return v1 + (v2 - v1) * t;
    }

    static inline void SmoothStep(const Vector2& v1, const Vector2& v2, float t, Vector2& result)
    {
        t = (t > 1.0f) ? 1.0f : ((t < 0.0f) ? 0.0f : t);  // Clamp value to 0 to 1
        t = t*t*(3.f - 2.f*t);
        result = v1 + (v2 - v1) * t;
    }

    static inline Vector2 SmoothStep(const Vector2& v1, const Vector2& v2, float t)
    {
        t = (t > 1.0f) ? 1.0f : ((t < 0.0f) ? 0.0f : t);  // Clamp value to 0 to 1
        t = t*t*(3.f - 2.f*t);
        return v1 + (v2 - v1) * t;
    }

    static inline void Barycentric(const Vector2& v1, const Vector2& v2, const Vector2& v3, float f, float g, Vector2& result)
    {
        auto p10 = v2 - v1;
        auto p20 = v3 - v1;
        result = v1 + p10 * f;
        result = result + p20 * g;
    }

    static inline Vector2 Barycentric(const Vector2& v1, const Vector2& v2, const Vector2& v3, float f, float g)
    {
        Vector2 result;
        auto p10 = v2 - v1;
        auto p20 = v3 - v1;
        result = v1 + p10 * f;
        result = result + p20 * g;
        return result;
    }

    static inline void CatmullRom(const Vector2& v1, const Vector2& v2, const Vector2& v3, const Vector2& v4, float t, Vector2& result)
    {
        float t2 = t * t;
        float t3 = t * t2;

        auto P0 = (-t3 + 2.0f * t2 - t) * 0.5f;
        auto P1 = (3.0f * t3 - 5.0f * t2 + 2.0f) * 0.5f;
        auto P2 = (-3.0f * t3 + 4.0f * t2 + t) * 0.5f;
        auto P3 = (t3 - t2) * 0.5f;

        result = P0 * v1;
        result += v2 * P1;
        result += v3 * P2;
        result += v4 * P3;
    }

    static inline Vector2 CatmullRom(const Vector2& v1, const Vector2& v2, const Vector2& v3, const Vector2& v4, float t)
    {
        Vector2 result;
        float t2 = t * t;
        float t3 = t * t2;

        auto P0 = (-t3 + 2.0f * t2 - t) * 0.5f;
        auto P1 = (3.0f * t3 - 5.0f * t2 + 2.0f) * 0.5f;
        auto P2 = (-3.0f * t3 + 4.0f * t2 + t) * 0.5f;
        auto P3 = (t3 - t2) * 0.5f;

        result = P0 * v1;
        result += v2 * P1;
        result += v3 * P2;
        result += v4 * P3;

        return result;
    }

    static inline void Hermite(const Vector2& Position0, const Vector2& Tangent0, const Vector2& Position1, const Vector2& Tangent1, float t, Vector2& result)
    {
        float t2 = t * t;
        float t3 = t * t2;

        auto P0 = (2.0f * t3 - 3.0f * t2 + 1.0f);
        auto T0 = (t3 - 2.0f * t2 + t);
        auto P1 = (-2.0f * t3 + 3.0f * t2);
        auto T1 = (t3 - t2);

        result = (P0 * Position0);
        result = result + Tangent0 * T0;
        result = result + Position1 * P1;
        result = result + Tangent1 * T1;
    }

    static inline Vector2 Hermite(const Vector2& Position0, const Vector2& Tangent0, const Vector2& Position1, const Vector2& Tangent1, float t)
    {
        Vector2 result;
        float t2 = t * t;
        float t3 = t * t2;

        auto P0 = (2.0f * t3 - 3.0f * t2 + 1.0f);
        auto T0 = (t3 - 2.0f * t2 + t);
        auto P1 = (-2.0f * t3 + 3.0f * t2);
        auto T1 = (t3 - t2);

        result = (P0 * Position0);
        result = result + Tangent0 * T0;
        result = result + Position1 * P1;
        result = result + Tangent1 * T1;
        return result;
    }

    static inline void Reflect(const Vector2& Incident, const Vector2& Normal, Vector2& result)
    {
        auto dot = Incident.Dot(Normal);
        dot *= 2.0f;
        result = Incident - Normal * dot;
    }

    static inline Vector2 Reflect(const Vector2& Incident, const Vector2& Normal)
    {
        auto dot = Incident.Dot(Normal);
        dot *= 2.0f;
        return+ Incident - Normal * dot;
    }

    static inline void Refract(const Vector2& Incident, const Vector2& Normal, float RefractionIndex, Vector2& result)
    {
        float IDotN = (Incident.x * Normal.x) + (Incident.y * Normal.y);
        // R = 1.0f - RefractionIndex * RefractionIndex * (1.0f - IDotN * IDotN)
        float RY = 1.0f - (IDotN*IDotN);
        float RX = 1.0f - (RY*RefractionIndex * RefractionIndex);
        RY = 1.0f - (RY*RefractionIndex * RefractionIndex);
        if (RX >= 0.0f)
        {
            RX = (RefractionIndex * Incident.x) - (Normal.x * ((RefractionIndex * IDotN) + std::sqrt(RX)));
        }
        else
        {
            RX = 0.0f;
        }
        if (RY >= 0.0f)
        {
            RY = (RefractionIndex * Incident.y) - (Normal.y * ((RefractionIndex * IDotN) + std::sqrt(RY)));
        }
        else
        {
            RY = 0.0f;
        }

        result.x = RX;
        result.y = RY;
    }

    static inline Vector2 Refract(const Vector2& Incident, const Vector2& Normal, float RefractionIndex)
    {
        float IDotN = (Incident.x * Normal.x) + (Incident.y * Normal.y);
        // R = 1.0f - RefractionIndex * RefractionIndex * (1.0f - IDotN * IDotN)
        float RY = 1.0f - (IDotN*IDotN);
        float RX = 1.0f - (RY*RefractionIndex * RefractionIndex);
        RY = 1.0f - (RY*RefractionIndex * RefractionIndex);
        if (RX >= 0.0f)
        {
            RX = (RefractionIndex * Incident.x) - (Normal.x * ((RefractionIndex * IDotN) + std::sqrt(RX)));
        }
        else
        {
            RX = 0.0f;
        }
        if (RY >= 0.0f)
        {
            RY = (RefractionIndex * Incident.y) - (Normal.y * ((RefractionIndex * IDotN) + std::sqrt(RY)));
        }
        else
        {
            RY = 0.0f;
        }

        Vector2 result;
        result.x = RX;
        result.y = RY;
        return result;
    }

    static void Transform(const Vector2& v, const Matrix& m, Vector2& result);
    static Vector2 Transform(const Vector2& v, const Matrix& m);
    static void TransformNormal(const Vector2& v, const Matrix& m, Vector2& result);
    static Vector2 TransformNormal(const Vector2& v, const Matrix& m);

    // Constants
    static const Vector2 Zero;
    static const Vector2 One;
    static const Vector2 UnitX;
    static const Vector2 UnitY;

    inline Vector2& operator=(onut::Align align)
    {
        switch (align)
        {
            case onut::Align::TopLeft:
                x = 0; y = 0;
                break;
            case onut::Align::Top:
                x = .5f; y = 0;
                break;
            case onut::Align::TopRight:
                x = 1; y = 0;
                break;
            case onut::Align::Left:
                x = 0; y = .5f;
                break;
            case onut::Align::Center:
                x = .5f; y = .5f;
                break;
            case onut::Align::Right:
                x = 1; y = .5f;
                break;
            case onut::Align::BottomLeft:
                x = 0; y = 1;
                break;
            case onut::Align::Bottom:
                x = .5f; y = 1;
                break;
            case onut::Align::BottomRight:
                x = 1; y = 1;
                break;
        }
        return *this;
    }
};

// Binary operators
inline Vector2 operator+ (const Vector2& V1, const Vector2& V2)
{
    return Vector2(
        V1.x + V2.x, 
        V1.y + V2.y);
}

inline Vector2 operator- (const Vector2& V1, const Vector2& V2)
{
    return Vector2(
        V1.x - V2.x,
        V1.y - V2.y);
}

inline Vector2 operator* (const Vector2& V1, const Vector2& V2)
{
    return Vector2(
        V1.x * V2.x,
        V1.y * V2.y);
}

inline Vector2 operator* (const Vector2& V, float S)
{
    return Vector2(
        V.x * S,
        V.y * S);
}

inline Vector2 operator/ (const Vector2& V1, const Vector2& V2)
{
    return Vector2(
        V1.x / V2.x,
        V1.y / V2.y);
}

inline Vector2 operator/ (const Vector2& V1, float S)
{
    return Vector2(
        V1.x / S,
        V1.y / S);
}

inline Vector2 operator* (float S, const Vector2& V)
{
    return Vector2(
        V.x * S,
        V.y * S);
}

namespace onut
{
    Vector2 tilesCollision(const Vector2& from, const Vector2& to, const Vector2& size,
                           bool* pPassableTiles, int width, int height, int tileSize);
    Vector2 tilesCollision(const Vector2& from, const Vector2& to, const Vector2& size,
                           float* pPassableTiles, int width, int height, int tileSize);
}

#endif
