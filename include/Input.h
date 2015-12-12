#pragma once
#define DIRECTINPUT_VERSION 0x0800
#include "dinput.h"
#include "SimpleMath.h"
using namespace DirectX::SimpleMath;

#define OINPUT_ESCAPE          0x01
#define OINPUT_1               0x02
#define OINPUT_2               0x03
#define OINPUT_3               0x04
#define OINPUT_4               0x05
#define OINPUT_5               0x06
#define OINPUT_6               0x07
#define OINPUT_7               0x08
#define OINPUT_8               0x09
#define OINPUT_9               0x0A
#define OINPUT_0               0x0B
#define OINPUT_MINUS           0x0C    /* - on main keyboard */
#define OINPUT_EQUALS          0x0D
#define OINPUT_BACK            0x0E    /* backspace */
#define OINPUT_TAB             0x0F
#define OINPUT_Q               0x10
#define OINPUT_W               0x11
#define OINPUT_E               0x12
#define OINPUT_R               0x13
#define OINPUT_T               0x14
#define OINPUT_Y               0x15
#define OINPUT_U               0x16
#define OINPUT_I               0x17
#define OINPUT_O               0x18
#define OINPUT_P               0x19
#define OINPUT_LBRACKET        0x1A
#define OINPUT_RBRACKET        0x1B
#define OINPUT_RETURN          0x1C    /* Enter on main keyboard */
#define OINPUT_LCONTROL        0x1D
#define OINPUT_A               0x1E
#define OINPUT_S               0x1F
#define OINPUT_D               0x20
#define OINPUT_F               0x21
#define OINPUT_G               0x22
#define OINPUT_H               0x23
#define OINPUT_J               0x24
#define OINPUT_K               0x25
#define OINPUT_L               0x26
#define OINPUT_SEMICOLON       0x27
#define OINPUT_APOSTROPHE      0x28
#define OINPUT_GRAVE           0x29    /* accent grave */
#define OINPUT_LSHIFT          0x2A
#define OINPUT_BACKSLASH       0x2B
#define OINPUT_Z               0x2C
#define OINPUT_X               0x2D
#define OINPUT_C               0x2E
#define OINPUT_V               0x2F
#define OINPUT_B               0x30
#define OINPUT_N               0x31
#define OINPUT_M               0x32
#define OINPUT_COMMA           0x33
#define OINPUT_PERIOD          0x34    /* . on main keyboard */
#define OINPUT_SLASH           0x35    /* / on main keyboard */
#define OINPUT_RSHIFT          0x36
#define OINPUT_MULTIPLY        0x37    /* * on numeric keypad */
#define OINPUT_LMENU           0x38    /* left Alt */
#define OINPUT_SPACE           0x39
#define OINPUT_CAPITAL         0x3A
#define OINPUT_F1              0x3B
#define OINPUT_F2              0x3C
#define OINPUT_F3              0x3D
#define OINPUT_F4              0x3E
#define OINPUT_F5              0x3F
#define OINPUT_F6              0x40
#define OINPUT_F7              0x41
#define OINPUT_F8              0x42
#define OINPUT_F9              0x43
#define OINPUT_F10             0x44
#define OINPUT_NUMLOCK         0x45
#define OINPUT_SCROLL          0x46    /* Scroll Lock */
#define OINPUT_NUMPAD7         0x47
#define OINPUT_NUMPAD8         0x48
#define OINPUT_NUMPAD9         0x49
#define OINPUT_SUBTRACT        0x4A    /* - on numeric keypad */
#define OINPUT_NUMPAD4         0x4B
#define OINPUT_NUMPAD5         0x4C
#define OINPUT_NUMPAD6         0x4D
#define OINPUT_ADD             0x4E    /* + on numeric keypad */
#define OINPUT_NUMPAD1         0x4F
#define OINPUT_NUMPAD2         0x50
#define OINPUT_NUMPAD3         0x51
#define OINPUT_NUMPAD0         0x52
#define OINPUT_DECIMAL         0x53    /* . on numeric keypad */
#define OINPUT_OEM_102         0x56    /* <> or \| on RT 102-key keyboard (Non-U.S.) */
#define OINPUT_F11             0x57
#define OINPUT_F12             0x58
#define OINPUT_F13             0x64    /*                     (NEC PC98) */
#define OINPUT_F14             0x65    /*                     (NEC PC98) */
#define OINPUT_F15             0x66    /*                     (NEC PC98) */
#define OINPUT_KANA            0x70    /* (Japanese keyboard)            */
#define OINPUT_ABNT_C1         0x73    /* /? on Brazilian keyboard */
#define OINPUT_CONVERT         0x79    /* (Japanese keyboard)            */
#define OINPUT_NOCONVERT       0x7B    /* (Japanese keyboard)            */
#define OINPUT_YEN             0x7D    /* (Japanese keyboard)            */
#define OINPUT_ABNT_C2         0x7E    /* Numpad . on Brazilian keyboard */
#define OINPUT_NUMPADEQUALS    0x8D    /* = on numeric keypad (NEC PC98) */
#define OINPUT_PREVTRACK       0x90    /* Previous Track (OINPUT_CIRCUMFLEX on Japanese keyboard) */
#define OINPUT_AT              0x91    /*                     (NEC PC98) */
#define OINPUT_COLON           0x92    /*                     (NEC PC98) */
#define OINPUT_UNDERLINE       0x93    /*                     (NEC PC98) */
#define OINPUT_KANJI           0x94    /* (Japanese keyboard)            */
#define OINPUT_STOP            0x95    /*                     (NEC PC98) */
#define OINPUT_AX              0x96    /*                     (Japan AX) */
#define OINPUT_UNLABELED       0x97    /*                        (J3100) */
#define OINPUT_NEXTTRACK       0x99    /* Next Track */
#define OINPUT_NUMPADENTER     0x9C    /* Enter on numeric keypad */
#define OINPUT_RCONTROL        0x9D
#define OINPUT_MUTE            0xA0    /* Mute */
#define OINPUT_CALCULATOR      0xA1    /* Calculator */
#define OINPUT_PLAYPAUSE       0xA2    /* Play / Pause */
#define OINPUT_MEDIASTOP       0xA4    /* Media Stop */
#define OINPUT_VOLUMEDOWN      0xAE    /* Volume - */
#define OINPUT_VOLUMEUP        0xB0    /* Volume + */
#define OINPUT_WEBHOME         0xB2    /* Web home */
#define OINPUT_NUMPADCOMMA     0xB3    /* , on numeric keypad (NEC PC98) */
#define OINPUT_DIVIDE          0xB5    /* / on numeric keypad */
#define OINPUT_SYSRQ           0xB7
#define OINPUT_RMENU           0xB8    /* right Alt */
#define OINPUT_PAUSE           0xC5    /* Pause */
#define OINPUT_HOME            0xC7    /* Home on arrow keypad */
#define OINPUT_UP              0xC8    /* UpArrow on arrow keypad */
#define OINPUT_PRIOR           0xC9    /* PgUp on arrow keypad */
#define OINPUT_LEFT            0xCB    /* LeftArrow on arrow keypad */
#define OINPUT_RIGHT           0xCD    /* RightArrow on arrow keypad */
#define OINPUT_END             0xCF    /* End on arrow keypad */
#define OINPUT_DOWN            0xD0    /* DownArrow on arrow keypad */
#define OINPUT_NEXT            0xD1    /* PgDn on arrow keypad */
#define OINPUT_INSERT          0xD2    /* Insert on arrow keypad */
#define OINPUT_DELETE          0xD3    /* Delete on arrow keypad */
#define OINPUT_LWIN            0xDB    /* Left Windows key */
#define OINPUT_RWIN            0xDC    /* Right Windows key */
#define OINPUT_APPS            0xDD    /* AppMenu key */
#define OINPUT_POWER           0xDE    /* System Power */
#define OINPUT_SLEEP           0xDF    /* System Sleep */
#define OINPUT_WAKE            0xE3    /* System Wake */
#define OINPUT_WEBSEARCH       0xE5    /* Web Search */
#define OINPUT_WEBFAVORITES    0xE6    /* Web Favorites */
#define OINPUT_WEBREFRESH      0xE7    /* Web Refresh */
#define OINPUT_WEBSTOP         0xE8    /* Web Stop */
#define OINPUT_WEBFORWARD      0xE9    /* Web Forward */
#define OINPUT_WEBBACK         0xEA    /* Web Back */
#define OINPUT_MYCOMPUTER      0xEB    /* My Computer */
#define OINPUT_MAIL            0xEC    /* Mail */
#define OINPUT_MEDIASELECT     0xED    /* Media Select */

/*
*  Alternate names for keys, to facilitate transition from DOS.
*/
#define OINPUT_BACKSPACE       OINPUT_BACK            /* backspace */
#define OINPUT_NUMPADSTAR      OINPUT_MULTIPLY        /* * on numeric keypad */
#define OINPUT_LALT            OINPUT_LMENU           /* left Alt */
#define OINPUT_CAPSLOCK        OINPUT_CAPITAL         /* CapsLock */
#define OINPUT_NUMPADMINUS     OINPUT_SUBTRACT        /* - on numeric keypad */
#define OINPUT_NUMPADPLUS      OINPUT_ADD             /* + on numeric keypad */
#define OINPUT_NUMPADPERIOD    OINPUT_DECIMAL         /* . on numeric keypad */
#define OINPUT_NUMPADSLASH     OINPUT_DIVIDE          /* / on numeric keypad */
#define OINPUT_RALT            OINPUT_RMENU           /* right Alt */
#define OINPUT_UPARROW         OINPUT_UP              /* UpArrow on arrow keypad */
#define OINPUT_PGUP            OINPUT_PRIOR           /* PgUp on arrow keypad */
#define OINPUT_LEFTARROW       OINPUT_LEFT            /* LeftArrow on arrow keypad */
#define OINPUT_RIGHTARROW      OINPUT_RIGHT           /* RightArrow on arrow keypad */
#define OINPUT_DOWNARROW       OINPUT_DOWN            /* DownArrow on arrow keypad */
#define OINPUT_PGDN            OINPUT_NEXT            /* PgDn on arrow keypad */

/*
*  Alternate names for keys originally not used on US keyboards.
*/
#define OINPUT_CIRCUMFLEX      OINPUT_PREVTRACK       /* Japanese keyboard */

#define OINPUT_MOUSEB1 256
#define OINPUT_MOUSEB2 257
#define OINPUT_MOUSEB3 258
#define OINPUT_MOUSEB4 259

#define OINPUT_MOUSEX 260
#define OINPUT_MOUSEY 261
#define OINPUT_MOUSEZ 262

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
