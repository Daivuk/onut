#pragma once

namespace onut
{
    enum class Tween
    {
        None,
        Linear,
        EaseIn,
        EaseOut,
        EaseBoth,
        BounceIn,
        BounceOut,
        SpringIn,
        SpringOut
    };

    float applyTween(float t, Tween tween);
    Tween invertTween(Tween tween);
};

#define OTweenNone onut::Tween::None
#define OTweenLinear onut::Tween::Linear
#define OTweenEaseIn onut::Tween::EaseIn
#define OTweenEaseOut onut::Tween::EaseOut
#define OTweenEaseBoth onut::Tween::EaseBoth
#define OTweenBounceIn onut::Tween::BounceIn
#define OTweenBounceOut onut::Tween::BounceOut
#define OTweenSpringIn onut::Tween::SpringIn
#define OTweenSpringOut onut::Tween::SpringOut

#define OApplyTween onut::applyTween
#define OInvertTween onut::invertTween
