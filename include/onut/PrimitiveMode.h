#pragma once

namespace onut
{
    enum class PrimitiveMode
    {
        PointList,
        LineList,
        LineStrip,
        TriangleList,
        TriangleStrip,

        COUNT
    };
};

#define OPrimitivePointList onut::PrimitiveMode::PointList
#define OPrimitiveLineList onut::PrimitiveMode::LineList
#define OPrimitiveLineStrip onut::PrimitiveMode::LineStrip
#define OPrimitiveTriangleList onut::PrimitiveMode::TriangleList
#define OPrimitiveTriangleStrip onut::PrimitiveMode::TriangleStrip
