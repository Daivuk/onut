// Onut
#include <onut/Tween.h>

namespace onut
{
    float applyTween(float t, Tween tween)
    {
        static const float SPRING_DIS = .3f;
        static const float INV_SPRING_DIS = .7f;

        switch (tween)
        {
            case Tween::None:
                return 0.f;
            case Tween::Linear:
                return t;
            case Tween::EaseIn:
                return t * t;
            case Tween::EaseOut:
            {
                auto inv = 1.f - t;
                return 1.f - inv * inv;
            }
            case Tween::EaseBoth:
            {
                if (t < .5f)
                {
                    return t * t * 2.f;
                }
                else
                {
                    auto clamped = (t - .5f) * 2.f;
                    auto inv = 1 - clamped;
                    clamped = 1 - inv * inv;
                    return clamped * .5f + .5f;
                }
                return 0.f;
            }
            case Tween::BounceIn:
            {
                auto inv = 1.f - t;
                float ret;
                if (inv < (1.f / 2.75f))
                {
                    ret = (7.5625f * inv * inv);
                }
                else if (inv < (2.f / 2.75f))
                {
                    auto postFix = inv - (1.5f / 2.75f);
                    ret = 7.5625f * postFix * postFix + .75f;
                }
                else if (inv < (2.5f / 2.7f))
                {
                    auto postFix = inv - (2.25f / 2.75f);
                    ret = 7.5625f * postFix * postFix + .9375f;
                }
                else
                {
                    auto postFix = inv - (2.625f / 2.75f);
                    ret = 7.5625f * postFix * postFix + .984375f;
                }
                return 1 - ret;
            }
            case Tween::BounceOut:
                if (t < (1.f / 2.75f))
                {
                    return (7.5625f * t * t);
                }
                else if (t < (2.f / 2.75f))
                {
                    auto postFix = t - (1.5f / 2.75f);
                    return (7.5625f * postFix * postFix + .75f);
                }
                else if (t < (2.5f / 2.75f))
                {
                    auto postFix = t - (2.25f / 2.75f);
                    return (7.5625f * postFix * postFix + .9375f);
                }
                else
                {
                    auto postFix = t - (2.625f / 2.75f);
                    return (7.5625f * postFix * postFix + .984375f);
                }
            case Tween::SpringIn:
            {
                float ret;
                if (t < SPRING_DIS)
                {
                    ret = t / SPRING_DIS;
                    ret = 1.f - (1.f - ret) * (1.f - ret);
                    ret = ret * -SPRING_DIS;
                }
                else
                {
                    ret = (t - SPRING_DIS) / INV_SPRING_DIS;
                    ret *= ret;
                    ret = ret * (1.f + SPRING_DIS) - SPRING_DIS;
                }
                return ret;
            }
            case Tween::SpringOut:
            {
                auto inv = 1.f - t;
                if (inv < SPRING_DIS)
                {
                    inv = inv / SPRING_DIS;
                    inv = 1.f - (1.f - inv) * (1.f - inv);
                    inv = inv * -SPRING_DIS;
                }
                else
                {
                    inv = (inv - SPRING_DIS) / INV_SPRING_DIS;
                    inv *= inv;
                    inv = inv * (1.f + SPRING_DIS) - SPRING_DIS;
                }
                return 1.f - inv;
            }
        }
        return t;
    }

    Tween invertTween(Tween tween)
    {
        switch (tween)
        {
            case Tween::None:
                return Tween::None;
            case Tween::Linear:
                return Tween::Linear;
            case Tween::EaseIn:
                return Tween::EaseOut;
            case Tween::EaseOut:
                return Tween::EaseIn;
            case Tween::EaseBoth:
                return Tween::EaseBoth;
            case Tween::BounceIn:
                return Tween::BounceOut;
            case Tween::BounceOut:
                return Tween::BounceIn;
            case Tween::SpringIn:
                return Tween::SpringOut;
            case Tween::SpringOut:
                return Tween::SpringIn;
        }
        return Tween::None;
    }
}
