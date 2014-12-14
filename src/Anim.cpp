#include "Anim.h"

namespace onut {
	float applyTween(const float t, TweenType tween)  {
		switch (tween) {
		case TweenType::NONE:
			return 0.f;
		case TweenType::LINEAR:
			return t;
		case TweenType::EASE_IN:
			return t * t;
		case TweenType::EASE_OUT: {
			auto inv = 1 - t;
			return 1 - inv * inv;
		}
		case TweenType::EASE_BOTH: {
			if (t < .5f) {
				return t * t * 2.f;
			}
			else {
				auto clamped = (t - .5f) * 2.f;
				auto inv = 1 - clamped;
				clamped = 1 - inv * inv;
				return clamped * .5f + .5f;
			}
			return 0.f;
		}
		case TweenType::BOUNCE_IN: {
			auto inv = 1 - t;
			float ret;
			if (inv < (1 / 2.75f))
			{
				ret =(7.5625f * inv * inv);
			}
			else if (inv < (2 / 2.75f))
			{
				float postFix = inv - (1.5f / 2.75f);
				ret = 7.5625f * postFix * postFix + .75f;
			}
			else if (inv < (2.5 / 2.75))
			{
				float postFix = inv - (2.25f / 2.75f);
				ret = 7.5625f * postFix * postFix + .9375f;
			}
			else
			{
				float postFix = inv - (2.625f / 2.75f);
				ret = 7.5625f * postFix * postFix + .984375f;
			}
			return 1 - ret;
		}
		case TweenType::BOUNCE_OUT:
			if (t < (1 / 2.75f))
			{
				return (7.5625f * t * t);
			}
			else if (t < (2 / 2.75f))
			{
				float postFix = t - (1.5f / 2.75f);
				return (7.5625f * postFix * postFix + .75f);
			}
			else if (t < (2.5 / 2.75))
			{
				float postFix = t - (2.25f / 2.75f);
				return (7.5625f * postFix * postFix + .9375f);
			}
			else
			{
				float postFix = t - (2.625f / 2.75f);
				return (7.5625f * postFix * postFix + .984375f);
			}
		case TweenType::SPRING_IN: {
#define SPRING_DIS .3f
#define INV_SPRING_DIS .7f
			float ret;
			if (t < SPRING_DIS)
			{
				ret = t / SPRING_DIS;
				ret = 1 - (1 - ret) * (1 - ret);
				ret = ret * -SPRING_DIS;
			}
			else
			{
				ret = (t - SPRING_DIS) / INV_SPRING_DIS;
				ret *= ret;
				ret = ret * (1 + SPRING_DIS) - SPRING_DIS;
			}
			return ret;
		}
		case TweenType::SPRING_OUT: {
			auto inv = 1 - t;
			if (inv < SPRING_DIS)
			{
				inv = inv / SPRING_DIS;
				inv = 1 - (1 - inv) * (1 - inv);
				inv = inv * -SPRING_DIS;
			}
			else
			{
				inv = (inv - SPRING_DIS) / INV_SPRING_DIS;
				inv *= inv;
				inv = inv * (1 + SPRING_DIS) - SPRING_DIS;
			}
			return 1 - inv;
		}
		}
		return t;
	}

	TweenType invertTween(TweenType tween) {
		switch (tween) {
		case TweenType::NONE:
			return TweenType::NONE;
		case TweenType::LINEAR:
			return TweenType::LINEAR;
		case TweenType::EASE_IN:
			return TweenType::EASE_OUT;
		case TweenType::EASE_OUT: 
			return TweenType::EASE_IN;
		case TweenType::EASE_BOTH: 
			return TweenType::EASE_BOTH;
		case TweenType::BOUNCE_IN: 
			return TweenType::BOUNCE_OUT;
		case TweenType::BOUNCE_OUT:
			return TweenType::BOUNCE_IN;
		case TweenType::SPRING_IN: 
			return TweenType::SPRING_OUT;
		case TweenType::SPRING_OUT: 
			return TweenType::SPRING_IN;
		}
		return TweenType::NONE;
	}
}
