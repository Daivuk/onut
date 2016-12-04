#ifndef IRECT_H_INCLUDED
#define IRECT_H_INCLUDED


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
};

#endif
