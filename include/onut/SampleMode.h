#ifndef SAMPLEMODE_H_INCLUDED
#define SAMPLEMODE_H_INCLUDED


namespace onut
{
    namespace sample
    {
        enum class Filtering
        {
            Nearest,
            Linear,
            Bilinear,
            Trilinear,
            Anisotropic,

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

#endif
