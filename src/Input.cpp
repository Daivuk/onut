// Onut
#include <onut/GamePad.h>
#include <onut/Input.h>
#include <onut/Joystick.h>
#include <onut/Renderer.h>
#include <onut/Strings.h>

// Private
#include "InputDevice.h"

// STL
#include <cassert>
#include <unordered_map>

OInputRef oInput;

namespace onut
{
    OInputRef Input::create(const OWindowRef& pWindow)
    {
        return std::make_shared<OInput>(pWindow);
    }

    Input::Input(const OWindowRef& pWindow)
    {
        m_pInputDevice = OInputDevice::create(this);
        oInputDevice = m_pInputDevice;

        m_prevStates = new InputState[STATE_COUNT];
        m_curStates = new InputState[STATE_COUNT];
        m_states = new InputState[STATE_COUNT];
        m_statesRestore = new InputState[STATE_COUNT];
        m_frameId = 1;

        for (int i = 0; i < GAME_PAD_COUNT; ++i)
        {
            m_gamePads[i] = OGamePad::create(i);
        }
    }

    Input::~Input()
    {
        oInputDevice = nullptr;
        delete[] m_prevStates;
        delete[] m_curStates;
        delete[] m_states;
        delete[] m_statesRestore;
    }

    void Input::setStateDown(State in_stateId)
    {
        assert(static_cast<int>(in_stateId) >= 0 && static_cast<int>(in_stateId) < STATE_COUNT && "Invalid Input State");
        auto& state = m_states[static_cast<int>(in_stateId)];
        m_statesRestore[static_cast<int>(in_stateId)].isDown = true;
        if (state.frameId != m_frameId)
        {
            state.isDown = true;
            state.frameId = m_frameId;
        }
    }

    void Input::setStateUp(State in_stateId)
    {
        assert(static_cast<int>(in_stateId) >= 0 && static_cast<int>(in_stateId) < STATE_COUNT && "Invalid Input State");
        auto& state = m_states[static_cast<int>(in_stateId)];
        m_statesRestore[static_cast<int>(in_stateId)].isDown = false;
        if (state.frameId != m_frameId)
        {
            state.isDown = false;
            state.frameId = m_frameId;
        }
    }

    void Input::setStateValue(State in_stateId, float in_value)
    {
        assert(static_cast<int>(in_stateId) >= 0 && static_cast<int>(in_stateId) < STATE_COUNT && "Invalid Input State");
        m_statesRestore[static_cast<int>(in_stateId)].value = in_value;
        m_states[static_cast<int>(in_stateId)].value = in_value;
    }

    bool Input::isStateDown(State in_stateId) const
    {
        assert(static_cast<int>(in_stateId) >= 0 && static_cast<int>(in_stateId) < STATE_COUNT && "Invalid Input State");
        return m_curStates[static_cast<int>(in_stateId)].isDown;
    }

    bool Input::isStateUp(State in_stateId) const
    {
        assert(static_cast<int>(in_stateId) >= 0 && static_cast<int>(in_stateId) < STATE_COUNT && "Invalid Input State");
        return !m_curStates[static_cast<int>(in_stateId)].isDown;
    }

    bool Input::isStateJustDown(State in_stateId) const
    {
        assert(static_cast<int>(in_stateId) >= 0 && static_cast<int>(in_stateId) < STATE_COUNT && "Invalid Input State");
        return (!m_prevStates[static_cast<int>(in_stateId)].isDown && m_curStates[static_cast<int>(in_stateId)].isDown);
    }

    bool Input::isStateJustUp(State in_stateId) const
    {
        assert(static_cast<int>(in_stateId) >= 0 && static_cast<int>(in_stateId) < STATE_COUNT && "Invalid Input State");
        return (m_prevStates[static_cast<int>(in_stateId)].isDown && !m_curStates[static_cast<int>(in_stateId)].isDown);
    }

    float Input::getStateValue(State in_stateId) const
    {
        assert(static_cast<int>(in_stateId) >= 0 && static_cast<int>(in_stateId) < STATE_COUNT && "Invalid Input State");
        return m_states[static_cast<int>(in_stateId)].value;
    }

    void Input::update()
    {
        m_pInputDevice->update();

        for (auto& gamePad : m_gamePads)
        {
            gamePad->update();
        }

        for (auto& joystick : m_joysticks)
        {
            joystick->update();
        }

        memcpy(m_prevStates, m_curStates, sizeof(InputState) * STATE_COUNT);
        memcpy(m_curStates, m_states, sizeof(InputState) * STATE_COUNT);
        memcpy(m_states, m_statesRestore, sizeof(InputState) * STATE_COUNT);

        if (m_fpsMouse)
        {
            mouseDelta = mousePos - (OScreen / 2);
            setMousePos(OScreen / 2);
        }
        else mouseDelta = {0, 0};

        ++m_frameId;
        m_lastDoubleClicked = m_doubleClicked;
        m_doubleClicked = false;
    }

    OGamePadRef Input::getGamePad(int index) const
    {
        if (index < 0 || index >= 4) return nullptr;
        return m_gamePads[index];
    }

    OJoystickRef Input::getJoystick(int index) const
    {
        if (index < 0 || index >= (int)m_joysticks.size()) return nullptr;
        return m_joysticks[index];
    }

    int Input::getJoystickCount() const
    {
        return (int)m_joysticks.size();
    }

    bool Input::getDoubleClicked() const
    {
        return m_lastDoubleClicked;
    }

    void Input::onDoubleClicked()
    {
        m_doubleClicked = true;
    }

    void Input::activateJoystick(int index)
    {
        m_joysticks.push_back(OJoystick::create(index));
        std::sort(m_joysticks.begin(), m_joysticks.end(), [](const OJoystickRef& a, const OJoystickRef& b)
        {
            return (int)a->getType() < (int)b->getType();
        });
    }

    void Input::deactivateJoystick(int index)
    {
        for (auto it = m_joysticks.begin(); it != m_joysticks.end(); ++it)
        {
            auto pJoystick = *it;
            if (pJoystick->getIndex() == index)
            {
                m_joysticks.erase(it);
                return;
            }
        }
    }

    void Input::setMouseVisible(bool isCursorVisible)
    {
        if (m_isCursorVisible != isCursorVisible)
        {
            m_pInputDevice->setMouseVisible(isCursorVisible);
            m_isCursorVisible = isCursorVisible;
        }
    }

    void Input::setFpsMouse(bool fpsMouse)
    {
        if (m_fpsMouse != fpsMouse)
        {
            if (fpsMouse)
            {
                setMouseVisible(false);
                setMousePos(OScreen / 2);
                mouseDelta = {0, 0};
            }
            else setMouseVisible(true);
            m_fpsMouse = fpsMouse;
        }
    }

    void Input::setMouseIcon(const std::string& name, const Point& hotSpot)
    {
        m_pInputDevice->setMouseIcon(name, hotSpot);
    }

    void Input::unsetMouseIcon()
    {
        m_pInputDevice->unsetMouseIcon();
    }

    void Input::setMousePos(const Point& mousePos)
    {
        m_pInputDevice->setMousePos(mousePos);
    }

    const char* Input::getStateName(State state)
    {
        switch (state)
        {
            case State::KeyEscape: return "Escape";
            case State::Key1: return "1";
            case State::Key2: return "2";
            case State::Key3: return "3";
            case State::Key4: return "4";
            case State::Key5: return "5";
            case State::Key6: return "6";
            case State::Key7: return "7";
            case State::Key8: return "8";
            case State::Key9: return "9";
            case State::Key0: return "0";
            case State::KeyMinus: return "-"; /* - on main keyboard */
            case State::KeyEquals: return "=";
            case State::KeyBackspace: return "Backspace"; /* backspace */
            case State::KeyTab: return "Tab";
            case State::KeyQ: return "Q";
            case State::KeyW: return "W";
            case State::KeyE: return "E";
            case State::KeyR: return "R";
            case State::KeyT: return "T";
            case State::KeyY: return "Y";
            case State::KeyU: return "U";
            case State::KeyI: return "I";
            case State::KeyO: return "O";
            case State::KeyP: return "P";
            case State::KeyLeftBracket: return "[";
            case State::KeyRightBracket: return "]";
            case State::KeyEnter: return "Enter"; /* Enter on main keyboard */
            case State::KeyLeftControl: return "Left Control";
            case State::KeyA: return "A";
            case State::KeyS: return "S";
            case State::KeyD: return "D";
            case State::KeyF: return "F";
            case State::KeyG: return "G";
            case State::KeyH: return "H";
            case State::KeyJ: return "J";
            case State::KeyK: return "K";
            case State::KeyL: return "L";
            case State::KeySemiColon: return ";";
            case State::KeyApostrophe: return "'";
            case State::KeyGrave: return "`";    /* accent grave */
            case State::KeyLeftShift: return "Left Shift";
            case State::KeyBackslash: return "\\";
            case State::KeyZ: return "Z";
            case State::KeyX: return "X";
            case State::KeyC: return "C";
            case State::KeyV: return "V";
            case State::KeyB: return "B";
            case State::KeyN: return "N";
            case State::KeyM: return "M";
            case State::KeyComma: return ",";
            case State::KeyPeriod: return "."; /* . on main keyboard */
            case State::KeySlash: return "/"; /* / on main keyboard */
            case State::KeyRightShift: return "Right Shift";
            case State::KeyMultiply: return "Num *"; /* * on numeric keypad */
            case State::KeyLeftAlt: return "Left Alt"; /* left Alt */
            case State::KeySpaceBar: return "Spacebar";
            case State::KeyCapsLock: return "Caps Lock";
            case State::KeyF1: return "F1";
            case State::KeyF2: return "F2";
            case State::KeyF3: return "F3";
            case State::KeyF4: return "F4";
            case State::KeyF5: return "F5";
            case State::KeyF6: return "F6";
            case State::KeyF7: return "F7";
            case State::KeyF8: return "F8";
            case State::KeyF9: return "F9";
            case State::KeyF10: return "F10";
            case State::KeyNumLock: return "Num Lock";
            case State::KeyScrollLock: return "Scroll Lock"; /* Scroll Lock */
            case State::KeyNumPad7: return "Num 7";
            case State::KeyNumPad8: return "Num 8";
            case State::KeyNumPad9: return "Num 9";
            case State::KeyNumPadMinus: return "Num -"; /* - on numeric keypad */
            case State::KeyNumPad4: return "Num 4";
            case State::KeyNumPad5: return "Num 5";
            case State::KeyNumPad6: return "Num 6";
            case State::KeyNumPadAdd: return "Num +"; /* + on numeric keypad */
            case State::KeyNumPad1: return "Num 1";
            case State::KeyNumPad2: return "Num 2";
            case State::KeyNumPad3: return "Num 3";
            case State::KeyNumPad0: return "Num 0";
            case State::KeyNumPadPeriod: return "Num ."; /* . on numeric keypad */
            //case State::KeyOEM102: return ""; /* <> or \| on RT 102-key keyboard (Non-U.S.) */
            case State::KeyF11: return "F11";
            case State::KeyF12: return "F12";
            case State::KeyF13: return "F13"; /* (NEC PC98) */
            case State::KeyF14: return "F14"; /* (NEC PC98) */
            case State::KeyF15: return "F15"; /* (NEC PC98) */
            case State::KeyKana: return "Kana"; /* (Japanese keyboard) */
            case State::KeyAbntC1: return "Abnt C1"; /* /? on Brazilian keyboard */
            case State::KeyConvert: return "Convert"; /* (Japanese keyboard) */
            case State::KeyNoConvert: return "No Convert"; /* (Japanese keyboard) */
            case State::KeyYen: return "Yen"; /* (Japanese keyboard) */
            case State::KeyAbntC2: return "Abnt C2"; /* Numpad . on Brazilian keyboard */
            case State::KeyNumPadEquals: return "Num ="; /* = on numeric keypad (NEC PC98) */
            case State::KeyPreviousTrack: return "Previous Track"; /* Previous Track (OINPUT_CIRCUMFLEX on Japanese keyboard) */
            case State::KeyAt: return "At"; /* (NEC PC98) */
            case State::KeyColon: return ":"; /* (NEC PC98) */
            case State::KeyUnderline: return "_"; /* (NEC PC98) */
            case State::KeyKanji: return "Kanji"; /* (Japanese keyboard) */
            case State::KeyStop: return "Stop"; /* (NEC PC98) */
            case State::KeyAx: return "Ax"; /* (Japan AX) */
            case State::KeyUnlabeled: return "J3100"; /* (J3100) */
            case State::KeyNextTrack: return "Next Track"; /* Next Track */
            case State::KeyNumPadEnter: return "Num Enter"; /* Enter on numeric keypad */
            case State::KeyRightControl: return "Right Control";
            case State::KeyMute: return "Mute"; /* Mute */
#if defined(MSVC)
            case State::KeyCalculator: return "Calculator"; /* Calculator */
#endif
            case State::KeyPlayPause: return "Play"; /* Play / Pause */
            case State::KeyMediaStop: return "Media Stop"; /* Media Stop */
            case State::KeyVolumeDown: return "Volume Down"; /* Volume - */
            case State::KeyVolumeUp: return "Volume Up"; /* Volume + */
            case State::KeyWebHome: return "Web Home"; /* Web home */
            case State::KeyNumPadComma: return "Num ,"; /* , on numeric keypad (NEC PC98) */
            case State::KeyNumPadDivide: return "Num /"; /* / on numeric keypad */
            case State::Key_SYSRQ: return "SYSRQ";
            case State::KeyRightAlt: return "Right Alt"; /* right Alt */
            //case State::KeyAltCar: return ""; /* right Alt */
            case State::KeyPause: return "Pause"; /* Pause */
            case State::KeyHome: return "Home"; /* Home on arrow keypad */
            case State::KeyUp: return "Up"; /* UpArrow on arrow keypad */
            case State::KeyPageUp: return "Page Up"; /* PgUp on arrow keypad */
            case State::KeyLeft: return "Left"; /* LeftArrow on arrow keypad */
            case State::KeyRight: return "Right"; /* RightArrow on arrow keypad */
            case State::KeyEnd: return "End"; /* End on arrow keypad */
            case State::KeyDown: return "Down"; /* DownArrow on arrow keypad */
            case State::KeyPageDown: return "Page Down"; /* PgDn on arrow keypad */
            case State::KeyInsert: return "Insert"; /* Insert on arrow keypad */
            case State::KeyDelete: return "Delete"; /* Delete on arrow keypad */
            case State::KeyLeftWindows: return "Left OS"; /* Left Windows key */
            case State::KeyRightWindows: return "Right OS"; /* Right Windows key */
            case State::KeyAppMenu: return "App Menu"; /* AppMenu key */
            case State::KeyPower: return "Power"; /* System Power */
            case State::KeySleep: return "Sleep"; /* System Sleep */
            case State::KeyWake: return "Wake"; /* System Wake */
            case State::KeyWebSearch: return "Web Search"; /* Web Search */
            case State::KeyWebFavorites: return "Web Favorites"; /* Web Favorites */
            case State::KeyWebRefresh: return "Web Refresh"; /* Web Refresh */
            case State::KeyWebStop: return "Web Stop"; /* Web Stop */
            case State::KeyWebForward: return "Web Forward"; /* Web Forward */
            case State::KeyWebBack: return "Web Back"; /* Web Back */
            case State::KeyMyComputer: return "My Computer"; /* My Computer */
            case State::KeyMailL: return "Mail"; /* Mail */
            case State::KeyMediaSelect: return "Media Select"; /* Media Select */
            case State::Mouse1: return "Left Mouse";
            case State::Mouse2: return "Right Mouse";
            case State::Mouse3: return "Middle Mouse";
            case State::Mouse4: return "Mouse 4";
        }
        return "UNKNOWN";
    }

    onut::Input::State Input::getStateFromName(const std::string& name)
    {
        static std::unordered_map<std::string, onut::Input::State> NAMES_TO_STATES = {
            { onut::toUpper("Escape"), State::KeyEscape },
            { onut::toUpper("1"), State::Key1 },
            { onut::toUpper("2"), State::Key2 },
            { onut::toUpper("3"), State::Key3 },
            { onut::toUpper("4"), State::Key4 },
            { onut::toUpper("5"), State::Key5 },
            { onut::toUpper("6"), State::Key6 },
            { onut::toUpper("7"), State::Key7 },
            { onut::toUpper("8"), State::Key8 },
            { onut::toUpper("9"), State::Key9 },
            { onut::toUpper("0"), State::Key0 },
            { onut::toUpper("-"), State::KeyMinus }, /* - on main keyboard */
            { onut::toUpper("="), State::KeyEquals },
            { onut::toUpper("Backspace"), State::KeyBackspace }, /* backspace */
            { onut::toUpper("Tab"), State::KeyTab },
            { onut::toUpper("Q"), State::KeyQ },
            { onut::toUpper("W"), State::KeyW },
            { onut::toUpper("E"), State::KeyE },
            { onut::toUpper("R"), State::KeyR },
            { onut::toUpper("T"), State::KeyT },
            { onut::toUpper("Y"), State::KeyY },
            { onut::toUpper("U"), State::KeyU },
            { onut::toUpper("I"), State::KeyI },
            { onut::toUpper("O"), State::KeyO },
            { onut::toUpper("P"), State::KeyP },
            { onut::toUpper("["), State::KeyLeftBracket },
            { onut::toUpper("]"), State::KeyRightBracket },
            { onut::toUpper("Enter"), State::KeyEnter }, /* Enter on main keyboard */
            { onut::toUpper("Left Control"), State::KeyLeftControl },
            { onut::toUpper("A"), State::KeyA },
            { onut::toUpper("S"), State::KeyS },
            { onut::toUpper("D"), State::KeyD },
            { onut::toUpper("F"), State::KeyF },
            { onut::toUpper("G"), State::KeyG },
            { onut::toUpper("H"), State::KeyH },
            { onut::toUpper("J"), State::KeyJ },
            { onut::toUpper("K"), State::KeyK },
            { onut::toUpper("L"), State::KeyL },
            { onut::toUpper(";"), State::KeySemiColon },
            { onut::toUpper("'"), State::KeyApostrophe },
            { onut::toUpper("`"), State::KeyGrave },    /* accent grave */
            { onut::toUpper("Left Shift"), State::KeyLeftShift },
            { onut::toUpper("\\"), State::KeyBackslash },
            { onut::toUpper("Z"), State::KeyZ },
            { onut::toUpper("X"), State::KeyX },
            { onut::toUpper("C"), State::KeyC },
            { onut::toUpper("V"), State::KeyV },
            { onut::toUpper("B"), State::KeyB },
            { onut::toUpper("N"), State::KeyN },
            { onut::toUpper("M"), State::KeyM },
            { onut::toUpper(","), State::KeyComma },
            { onut::toUpper("."), State::KeyPeriod }, /* . on main keyboard */
            { onut::toUpper("/"), State::KeySlash }, /* / on main keyboard */
            { onut::toUpper("Right Shift"), State::KeyRightShift },
            { onut::toUpper("Num *"), State::KeyMultiply }, /* * on numeric keypad */
            { onut::toUpper("Left Alt"), State::KeyLeftAlt }, /* left Alt */
            { onut::toUpper("Spacebar"), State::KeySpaceBar },
            { onut::toUpper("Caps Lock"), State::KeyCapsLock },
            { onut::toUpper("F1"), State::KeyF1 },
            { onut::toUpper("F2"), State::KeyF2 },
            { onut::toUpper("F3"), State::KeyF3 },
            { onut::toUpper("F4"), State::KeyF4 },
            { onut::toUpper("F5"), State::KeyF5 },
            { onut::toUpper("F6"), State::KeyF6 },
            { onut::toUpper("F7"), State::KeyF7 },
            { onut::toUpper("F8"), State::KeyF8 },
            { onut::toUpper("F9"), State::KeyF9 },
            { onut::toUpper("F10"), State::KeyF10 },
            { onut::toUpper("Num Lock"), State::KeyNumLock },
            { onut::toUpper("Scroll Lock"), State::KeyScrollLock }, /* Scroll Lock */
            { onut::toUpper("Num 7"), State::KeyNumPad7 },
            { onut::toUpper("Num 8"), State::KeyNumPad8 },
            { onut::toUpper("Num 9"), State::KeyNumPad9 },
            { onut::toUpper("Num -"), State::KeyNumPadMinus }, /* - on numeric keypad */
            { onut::toUpper("Num 4"), State::KeyNumPad4 },
            { onut::toUpper("Num 5"), State::KeyNumPad5 },
            { onut::toUpper("Num 6"), State::KeyNumPad6 },
            { onut::toUpper("Num +"), State::KeyNumPadAdd }, /* + on numeric keypad */
            { onut::toUpper("Num 1"), State::KeyNumPad1 },
            { onut::toUpper("Num 2"), State::KeyNumPad2 },
            { onut::toUpper("Num 3"), State::KeyNumPad3 },
            { onut::toUpper("Num 0"), State::KeyNumPad0 },
            { onut::toUpper("Num ."), State::KeyNumPadPeriod }, /* . on numeric keypad */
            //{ onut::toUpper(""), State::KeyOEM102 }, /* <> or \| on RT 102-key keyboard (Non-U.S.) */
            { onut::toUpper("F11"), State::KeyF11 },
            { onut::toUpper("F12"), State::KeyF12 },
            { onut::toUpper("F13"), State::KeyF13 }, /* (NEC PC98) */
            { onut::toUpper("F14"), State::KeyF14 }, /* (NEC PC98) */
            { onut::toUpper("F15"), State::KeyF15 }, /* (NEC PC98) */
            { onut::toUpper("Kana"), State::KeyKana }, /* (Japanese keyboard) */
            { onut::toUpper("Abnt C1"), State::KeyAbntC1 }, /* /? on Brazilian keyboard */
            { onut::toUpper("Convert"), State::KeyConvert }, /* (Japanese keyboard) */
            { onut::toUpper("No Convert"), State::KeyNoConvert }, /* (Japanese keyboard) */
            { onut::toUpper("Yen"), State::KeyYen }, /* (Japanese keyboard) */
            { onut::toUpper("Abnt C2"), State::KeyAbntC2 }, /* Numpad . on Brazilian keyboard */
            { onut::toUpper("Num ="), State::KeyNumPadEquals }, /* = on numeric keypad (NEC PC98) */
            { onut::toUpper("Previous Track"), State::KeyPreviousTrack }, /* Previous Track (OINPUT_CIRCUMFLEX on Japanese keyboard) */
            { onut::toUpper("At"), State::KeyAt }, /* (NEC PC98) */
            { onut::toUpper(":"), State::KeyColon }, /* (NEC PC98) */
            { onut::toUpper("_"), State::KeyUnderline }, /* (NEC PC98) */
            { onut::toUpper("Kanji"), State::KeyKanji }, /* (Japanese keyboard) */
            { onut::toUpper("Stop"), State::KeyStop }, /* (NEC PC98) */
            { onut::toUpper("Ax"), State::KeyAx }, /* (Japan AX) */
            { onut::toUpper("J3100"), State::KeyUnlabeled }, /* (J3100) */
            { onut::toUpper("Next Track"), State::KeyNextTrack }, /* Next Track */
            { onut::toUpper("Num Enter"), State::KeyNumPadEnter }, /* Enter on numeric keypad */
            { onut::toUpper("Right Control"), State::KeyRightControl },
            { onut::toUpper("Mute"), State::KeyMute }, /* Mute */
            { onut::toUpper("Calculator"), State::KeyCalculator }, /* Calculator */
            { onut::toUpper("Play"), State::KeyPlayPause }, /* Play / Pause */
            { onut::toUpper("Media Stop"), State::KeyMediaStop }, /* Media Stop */
            { onut::toUpper("Volume Down"), State::KeyVolumeDown }, /* Volume - */
            { onut::toUpper("Volume Up"), State::KeyVolumeUp }, /* Volume + */
            { onut::toUpper("Web Home"), State::KeyWebHome }, /* Web home */
            { onut::toUpper("Num ,"), State::KeyNumPadComma }, /* , on numeric keypad (NEC PC98) */
            { onut::toUpper("Num /"), State::KeyNumPadDivide }, /* / on numeric keypad */
            { onut::toUpper("SYSRQ"), State::Key_SYSRQ },
            { onut::toUpper("Right Alt"), State::KeyRightAlt }, /* right Alt */
            //{ onut::toUpper(""), State::KeyAltCar }, /* right Alt */
            { onut::toUpper("Pause"), State::KeyPause }, /* Pause */
            { onut::toUpper("Home"), State::KeyHome }, /* Home on arrow keypad */
            { onut::toUpper("Up"), State::KeyUp }, /* UpArrow on arrow keypad */
            { onut::toUpper("Page Up"), State::KeyPageUp }, /* PgUp on arrow keypad */
            { onut::toUpper("Left"), State::KeyLeft }, /* LeftArrow on arrow keypad */
            { onut::toUpper("Right"), State::KeyRight }, /* RightArrow on arrow keypad */
            { onut::toUpper("End"), State::KeyEnd }, /* End on arrow keypad */
            { onut::toUpper("Down"), State::KeyDown }, /* DownArrow on arrow keypad */
            { onut::toUpper("Page Down"), State::KeyPageDown }, /* PgDn on arrow keypad */
            { onut::toUpper("Insert"), State::KeyInsert }, /* Insert on arrow keypad */
            { onut::toUpper("Delete"), State::KeyDelete }, /* Delete on arrow keypad */
            { onut::toUpper("Left OS"), State::KeyLeftWindows }, /* Left Windows key */
            { onut::toUpper("Right OS"), State::KeyRightWindows }, /* Right Windows key */
            { onut::toUpper("App Menu"), State::KeyAppMenu }, /* AppMenu key */
            { onut::toUpper("Power"), State::KeyPower }, /* System Power */
            { onut::toUpper("Sleep"), State::KeySleep }, /* System Sleep */
            { onut::toUpper("Wake"), State::KeyWake }, /* System Wake */
            { onut::toUpper("Web Search"), State::KeyWebSearch }, /* Web Search */
            { onut::toUpper("Web Favorites"), State::KeyWebFavorites }, /* Web Favorites */
            { onut::toUpper("Web Refresh"), State::KeyWebRefresh }, /* Web Refresh */
            { onut::toUpper("Web Stop"), State::KeyWebStop }, /* Web Stop */
            { onut::toUpper("Web Forward"), State::KeyWebForward }, /* Web Forward */
            { onut::toUpper("Web Back"), State::KeyWebBack }, /* Web Back */
            { onut::toUpper("My Computer"), State::KeyMyComputer }, /* My Computer */
            { onut::toUpper("Mail"), State::KeyMailL }, /* Mail */
            { onut::toUpper("Media Select"), State::KeyMediaSelect }, /* Media Select */
            { onut::toUpper("Left Mouse"), State::Mouse1 },
            { onut::toUpper("Right Mouse"), State::Mouse2 },
            { onut::toUpper("Middle Mouse"), State::Mouse3 },
            { onut::toUpper("Mouse 4"), State::Mouse4 }
        };

        auto name_caps = onut::toUpper(name);
        auto it = NAMES_TO_STATES.find(name_caps);
        if (it == NAMES_TO_STATES.end()) return State::None;
        return it->second;
    }
}

const Vector2& OGetMousePos()
{
    return oInput->mousePosf;
}

bool OInputPressed(onut::Input::State state)
{
    return oInput->isStateDown(state);
}

bool OInputJustPressed(onut::Input::State state)
{
    return oInput->isStateJustDown(state);
}

bool OInputJustReleased(onut::Input::State state)
{
    return oInput->isStateJustUp(state);
}

bool OInputReleased(onut::Input::State state)
{
    return oInput->isStateUp(state);
}
