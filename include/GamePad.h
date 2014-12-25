#pragma once
#include <Windows.h>
#include <Xinput.h>
#include "SimpleMath.h"
using namespace DirectX::SimpleMath;

namespace onut
{
    class GamePad
    {
    public:
        enum eGamePad
        {
            A,
            B,
            X,
            Y,
            DPAD_UP,
            DPAD_DOWN,
            DPAD_LEFT,
            DPAD_RIGHT,
            LT,
            LB,
            RT,
            RB,
            LTHUMB,
            RTHUMB,
            START,
            BACK,
            LTHUMB_LEFT,
            LTHUMB_RIGHT,
            LTHUMB_UP,
            LTHUMB_DOWN,
            RTHUMB_LEFT,
            RTHUMB_RIGHT,
            RTHUMB_UP,
            RTHUMB_DOWN
        };

        GamePad(int index);

        void            update();
        bool            isConnected() const;
        bool            isPressed(eGamePad button) const;
        bool            isJustPressed(eGamePad button) const;
        bool            isJustReleased(eGamePad button) const;
        const Vector2&  getLeftThumb() const { return m_cachedLeftThumb; }
        const Vector2&  getRightThumb() const { return m_cachedRightThumb; }

    private:
        bool isPressed(eGamePad button, const XINPUT_STATE& state) const;

        XINPUT_STATE    m_currentState;
        XINPUT_STATE    m_previousState;
        Vector2         m_cachedLeftThumb;
        Vector2         m_cachedRightThumb;
        int             m_index = 0;
        bool            m_isConnected = false;
    };
};
