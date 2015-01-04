#pragma once
#include "SimpleMath.h"
#include <vector>
using namespace DirectX::SimpleMath;

namespace onut
{
    /**
    Seed
    */
    void randomizeSeed();

    /**
    Get a random int
    @param max Inclusive maximum
    @param min Inclusive minimum
    */
    int randi();
    int randi(int max);
    int randi(int min, int max);

    /**
    * @fn Returns random bool. True or false
    * @param chances Percentage (0-1) of changes to return true
    */
    bool randb();
    bool randb(float chances);

    /**
    Get a random float
    @param max Inclusive maximum
    @param min Inclusive minimum
    */
    float randf(float max);
    float randf(float min, float max);

    /**
    Random on vector maths
    */
    Vector2 rand2f(const Vector2& max);
    Vector2 rand2f(const Vector2& min, const Vector2& max);
    Vector3 rand3f(const Vector3& max);
    Vector3 rand3f(const Vector3& min, const Vector3& max);
    Vector4 rand4f(const Vector4& max);
    Vector4 rand4f(const Vector4& min, const Vector4& max);

    /**
    Random coloring
    */
    Color randc(float alpha = 1.f);
    Color randc(const Color& max);
    Color randc(const Color& min, const Color& max);
    Color randc(const Color& color, float variation);
    Color randc(const std::vector<Color>& palette, float variation = 0.f);

    /**
    Random pick in an array
    */
    template<typename T,
        int N>
        const T& randa(const T(&arr)[N])
    {
        auto rnd = randi(N - 1);
        return arr[rnd];
    }

    /**
    Random pick in an iteratable (Vector, list, etc)
    */
    template<typename T>
    const T& randv(const std::vector<T>& vec)
    {
        auto rnd = randi(vec.size() - 1);
        return vec[rnd];
    }

    /**
    Templated random function
    */
    template<typename Ttype>
    Ttype randt(const Ttype& min, const Ttype& max);
    template<typename Ttype>
    Ttype randt(const Ttype& max);
}
