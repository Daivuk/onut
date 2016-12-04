#ifndef CURVE_H_INCLUDED
#define CURVE_H_INCLUDED

// Onut
#include <onut/Color.h>
#include <onut/Matrix.h>
#include <onut/Point.h>
#include <onut/Vector2.h>
#include <onut/Vector3.h>
#include <onut/Vector4.h>

// STL
#include <string>

namespace onut
{
    bool lerp(bool from, bool to, float t);
    int lerp(int from, int to, float t);
    unsigned int lerp(unsigned int from, unsigned int to, float t);
    float lerp(float from, float to, float t);
    double lerp(double from, double to, float t);
    Point lerp(const Point& from, const Point& to, float t);
    Vector2 lerp(const Vector2& from, const Vector2& to, float t);
    Vector3 lerp(const Vector3& from, const Vector3& to, float t);
    Vector4 lerp(const Vector4& from, const Vector4& to, float t);
    Matrix lerp(const Matrix& from, const Matrix& to, float t);
    Color lerp(const Color& from, const Color& to, float t);
    std::string lerp(const std::string& from, const std::string& to, float t);

    template<typename T>
    T bezier(const T& p0, const T& p1, const T& p2, const T& p3, float t)
    {
        float invT = 1.0f - t;
        return
            p0 * invT * invT * invT +
            p1 * t * invT * invT * 3.0f +
            p2 * t * t * invT * 3.0f +
            p3 * t * t * t;
    }
};

#define OLerp onut::lerp

#endif
