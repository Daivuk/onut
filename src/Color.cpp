#include <onut/Color.h>

const Color Color::Black(0.0f, 0.0f, 0.0f, 1.0f);
const Color Color::White(1.0f, 1.0f, 1.0f, 1.0f);
const Color Color::Transparent(0.0f, 0.0f, 0.0f, 0.0f);
const Color Color::TransparentWhite(1.0f, 1.0f, 1.0f, 0.0f);

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
