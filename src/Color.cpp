// Onut
#include <onut/Color.h>
#include <onut/Vector3.h>
#include <onut/Vector4.h>

const Color Color::Black(0.0f, 0.0f, 0.0f, 1.0f);
const Color Color::White(1.0f, 1.0f, 1.0f, 1.0f);
const Color Color::Transparent(0.0f, 0.0f, 0.0f, 0.0f);
const Color Color::TransparentWhite(1.0f, 1.0f, 1.0f, 0.0f);

Color::Color(const Vector3& rgb, float _a) : r(rgb.x), g(rgb.y), b(rgb.z), a(_a) {}
Color::Color(const Vector3& clr) : r(clr.x), g(clr.y), b(clr.z), a(1.f) {}
Color::Color(const Vector4& clr) : r(clr.x), g(clr.y), b(clr.z), a(clr.w) {}

Vector3 Color::ToVector3() const { return Vector3(r, g, b); }
Vector4 Color::ToVector4() const { return Vector4(r, g, b, a); }

// Binary operators
Color operator+ (const Color& C1, const Color& C2)
{
    Color result;
    result.r = C1.r + C2.r;
    result.g = C1.g + C2.g;
    result.b = C1.b + C2.b;
    result.a = C1.a + C2.a;
    return std::move(result);
}

Color operator- (const Color& C1, const Color& C2)
{
    Color result;
    result.r = C1.r - C2.r;
    result.g = C1.g - C2.g;
    result.b = C1.b - C2.b;
    result.a = C1.a - C2.a;
    return std::move(result);
}

Color operator* (const Color& C1, const Color& C2)
{
    Color result;
    result.r = C1.r * C2.r;
    result.g = C1.g * C2.g;
    result.b = C1.b * C2.b;
    result.a = C1.a * C2.a;
    return std::move(result);
}

Color operator* (const Color& C, float S)
{
    Color result;
    result.r = C.r * S;
    result.g = C.g * S;
    result.b = C.b * S;
    result.a = C.a * S;
    return std::move(result);
}

Color operator/ (const Color& C1, const Color& C2)
{
    Color result;
    result.r = C1.r / C2.r;
    result.g = C1.g / C2.g;
    result.b = C1.b / C2.b;
    result.a = C1.a / C2.a;
    return std::move(result);
}

Color operator* (float S, const Color& C)
{
    Color result;
    result.r = C.r * S;
    result.g = C.g * S;
    result.b = C.b * S;
    result.a = C.a * S;
    return std::move(result);
}
