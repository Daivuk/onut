#pragma once

#include <onut/Rect.h>
#include <onut/Matrix.h>
#include <onut/Vector2.h>

class TransformHelper final
{
public:
    static Rect getWorldRect(const Matrix& transform, const Vector2& size, const Vector2& origin);
};
