#include <onut/Maths.h>

namespace onut
{
    static bool walkableAt(const Vector2& pos, bool* pPassableTiles, int width, int height, float tileSizef)
    {
        Point mapPos((int)(pos.x / tileSizef), (int)(pos.y / tileSizef));
        if (mapPos.x < 0 || mapPos.y < 0 || mapPos.x >= width || mapPos.y >= height) return false;
        return pPassableTiles[mapPos.y * width + mapPos.x];
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
};
