#ifndef GAMEPADXINPUT_H_INCLUDED
#define GAMEPADXINPUT_H_INCLUDED

#if defined(WIN32)
// onut
#include <onut/GamePad.h>

// Third party
#include <Windows.h>
#include <Xinput.h>

// Forward
#include <onut/ForwardDeclaration.h>
OForwardDeclare(GamePadXInput);

namespace onut
{
    class GamePadXInput final : public GamePad
    {
    public:
        GamePadXInput(int index);
        ~GamePadXInput();

        void update() override;
        bool isConnected() const override;
        bool isPressed(Button button) const override;
        bool isJustPressed(Button button) const override;
        bool isJustReleased(Button button) const override;

    private:
        bool isPressed(Button button, const XINPUT_STATE& state) const;

        XINPUT_STATE    m_currentState;
        XINPUT_STATE    m_previousState;
        bool            m_isConnected = false;
    };
}

#endif

#endif
