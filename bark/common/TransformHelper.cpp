#include "TransformHelper.h"

Rect TransformHelper::getWorldRect(const Matrix& transform, const Vector2& size, const Vector2& origin)
{
    auto invOrigin = Vector2(1.f - origin.x, 1.f - origin.y);

    Vector2 tl(-size.x * origin.x, -size.y * origin.y);
    Vector2 bl(-size.x * origin.x, size.y * invOrigin.y);
    Vector2 br(size.x * invOrigin.x, size.y * invOrigin.y);
    Vector2 tr(size.x * invOrigin.x, -size.y * origin.y);

    Vector2 corners[4] = {
        Vector2::Transform(tl, transform),
        Vector2::Transform(bl, transform),
        Vector2::Transform(br, transform),
        Vector2::Transform(tr, transform)
    };

    Vector2 mins = corners[0];
    Vector2 maxs = corners[0];

    for (int i = 1; i < 4; ++i)
    {
        mins.x = std::min(mins.x, corners[i].x);
        mins.y = std::min(mins.y, corners[i].y);
        maxs.x = std::max(maxs.x, corners[i].x);
        maxs.y = std::max(maxs.y, corners[i].y);
    }

    return Rect(mins, maxs - mins);
}
