#pragma once
// Onut
#include <onut/Maths.h>
#include <onut/Point.h>

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
};

#define OLerp onut::lerp
