#ifndef MATRIX_H_INCLUDED
#define MATRIX_H_INCLUDED

// STL
#include <cmath>
#include <memory.h>
#include <utility>

// Forward Declaration
struct Vector2;
struct Vector3;
struct Vector4;

struct Matrix
{
    union
    {
        struct
        {
            float _11, _12, _13, _14;
            float _21, _22, _23, _24;
            float _31, _32, _33, _34;
            float _41, _42, _43, _44;
        };
        float m[4][4];
        float v[16];
    };

    Matrix() 
        : _11(1.0f), _12(0.0f), _13(0.0f), _14(0.0f)
        , _21(0.0f), _22(1.0f), _23(0.0f), _24(0.0f)
        , _31(0.0f), _32(0.0f), _33(1.0f), _34(0.0f)
        , _41(0.0f), _42(0.0f), _43(0.0f), _44(1.0f)
    {
    }
    Matrix(float m00, float m01, float m02, float m03,
           float m10, float m11, float m12, float m13,
           float m20, float m21, float m22, float m23,
           float m30, float m31, float m32, float m33)
        : _11(m00), _12(m01), _13(m02), _14(m03)
        , _21(m10), _22(m11), _23(m12), _24(m13)
        , _31(m20), _32(m21), _33(m22), _34(m23)
        , _41(m30), _42(m31), _43(m32), _44(m33)
    {
    }
    explicit Matrix(const Vector3& r0, const Vector3& r1, const Vector3& r2);
    explicit Matrix(const Vector4& r0, const Vector4& r1, const Vector4& r2, const Vector4& r3);
    explicit Matrix(const float *pArray)
        : _11(pArray[0]), _12(pArray[1]), _13(pArray[2]), _14(pArray[3])
        , _21(pArray[4]), _22(pArray[5]), _23(pArray[6]), _24(pArray[7])
        , _31(pArray[8]), _32(pArray[9]), _33(pArray[10]), _34(pArray[11])
        , _41(pArray[12]), _42(pArray[13]), _43(pArray[14]), _44(pArray[15])
    {
    }

    // Comparision operators
    bool operator == (const Matrix& M) const
    {
        return
            M._11 == _11 && M._12 == _12 && M._13 == _13 && M._14 == _14 &&
            M._21 == _21 && M._22 == _22 && M._23 == _23 && M._24 == _24 &&
            M._31 == _31 && M._32 == _32 && M._33 == _33 && M._34 == _34 &&
            M._41 == _41 && M._42 == _42 && M._43 == _43 && M._44 == _44;
    }
    bool operator != (const Matrix& M) const
    {
        return
            M._11 != _11 || M._12 != _12 || M._13 != _13 || M._14 != _14 ||
            M._21 != _21 || M._22 != _22 || M._23 != _23 || M._24 != _24 ||
            M._31 != _31 || M._32 != _32 || M._33 != _33 || M._34 != _34 ||
            M._41 != _41 || M._42 != _42 || M._43 != _43 || M._44 != _44;
    }

    // Assignment operators
    Matrix& operator= (const Matrix& M) { memcpy(this, &M, sizeof(float) * 16); return *this; }
    Matrix& operator+= (const Matrix& M)
    {
        _11 += M._11;
        _12 += M._12;
        _13 += M._13;
        _14 += M._14;
        _21 += M._21;
        _22 += M._22;
        _23 += M._23;
        _24 += M._24;
        _31 += M._31;
        _32 += M._32;
        _33 += M._33;
        _34 += M._34;
        _41 += M._41;
        _42 += M._42;
        _43 += M._43;
        _44 += M._44;
        return *this;
    }
    Matrix& operator-= (const Matrix& M)
    {
        _11 -= M._11;
        _12 -= M._12;
        _13 -= M._13;
        _14 -= M._14;
        _21 -= M._21;
        _22 -= M._22;
        _23 -= M._23;
        _24 -= M._24;
        _31 -= M._31;
        _32 -= M._32;
        _33 -= M._33;
        _34 -= M._34;
        _41 -= M._41;
        _42 -= M._42;
        _43 -= M._43;
        _44 -= M._44;
        return *this;
    }
    Matrix& operator*= (const Matrix& M)
    {
        // Cache the invariants in registers
        float x = m[0][0];
        float y = m[0][1];
        float z = m[0][2];
        float w = m[0][3];
        // Perform the operation on the first row
        m[0][0] = (M.m[0][0] * x) + (M.m[1][0] * y) + (M.m[2][0] * z) + (M.m[3][0] * w);
        m[0][1] = (M.m[0][1] * x) + (M.m[1][1] * y) + (M.m[2][1] * z) + (M.m[3][1] * w);
        m[0][2] = (M.m[0][2] * x) + (M.m[1][2] * y) + (M.m[2][2] * z) + (M.m[3][2] * w);
        m[0][3] = (M.m[0][3] * x) + (M.m[1][3] * y) + (M.m[2][3] * z) + (M.m[3][3] * w);
        // Repeat for all the other rows
        x = m[1][0];
        y = m[1][1];
        z = m[1][2];
        w = m[1][3];
        m[1][0] = (M.m[0][0] * x) + (M.m[1][0] * y) + (M.m[2][0] * z) + (M.m[3][0] * w);
        m[1][1] = (M.m[0][1] * x) + (M.m[1][1] * y) + (M.m[2][1] * z) + (M.m[3][1] * w);
        m[1][2] = (M.m[0][2] * x) + (M.m[1][2] * y) + (M.m[2][2] * z) + (M.m[3][2] * w);
        m[1][3] = (M.m[0][3] * x) + (M.m[1][3] * y) + (M.m[2][3] * z) + (M.m[3][3] * w);
        x = m[2][0];
        y = m[2][1];
        z = m[2][2];
        w = m[2][3];
        m[2][0] = (M.m[0][0] * x) + (M.m[1][0] * y) + (M.m[2][0] * z) + (M.m[3][0] * w);
        m[2][1] = (M.m[0][1] * x) + (M.m[1][1] * y) + (M.m[2][1] * z) + (M.m[3][1] * w);
        m[2][2] = (M.m[0][2] * x) + (M.m[1][2] * y) + (M.m[2][2] * z) + (M.m[3][2] * w);
        m[2][3] = (M.m[0][3] * x) + (M.m[1][3] * y) + (M.m[2][3] * z) + (M.m[3][3] * w);
        x = m[3][0];
        y = m[3][1];
        z = m[3][2];
        w = m[3][3];
        m[3][0] = (M.m[0][0] * x) + (M.m[1][0] * y) + (M.m[2][0] * z) + (M.m[3][0] * w);
        m[3][1] = (M.m[0][1] * x) + (M.m[1][1] * y) + (M.m[2][1] * z) + (M.m[3][1] * w);
        m[3][2] = (M.m[0][2] * x) + (M.m[1][2] * y) + (M.m[2][2] * z) + (M.m[3][2] * w);
        m[3][3] = (M.m[0][3] * x) + (M.m[1][3] * y) + (M.m[2][3] * z) + (M.m[3][3] * w);
        return *this;
    }
    Matrix& operator*= (float S)
    {
        _11 *= S;
        _12 *= S;
        _13 *= S;
        _14 *= S;
        _21 *= S;
        _22 *= S;
        _23 *= S;
        _24 *= S;
        _31 *= S;
        _32 *= S;
        _33 *= S;
        _34 *= S;
        _41 *= S;
        _42 *= S;
        _43 *= S;
        _44 *= S;
        return *this;
    }
    Matrix& operator/= (float S)
    {
        float invS = 1.0f / S;
        _11 *= invS;
        _12 *= invS;
        _13 *= invS;
        _14 *= invS;
        _21 *= invS;
        _22 *= invS;
        _23 *= invS;
        _24 *= invS;
        _31 *= invS;
        _32 *= invS;
        _33 *= invS;
        _34 *= invS;
        _41 *= invS;
        _42 *= invS;
        _43 *= invS;
        _44 *= invS;
        return *this;
    }

    // Element-wise divide
    Matrix& operator/= (const Matrix& M)
    {
        _11 /= M._11;
        _12 /= M._12;
        _13 /= M._13;
        _14 /= M._14;
        _21 /= M._21;
        _22 /= M._22;
        _23 /= M._23;
        _24 /= M._24;
        _31 /= M._31;
        _32 /= M._32;
        _33 /= M._33;
        _34 /= M._34;
        _41 /= M._41;
        _42 /= M._42;
        _43 /= M._43;
        _44 /= M._44;
        return *this;
    }

    // Urnary operators
    Matrix operator+ () const { return *this; }
    Matrix operator- () const
    {
        return Matrix(
            -_11, -_12, -_13, -_14,
            -_21, -_22, -_23, -_24,
            -_31, -_32, -_33, -_34,
            -_41, -_42, -_43, -_44);
    }

    // Properties
    Vector3 Up() const;
    void Up(const Vector3& v);

    Vector3 Down() const;
    void Down(const Vector3& v);

    Vector3 Right() const;
    void Right(const Vector3& v);

    Vector3 Left() const;
    void Left(const Vector3& v);

    Vector3 Forward() const;
    void Forward(const Vector3& v);

    Vector3 Backward() const;
    void Backward(const Vector3& v);

    Vector3 AxisY() const;
    void AxisY(const Vector3& v);

    Vector3 AxisX() const;
    void AxisX(const Vector3& v);

    Vector3 AxisZ() const;
    void AxisZ(const Vector3& v);

    Vector3 Translation() const;
    void Translation(const Vector3& v);

    // Matrix operations
    Matrix Transpose() const
    {
        Matrix result;
        result._11 = _11;
        result._12 = _21;
        result._13 = _31;
        result._14 = _41;
        result._21 = _12;
        result._22 = _22;
        result._23 = _32;
        result._24 = _42;
        result._31 = _13;
        result._32 = _23;
        result._33 = _33;
        result._34 = _43;
        result._41 = _14;
        result._42 = _24;
        result._43 = _34;
        result._44 = _44;
        return std::move(result);
    }
    void Transpose(Matrix& result) const
    {
        result._11 = _11;
        result._12 = _21;
        result._13 = _31;
        result._14 = _41;
        result._21 = _12;
        result._22 = _22;
        result._23 = _32;
        result._24 = _42;
        result._31 = _13;
        result._32 = _23;
        result._33 = _33;
        result._34 = _43;
        result._41 = _14;
        result._42 = _24;
        result._43 = _34;
        result._44 = _44;
    }

    Matrix Invert() const
    {
        Matrix inv;
        float det;
        int i;

        inv.v[0] = v[5] * v[10] * v[15] -
            v[5] * v[11] * v[14] -
            v[9] * v[6] * v[15] +
            v[9] * v[7] * v[14] +
            v[13] * v[6] * v[11] -
            v[13] * v[7] * v[10];

        inv.v[4] = -v[4] * v[10] * v[15] +
            v[4] * v[11] * v[14] +
            v[8] * v[6] * v[15] -
            v[8] * v[7] * v[14] -
            v[12] * v[6] * v[11] +
            v[12] * v[7] * v[10];

        inv.v[8] = v[4] * v[9] * v[15] -
            v[4] * v[11] * v[13] -
            v[8] * v[5] * v[15] +
            v[8] * v[7] * v[13] +
            v[12] * v[5] * v[11] -
            v[12] * v[7] * v[9];

        inv.v[12] = -v[4] * v[9] * v[14] +
            v[4] * v[10] * v[13] +
            v[8] * v[5] * v[14] -
            v[8] * v[6] * v[13] -
            v[12] * v[5] * v[10] +
            v[12] * v[6] * v[9];

        inv.v[1] = -v[1] * v[10] * v[15] +
            v[1] * v[11] * v[14] +
            v[9] * v[2] * v[15] -
            v[9] * v[3] * v[14] -
            v[13] * v[2] * v[11] +
            v[13] * v[3] * v[10];

        inv.v[5] = v[0] * v[10] * v[15] -
            v[0] * v[11] * v[14] -
            v[8] * v[2] * v[15] +
            v[8] * v[3] * v[14] +
            v[12] * v[2] * v[11] -
            v[12] * v[3] * v[10];

        inv.v[9] = -v[0] * v[9] * v[15] +
            v[0] * v[11] * v[13] +
            v[8] * v[1] * v[15] -
            v[8] * v[3] * v[13] -
            v[12] * v[1] * v[11] +
            v[12] * v[3] * v[9];

        inv.v[13] = v[0] * v[9] * v[14] -
            v[0] * v[10] * v[13] -
            v[8] * v[1] * v[14] +
            v[8] * v[2] * v[13] +
            v[12] * v[1] * v[10] -
            v[12] * v[2] * v[9];

        inv.v[2] = v[1] * v[6] * v[15] -
            v[1] * v[7] * v[14] -
            v[5] * v[2] * v[15] +
            v[5] * v[3] * v[14] +
            v[13] * v[2] * v[7] -
            v[13] * v[3] * v[6];

        inv.v[6] = -v[0] * v[6] * v[15] +
            v[0] * v[7] * v[14] +
            v[4] * v[2] * v[15] -
            v[4] * v[3] * v[14] -
            v[12] * v[2] * v[7] +
            v[12] * v[3] * v[6];

        inv.v[10] = v[0] * v[5] * v[15] -
            v[0] * v[7] * v[13] -
            v[4] * v[1] * v[15] +
            v[4] * v[3] * v[13] +
            v[12] * v[1] * v[7] -
            v[12] * v[3] * v[5];

        inv.v[14] = -v[0] * v[5] * v[14] +
            v[0] * v[6] * v[13] +
            v[4] * v[1] * v[14] -
            v[4] * v[2] * v[13] -
            v[12] * v[1] * v[6] +
            v[12] * v[2] * v[5];

        inv.v[3] = -v[1] * v[6] * v[11] +
            v[1] * v[7] * v[10] +
            v[5] * v[2] * v[11] -
            v[5] * v[3] * v[10] -
            v[9] * v[2] * v[7] +
            v[9] * v[3] * v[6];

        inv.v[7] = v[0] * v[6] * v[11] -
            v[0] * v[7] * v[10] -
            v[4] * v[2] * v[11] +
            v[4] * v[3] * v[10] +
            v[8] * v[2] * v[7] -
            v[8] * v[3] * v[6];

        inv.v[11] = -v[0] * v[5] * v[11] +
            v[0] * v[7] * v[9] +
            v[4] * v[1] * v[11] -
            v[4] * v[3] * v[9] -
            v[8] * v[1] * v[7] +
            v[8] * v[3] * v[5];

        inv.v[15] = v[0] * v[5] * v[10] -
            v[0] * v[6] * v[9] -
            v[4] * v[1] * v[10] +
            v[4] * v[2] * v[9] +
            v[8] * v[1] * v[6] -
            v[8] * v[2] * v[5];

        det = v[0] * inv.v[0] + v[1] * inv.v[4] + v[2] * inv.v[8] + v[3] * inv.v[12];

        if (det != 0)
            det = 1.0f / det;

        for (i = 0; i < 16; i++)
            inv.v[i] = inv.v[i] * det;

        return std::move(inv);
    }
    void Invert(Matrix& result) const
    {
        result = Invert();
    }

    float Determinant() const
    {
        float inv[16], det;

        inv[0] = v[5] * v[10] * v[15] -
            v[5] * v[11] * v[14] -
            v[9] * v[6] * v[15] +
            v[9] * v[7] * v[14] +
            v[13] * v[6] * v[11] -
            v[13] * v[7] * v[10];

        inv[4] = -v[4] * v[10] * v[15] +
            v[4] * v[11] * v[14] +
            v[8] * v[6] * v[15] -
            v[8] * v[7] * v[14] -
            v[12] * v[6] * v[11] +
            v[12] * v[7] * v[10];

        inv[8] = v[4] * v[9] * v[15] -
            v[4] * v[11] * v[13] -
            v[8] * v[5] * v[15] +
            v[8] * v[7] * v[13] +
            v[12] * v[5] * v[11] -
            v[12] * v[7] * v[9];

        inv[12] = -v[4] * v[9] * v[14] +
            v[4] * v[10] * v[13] +
            v[8] * v[5] * v[14] -
            v[8] * v[6] * v[13] -
            v[12] * v[5] * v[10] +
            v[12] * v[6] * v[9];

        det = v[0] * inv[0] + v[1] * inv[4] + v[2] * inv[8] + v[3] * inv[12];

        if (det != 0)
            det = 1.0f / det;

        return det;
    }

    // Static functions
    static Matrix CreateBillboard(const Vector3& object, const Vector3& cameraPosition, const Vector3& cameraUp, const Vector3* cameraForward = nullptr);
    static Matrix CreateConstrainedBillboard(const Vector3& object, const Vector3& cameraPosition, const Vector3& rotateAxis, const Vector3* cameraForward = nullptr, const Vector3* objectForward = nullptr);
    static Matrix CreateTranslation(const Vector3& position);
    static Matrix CreateTranslation(float x, float y, float z)
    {
        return Matrix(
            1, 0, 0, 0,
            0, 1, 0, 0,
            0, 0, 1, 0,
            x, y, z, 1);
    }
    static Matrix CreateScale(const Vector3& scales);
    static Matrix CreateScale(float xs, float ys, float zs)
    {
        return Matrix(
            xs, 0, 0, 0,
            0, ys, 0, 0,
            0, 0, zs, 0,
            0, 0, 0, 1);
    }
    static Matrix CreateScale(float scale)
    {
        return Matrix(
            scale, 0, 0, 0,
            0, scale, 0, 0,
            0, 0, scale, 0,
            0, 0, 0, 1);
    }
    static Matrix Create2DTranslationZoom(const Vector2& resolution, const Vector2& camera, float zoom = 1.0f);

    static Matrix CreateRotationX(float radians)
    {
        auto sinTheta = std::sin(radians);
        auto cosTheta = std::cos(radians);
        return Matrix(
            1, 0, 0, 0,
            0, cosTheta, sinTheta, 0,
            0, -sinTheta, cosTheta, 0,
            0, 0, 0, 1);
    }
    static Matrix CreateRotationY(float radians)
    {
        auto sinTheta = std::sin(radians);
        auto cosTheta = std::cos(radians);
        return Matrix(
            cosTheta, 0, -sinTheta, 0,
            0, 1, 0, 0,
            sinTheta, 0, cosTheta, 0,
            0, 0, 0, 1);
    }
    static Matrix CreateRotationZ(float radians)
    {
        auto sinTheta = std::sin(radians);
        auto cosTheta = std::cos(radians);
        return Matrix(
            cosTheta, sinTheta, 0, 0,
            -sinTheta, cosTheta, 0, 0,
            0, 0, 1, 0,
            0, 0, 0, 1);
    }

    static Matrix CreateFromAxisAngle(const Vector3& axis, float angle);

    static Matrix CreatePerspectiveFieldOfView(float fov, float aspectRatio, float nearPlane, float farPlane)
    {
        float CosFov = std::cos(0.5f * fov);
        float SinFov = std::sin(0.5f * fov);

        float Height = CosFov / SinFov;
        float Width = Height / aspectRatio;
        float fRange = farPlane / (nearPlane - farPlane);

        Matrix M;
        M.m[0][0] = Width;
        M.m[0][1] = 0.0f;
        M.m[0][2] = 0.0f;
        M.m[0][3] = 0.0f;

        M.m[1][0] = 0.0f;
        M.m[1][1] = Height;
        M.m[1][2] = 0.0f;
        M.m[1][3] = 0.0f;

        M.m[2][0] = 0.0f;
        M.m[2][1] = 0.0f;
        M.m[2][2] = fRange;
        M.m[2][3] = -1.0f;

        M.m[3][0] = 0.0f;
        M.m[3][1] = 0.0f;
        M.m[3][2] = fRange * nearPlane;
        M.m[3][3] = 0.0f;
        return M;
    }
    static Matrix CreatePerspective(float width, float height, float nearPlane, float farPlane)
    {
        float TwoNearZ = nearPlane + nearPlane;
        float fRange = farPlane / (nearPlane - farPlane);

        Matrix M;
        M.m[0][0] = TwoNearZ / width;
        M.m[0][1] = 0.0f;
        M.m[0][2] = 0.0f;
        M.m[0][3] = 0.0f;

        M.m[1][0] = 0.0f;
        M.m[1][1] = TwoNearZ / height;
        M.m[1][2] = 0.0f;
        M.m[1][3] = 0.0f;

        M.m[2][0] = 0.0f;
        M.m[2][1] = 0.0f;
        M.m[2][2] = fRange;
        M.m[2][3] = -1.0f;

        M.m[3][0] = 0.0f;
        M.m[3][1] = 0.0f;
        M.m[3][2] = fRange * nearPlane;
        M.m[3][3] = 0.0f;
        return M;
    }
    static Matrix CreatePerspectiveOffCenter(float left, float right, float bottom, float top, float nearPlane, float farPlane)
    {
        float TwoNearZ = nearPlane + nearPlane;
        float ReciprocalWidth = 1.0f / (right - left);
        float ReciprocalHeight = 1.0f / (top - bottom);
        float fRange = farPlane / (nearPlane - farPlane);

        Matrix M;
        M.m[0][0] = TwoNearZ * ReciprocalWidth;
        M.m[0][1] = 0.0f;
        M.m[0][2] = 0.0f;
        M.m[0][3] = 0.0f;

        M.m[1][0] = 0.0f;
        M.m[1][1] = TwoNearZ * ReciprocalHeight;
        M.m[1][2] = 0.0f;
        M.m[1][3] = 0.0f;

        M.m[2][0] = (left + right) * ReciprocalWidth;
        M.m[2][1] = (top + bottom) * ReciprocalHeight;
        M.m[2][2] = fRange;
        M.m[2][3] = -1.0f;

        M.m[3][0] = 0.0f;
        M.m[3][1] = 0.0f;
        M.m[3][2] = fRange * nearPlane;
        M.m[3][3] = 0.0f;
        return M;
    }
    static Matrix CreateOrthographic(float width, float height, float zNearPlane, float zFarPlane)
    {
        float fRange = 1.0f / (zNearPlane - zFarPlane);

        Matrix M;
        M.m[0][0] = 2.0f / width;
        M.m[0][1] = 0.0f;
        M.m[0][2] = 0.0f;
        M.m[0][3] = 0.0f;

        M.m[1][0] = 0.0f;
        M.m[1][1] = 2.0f / height;
        M.m[1][2] = 0.0f;
        M.m[1][3] = 0.0f;

        M.m[2][0] = 0.0f;
        M.m[2][1] = 0.0f;
        M.m[2][2] = fRange;
        M.m[2][3] = 0.0f;

        M.m[3][0] = 0.0f;
        M.m[3][1] = 0.0f;
        M.m[3][2] = fRange * zNearPlane;
        M.m[3][3] = 1.0f;
        return M;
    }
    static Matrix CreateOrthographicOffCenter(float left, float right, float bottom, float top, float zNearPlane, float zFarPlane)
    {
        float ReciprocalWidth = 1.0f / (right - left);
        float ReciprocalHeight = 1.0f / (top - bottom);
        float fRange = 1.0f / (zNearPlane - zFarPlane);

        Matrix M;
        M.m[0][0] = ReciprocalWidth + ReciprocalWidth;
        M.m[0][1] = 0.0f;
        M.m[0][2] = 0.0f;
        M.m[0][3] = 0.0f;

        M.m[1][0] = 0.0f;
        M.m[1][1] = ReciprocalHeight + ReciprocalHeight;
        M.m[1][2] = 0.0f;
        M.m[1][3] = 0.0f;

        M.m[2][0] = 0.0f;
        M.m[2][1] = 0.0f;
        M.m[2][2] = fRange;
        M.m[2][3] = 0.0f;

        M.m[3][0] = -(left + right) * ReciprocalWidth;
        M.m[3][1] = -(top + bottom) * ReciprocalHeight;
        M.m[3][2] = fRange * zNearPlane;
        M.m[3][3] = 1.0f;
        return std::move(M);
    }

    static Matrix CreateLookAt(const Vector3& position, const Vector3& target, const Vector3& up);
    static Matrix CreateWorld(const Vector3& position, const Vector3& forward, const Vector3& up);

    static Matrix CreateFromYawPitchRoll(float yaw, float pitch, float roll)
    {
        float cosPhi = std::cos(pitch);
        float sinPhi = std::sin(pitch);
        float cosTheta = std::cos(roll);
        float sinTheta = std::sin(roll);
        float cosPsi = std::cos(yaw);
        float sinPsi = std::sin(yaw);

        return Matrix(
            cosTheta * cosPsi, -cosPhi * sinPsi + sinPhi * sinTheta * cosPsi, sinPhi * sinPsi + cosPhi * sinTheta * cosPsi, 0,
            cosTheta * sinPsi, cosPhi * cosPsi + sinPhi * sinTheta * sinPsi, -sinPhi * cosPsi + cosPhi * sinTheta * sinPsi, 0,
            sinTheta, sinPhi * cosTheta, cosPhi * cosTheta, 0,
            0, 0, 0, 1);
    }

    static void Lerp(const Matrix& M1, const Matrix& M2, float t, Matrix& result)
    {
        result._11 = M1._11 + (M2._11 - M1._11) * t;
        result._12 = M1._12 + (M2._12 - M1._12) * t;
        result._12 = M1._13 + (M2._13 - M1._13) * t;
        result._13 = M1._14 + (M2._14 - M1._14) * t;
        result._21 = M1._21 + (M2._21 - M1._21) * t;
        result._22 = M1._22 + (M2._22 - M1._22) * t;
        result._23 = M1._23 + (M2._23 - M1._23) * t;
        result._24 = M1._24 + (M2._24 - M1._24) * t;
        result._31 = M1._31 + (M2._31 - M1._31) * t;
        result._32 = M1._32 + (M2._32 - M1._32) * t;
        result._33 = M1._33 + (M2._33 - M1._33) * t;
        result._34 = M1._34 + (M2._34 - M1._34) * t;
        result._41 = M1._41 + (M2._41 - M1._41) * t;
        result._42 = M1._42 + (M2._42 - M1._42) * t;
        result._43 = M1._43 + (M2._43 - M1._43) * t;
        result._44 = M1._44 + (M2._44 - M1._44) * t;
    }
    static Matrix Lerp(const Matrix& M1, const Matrix& M2, float t)
    {
        return Matrix(
            M1._11 + (M2._11 - M1._11) * t,
            M1._12 + (M2._12 - M1._12) * t,
            M1._13 + (M2._13 - M1._13) * t,
            M1._14 + (M2._14 - M1._14) * t,
            M1._21 + (M2._21 - M1._21) * t,
            M1._22 + (M2._22 - M1._22) * t,
            M1._23 + (M2._23 - M1._23) * t,
            M1._24 + (M2._24 - M1._24) * t,
            M1._31 + (M2._31 - M1._31) * t,
            M1._32 + (M2._32 - M1._32) * t,
            M1._33 + (M2._33 - M1._33) * t,
            M1._34 + (M2._34 - M1._34) * t,
            M1._41 + (M2._41 - M1._41) * t,
            M1._42 + (M2._42 - M1._42) * t,
            M1._43 + (M2._43 - M1._43) * t,
            M1._44 + (M2._44 - M1._44) * t);
    }

    // Constants
    static const Matrix Identity;
};

// Binary operators
inline Matrix operator+ (const Matrix& M1, const Matrix& M2)
{
    return Matrix(
        M1._11 + M2._11,
        M1._12 + M2._12,
        M1._13 + M2._13,
        M1._14 + M2._14,
        M1._21 + M2._21,
        M1._22 + M2._22,
        M1._23 + M2._23,
        M1._24 + M2._24,
        M1._31 + M2._31,
        M1._32 + M2._32,
        M1._33 + M2._33,
        M1._34 + M2._34,
        M1._41 + M2._41,
        M1._42 + M2._42,
        M1._43 + M2._43,
        M1._44 + M2._44);
}
inline Matrix operator- (const Matrix& M1, const Matrix& M2)
{
    return Matrix(
        M1._11 - M2._11,
        M1._12 - M2._12,
        M1._13 - M2._13,
        M1._14 - M2._14,
        M1._21 - M2._21,
        M1._22 - M2._22,
        M1._23 - M2._23,
        M1._24 - M2._24,
        M1._31 - M2._31,
        M1._32 - M2._32,
        M1._33 - M2._33,
        M1._34 - M2._34,
        M1._41 - M2._41,
        M1._42 - M2._42,
        M1._43 - M2._43,
        M1._44 - M2._44);
}
inline Matrix operator* (const Matrix& M1, const Matrix& M2)
{
    Matrix ret = M1;
    ret *= M2;
    return std::move(ret);
}
inline Matrix operator* (const Matrix& M, float S)
{
    return Matrix(
        M._11 * S,
        M._12 * S,
        M._13 * S,
        M._14 * S,
        M._21 * S,
        M._22 * S,
        M._23 * S,
        M._24 * S,
        M._31 * S,
        M._32 * S,
        M._33 * S,
        M._34 * S,
        M._41 * S,
        M._42 * S,
        M._43 * S,
        M._44 * S);
}
inline Matrix operator/ (const Matrix& M, float S)
{
    auto invS = 1.0f / S;
    return Matrix(
        M._11 * invS,
        M._12 * invS,
        M._13 * invS,
        M._14 * invS,
        M._21 * invS,
        M._22 * invS,
        M._23 * invS,
        M._24 * invS,
        M._31 * invS,
        M._32 * invS,
        M._33 * invS,
        M._34 * invS,
        M._41 * invS,
        M._42 * invS,
        M._43 * invS,
        M._44 * invS);
}
// Element-wise divide
inline Matrix operator/ (const Matrix& M1, const Matrix& M2)
{
    return Matrix(
        M1._11 / M2._11,
        M1._12 / M2._12,
        M1._13 / M2._13,
        M1._14 / M2._14,
        M1._21 / M2._21,
        M1._22 / M2._22,
        M1._23 / M2._23,
        M1._24 / M2._24,
        M1._31 / M2._31,
        M1._32 / M2._32,
        M1._33 / M2._33,
        M1._34 / M2._34,
        M1._41 / M2._41,
        M1._42 / M2._42,
        M1._43 / M2._43,
        M1._44 / M2._44);
}
inline Matrix operator* (float S, const Matrix& M)
{
    return Matrix(
        M._11 * S,
        M._12 * S,
        M._13 * S,
        M._14 * S,
        M._21 * S,
        M._22 * S,
        M._23 * S,
        M._24 * S,
        M._31 * S,
        M._32 * S,
        M._33 * S,
        M._34 * S,
        M._41 * S,
        M._42 * S,
        M._43 * S,
        M._44 * S);
}

#endif
