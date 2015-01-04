#include <stdlib.h>
#include <time.h>

#include "Random.h"

namespace onut
{
    void randomizeSeed()
    {
        srand(static_cast<unsigned int>(time(nullptr)));
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
        return{randf(1.f), randf(1.f), randf(1.f), alpha};
    }

    Color randc(const Color& max)
    {
        return{randf(max.x), randf(max.y), randf(max.z), randf(max.w)};
    }

    Color randc(const Color& min, const Color& max)
    {
        return{randf(min.x, max.x), randf(min.y, max.y), randf(min.z, max.z), randf(min.w, max.w)};
    }

    Color randc(const Color& color, float variation)
    {
        return color;
    }

    Color randc(const std::vector<Color>& palette, float variation)
    {
        return randc(randv(palette), variation);
    }

    int randt(const int& min, const int& max)
    {
        return randi(min, max);
    }

    int randt(const int& max)
    {
        return randi(max);
    }

    unsigned int randt(const unsigned int& min, const unsigned int& max)
    {
        auto range = max - min + 1;
        return rand() % range + min;
    }

    unsigned int randt(const unsigned int& max)
    {
        return rand() % (max + 1);
    }

    float randt(const float& min, const float& max)
    {
        return randf(min, max);
    }

    float randt(const float& max)
    {
        return randf(max);
    }

    double randt(const double& min, const double& max)
    {
        auto rnd = rand();
        auto rndf = static_cast<double>(rnd) / static_cast<double>(RAND_MAX - 1);
        rndf *= max - min;
        return rndf + min;
    }

    double randt(const double& max)
    {
        auto rnd = rand();
        auto rndf = static_cast<double>(rnd) / static_cast<double>(RAND_MAX - 1);
        rndf *= max;
        return rndf;
    }

    Vector2 randt(const Vector2& min, const Vector2& max)
    {
        return rand2f(min, max);
    }

    Vector2 randt(const Vector2& max)
    {
        return rand2f(max);
    }

    Vector3 randt(const Vector3& min, const Vector3& max)
    {
        return rand3f(min, max);
    }

    Vector3 randt(const Vector3& max)
    {
        return rand3f(max);
    }

    Vector4 randt(const Vector4& min, const Vector4& max)
    {
        return rand4f(min, max);
    }

    Vector4 randt(const Vector4& max)
    {
        return rand4f(max);
    }

    Color randt(const Color& min, const Color& max)
    {
        return randc(min, max);
    }

    Color randt(const Color& max)
    {
        return randc(max);
    }
}
