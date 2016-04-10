#pragma once
// Onut
#include <onut/Maths.h>
#include <onut/Point.h>

// Forward
#include <onut/ForwardDeclaration.h>
OForwardDeclare(GamePad);
OForwardDeclare(Input);
OForwardDeclare(InputDevice);
OForwardDeclare(Window);

namespace onut
{
    class Window;

    class Input final
    {
    public:
        enum class State : int
        {
            None = 0,

            KeyEscape = 0x01,
            Key1 = 0x02,
            Key2 = 0x03,
            Key3 = 0x04,
            Key4 = 0x05,
            Key5 = 0x06,
            Key6 = 0x07,
            Key7 = 0x08,
            Key8 = 0x09,
            Key9 = 0x0A,
            Key0 = 0x0B,
            KeyMinus = 0x0C, /* - on main keyboard */
            KeyEquals = 0x0D,
            KeyBackspace = 0x0E, /* backspace */
            KeyTab = 0x0F,
            KeyQ = 0x10,
            KeyW = 0x11,
            KeyE = 0x12,
            KeyR = 0x13,
            KeyT = 0x14,
            KeyY = 0x15,
            KeyU = 0x16,
            KeyI = 0x17,
            KeyO = 0x18,
            KeyP = 0x19,
            KeyLeftBracket = 0x1A,
            KeyRightBracket = 0x1B,
            KeyEnter = 0x1C, /* Enter on main keyboard */
            KeyLeftControl = 0x1D,
            KeyA = 0x1E,
            KeyS = 0x1F,
            KeyD = 0x20,
            KeyF = 0x21,
            KeyG = 0x22,
            KeyH = 0x23,
            KeyJ = 0x24,
            KeyK = 0x25,
            KeyL = 0x26,
            KeySemiColon = 0x27,
            KeyApostrophe = 0x28,
            KeyGrave = 0x29,    /* accent grave */
            KeyLeftShift = 0x2A,
            KeyBackslash = 0x2B,
            KeyZ = 0x2C,
            KeyX = 0x2D,
            KeyC = 0x2E,
            KeyV = 0x2F,
            KeyB = 0x30,
            KeyN = 0x31,
            KeyM = 0x32,
            KeyComma = 0x33,
            KeyPeriod = 0x34, /* . on main keyboard */
            KeySlack = 0x35, /* / on main keyboard */
            KeyRightShift = 0x36,
            KeyMultiply = 0x37, /* * on numeric keypad */
            KeyLeftAlt = 0x38, /* left Alt */
            KeySpaceBar = 0x39,
            KeyCapsLock = 0x3A,
            KeyF1 = 0x3B,
            KeyF2 = 0x3C,
            KeyF3 = 0x3D,
            KeyF4 = 0x3E,
            KeyF5 = 0x3F,
            KeyF6 = 0x40,
            KeyF7 = 0x41,
            KeyF8 = 0x42,
            KeyF9 = 0x43,
            KeyF10 = 0x44,
            KeyNumLock = 0x45,
            KeyScrollLock = 0x46, /* Scroll Lock */
            KeyNumPad7 = 0x47,
            KeyNumPad8 = 0x48,
            KeyNumPad9 = 0x49,
            KeyNumPadMinus = 0x4A, /* - on numeric keypad */
            KeyNumPad4 = 0x4B,
            KeyNumPad5 = 0x4C,
            KeyNumPad6 = 0x4D,
            KeyNumPadAdd = 0x4E, /* + on numeric keypad */
            KeyNumPad1 = 0x4F,
            KeyNumPad2 = 0x50,
            KeyNumPad3 = 0x51,
            KeyNumPad0 = 0x52,
            KeyNumPadPeriod = 0x53, /* . on numeric keypad */
            KeyOEM102 = 0x56, /* <> or \| on RT 102-key keyboard (Non-U.S.) */
            KeyF11 = 0x57,
            KeyF12 = 0x58,
            KeyF13 = 0x64, /* (NEC PC98) */
            KeyF14 = 0x65, /* (NEC PC98) */
            KeyF15 = 0x66, /* (NEC PC98) */
            KeyKana = 0x70, /* (Japanese keyboard) */
            KeyAbntC1 = 0x73, /* /? on Brazilian keyboard */
            KeyConvert = 0x79, /* (Japanese keyboard) */
            KeyNoConvert = 0x7B, /* (Japanese keyboard) */
            KeyYen = 0x7D, /* (Japanese keyboard) */
            KeyAbntC2 = 0x7E, /* Numpad . on Brazilian keyboard */
            KeyNumPadEqualds = 0x8D, /* = on numeric keypad (NEC PC98) */
            KeyPreviousTrack = 0x90, /* Previous Track (OINPUT_CIRCUMFLEX on Japanese keyboard) */
            KeyAt = 0x91, /* (NEC PC98) */
            KeyColon = 0x92, /* (NEC PC98) */
            KeyUnderline = 0x93, /* (NEC PC98) */
            KeyKanji = 0x94, /* (Japanese keyboard) */
            KeyStop = 0x95, /* (NEC PC98) */
            KeyAx = 0x96, /* (Japan AX) */
            KeyUnlabeled = 0x97, /* (J3100) */
            KeyNextTrack = 0x99, /* Next Track */
            KeyNumPadEnter = 0x9C, /* Enter on numeric keypad */
            KeyRightControl = 0x9D,
            KeyMutex = 0xA0, /* Mute */
            KeyCalculator = 0xA1, /* Calculator */
            KeyPlayPause = 0xA2, /* Play / Pause */
            KeyMediaStop = 0xA4, /* Media Stop */
            KeyVolumeDown = 0xAE, /* Volume - */
            KeyVolumeUp = 0xB0, /* Volume + */
            KeyWebHome = 0xB2, /* Web home */
            KeyNumPadComma = 0xB3, /* , on numeric keypad (NEC PC98) */
            KeyNumPadDivide = 0xB5, /* / on numeric keypad */
            Key_SYSRQ = 0xB7,
            KeyRightAlt = 0xB8, /* right Alt */
            KeyAltCar = KeyRightAlt, /* right Alt */
            KeyPause = 0xC5, /* Pause */
            KeyHome = 0xC7, /* Home on arrow keypad */
            KeyUp = 0xC8, /* UpArrow on arrow keypad */
            KeyPageUp = 0xC9, /* PgUp on arrow keypad */
            KeyLeft = 0xCB, /* LeftArrow on arrow keypad */
            KeyRight = 0xCD, /* RightArrow on arrow keypad */
            KeyEnd = 0xCF, /* End on arrow keypad */
            KeyDown = 0xD0, /* DownArrow on arrow keypad */
            KeyPageDown = 0xD1, /* PgDn on arrow keypad */
            KeyInsert = 0xD2, /* Insert on arrow keypad */
            KeyDelete = 0xD3, /* Delete on arrow keypad */
            KeyLeftWindows = 0xDB, /* Left Windows key */
            KeyRightWindows = 0xDC, /* Right Windows key */
            KeyAppMenu = 0xDD, /* AppMenu key */
            KeyPower = 0xDE, /* System Power */
            KeySleep = 0xDF, /* System Sleep */
            KeyWake = 0xE3, /* System Wake */
            KeyWebSearch = 0xE5, /* Web Search */
            KeyWebFavorites = 0xE6, /* Web Favorites */
            KeyWebRefresh = 0xE7, /* Web Refresh */
            KeyWebStop = 0xE8, /* Web Stop */
            KeyWebForward = 0xE9, /* Web Forward */
            KeyWebBack = 0xEA, /* Web Back */
            KeyMyComputer = 0xEB, /* My Computer */
            KeyMailL = 0xEC, /* Mail */
            KeyMediaSelect = 0xED, /* Media Select */

            /*
            *  Alternate names for keys originally not used on US keyboards.
            */
            KeyCircomflex = KeyPreviousTrack, /* Japanese keyboard */

            /*
            * Mouse
            */
            Mouse1 = 256,
            Mouse2 = 257,
            Mouse3 = 258,
            Mouse4 = 259,
            MouseX = 260,
            MouseY = 261,
            MouseZ = 262,

            STATE_COUNT
        };

        static const int STATE_COUNT = static_cast<int>(State::STATE_COUNT);
        static const int GAME_PAD_COUNT = 4;

        static OInputRef create(const OWindowRef& pWindow);

        Input(const OWindowRef& pWindow);
        ~Input();

        void update();
        void setStateDown(State state);
        void setStateUp(State state);
        void setStateValue(State state, float in_value);
        bool isStateDown(State state) const;
        bool isStateUp(State state) const;
        bool isStateJustDown(State state) const;
        bool isStateJustUp(State state) const;
        float getStateValue(State state) const;

        const OGamePadRef& getGamePad(int index) const;

        Point mousePos;
        Vector2 mousePosf;

    protected:
        struct InputState
        {
            float value = 0;
            bool isDown = false;
            int frameId = 0;
        };

        int m_frameId;
        OInputDeviceRef m_pInputDevice;
        InputState*	m_prevStates;
        InputState*	m_curStates;
        InputState*	m_states;
        InputState*	m_statesRestore;
        OGamePadRef m_gamePads[GAME_PAD_COUNT];
    };
}

#define OKeyEscape onut::Input::State::KeyEscape
#define OKey1 onut::Input::State::Key1
#define OKey2 onut::Input::State::Key2
#define OKey3 onut::Input::State::Key3
#define OKey4 onut::Input::State::Key4
#define OKey5 onut::Input::State::Key5
#define OKey6 onut::Input::State::Key6
#define OKey7 onut::Input::State::Key7
#define OKey8 onut::Input::State::Key8
#define OKey9 onut::Input::State::Key9
#define OKey0 onut::Input::State::Key0
#define OKeyMinus onut::Input::State::KeyMinus /* - on main keyboard */
#define OKeyEquals onut::Input::State::KeyEquals
#define OKeyBackspace onut::Input::State::KeyBackspace /* backspace */
#define OKeyTab onut::Input::State::KeyTab
#define OKeyQ onut::Input::State::KeyQ
#define OKeyW onut::Input::State::KeyW
#define OKeyE onut::Input::State::KeyE
#define OKeyR onut::Input::State::KeyR
#define OKeyT onut::Input::State::KeyT
#define OKeyY onut::Input::State::KeyY
#define OKeyU onut::Input::State::KeyU
#define OKeyI onut::Input::State::KeyI
#define OKeyO onut::Input::State::KeyO
#define OKeyP onut::Input::State::KeyP
#define OKeyLeftBracket onut::Input::State::KeyLeftBracket
#define OKeyRightBracket onut::Input::State::KeyRightBracket
#define OKeyEnter onut::Input::State::KeyEnter /* Enter on main keyboard */
#define OKeyLeftControl onut::Input::State::KeyLeftControl
#define OKeyA onut::Input::State::KeyA
#define OKeyS onut::Input::State::KeyS
#define OKeyD onut::Input::State::KeyD
#define OKeyF onut::Input::State::KeyF
#define OKeyG onut::Input::State::KeyG
#define OKeyH onut::Input::State::KeyH
#define OKeyJ onut::Input::State::KeyJ
#define OKeyK onut::Input::State::KeyK
#define OKeyL onut::Input::State::KeyL
#define OKeySemiColon onut::Input::State::KeySemiColon
#define OKeyApostrophe onut::Input::State::KeyApostrophe
#define OKeyGrave onut::Input::State::KeyGrave /* accent grave */
#define OKeyLeftShift onut::Input::State::KeyLeftShift
#define OKeyBackslash onut::Input::State::KeyBackslash
#define OKeyZ onut::Input::State::KeyZ
#define OKeyX onut::Input::State::KeyX
#define OKeyC onut::Input::State::KeyC
#define OKeyV onut::Input::State::KeyV
#define OKeyB onut::Input::State::KeyB
#define OKeyN onut::Input::State::KeyN
#define OKeyM onut::Input::State::KeyM
#define OKeyComma onut::Input::State::KeyComma
#define OKeyPeriod onut::Input::State::KeyPeriod /* . on main keyboard */
#define OKeySlack onut::Input::State::KeySlack /* / on main keyboard */
#define OKeyRightShift onut::Input::State::KeyRightShift
#define OKeyMultiply onut::Input::State::KeyMultiply /* * on numeric keypad */
#define OKeyLeftAlt onut::Input::State::KeyLeftAlt /* left Alt */
#define OKeySpaceBar onut::Input::State::KeySpaceBar
#define OKeyCapsLock onut::Input::State::KeyCapsLock
#define OKeyF1 onut::Input::State::KeyF1
#define OKeyF2 onut::Input::State::KeyF2
#define OKeyF3 onut::Input::State::KeyF3
#define OKeyF4 onut::Input::State::KeyF4
#define OKeyF5 onut::Input::State::KeyF5
#define OKeyF6 onut::Input::State::KeyF6
#define OKeyF7 onut::Input::State::KeyF7
#define OKeyF8 onut::Input::State::KeyF8
#define OKeyF9 onut::Input::State::KeyF9
#define OKeyF10 onut::Input::State::KeyF10
#define OKeyNumLock onut::Input::State::KeyNumLock
#define OKeyScrollLock onut::Input::State::KeyScrollLock /* Scroll Lock */
#define OKeyNumPad7 onut::Input::State::KeyNumPad7
#define OKeyNumPad8 onut::Input::State::KeyNumPad8
#define OKeyNumPad9 onut::Input::State::KeyNumPad9
#define OKeyNumPadMinus onut::Input::State::KeyNumPadMinus /* - on numeric keypad */
#define OKeyNumPad4 onut::Input::State::KeyNumPad4
#define OKeyNumPad5 onut::Input::State::KeyNumPad5
#define OKeyNumPad6 onut::Input::State::KeyNumPad6
#define OKeyNumPadAdd onut::Input::State::KeyNumPadAdd /* + on numeric keypad */
#define OKeyNumPad1 onut::Input::State::KeyNumPad1
#define OKeyNumPad2 onut::Input::State::KeyNumPad2
#define OKeyNumPad3 onut::Input::State::KeyNumPad3
#define OKeyNumPad0 onut::Input::State::KeyNumPad0
#define OKeyNumPadPeriod onut::Input::State::KeyNumPadPeriod /* . on numeric keypad */
#define OKeyOEM102 onut::Input::State::KeyOEM102 /* <> or \| on RT 102-key keyboard (Non-U.S.) */
#define OKeyF11 onut::Input::State::KeyF11
#define OKeyF12 onut::Input::State::KeyF12
#define OKeyF13 onut::Input::State::KeyF13 /* (NEC PC98) */
#define OKeyF14 onut::Input::State::KeyF14 /* (NEC PC98) */
#define OKeyF15 onut::Input::State::KeyF15 /* (NEC PC98) */
#define OKeyKana onut::Input::State::KeyKana /* (Japanese keyboard) */
#define OKeyAbntC1 onut::Input::State::KeyAbntC1 /* /? on Brazilian keyboard */
#define OKeyConvert onut::Input::State::KeyConvert /* (Japanese keyboard) */
#define OKeyNoConvert onut::Input::State::KeyNoConvert /* (Japanese keyboard) */
#define OKeyYen onut::Input::State::KeyYen /* (Japanese keyboard) */
#define OKeyAbntC2 onut::Input::State::KeyAbntC2 /* Numpad . on Brazilian keyboard */
#define OKeyNumPadEqualds onut::Input::State::KeyNumPadEqualds /* = on numeric keypad (NEC PC98) */
#define OKeyPreviousTrack onut::Input::State::KeyPreviousTrack /* Previous Track (OINPUT_CIRCUMFLEX on Japanese keyboard) */
#define OKeyAt onut::Input::State::KeyAt /* (NEC PC98) */
#define OKeyColon onut::Input::State::KeyColon /* (NEC PC98) */
#define OKeyUnderline onut::Input::State::KeyUnderline /* (NEC PC98) */
#define OKeyKanji onut::Input::State::KeyKanji /* (Japanese keyboard) */
#define OKeyStop onut::Input::State::KeyStop /* (NEC PC98) */
#define OKeyAx onut::Input::State::KeyAx /* (Japan AX) */
#define OKeyUnlabeled onut::Input::State::KeyUnlabeled /* (J3100) */
#define OKeyNextTrack onut::Input::State::KeyNextTrack /* Next Track */
#define OKeyNumPadEnter onut::Input::State::KeyNumPadEnter /* Enter on numeric keypad */
#define OKeyRightControl onut::Input::State::KeyRightControl
#define OKeyMutex onut::Input::State::KeyMutex /* Mute */
#define OKeyCalculator onut::Input::State::KeyCalculator /* Calculator */
#define OKeyPlayPause onut::Input::State::KeyPlayPause /* Play / Pause */
#define OKeyMediaStop onut::Input::State::KeyMediaStop /* Media Stop */
#define OKeyVolumeDown onut::Input::State::KeyVolumeDown /* Volume - */
#define OKeyVolumeUp onut::Input::State::KeyVolumeUp /* Volume + */
#define OKeyWebHome onut::Input::State::KeyWebHome /* Web home */
#define OKeyNumPadComma onut::Input::State::KeyNumPadComma /* on numeric keypad (NEC PC98) */
#define OKeyNumPadDivide onut::Input::State::KeyNumPadDivide /* / on numeric keypad */
#define OKey_SYSRQ onut::Input::State::Key_SYSRQ
#define OKeyRightAlt onut::Input::State::KeyRightAlt /* right Alt */
#define OKeyAltCar onut::Input::State::KeyAltCar /* right Alt */
#define OKeyPause onut::Input::State::KeyPause /* Pause */
#define OKeyHome onut::Input::State::KeyHome /* Home on arrow keypad */
#define OKeyUp onut::Input::State::KeyUp /* UpArrow on arrow keypad */
#define OKeyPageUp onut::Input::State::KeyPageUp /* PgUp on arrow keypad */
#define OKeyLeft onut::Input::State::KeyLeft /* LeftArrow on arrow keypad */
#define OKeyRight onut::Input::State::KeyRight /* RightArrow on arrow keypad */
#define OKeyEnd onut::Input::State::KeyEnd /* End on arrow keypad */
#define OKeyDown onut::Input::State::KeyDown /* DownArrow on arrow keypad */
#define OKeyPageDown onut::Input::State::KeyPageDown /* PgDn on arrow keypad */
#define OKeyInsert onut::Input::State::KeyInsert /* Insert on arrow keypad */
#define OKeyDelete onut::Input::State::KeyDelete /* Delete on arrow keypad */
#define OKeyLeftWindows onut::Input::State::KeyLeftWindows /* Left Windows key */
#define OKeyRightWindows onut::Input::State::KeyRightWindows /* Right Windows key */
#define OKeyAppMenu onut::Input::State::KeyAppMenu /* AppMenu key */
#define OKeyPower onut::Input::State::KeyPower /* System Power */
#define OKeySleep onut::Input::State::KeySleep /* System Sleep */
#define OKeyWake onut::Input::State::KeyWake /* System Wake */
#define OKeyWebSearch onut::Input::State::KeyWebSearch /* Web Search */
#define OKeyWebFavorites onut::Input::State::KeyWebFavorites /* Web Favorites */
#define OKeyWebRefresh onut::Input::State::KeyWebRefresh /* Web Refresh */
#define OKeyWebStop onut::Input::State::KeyWebStop /* Web Stop */
#define OKeyWebForward onut::Input::State::KeyWebForward /* Web Forward */
#define OKeyWebBack onut::Input::State::KeyWebBack /* Web Back */
#define OKeyMyComputer onut::Input::State::KeyMyComputer /* My Computer */
#define OKeyMailL onut::Input::State::KeyMailL /* Mail */
#define OKeyMediaSelect onut::Input::State::KeyMediaSelect /* Media Select */

/*
*  Alternate names for keys originally not used on US keyboards.
*/
#define OKeyCircomflex onut::Input::State::KeyCircomflex /* Japanese keyboard */

/*
* Mouse
*/
#define OMouse1 onut::Input::State::Mouse1
#define OMouse2 onut::Input::State::Mouse2
#define OMouse3 onut::Input::State::Mouse3
#define OMouse4 onut::Input::State::Mouse4
#define OMouseX onut::Input::State::MouseX
#define OMouseY onut::Input::State::MouseY
#define OMouseZ onut::Input::State::MouseZ

extern OInputRef oInput;

const Vector2& OGetMousePos();
bool OInputPressed(onut::Input::State state);
bool OInputJustPressed(onut::Input::State state);
bool OInputJustReleased(onut::Input::State state);
bool OInputReleased(onut::Input::State state);
