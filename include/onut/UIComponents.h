#pragma once
// Onut
#include <onut/Maths.h>

// STL
#include <cinttypes>

// Forward
#include <onut/ForwardDeclaration.h>
OForwardDeclare(Texture)

namespace onut
{
    class UIFontComponent
    {
    public:
        enum Flag : uint8_t
        {
            AutoFitSize = 0x01,
            Ellipsis = 0x02,
            WordWrap = 0x04
        };
        Color color = Color::White;
        onut::Align align;
        Vector4 padding;
        std::string typeFace;
        float size;
        uint8_t flags;
        float minSize;
        UIFontComponent() : align(onut::Align::TopLeft), typeFace("Arial"), size(12.f), flags(0), minSize(12.f) {}
    };

    class UITextComponent
    {
    public:
        std::string text;
        UIFontComponent font;
    };

    class UIImageComponent
    {
    public:
        std::string filename;
        Color color = Color::White;
        OTextureRef pTexture;
    };

    class UIScale9Component
    {
    public:
        UIImageComponent image;
        bool isScaled9;
        bool isRepeat;
        Vector4 padding;
        UIScale9Component() : isScaled9(false), isRepeat(false) {}
    };

    class UIIconComponent
    {
    public:
        UIScale9Component background;
        onut::Align align;
        Rect rect;
        UIIconComponent() : align(onut::Align::TopLeft) {}
    };
};
