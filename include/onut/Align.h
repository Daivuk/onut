#ifndef ALIGN_H_INCLUDED
#define ALIGN_H_INCLUDED


// Alignment
namespace onut
{
    enum class Align
    {
        TopLeft,
        Top,
        TopRight,
        Left,
        Center,
        Right,
        BottomLeft,
        Bottom,
        BottomRight
    };
}

#define OTopLeft onut::Align::TopLeft
#define OTop onut::Align::Top
#define OTopRight onut::Align::TopRight
#define OLeft onut::Align::Left
#define OCenter onut::Align::Center
#define ORight onut::Align::Right
#define OBottomLeft onut::Align::BottomLeft
#define OBottom onut::Align::Bottom
#define OBottomRight onut::Align::BottomRight

#endif
