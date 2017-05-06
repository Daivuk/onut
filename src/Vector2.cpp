#include <onut/Matrix.h>
#include <onut/Point.h>
#include <onut/Vector2.h>
#include <onut/Vector3.h>

const Vector2 Vector2::Zero(0.f, 0.f);
const Vector2 Vector2::One(1.f, 1.f);
const Vector2 Vector2::UnitX(1.f, 0.f);
const Vector2 Vector2::UnitY(0.f, 1.f);

Vector2::Vector2(const Vector3& v3) : x(v3.x), y(v3.y)
{
}

void Vector2::Cross(const Vector2& V, Vector3& result) const
{
    result = Vector3(0, 0, (x * V.y) - (y * V.x));
}

Vector3 Vector2::Cross(const Vector2& V) const
{
    return Vector3(0, 0, (x * V.y) - (y * V.x));
}

void Vector2::Transform(const Vector2& v, const Matrix& m, Vector2& result)
{
    float w = 0.0f;
    result.x = (m._11 * v.x) + (m._21 * v.y) + m._41;
    result.y = (m._12 * v.x) + (m._22 * v.y) + m._42;
    w = (m._14 * v.x) + (m._24 * v.y) + m._44;
    result /= w;
}

Vector2 Vector2::Transform(const Vector2& v, const Matrix& m)
{
    Vector2 result;
    float w = 0.0f;
    result.x = (m._11 * v.x) + (m._21 * v.y) + m._41;
    result.y = (m._12 * v.x) + (m._22 * v.y) + m._42;
    w = (m._14 * v.x) + (m._24 * v.y) + m._44;
    result /= w;
    return result / w;
}

void Vector2::TransformNormal(const Vector2& v, const Matrix& m, Vector2& result)
{
    result.x = (m._11 * v.x) + (m._21 * v.y) + m._41;
    result.y = (m._12 * v.x) + (m._22 * v.y) + m._42;
}

Vector2 Vector2::TransformNormal(const Vector2& v, const Matrix& m)
{
    Vector2 result;
    result.x = (m._11 * v.x) + (m._21 * v.y) + m._41;
    result.y = (m._12 * v.x) + (m._22 * v.y) + m._42;
    return result;
}
namespace onut
{
    static bool walkableAt(const Vector2& pos, bool* pPassableTiles, int width, int height, float tileSizef)
    {
        Point mapPos((int)(pos.x / tileSizef), (int)(pos.y / tileSizef));
        if (mapPos.x < 0 || mapPos.y < 0 || mapPos.x >= width || mapPos.y >= height) return false;
        return pPassableTiles[mapPos.y * width + mapPos.x];
    }

    static bool walkableAt(const Vector2& pos, float* pPassableTiles, int width, int height, float tileSizef)
    {
        Point mapPos((int)(pos.x / tileSizef), (int)(pos.y / tileSizef));
        if (mapPos.x < 0 || mapPos.y < 0 || mapPos.x >= width || mapPos.y >= height) return false;
        return pPassableTiles[mapPos.y * width + mapPos.x] > 0.0f;
    }

    Vector2 tilesCollision(const Vector2& from, const Vector2& to, const Vector2& size, bool* pPassableTiles, int width, int height, int tileSize)
    {
        if (to == from) return to;

        float tileSizef = (float)tileSize;
        Point lastMapPos((int)(from.x / tileSizef), (int)(from.y / tileSizef));
        Vector2 dir = to - from;
        Vector2 result = to;
        Vector2 hSize = size / 2.0f;

        if (dir.x < 0)
        {
            if (!walkableAt(Vector2(to.x - hSize.x, from.y - hSize.y), pPassableTiles, width, height, tileSizef))
            {
                result.x = (float)(lastMapPos.x) * tileSizef + hSize.x;
            }
            else if (!walkableAt(Vector2(to.x - hSize.x, from.y + hSize.y), pPassableTiles, width, height, tileSizef))
            {
                result.x = (float)(lastMapPos.x) * tileSizef + hSize.x;
            }
        }
        else if (dir.x > 0)
        {
            if (!walkableAt(Vector2(to.x + hSize.x, from.y - hSize.y), pPassableTiles, width, height, tileSizef))
            {
                result.x = (float)(lastMapPos.x + 1) * tileSizef - hSize.x - (tileSizef / 100.0f);
            }
            else if (!walkableAt(Vector2(to.x + hSize.x, from.y + hSize.y), pPassableTiles, width, height, tileSizef))
            {
                result.x = (float)(lastMapPos.x + 1) * tileSizef - hSize.x - (tileSizef / 100.0f);
            }
        }
        if (dir.y < 0)
        {
            if (!walkableAt(Vector2(from.x - hSize.x, to.y - hSize.y), pPassableTiles, width, height, tileSizef))
            {
                result.y = (float)(lastMapPos.y) * tileSizef + hSize.y;
            }
            else if (!walkableAt(Vector2(from.x + hSize.x, to.y - hSize.y), pPassableTiles, width, height, tileSizef))
            {
                result.y = (float)(lastMapPos.y) * tileSizef + hSize.y;
            }
        }
        else if (dir.y > 0)
        {
            if (!walkableAt(Vector2(from.x - hSize.x, to.y + hSize.y), pPassableTiles, width, height, tileSizef))
            {
                result.y = (float)(lastMapPos.y + 1) * tileSizef - hSize.y - (tileSizef / 100.0f);
            }
            else if (!walkableAt(Vector2(from.x + hSize.x, to.y + hSize.y), pPassableTiles, width, height, tileSizef))
            {
                result.y = (float)(lastMapPos.y + 1) * tileSizef - hSize.y - (tileSizef / 100.0f);
            }
        }

        return result;
    }

    Vector2 tilesCollision(const Vector2& from, const Vector2& to, const Vector2& size, float* pPassableTiles, int width, int height, int tileSize)
    {
        if (to == from) return to;

        float tileSizef = (float)tileSize;
        Point lastMapPos((int)(from.x / tileSizef), (int)(from.y / tileSizef));
        Vector2 dir = to - from;
        Vector2 result = to;
        Vector2 hSize = size / 2.0f;

        if (dir.x < 0)
        {
            if (!walkableAt(Vector2(to.x - hSize.x, from.y - hSize.y), pPassableTiles, width, height, tileSizef))
            {
                result.x = (float)(lastMapPos.x) * tileSizef + hSize.x;
            }
            else if (!walkableAt(Vector2(to.x - hSize.x, from.y + hSize.y), pPassableTiles, width, height, tileSizef))
            {
                result.x = (float)(lastMapPos.x) * tileSizef + hSize.x;
            }
        }
        else if (dir.x > 0)
        {
            if (!walkableAt(Vector2(to.x + hSize.x, from.y - hSize.y), pPassableTiles, width, height, tileSizef))
            {
                result.x = (float)(lastMapPos.x + 1) * tileSizef - hSize.x - (tileSizef / 100.0f);
            }
            else if (!walkableAt(Vector2(to.x + hSize.x, from.y + hSize.y), pPassableTiles, width, height, tileSizef))
            {
                result.x = (float)(lastMapPos.x + 1) * tileSizef - hSize.x - (tileSizef / 100.0f);
            }
        }
        if (dir.y < 0)
        {
            if (!walkableAt(Vector2(from.x - hSize.x, to.y - hSize.y), pPassableTiles, width, height, tileSizef))
            {
                result.y = (float)(lastMapPos.y) * tileSizef + hSize.y;
            }
            else if (!walkableAt(Vector2(from.x + hSize.x, to.y - hSize.y), pPassableTiles, width, height, tileSizef))
            {
                result.y = (float)(lastMapPos.y) * tileSizef + hSize.y;
            }
        }
        else if (dir.y > 0)
        {
            if (!walkableAt(Vector2(from.x - hSize.x, to.y + hSize.y), pPassableTiles, width, height, tileSizef))
            {
                result.y = (float)(lastMapPos.y + 1) * tileSizef - hSize.y - (tileSizef / 100.0f);
            }
            else if (!walkableAt(Vector2(from.x + hSize.x, to.y + hSize.y), pPassableTiles, width, height, tileSizef))
            {
                result.y = (float)(lastMapPos.y + 1) * tileSizef - hSize.y - (tileSizef / 100.0f);
            }
        }

        return result;
    }
};
