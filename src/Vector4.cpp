#include <onut/Matrix.h>
#include <onut/Renderer.h>
#include <onut/Vector2.h>
#include <onut/Vector3.h>
#include <onut/Vector4.h>

const Vector4 Vector4::Zero(0.f, 0.f, 0.f, 0.f);
const Vector4 Vector4::One(1.f, 1.f, 1.f, 1.f);
const Vector4 Vector4::UnitX(1.f, 0.f, 0.f, 0.f);
const Vector4 Vector4::UnitY(0.f, 1.f, 0.f, 0.f);
const Vector4 Vector4::UnitZ(0.f, 0.f, 1.f, 0.f);
const Vector4 Vector4::UnitW(0.f, 0.f, 0.f, 1.f);

Vector4::Vector4(const Vector2& pos, float _z, float _w)
    : x(pos.x), y(pos.y), z(_z), w(_w)
{
}
Vector4::Vector4(const Vector2& pos, const Vector2& size)
    : x(pos.x), y(pos.y), z(size.x), w(size.y)
{
}
Vector4::Vector4(float _x, float _y, const Vector2& size)
    : x(_x), y(_y), z(size.x), w(size.y)
{
}
Vector4::Vector4(const Vector3& pos, float _w)
    : x(pos.x), y(pos.y), z(pos.z), w(_w)
{
}
Vector4::Vector4(const Vector3& pos)
    : x(pos.x), y(pos.y), z(pos.z), w(1.0f)
{
}

Vector2 Vector4::TopLeft() const
{
    return std::move(Vector2{x, y});
}
Vector2 Vector4::Top() const
{
    return std::move(Vector2{x + z * .5f, y});
}
Vector2 Vector4::TopRight() const
{
    return std::move(Vector2{x + z, y});
}
Vector2 Vector4::Left() const
{
    return std::move(Vector2{x, y + w * .5f});
}
Vector2 Vector4::Center() const
{
    return std::move(Vector2{x + z * .5f, y + w * .5f});
}
Vector2 Vector4::Right() const
{
    return std::move(Vector2{x + z, y + w * .5f});
}
Vector2 Vector4::BottomLeft() const
{
    return std::move(Vector2{x, y + w});
}
Vector2 Vector4::Bottom() const
{
    return std::move(Vector2{x + z * .5f, y + w});
}
Vector2 Vector4::BottomRight() const
{
    return std::move(Vector2{x + z, y + w});
}

Vector2 Vector4::TopLeft(float offset) const
{
    return std::move(Vector2{x + offset, y + offset});
}
Vector2 Vector4::Top(float offset) const
{
    return std::move(Vector2{x + z * .5f, y + offset});
}
Vector2 Vector4::TopRight(float offset) const
{
    return std::move(Vector2{x + z - offset, y + offset});
}
Vector2 Vector4::Left(float offset) const
{
    return std::move(Vector2{x + offset, y + w * .5f});
}
Vector2 Vector4::Center(float offset) const
{
    return std::move(Vector2{x + z * .5f, y + w * .5f});
}
Vector2 Vector4::Right(float offset) const
{
    return std::move(Vector2{x + z - offset, y + w * .5f});
}
Vector2 Vector4::BottomLeft(float offset) const
{
    return std::move(Vector2{x + offset, y + w - offset});
}
Vector2 Vector4::Bottom(float offset) const
{
    return std::move(Vector2{x + z * .5f, y + w - offset});
}
Vector2 Vector4::BottomRight(float offset) const
{
    return std::move(Vector2{x + z - offset, y + w - offset});
}

Vector2 Vector4::TopLeft(const Vector2& offset) const
{
    return std::move(Vector2{x + offset.x, y + offset.y});
}
Vector2 Vector4::TopRight(const Vector2& offset) const
{
    return std::move(Vector2{x + z - offset.x, y + offset.y});
}
Vector2 Vector4::BottomLeft(const Vector2& offset) const
{
    return std::move(Vector2{x + offset.x, y + w - offset.y});
}
Vector2 Vector4::BottomRight(const Vector2& offset) const
{
    return std::move(Vector2{x + z - offset.x, y + w - offset.y});
}

Vector4 Vector4::Fill(const Vector2& size) const
{
    float maxScale = std::max<float>(z / size.x, w / size.y);
    return std::move(Center({0, 0, size * maxScale}));
}

Vector4 Vector4::Fit(const Vector2& size) const
{
    Rect ret;

    float scale = std::min<>(z / size.x, w / size.y);

    ret.x = x + z * .5f - size.x * scale * .5f;
    ret.y = y + w * .5f - size.y * scale * .5f;
    ret.z = size.x * scale;
    ret.w = size.y * scale;

    return std::move(ret);
}

float Vector4::Distance(const Vector2& p) const
{
    auto dx = std::max<float>(x - p.x, p.x - (x + z));
    auto dy = std::max<float>(y - p.y, p.y - (y + w));
    dx = std::max<float>(0, dx);
    dy = std::max<float>(0, dy);
    return std::sqrt(dx*dx + dy*dy);
}

void Vector4::Transform(const Vector4& v, const Matrix& m, Vector4& result)
{
    result.x = (m._11 * v.x) + (m._21 * v.y) + (m._31 * v.z) + (m._41 * v.w);
    result.y = (m._12 * v.x) + (m._22 * v.y) + (m._32 * v.z) + (m._42 * v.w);
    result.z = (m._13 * v.x) + (m._23 * v.y) + (m._33 * v.z) + (m._43 * v.w);
    result.w = (m._14 * v.x) + (m._24 * v.y) + (m._34 * v.z) + (m._44 * v.w);
}

Vector4 Vector4::Transform(const Vector4& v, const Matrix& m)
{
    Vector4 result;
    result.x = (m._11 * v.x) + (m._21 * v.y) + (m._31 * v.z) + (m._41 * v.w);
    result.y = (m._12 * v.x) + (m._22 * v.y) + (m._32 * v.z) + (m._42 * v.w);
    result.z = (m._13 * v.x) + (m._23 * v.y) + (m._33 * v.z) + (m._43 * v.w);
    result.w = (m._14 * v.x) + (m._24 * v.y) + (m._34 * v.z) + (m._44 * v.w);
    return std::move(result);
}

bool Vector4::Contains(const Vector2& point) const
{
    return
        point.x >= x &&
        point.x <= x + z &&
        point.y >= y &&
        point.y <= y + w;
}

namespace onut
{
    Rect alignedRect(float xOffset, float yOffset, float width, float height, float padding, Align align)
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

    Vector2 alignedRect(const Rect& rect, Align align)
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
}
