#ifndef VECTOR3_H_INCLUDED
#define VECTOR3_H_INCLUDED


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

Vector3 operator+(const Vector3& V1, const Vector3& V2);
Vector3 operator-(const Vector3& V1, const Vector3& V2);
Vector3 operator*(const Vector3& V1, const Vector3& V2);
Vector3 operator*(const Vector3& V, float S);
Vector3 operator/(const Vector3& V1, const Vector3& V2);
Vector3 operator*(float S, const Vector3& V);

struct Vector3
{
    float x, y, z;

    Vector3() : x(0.f), y(0.f), z(0.f) {}
    explicit Vector3(float _s) : x(_s), y(_s), z(_s) {}
    Vector3(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}
    Vector3(const Vector2& v2, float _z);
    Vector3(const Vector2& v2);
    explicit Vector3(const float *pArray) : x(pArray[0]), y(pArray[1]), z(pArray[2]) {}

    // Comparison operators
    bool operator == (const Vector3& V) const
    {
        return x == V.x && y == V.y && z == V.z;
    }
    bool operator != (const Vector3& V) const
    {
        return x != V.x || y != V.y || z != V.z;
    }

    // Assignment operators
    Vector3& operator= (const Vector3& V) { x = V.x; y = V.y; z = V.z; return *this; }
    Vector3& operator+= (const Vector3& V) { x += V.x; y += V.y; z += V.z; return *this; }
    Vector3& operator-= (const Vector3& V) { x -= V.x; y -= V.y; z -= V.z; return *this; }
    Vector3& operator*= (const Vector3& V) { x *= V.x; y *= V.y; z *= V.z; return *this; }
    Vector3& operator*= (float S) { x *= S; y *= S; z *= S; return *this; }
    Vector3& operator/= (float S) 
    {
        auto invS = 1.0f / S;
        x *= invS; y *= invS; z *= invS;
        return *this;
    }

    // Urnary operators
    Vector3 operator+ () const { return *this; }
    Vector3 operator- () const
    {
        return Vector3(-x, -y, -z);
    }

    // Vector operations
    bool InBounds(const Vector3& Bounds) const
    {
        return !(
            (x <= Bounds.x && x >= -Bounds.x) &&
            (y <= Bounds.y && y >= -Bounds.y) &&
            (z <= Bounds.z && z >= -Bounds.z));
    }

    float Length() const
    {
        return std::sqrt(x * x + y * y + z * z);
    }
    float LengthSquared() const
    {
        return x * x + y * y + z * z;
    }

    float Dot(const Vector3& V) const
    {
        return x * V.x + y * V.y + z * V.z;
    }
    void Cross(const Vector3& V, Vector3& result) const
    {
        result.x = y * V.z - z * V.y;
        result.y = z * V.x - x * V.z;
        result.z = x * V.y - y * V.x;
    }
    Vector3 Cross(const Vector3& V) const
    {
        Vector3 result;
        result.x = y * V.z - z * V.y;
        result.y = z * V.x - x * V.z;
        result.z = x * V.y - y * V.x;
        return std::move(result);
    }

    void Normalize()
    {
        auto len = std::sqrt(x * x + y * y + z * z);
        if (len > 0.0f)
        {
            len = 1.0f / len;
        }
        x *= len;
        y *= len;
        z *= len;
    }
    void Normalize(Vector3& result) const
    {
        auto len = std::sqrt(x * x + y * y + z * z);
        if (len > 0.0f)
        {
            len = 1.0f / len;
        }
        result.x = x * len;
        result.y = y * len;
        result.z = z * len;
    }

    void Clamp(const Vector3& vmin, const Vector3& vmax)
    {
        x = std::max(vmin.x, x);
        x = std::min(vmax.x, x);
        y = std::max(vmin.y, y);
        y = std::min(vmax.y, y);
        z = std::max(vmin.z, z);
        z = std::min(vmax.z, z);
    }
    void Clamp(const Vector3& vmin, const Vector3& vmax, Vector3& result) const
    {
        result.x = std::max(vmin.x, x);
        result.x = std::min(vmax.x, result.x);
        result.y = std::max(vmin.y, y);
        result.y = std::min(vmax.y, result.y);
        result.z = std::max(vmin.z, z);
        result.z = std::min(vmax.z, result.z);
    }

    // Static functions
    static float Distance(const Vector3& v1, const Vector3& v2)
    {
        auto diff = v2 - v1;
        return diff.Length();
    }

    static float DistanceSquared(const Vector3& v1, const Vector3& v2)
    {
        auto diff = v2 - v1;
        return diff.LengthSquared();
    }

    static void Min(const Vector3& v1, const Vector3& v2, Vector3& result)
    {
        result.x = std::min(v1.x, v2.x);
        result.y = std::min(v1.y, v2.y);
        result.z = std::min(v1.z, v2.z);
    }

    static Vector3 Min(const Vector3& v1, const Vector3& v2)
    {
        Vector3 result;
        result.x = std::min(v1.x, v2.x);
        result.y = std::min(v1.y, v2.y);
        result.z = std::min(v1.z, v2.z);
        return result;
    }

    static void Max(const Vector3& v1, const Vector3& v2, Vector3& result)
    {
        result.x = std::max(v1.x, v2.x);
        result.y = std::max(v1.y, v2.y);
        result.z = std::max(v1.z, v2.z);
    }
    static Vector3 Max(const Vector3& v1, const Vector3& v2)
    {
        Vector3 result;
        result.x = std::max(v1.x, v2.x);
        result.y = std::max(v1.y, v2.y);
        result.z = std::max(v1.z, v2.z);
        return result;
    }

    static void Lerp(const Vector3& v1, const Vector3& v2, float t, Vector3& result)
    {
        result = v1 + (v2 - v1) * t;
    }
    static Vector3 Lerp(const Vector3& v1, const Vector3& v2, float t)
    {
        return v1 + (v2 - v1) * t;
    }

    static void SmoothStep(const Vector3& v1, const Vector3& v2, float t, Vector3& result)
    {
        t = (t > 1.0f) ? 1.0f : ((t < 0.0f) ? 0.0f : t);  // Clamp value to 0 to 1
        t = t*t*(3.f - 2.f*t);
        result = v1 + (v2 - v1) * t;
    }
    static Vector3 SmoothStep(const Vector3& v1, const Vector3& v2, float t)
    {
        t = (t > 1.0f) ? 1.0f : ((t < 0.0f) ? 0.0f : t);  // Clamp value to 0 to 1
        t = t*t*(3.f - 2.f*t);
        return v1 + (v2 - v1) * t;
    }

    static void Barycentric(const Vector3& v1, const Vector3& v2, const Vector3& v3, float f, float g, Vector3& result)
    {
        auto p10 = v2 - v1;
        auto p20 = v3 - v1;
        result = v1 + p10 * f;
        result = result + p20 * g;
    }
    static Vector3 Barycentric(const Vector3& v1, const Vector3& v2, const Vector3& v3, float f, float g)
    {
        Vector3 result;
        auto p10 = v2 - v1;
        auto p20 = v3 - v1;
        result = v1 + p10 * f;
        result = result + p20 * g;
        return result;
    }

    static void CatmullRom(const Vector3& v1, const Vector3& v2, const Vector3& v3, const Vector3& v4, float t, Vector3& result)
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
    static Vector3 CatmullRom(const Vector3& v1, const Vector3& v2, const Vector3& v3, const Vector3& v4, float t)
    {
        Vector3 result;
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

    static void Hermite(const Vector3& Position0, const Vector3& Tangent0, const Vector3& Position1, const Vector3& Tangent1, float t, Vector3& result)
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
    static Vector3 Hermite(const Vector3& Position0, const Vector3& Tangent0, const Vector3& Position1, const Vector3& Tangent1, float t)
    {
        Vector3 result;
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

    static void Reflect(const Vector3& Incident, const Vector3& Normal, Vector3& result)
    {
        auto dot = Incident.Dot(Normal);
        dot *= 2.0f;
        result = Incident - Normal * dot;
    }
    static Vector3 Reflect(const Vector3& Incident, const Vector3& Normal)
    {
        auto dot = Incident.Dot(Normal);
        dot *= 2.0f;
        return+Incident - Normal * dot;
    }

    static void Refract(const Vector3& Incident, const Vector3& Normal, float RefractionIndex, Vector3& result)
    {
        float IDotN = Incident.Dot(Normal);

        auto R = 1.0f - RefractionIndex * RefractionIndex * (1.0f - IDotN * IDotN);

        if (R <= 0.0f)
        {
            // Total internal reflection
            result = Vector3::Zero;
        }
        else
        {
            R = RefractionIndex * IDotN + std::sqrt(R);
            result = RefractionIndex * Incident - Normal * R;
        }
    }
    static Vector3 Refract(const Vector3& Incident, const Vector3& Normal, float RefractionIndex)
    {
        Vector3 result;
        float IDotN = Incident.Dot(Normal);

        auto R = 1.0f - RefractionIndex * RefractionIndex * (1.0f - IDotN * IDotN);

        if (R <= 0.0f)
        {
            // Total internal reflection
            result = Vector3::Zero;
        }
        else
        {
            R = RefractionIndex * IDotN + std::sqrt(R);
            result = RefractionIndex * Incident - Normal * R;
        }
        return std::move(result);
    }

    static void Transform(const Vector3& v, const Matrix& m, Vector3& result);
    static Vector3 Transform(const Vector3& v, const Matrix& m);
    static void TransformNormal(const Vector3& v, const Matrix& m, Vector3& result);
    static Vector3 TransformNormal(const Vector3& v, const Matrix& m);

    // Constants
    static const Vector3 Zero;
    static const Vector3 One;
    static const Vector3 UnitX;
    static const Vector3 UnitY;
    static const Vector3 UnitZ;
    static const Vector3 Up;
    static const Vector3 Down;
    static const Vector3 Right;
    static const Vector3 Left;
    static const Vector3 Forward;
    static const Vector3 Backward;
};

// Binary operators
inline Vector3 operator+(const Vector3& V1, const Vector3& V2)
{
    return Vector3(
        V1.x + V2.x,
        V1.y + V2.y,
        V1.z + V2.z);
}

inline Vector3 operator-(const Vector3& V1, const Vector3& V2)
{
    return Vector3(
        V1.x - V2.x,
        V1.y - V2.y,
        V1.z - V2.z);
}

inline Vector3 operator*(const Vector3& V1, const Vector3& V2)
{
    return Vector3(
        V1.x * V2.x,
        V1.y * V2.y,
        V1.z * V2.z);
}

inline Vector3 operator*(const Vector3& V, float S)
{
    return Vector3(
        V.x * S,
        V.y * S,
        V.z * S);
}

inline Vector3 operator/(const Vector3& V1, const Vector3& V2)
{
    return Vector3(
        V1.x / V2.x,
        V1.y / V2.y,
        V1.z / V2.z);
}

inline Vector3 operator*(float S, const Vector3& V)
{
    return Vector3(
        V.x * S,
        V.y * S,
        V.z * S);
}

#endif
