#ifndef RAY_H_INCLUDED
#define RAY_H_INCLUDED


class Ray
{
public:
    Vector3 position;
    Vector3 direction;

    Ray() : position(0, 0, 0), direction(0, 0, 1) {}
    Ray(const Vector3& pos, const Vector3& dir) : position(pos), direction(dir) {}

    // Comparision operators
    bool operator == (const Ray& r) const;
    bool operator != (const Ray& r) const;

    // Ray operations
    bool Intersects(const BoundingSphere& sphere, _Out_ float& Dist) const;
    bool Intersects(const BoundingBox& box, _Out_ float& Dist) const;
    bool Intersects(const Vector3& tri0, const Vector3& tri1, const Vector3& tri2, _Out_ float& Dist) const;
    bool Intersects(const Plane& plane, _Out_ float& Dist) const;
};

namespace std
{
    template<> struct less < DirectX::SimpleMath::Ray >
    {
        bool operator()(const DirectX::SimpleMath::Ray& R1, const DirectX::SimpleMath::Ray& R2) const
        {
            if (R1.position.x != R2.position.x) return R1.position.x < R2.position.x;
            if (R1.position.y != R2.position.y) return R1.position.y < R2.position.y;
            if (R1.position.z != R2.position.z) return R1.position.z < R2.position.z;

            if (R1.direction.x != R2.direction.x) return R1.direction.x < R2.direction.x;
            if (R1.direction.y != R2.direction.y) return R1.direction.y < R2.direction.y;
            if (R1.direction.z != R2.direction.z) return R1.direction.z < R2.direction.z;

            return false;
        }
    };
}

#endif
