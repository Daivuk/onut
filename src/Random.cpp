// Onut
#include <onut/Random.h>

// STL
#include <stdlib.h>
#include <time.h>

namespace onut
{
    void setSeed(unsigned int seed)
    {
        srand(seed);
    }

    unsigned int randomizeSeed()
    {
        auto seed = static_cast<unsigned int>(time(nullptr));
        setSeed(seed);
        return seed;
    }

    int randi()
    {
        return rand();
    }

    int randi(int max)
    {
        return rand() % (max + 1);
    }

    int randi(int min, int max)
    {
        auto range = max - min + 1;
        return rand() % range + min;
    }

    bool randb()
    {
        return randi(0, 1) ? true : false;
    }

    bool randb(float chances)
    {
        return (randf(0.f, 1.f) <= chances);
    }

    float randf(float max)
    {
        auto rnd = rand();
        auto rndf = static_cast<double>(rnd) / static_cast<double>(RAND_MAX - 1);
        rndf *= static_cast<double>(max);
        return static_cast<float>(rndf);
    }

    float randf(float min, float max)
    {
        auto rnd = rand();
        auto rndf = static_cast<double>(rnd) / static_cast<double>(RAND_MAX - 1);
        rndf *= static_cast<double>(max - min);
        return static_cast<float>(rndf)+min;
    }

    Vector2 rand2f(const Vector2& max)
    {
        return{randf(max.x), randf(max.y)};
    }

    Vector2 rand2f(const Vector2& min, const Vector2& max)
    {
        return{randf(min.x, max.x), randf(min.y, max.y)};
    }

    Vector2 randCircle(const Vector2& center, float radius)
    {
        float dist = ORandFloat(0.0f, 1.0f);
        dist *= dist;
        dist = 1 - dist;
        dist *= radius;
        float angle = ORandFloat(0.0f, O2PI);
        return{center.x + cosf(angle) * dist, center.y + sinf(angle) * dist};
    }

    Vector2 randCircleEdge(const Vector2& center, float radius)
    {
        float angle = ORandFloat(0.0f, O2PI);
        return{center.x + cosf(angle) * radius, center.y + sinf(angle) * radius};
    }

    Vector3 rand3f(const Vector3& max)
    {
        return{randf(max.x), randf(max.y), randf(max.z)};
    }

    Vector3 rand3f(const Vector3& min, const Vector3& max)
    {
        return{randf(min.x, max.x), randf(min.y, max.y), randf(min.z, max.z)};
    }

    Vector4 rand4f(const Vector4& max)
    {
        return{randf(max.x), randf(max.y), randf(max.z), randf(max.w)};
    }

    Vector4 rand4f(const Vector4& min, const Vector4& max)
    {
        return{randf(min.x, max.x), randf(min.y, max.y), randf(min.z, max.z), randf(min.w, max.w)};
    }

    Color randc(float alpha)
    {
        return{randf(1.f) * alpha, randf(1.f) * alpha, randf(1.f) * alpha, alpha};
    }

    Color randc(const Color& max)
    {
        return{randf(max.r), randf(max.g), randf(max.b), randf(max.a)};
    }

    Color randc(const Color& min, const Color& max)
    {
        return{randf(min.r, max.r), randf(min.g, max.g), randf(min.b, max.b), randf(min.a, max.a)};
    }

    static float saturate(float a)
    {
        return std::min(1.0f, std::max(a, 0.0f));
    }

    Color randc(const Color& color, float variation)
    {
        // TODO: Use HSB instead
        return Color(
            saturate(randf(color.r - variation, color.r + variation)),
            saturate(randf(color.g - variation, color.g + variation)),
            saturate(randf(color.b - variation, color.b + variation)),
            color.a
        );
    }

    Color randc(const std::vector<Color>& palette, float variation)
    {
        return randc(randv(palette), variation);
    }

    template<> int randt<int>(const int& min, const int& max)
    {
        return randi(min, max);
    }

    template<> int randt<int>(const int& max)
    {
        return randi(max);
    }

    template<> unsigned int randt<unsigned int>(const unsigned int& min, const unsigned int& max)
    {
        auto range = max - min + 1;
        return rand() % range + min;
    }

    template<> unsigned int randt<unsigned int>(const unsigned int& max)
    {
        return rand() % (max + 1);
    }

    template<> float randt<float>(const float& min, const float& max)
    {
        return randf(min, max);
    }

    template<> float randt<float>(const float& max)
    {
        return randf(max);
    }

    template<> double randt<double>(const double& min, const double& max)
    {
        auto rnd = rand();
        auto rndf = static_cast<double>(rnd) / static_cast<double>(RAND_MAX - 1);
        rndf *= max - min;
        return rndf + min;
    }

    template<> double randt<double>(const double& max)
    {
        auto rnd = rand();
        auto rndf = static_cast<double>(rnd) / static_cast<double>(RAND_MAX - 1);
        rndf *= max;
        return rndf;
    }

    template<> Vector2 randt<Vector2>(const Vector2& min, const Vector2& max)
    {
        return rand2f(min, max);
    }

    template<> Vector2 randt<Vector2>(const Vector2& max)
    {
        return rand2f(max);
    }

    template<> Vector3 randt<Vector3>(const Vector3& min, const Vector3& max)
    {
        return rand3f(min, max);
    }

    template<> Vector3 randt<Vector3>(const Vector3& max)
    {
        return rand3f(max);
    }

    template<> Vector4 randt<Vector4>(const Vector4& min, const Vector4& max)
    {
        return rand4f(min, max);
    }

    template<> Vector4 randt<Vector4>(const Vector4& max)
    {
        return rand4f(max);
    }

    template<> Color randt<Color>(const Color& min, const Color& max)
    {
        return randc(min, max);
    }

    template<> Color randt<Color>(const Color& max)
    {
        return randc(max);
    }
}
