#include <onut/Matrix.h>
#include <onut/Vector2.h>
#include <onut/Vector3.h>

const Vector3 Vector3::Zero(0.f, 0.f, 0.f);
const Vector3 Vector3::One(1.f, 1.f, 1.f);
const Vector3 Vector3::UnitX(1.f, 0.f, 0.f);
const Vector3 Vector3::UnitY(0.f, 1.f, 0.f);
const Vector3 Vector3::UnitZ(0.f, 0.f, 1.f);
const Vector3 Vector3::Up(0.f, 0.f, 1.f);
const Vector3 Vector3::Down(0.f, 0.f, -1.f);
const Vector3 Vector3::Right(1.f, 0.f, 0.f);
const Vector3 Vector3::Left(-1.f, 0.f, 0.f);
const Vector3 Vector3::Forward(0.f, 1.f, 0.f);
const Vector3 Vector3::Backward(0.f, -1.f, 0.f);

Vector3::Vector3(const Vector2& v2, float _z) 
    : x(v2.x), y(v2.y), z(_z)
{
}

Vector3::Vector3(const Vector2& v2) 
    : x(v2.x), y(v2.y), z(0.f)
{
}

void Vector3::Transform(const Vector3& v, const Matrix& m, Vector3& result)
{
    float w = 0.0f;
    result.x = (m._11 * v.x) + (m._21 * v.y) + (m._31 * v.z) + m._41;
    result.y = (m._12 * v.x) + (m._22 * v.y) + (m._32 * v.z) + m._42;
    result.z = (m._13 * v.x) + (m._23 * v.y) + (m._33 * v.z) + m._43;
    w = (m._14 * v.x) + (m._24 * v.y) + (m._34 * v.z) + m._44;
    result /= w;
}

Vector3 Vector3::Transform(const Vector3& v, const Matrix& m)
{
    Vector3 result;
    float w = 0.0f;
    result.x = (m._11 * v.x) + (m._21 * v.y) + (m._31 * v.z) + m._41;
    result.y = (m._12 * v.x) + (m._22 * v.y) + (m._32 * v.z) + m._42;
    result.z = (m._13 * v.x) + (m._23 * v.y) + (m._33 * v.z) + m._43;
    w = (m._14 * v.x) + (m._24 * v.y) + (m._34 * v.z) + m._44;
    result /= w;
    return std::move(result);
}

void Vector3::TransformNormal(const Vector3& v, const Matrix& m, Vector3& result)
{
    result.x = (m._11 * v.x) + (m._21 * v.y) + (m._31 * v.z) + m._41;
    result.y = (m._12 * v.x) + (m._22 * v.y) + (m._32 * v.z) + m._42;
    result.z = (m._13 * v.x) + (m._23 * v.y) + (m._33 * v.z) + m._43;
}

Vector3 Vector3::TransformNormal(const Vector3& v, const Matrix& m)
{
    Vector3 result;
    result.x = (m._11 * v.x) + (m._21 * v.y) + (m._31 * v.z) + m._41;
    result.y = (m._12 * v.x) + (m._22 * v.y) + (m._32 * v.z) + m._42;
    result.z = (m._13 * v.x) + (m._23 * v.y) + (m._33 * v.z) + m._43;
    return std::move(result);
}
