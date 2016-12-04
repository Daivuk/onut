#ifndef PLANE_H_INCLUDED
#define PLANE_H_INCLUDED


struct Plane : public XMFLOAT4
{
    Plane() : XMFLOAT4(0.f, 1.f, 0.f, 0.f) {}
    Plane(float _x, float _y, float _z, float _w) : XMFLOAT4(_x, _y, _z, _w) {}
    Plane(const Vector3& normal, float d) : XMFLOAT4(normal.x, normal.y, normal.z, d) {}
    Plane(const Vector3& point1, const Vector3& point2, const Vector3& point3);
    Plane(const Vector3& point, const Vector3& normal);
    explicit Plane(const Vector4& v) : XMFLOAT4(v.x, v.y, v.z, v.w) {}
    explicit Plane(_In_reads_(4) const float *pArray) : XMFLOAT4(pArray) {}
    Plane(FXMVECTOR V) { XMStoreFloat4(this, V); }

    operator XMVECTOR() const { return XMLoadFloat4(this); }

    // Comparision operators
    bool operator == (const Plane& p) const;
    bool operator != (const Plane& p) const;

    // Assignment operators
    Plane& operator= (const Plane& p) { x = p.x; y = p.y; z = p.z; w = p.w; return *this; }

    // Properties
    Vector3 Normal() const { return Vector3(x, y, z); }
    void Normal(const Vector3& normal) { x = normal.x; y = normal.y; z = normal.z; }

    float D() const { return w; }
    void D(float d) { w = d; }

    // Plane operations
    void Normalize();
    void Normalize(Plane& result) const;

    float Dot(const Vector4& v) const;
    float DotCoordinate(const Vector3& position) const;
    float DotNormal(const Vector3& normal) const;

    // Static functions
    static void Transform(const Plane& plane, const Matrix& M, Plane& result);
    static Plane Transform(const Plane& plane, const Matrix& M);

    static void Transform(const Plane& plane, const Quaternion& rotation, Plane& result);
    static Plane Transform(const Plane& plane, const Quaternion& rotation);
    // Input quaternion must be the inverse transpose of the transformation
};

#endif
