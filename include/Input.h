#pragma once
#define DIRECTINPUT_VERSION 0x0800
#include "dinput.h"
#include "SimpleMath.h"
using namespace DirectX::SimpleMath;

#define DIK_MOUSEB1 256
#define DIK_MOUSEB2 257
#define DIK_MOUSEB3 258
#define DIK_MOUSEB4 259

#define DIK_MOUSEX 260
#define DIK_MOUSEY 261
#define DIK_MOUSEZ 262

namespace onut
{
    class Input
    {
    public:
        Input(int in_stateCount);
        virtual ~Input();

        void update();
        void setStateDown(int in_stateId);
        void setStateUp(int in_stateId);
        void setStateValue(int in_stateId, float in_value);
        bool isStateDown(int in_stateId) const;
        bool isStateUp(int in_stateId) const;
        bool isStateJustDown(int in_stateId) const;
        bool isStateJustUp(int in_stateId) const;
        float getStateValue(int in_stateId) const;

        POINT   mousePos;
        Vector2 mousePosf;

    protected:
        struct sInputState
        {
            float	value = 0;
            bool	isDown = false;
            int		frameId = 0;
        };

        int				m_stateCount;
        int				m_frameId;
        sInputState*	m_prevStates;
        sInputState*	m_curStates;
        sInputState*	m_states;
        sInputState*	m_statesRestore;
    };
}
