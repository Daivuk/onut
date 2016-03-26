#pragma once
// Onut
#include <onut/Maths.h>

// Third party
#if defined(WIN32)
#include <Windows.h>
#include <Xinput.h>
#endif

// Forward
#include <onut/ForwardDeclaration.h>
OForwardDeclare(GamePad);

namespace onut
{
    class GamePad
    {
    public:
        enum Button
        {
            A,
            B,
            X,
            Y,
            DPadUp,
            DPadDown,
            DPadLeft,
            DPadRight,
            LeftTrigger,
            LeftBumper,
            RightTrigger,
            RightBumper,
            LeftThumbStick,
            RightThumbStick,
            Start,
            Back,
            LeftThumbStickLeft,
            LeftThumbStickRight,
            LeftThumbStickUp,
            LeftThumbStickDown,
            RightThumbStickLeft,
            RightThumbStickRight,
            RightThumbStickUp,
            RightThumbStickDown
        };

        static OGamePadRef create(int index);

        GamePad(int index);

        void update();
        bool isConnected() const;
        bool isPressed(Button button) const;
        bool isJustPressed(Button button) const;
        bool isJustReleased(Button button) const;
        const Vector2& getLeftThumb() const { return m_cachedLeftThumb; }
        const Vector2& getRightThumb() const { return m_cachedRightThumb; }

    private:
        bool isPressed(Button button, const XINPUT_STATE& state) const;

#if defined(WIN32)
        XINPUT_STATE    m_currentState;
        XINPUT_STATE    m_previousState;
#endif
        Vector2         m_cachedLeftThumb;
        Vector2         m_cachedRightThumb;
        int             m_index = 0;
        bool            m_isConnected = false;
    };
};

#define OGamePadA onut::GamePad::Button::A
#define OGamePadB onut::GamePad::Button::B
#define OGamePadX onut::GamePad::Button::X
#define OGamePadY onut::GamePad::Button::Y
#define OGamePadDPadUp onut::GamePad::Button::DPadUp
#define OGamePadDPadDown onut::GamePad::Button::DPadDown
#define OGamePadDPadLeft onut::GamePad::Button::DPadLeft
#define OGamePadDPadRight onut::GamePad::Button::DPadRight
#define OGamePadLeftTrigger onut::GamePad::Button::LeftTrigger
#define OGamePadLeftBumper onut::GamePad::Button::LeftBumper
#define OGamePadRightTrigger onut::GamePad::Button::RightTrigger
#define OGamePadRightBumper onut::GamePad::Button::RightBumper
#define OGamePadLeftThumb onut::GamePad::Button::LeftThumbStick
#define OGamePadRightThumb onut::GamePad::Button::RightThumbStick
#define OGamePadStart onut::GamePad::Button::Start
#define OGamePadBack onut::GamePad::Button::Back
#define OGamePadLeftThumbLeft onut::GamePad::Button::LeftThumbStickLeft
#define OGamePadLeftThumbRight onut::GamePad::Button::LeftThumbStickRight
#define OGamePadLeftThumbUp onut::GamePad::Button::LeftThumbStickUp
#define OGamePadLeftThumbDown onut::GamePad::Button::LeftThumbStickDown
#define OGamePadRightThumbLeft onut::GamePad::Button::RightThumbStickLeft
#define OGamePadRightThumbRight onut::GamePad::Button::RightThumbStickRight
#define OGamePadRightThumbUp onut::GamePad::Button::RightThumbStickUp
#define OGamePadRightThumbDown onut::GamePad::Button::RightThumbStickDown

const OGamePadRef& OGetGamePad(int index);
bool OGamePadPressed(onut::GamePad::Button button, int gamePadIndex = 0);
bool OGamePadJustPressed(onut::GamePad::Button button, int gamePadIndex = 0);
bool OGamePadJustReleased(onut::GamePad::Button button, int gamePadIndex = 0);
const Vector2& OGetGamePadLeftThumb(int gamePadIndex = 0);
const Vector2& OGetGamePadRightThumb(int gamePadIndex = 0);
