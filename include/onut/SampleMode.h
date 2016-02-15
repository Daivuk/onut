#pragma once

namespace onut
{
    namespace sample
    {
        enum class Filtering
        {
            Nearest,
            Linear,

            COUNT
        };

        enum class AddressMode
        {
            Wrap,
            Clamp,

            COUNT
        };
    };
};

#define OFilterNearest onut::sample::Filtering::Nearest
#define OFilterLinear onut::sample::Filtering::Linear

#define OTextureWrap onut::sample::AddressMode::Wrap
#define OTextureClamp onut::sample::AddressMode::Clamp
