#pragma once

namespace onut
{
    enum class BlendMode
    {
        Opaque,
        Alpha,
        Add,
        PreMultiplied,
        Multiply,
        ForceWrite,

        COUNT
    };
};

#define OBlendOpaque onut::BlendMode::Opaque
#define OBlendAlpha onut::BlendMode::Alpha
#define OBlendAdd onut::BlendMode::Add
#define OBlendPreMultiplied onut::BlendMode::PreMultiplied
#define OBlendMultiply onut::BlendMode::Multiply
#define OBlendForceWrite onut::BlendMode::ForceWrite
