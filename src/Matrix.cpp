#include <onut/Matrix.h>
#include <onut/Vector2.h>
#include <onut/Vector3.h>
#include <onut/Vector4.h>

const Matrix Matrix::Identity(1, 0, 0, 0, 
                              0, 1, 0, 0, 
                              0, 0, 1, 0, 
                              0, 0, 0, 1);

Matrix::Matrix(const Vector3& r0, const Vector3& r1, const Vector3& r2)
    : _11(r0.x), _12(r0.y), _13(r0.z), _14(0.0f)
    , _21(r1.x), _22(r1.y), _23(r1.z), _24(0.0f)
    , _31(r2.x), _32(r2.y), _33(r2.z), _34(0.0f)
    , _41(0.0f), _42(0.0f), _43(0.0f), _44(1.0f)
{
}

Matrix::Matrix(const Vector4& r0, const Vector4& r1, const Vector4& r2, const Vector4& r3)
    : _11(r0.x), _12(r0.y), _13(r0.z), _14(r0.w)
    , _21(r1.x), _22(r1.y), _23(r1.z), _24(r1.w)
    , _31(r2.x), _32(r2.y), _33(r2.z), _34(r2.w)
    , _41(r3.x), _42(r3.y), _43(r3.z), _44(r3.w)
{
}

Vector3 Matrix::Up() const { return Vector3(_21, _22, _23); }
void Matrix::Up(const Vector3& v) { _21 = v.x; _22 = v.y; _23 = v.z; }

Vector3 Matrix::Down() const { return Vector3(-_21, -_22, -_23); }
void Matrix::Down(const Vector3& v) { _21 = -v.x; _22 = -v.y; _23 = -v.z; }

Vector3 Matrix::Right() const { return Vector3(_11, _12, _13); }
void Matrix::Right(const Vector3& v) { _11 = v.x; _12 = v.y; _13 = v.z; }

Vector3 Matrix::Left() const { return Vector3(-_11, -_12, -_13); }
void Matrix::Left(const Vector3& v) { _11 = -v.x; _12 = -v.y; _13 = -v.z; }

Vector3 Matrix::Forward() const { return Vector3(-_31, -_32, -_33); }
void Matrix::Forward(const Vector3& v) { _31 = -v.x; _32 = -v.y; _33 = -v.z; }

Vector3 Matrix::Backward() const { return Vector3(_31, _32, _33); }
void Matrix::Backward(const Vector3& v) { _31 = v.x; _32 = v.y; _33 = v.z; }

Vector3 Matrix::AxisY() const { return Vector3(_21, _22, _23); }
void Matrix::AxisY(const Vector3& v) { _21 = v.x; _22 = v.y; _23 = v.z; }

Vector3 Matrix::AxisX() const { return Vector3(_11, _12, _13); }
void Matrix::AxisX(const Vector3& v) { _11 = v.x; _12 = v.y; _13 = v.z; }

Vector3 Matrix::AxisZ() const { return Vector3(_31, _32, _33); }
void Matrix::AxisZ(const Vector3& v) { _31 = v.x; _32 = v.y; _33 = v.z; }

Vector3 Matrix::Translation() const { return Vector3(_41, _42, _43); }
void Matrix::Translation(const Vector3& v) { _41 = v.x; _42 = v.y; _43 = v.z; }

Matrix Matrix::CreateBillboard(const Vector3& object, const Vector3& cameraPosition, const Vector3& cameraUp, const Vector3* cameraForward)
{
    auto Z = cameraPosition - object;
    static const float EPSILON = 1.192092896e-7f;

    auto N = Z.LengthSquared();
    if (N < EPSILON)
    {
        if (cameraForward)
        {
            Z = -(*cameraForward);
        }
        else
            Z = Vector3(0, 0, -1);
    }
    else
    {
        Z.Normalize();
    }

    auto X = cameraUp.Cross(Z);
    X.Normalize();

    auto Y = Z.Cross(X);

    Matrix M(X, Y, Z);
    M._41 = object.x;
    M._42 = object.y;
    M._43 = object.z;
    M._44 = 1.f;
    return M;
}

Matrix Matrix::CreateConstrainedBillboard(const Vector3& object, const Vector3& cameraPosition, const Vector3& rotateAxis, const Vector3* cameraForward, const Vector3* objectForward)
{
    static const float s_minAngle = 0.99825467075f;
    static const float EPSILON = 1.192092896e-7f;

    auto faceDir = object - cameraPosition;

    auto N = faceDir.LengthSquared();
    if (N < EPSILON)
    {
        if (cameraForward)
        {
            faceDir = -(*cameraForward);
        }
        else
            faceDir = Vector3(0, 0, -1);
    }
    else
    {
        faceDir.Normalize();
    }

    auto dot = std::fabs(rotateAxis.Dot(faceDir));
    Vector3 X, Z;
    if (dot > s_minAngle)
    {
        if (objectForward)
        {
            Z = *objectForward;
            dot = std::fabs(rotateAxis.Dot(Z));
            if (dot > s_minAngle)
            {
                dot = std::fabs(rotateAxis.Dot(Vector3(0, 0, -1)));
                Z = (dot > s_minAngle) ? Vector3(1, 0, 0) : Vector3(0, 0, -1);
            }
        }
        else
        {
            dot = std::fabs(rotateAxis.Dot(Vector3(0, 0, -1)));
            Z = (dot > s_minAngle) ? Vector3(1, 0, 0) : Vector3(0, 0, -1);
        }

        X = rotateAxis.Cross(Z);
        X.Normalize();

        Z = X.Cross(rotateAxis);
        Z.Normalize();
    }
    else
    {
        X = rotateAxis.Cross(faceDir);
        X.Normalize();

        Z = X.Cross(rotateAxis);
        Z.Normalize();
    }

    Matrix M(X, rotateAxis, Z);
    M._41 = object.x;
    M._42 = object.y;
    M._43 = object.z;
    M._44 = 1.0f;
    return std::move(M);
}

Matrix Matrix::CreateTranslation(const Vector3& position)
{
    return Matrix(
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        position.x, position.y, position.z, 1);
}

Matrix Matrix::CreateScale(const Vector3& scales)
{
    return Matrix(
        scales.x, 0, 0, 0,
        0, scales.y, 0, 0,
        0, 0, scales.z, 0,
        0, 0, 0, 1);
}

Matrix Matrix::Create2DTranslationZoom(const Vector2& resolution, const Vector2& camera, float zoom)
{
    return Matrix(
        zoom, 0, 0, 0,
        0, zoom, 0, 0,
        0, 0, zoom, 0,
        -camera.x * zoom + resolution.x * 0.5f, -camera.y * zoom + resolution.y * 0.5f, 0, 1);
}

Matrix Matrix::CreateFromAxisAngle(const Vector3& axis, float angle)
{
    float cosTheta = std::cos(angle);
    float sinTheta = std::sin(angle);
    float invCosTheta = 1 - cosTheta;

    return Matrix(
        cosTheta + axis.x * axis.x * invCosTheta, axis.x * axis.y * invCosTheta - axis.z * sinTheta, axis.x * axis.z * invCosTheta + axis.y * sinTheta, 0,
        axis.y * axis.x * invCosTheta + axis.z * sinTheta, cosTheta + axis.y * axis.y * invCosTheta, axis.y * axis.z * invCosTheta - axis.x * sinTheta, 0,
        axis.z * axis.x * invCosTheta - axis.y * sinTheta, axis.z * axis.y * invCosTheta + axis.x * sinTheta, cosTheta + axis.z * axis.z * invCosTheta, 0,
        0, 0, 0, 1);
}

Matrix Matrix::CreateLookAt(const Vector3& position, const Vector3& target, const Vector3& up)
{
    auto EyeDirection = position - target;
    auto R2 = EyeDirection;
    R2.Normalize();

    auto R0 = up.Cross(R2);
    R0.Normalize();

    auto R1 = R2.Cross(R0);

    auto NegEyePosition = -position;

    auto D0 = R0.Dot(NegEyePosition);
    auto D1 = R1.Dot(NegEyePosition);
    auto D2 = R2.Dot(NegEyePosition);

    return Matrix(
        R0.x, R1.x, R2.x, 0,
        R0.y, R1.y, R2.y, 0,
        R0.z, R1.z, R2.z, 0,
        D0, D1, D2, 1);
}

Matrix Matrix::CreateWorld(const Vector3& position, const Vector3& forward, const Vector3& up)
{
    auto yaxis = forward;
    yaxis.Normalize();
    auto zaxis = up;
    auto xaxis = yaxis.Cross(zaxis);
    xaxis.Normalize();
    yaxis = zaxis.Cross(xaxis);

    return Matrix(
        xaxis.x, xaxis.y, xaxis.z, 0,
        yaxis.x, yaxis.y, yaxis.z, 0,
        zaxis.x, zaxis.y, zaxis.z, 0,
        position.x, position.y, position.z, 1);
}
