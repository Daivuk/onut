// Onut
#include <onut/Curve.h>

namespace onut
{

    bool lerp(bool from, bool to, float t)
    {
        return (t < .5f) ? from : to;
    }

    int lerp(int from, int to, float t)
    {
        auto result = static_cast<float>(from)+(static_cast<float>(to)-static_cast<float>(from)) * t;
        return static_cast<int>(std::round(result));
    }

    unsigned int lerp(unsigned int from, unsigned int to, float t)
    {
        auto result = static_cast<float>(from)+(static_cast<float>(to)-static_cast<float>(from)) * t;
        result = std::round(result);
        if (result < 0.f) result = 0.f;
        return static_cast<unsigned int>(result);
    }

    float lerp(float from, float to, float t)
    {
        return from + (to - from) * t;
    }

    double lerp(double from, double to, float t)
    {
        return from + (to - from) * static_cast<double>(t);
    }

    Point lerp(const Point& from, const Point& to, float t)
    {
        return Point(lerp(from.x, to.x, t), lerp(from.y, to.y, t));
    }

    Vector2 lerp(const Vector2& from, const Vector2& to, float t)
    {
        return Vector2::Lerp(from, to, t);
    }

    Vector3 lerp(const Vector3& from, const Vector3& to, float t)
    {
        return Vector3::Lerp(from, to, t);
    }

    Vector4 lerp(const Vector4& from, const Vector4& to, float t)
    {
        return Vector4::Lerp(from, to, t);
    }

    Matrix lerp(const Matrix& from, const Matrix& to, float t)
    {
        return Matrix::Lerp(from, to, t);
    }

    Color lerp(const Color& from, const Color& to, float t)
    {
        return Color::Lerp(from, to, t);
    }

    std::string lerp(const std::string& from, const std::string& to, float t)
    {
        auto fromLen = from.size();
        auto toLen = to.size();
        auto newLen = static_cast<float>(fromLen)+(static_cast<float>(toLen)-static_cast<float>(fromLen)) * t;
        newLen = round(newLen);
        if (toLen > fromLen)
        {
            auto ret = to.substr(0, static_cast<size_t>(newLen));
            return std::move(ret);
        }
        else
        {
            auto ret = from.substr(0, static_cast<size_t>(newLen));
            return std::move(ret);
        }
    }

    // Bezier
    //\mathbf{P}(t) = \mathbf{P}_0(1 - t) ^ 3 + 3 \mathbf{P}_1 t(1 - t) ^ 2 + 3 \mathbf{P}_2 t ^ 2(1 - t) + \mathbf{P}_3 t ^ 3
}
