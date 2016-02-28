#pragma once

struct Point
{
    Point() {}
    Point(int in_x) : x(in_x), y(in_x) {}
    Point(int in_x, int in_y) : x(in_x), y(in_y) {}

    inline Point operator+(const Point& other) const
    {
        return Point(x + other.x, y + other.y);
    }

    inline Point operator-(const Point& other) const
    {
        return Point(x - other.x, y - other.y);
    }

    inline Point operator*(const Point& other) const
    {
        return Point(x * other.x, y * other.y);
    }

    inline Point operator/(const Point& other) const
    {
        return Point(x / other.x, y / other.y);
    }

    inline Point operator+(const int& scalar) const
    {
        return Point(x + scalar, y + scalar);
    }

    inline Point operator-(const int& scalar) const
    {
        return Point(x - scalar, y - scalar);
    }

    inline Point operator*(const int& scalar) const
    {
        return Point(x * scalar, y * scalar);
    }

    inline Point operator/(const int& scalar) const
    {
        return Point(x / scalar, y / scalar);
    }

    inline Point& operator+=(const Point& other)
    {
        x += other.x;
        y += other.y;
        return *this;
    }

    inline Point& operator-=(const Point& other)
    {
        x -= other.x;
        y -= other.y;
        return *this;
    }

    inline Point& operator*=(const Point& other)
    {
        x *= other.x;
        y *= other.y;
        return *this;
    }

    inline Point& operator/=(const Point& other)
    {
        x /= other.x;
        y /= other.y;
        return *this;
    }

    inline Point& operator+=(const int& scalar)
    {
        x += scalar;
        y += scalar;
        return *this;
    }

    inline Point& operator-=(const int& scalar)
    {
        x -= scalar;
        y -= scalar;
        return *this;
    }

    inline Point& operator*=(const int& scalar)
    {
        x *= scalar;
        y *= scalar;
        return *this;
    }

    inline Point& operator/=(const int& scalar)
    {
        x /= scalar;
        y /= scalar;
        return *this;
    }

    inline bool operator==(const Point& other) const
    {
        return x == other.x && y == other.y;
    }

    inline bool operator!=(const Point& other) const
    {
        return x != other.x || y != other.y;
    }

    inline Point operator-() const
    {
        return{-x, -y};
    }

    int x = 0;
    int y = 0;
};
