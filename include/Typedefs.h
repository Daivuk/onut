#pragma once
#include "SimpleMath.h"
using namespace DirectX::SimpleMath;

typedef Vector4 Rect;

namespace onut {
	enum class Align {
		TOP_LEFT,
		TOP,
		TOP_RIGHT,
		LEFT,
		CENTER,
		RIGHT,
		BOTTOM_LEFT,
		BOTTOM,
		BOTTOM_RIGHT
	};
}
