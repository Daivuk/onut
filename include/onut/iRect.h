#ifndef IRECT_H_INCLUDED
#define IRECT_H_INCLUDED

#include <algorithm>

struct iRect
{
    int left;
    int top;
    int right;
    int bottom;

    bool operator==(const iRect& other) const
    {
        return
            other.left == left &&
            other.right == right &&
            other.top == top &&
            other.bottom == bottom;
    }

    bool operator!=(const iRect& other) const
    {
        return
            other.left != left ||
            other.right != right ||
            other.top != top ||
            other.bottom != bottom;
    }

    iRect operator*(int scalar) const
    {
        return { left * scalar, top * scalar, right * scalar, bottom * scalar };
    }

    iRect operator/(int scalar) const
    {
        return { left / scalar, top / scalar, right / scalar, bottom / scalar };
    }

    // New rect containing only overlapping part. Otherwise, 0,0,0,0 if not overlapping
    iRect Difference(const iRect& rect) const
    {
        iRect ret{ 0, 0, 0, 0 };

        auto _left = std::max(rect.left, left);
        auto _top = std::max(rect.top, top);
        auto _right = std::min(rect.right, right);
        auto _bottom = std::min(rect.bottom, bottom);

        if (_left > _right)
            return ret;

        if (_top > _bottom)
            return ret;

        ret.left = _left;
        ret.top = _top;
        ret.right = _right;
        ret.bottom = _bottom;

        return ret;
    }
};

#endif
