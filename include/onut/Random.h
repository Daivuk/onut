#pragma once
// Onut
#include <onut/Maths.h>

// STL
#include <algorithm>
#include <vector>

namespace onut
{
    using Palette = std::vector<Color>;

    void setSeed(unsigned int seed);
    unsigned int randomizeSeed();

    int randi();
    int randi(int max);
    int randi(int min, int max);

    bool randb();
    bool randb(float chances);

    float randf(float max);
    float randf(float min, float max);

    Vector2 rand2f(const Vector2& max);
    Vector2 rand2f(const Vector2& min, const Vector2& max);
    Vector3 rand3f(const Vector3& max);
    Vector3 rand3f(const Vector3& min, const Vector3& max);
    Vector4 rand4f(const Vector4& max);
    Vector4 rand4f(const Vector4& min, const Vector4& max);

    Color randc(float alpha = 1.f);
    Color randc(const Color& max);
    Color randc(const Color& min, const Color& max);
    Color randc(const Color& color, float variation);
    Color randc(const Palette& palette, float variation = 0.f);

    template<typename T, int N>
    const T& randa(const T(&arr)[N])
    {
        auto rnd = randi(N - 1);
        return arr[rnd];
    }

    template<typename T>
    const T& randv(const std::vector<T>& vec)
    {
        auto rnd = randi(vec.size() - 1);
        return vec[rnd];
    }

    template<typename Ttype>
    Ttype randt(const Ttype& min, const Ttype& max);
    template<typename Ttype>
    Ttype randt(const Ttype& max);

    template<typename T>
    void shuffle(std::vector<T>& vec)
    {
        std::random_shuffle(vec.begin(), vec.end());
    }

    Vector2 randCircle(const Vector2& center, float radius);
    Vector2 randCircleEdge(const Vector2& center, float radius);
}

#define ORandColor onut::randc
#define ORandBool onut::randb
#define ORandInt onut::randi
#define ORandFloat onut::randf
#define ORandArray onut::randa
#define ORandVector onut::randv
#define ORandVector2 onut::rand2f
#define ORandVector3 onut::rand3f
#define ORandVector4 onut::rand4f
#define ORand onut::randt
