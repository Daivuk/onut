// Onut
#include <onut/Joystick.h>
#include <onut/Input.h>
#include <onut/Strings.h>

namespace onut
{
    Joystick::Joystick(int index)
        : m_index(index)
        , m_name("Joystick")
    {
    }

    Joystick::~Joystick()
    {
    }

    Joystick::Type Joystick::getType() const
    {
        return m_type;
    }

    int Joystick::getIndex() const
    {
        return m_index;
    }

    const std::string& Joystick::getName() const
    {
        return m_name;
    }

    std::string Joystick::getTypeName() const
    {
        switch (m_type)
        {
            case Type::Stick:
                return "Stick";
            case Type::Throttle:
                return "Throttle";
            case Type::Other:
                return "Other";
        }
        return "Unknown";
    }

    std::string Joystick::getAxisName(int id) const
    {
        return "axis" + std::to_string(id);
    }

    std::string Joystick::getButtonName(int button) const
    {
        auto hatBase = getHatButtonBase();
        auto axisBase = getAxisButtonBase();
        if (button < hatBase)
        {
            return "btn" + std::to_string(button);
        }
        else if (button < axisBase)
        {
            auto hatId = (button - hatBase) / 4;
            auto hatDir = (button - hatBase) % 4;
            switch (hatDir)
            {
                case 0: return "hat" + std::to_string(hatId) + "_up";
                case 1: return "hat" + std::to_string(hatId) + "_right";
                case 2: return "hat" + std::to_string(hatId) + "_down";
                case 3: return "hat" + std::to_string(hatId) + "_left";
            }
            return "wat";
        }
        else
        {
            auto base = button - axisBase;
            return (base % 2 == 0 ? "-axis" : "+axis") + std::to_string(base / 2);
        }
    }
}

OJoystickRef OGetJoystick(int index)
{
    return oInput->getJoystick(index);
}

bool OJoystickPressed(int button, int joystickIndex)
{
    auto pJoystick = OGetJoystick(joystickIndex);
    if (!pJoystick) return false;
    return pJoystick->isPressed(button);
}

bool OJoystickJustPressed(int button, int joystickIndex)
{
    auto pJoystick = OGetJoystick(joystickIndex);
    if (!pJoystick) return false;
    return pJoystick->isJustPressed(button);
}

bool OJoystickJustReleased(int button, int joystickIndex)
{
    auto pJoystick = OGetJoystick(joystickIndex);
    if (!pJoystick) return false;
    return pJoystick->isJustReleased(button);
}

float OJoystickAxis(int axis, int joystickIndex)
{
    auto pJoystick = OGetJoystick(joystickIndex);
    if (!pJoystick) return 0.0f;
    return pJoystick->getAxis(axis);
}

namespace onut
{
    static OJoystickRef parseJoystickButton(const std::string& fullName, int& button)
    {
        auto split = onut::splitString(fullName, ' ');
        if (split.size() < 2) return nullptr;

        const auto& joyName = split[0];
        const auto& componentName = split[1];

        OJoystickRef pJoystick;
        try
        {
            pJoystick = OGetJoystick(std::stoi(joyName.substr(3)));
            if (!pJoystick) return nullptr;
        }
        catch (...)
        {
            return nullptr;
        }

        if (componentName.substr(0, 3) == "btn")
        {
            try
            {
                button = std::stoi(componentName.substr(3));
                return pJoystick;
            }
            catch (...)
            {
                return nullptr;
            }
        }
        else if (componentName.substr(0, 3) == "hat")
        {
            try
            {
                if (split.size() < 3) return nullptr;

                button = std::stoi(componentName.substr(3));
                button += pJoystick->getHatButtonBase();
                
                const auto& direction = split[2];
                if (direction == "right") button += 1;
                else if (direction == "down") button += 2;
                else if (direction == "left") button += 3;
                else if (direction != "up") return nullptr;

                return pJoystick;
            }
            catch (...)
            {
                return nullptr;
            }
        }
        else if (componentName.substr(1, 4) == "axis")
        {
            try
            {
                auto axis = std::stoi(componentName.substr(5));

                if (componentName[0] == '-')
                {
                    button = pJoystick->getAxisButtonBase() + axis * 2;
                }
                else if (componentName[0] == '+')
                {
                    button = pJoystick->getAxisButtonBase() + axis * 2 + 1;
                }
                else return nullptr;

                return pJoystick;
            }
            catch (...)
            {
                return nullptr;
            }
        }

        return nullptr;
    }

    static OJoystickRef parseJoystickAxis(const std::string& fullName, int& axis)
    {
        auto split = onut::splitString(fullName, ' ');
        if (split.size() < 2) return nullptr;

        const auto& joyName = split[0];
        const auto& axisName = split[1];

        OJoystickRef pJoystick;
        try
        {
            pJoystick = OGetJoystick(std::stoi(joyName.substr(3)));
            if (!pJoystick) return nullptr;
        }
        catch (...)
        {
            return nullptr;
        }

        try
        {
            axis = std::stoi(axisName.substr(4));
        }
        catch (...)
        {
            return nullptr;
        }

        return pJoystick;
    }
}

bool OJoystickPressed(const std::string& fullName)
{
    int button;
    auto pJoystick = onut::parseJoystickButton(fullName, button);
    if (!pJoystick) return false;
    return pJoystick->isPressed(button);
}

bool OJoystickJustPressed(const std::string& fullName)
{
    int button;
    auto pJoystick = onut::parseJoystickButton(fullName, button);
    if (!pJoystick) return false;
    return pJoystick->isJustPressed(button);
}

bool OJoystickJustReleased(const std::string& fullName)
{
    int button;
    auto pJoystick = onut::parseJoystickButton(fullName, button);
    if (!pJoystick) return false;
    return pJoystick->isJustReleased(button);
}

float OJoystickAxis(const std::string& fullName)
{
    int axis;
    auto pJoystick = onut::parseJoystickAxis(fullName, axis);
    if (!pJoystick) return false;
    return pJoystick->getAxis(axis);
}

std::string OGetJoystickAxisName(int axis, int joystickIndex)
{
    auto pJoystick = OGetJoystick(joystickIndex);
    if (!pJoystick) return "N/A";

    return "joy" + std::to_string(joystickIndex) + "_" + pJoystick->getAxisName(axis);
}

std::string OGetJoystickButtonName(int button, int joystickIndex)
{
    auto pJoystick = OGetJoystick(joystickIndex);
    if (!pJoystick) return "N/A";

    return "joy" + std::to_string(joystickIndex) + "_" + pJoystick->getButtonName(button);
}

namespace onut
{
    static std::string getJoystickPrettyName(int joystickIndex, bool* pHasIndex = nullptr)
    {
        auto pJoystick = OGetJoystick(joystickIndex);
        if (!pJoystick) return "N/A";

        auto joyCount = oInput->getJoystickCount();
        int sameTypeIndex = 1;
        int sameTypeCount = 0;
        for (int i = 0; i < joyCount; ++i)
        {
            auto pOtherJoystick = oInput->getJoystick(i);
            auto isSameType = pOtherJoystick->getType() == pJoystick->getType();
            if (isSameType) sameTypeCount++;
            if (pOtherJoystick == pJoystick) break;
            if (isSameType) sameTypeIndex++;
        }

        std::string joyIndex = "";
        if (sameTypeCount != 1)
        {
            joyIndex += 'A' + sameTypeIndex;
            if (pHasIndex) *pHasIndex = true;
        }
        else
        {
            if (pHasIndex) *pHasIndex = false;
        }

        return pJoystick->getTypeName() + joyIndex;
    }

    static std::string getAxisPrettyName(int axis, int joystickIndex)
    {
        auto pJoystick = OGetJoystick(joystickIndex);
        if (!pJoystick) return "N/A";

        int axisCount = pJoystick->getAxisCount();
        std::string axisName;

        int pairCount = axisCount / 2;
        int pairIndex = axis / 2;
        if (axis == pairCount * 2)
        {
            pairIndex = 0;
            axisName = "Z";
        }
        else if (axis % 2 == 0)
        {
            axisName = "X";
        }
        else
        {
            axisName = "Y";
        }
        if (pairIndex == 0) pairCount = 1;

        std::string axisPairIndex = (pairCount == 1) ? "" : std::to_string(pairIndex + 1);
        return axisName + axisPairIndex;
    }
}

std::string OGetJoystickPrettyAxisName(int axis, int joystickIndex)
{
    auto pJoystick = OGetJoystick(joystickIndex);
    if (!pJoystick) return "N/A";

    auto joystickName = pJoystick->getName();
    std::string axisName = onut::getAxisPrettyName(axis, joystickIndex) + "-Axis";
    bool hasIndex = false;
    std::string joystickPrettyname = onut::getJoystickPrettyName(joystickIndex, &hasIndex) + " ";

    if (joystickName.find("X-56") != std::string::npos)
    {
        if (pJoystick->getType() == OJoystick::Type::Stick)
        {
            switch (axis)
            {
                case 0: axisName = "X Axis"; break; 
                case 1: axisName = "Y Axis"; break;
                case 2: axisName = "Cx Axis"; break;
                case 3: axisName = "Cy Axis"; break;
                case 4: axisName = "Twist"; break;
            }
        }
        else if (pJoystick->getType() == OJoystick::Type::Throttle)
        {
            switch (axis)
            {
                case 0: axisName = "Left Throttle"; break;
                case 1: axisName = "Right Throttle"; break;
                case 2: axisName = "RTY F"; break;
                case 3: axisName = "Ministick X"; break;
                case 4: axisName = "Ministick Y"; break;
                case 5: axisName = "RTY G"; break;
                case 6: axisName = "RTY 3"; break;
                case 7: axisName = "RTY 4"; break;
            }
        }
        if (!hasIndex) joystickPrettyname.clear();
    }

    return joystickPrettyname + axisName;
}

std::string OGetJoystickPrettyButtonName(int button, int joystickIndex)
{
    auto pJoystick = OGetJoystick(joystickIndex);
    if (!pJoystick) return "N/A";

    auto hatBase = pJoystick->getHatButtonBase();
    auto axisBase = pJoystick->getAxisButtonBase();
    auto stickName = onut::getJoystickPrettyName(joystickIndex);

    auto joystickName = pJoystick->getName();
    bool hasIndex = false;
    std::string joystickPrettyname = onut::getJoystickPrettyName(joystickIndex, &hasIndex) + " ";

    // Specify hardware names (Are we going to do this for all?)
    if (joystickName.find("X-56") != std::string::npos)
    {
        std::string buttonName;
        if (pJoystick->getType() == OJoystick::Type::Stick)
        {
            switch (button)
            {
                case 0: buttonName = "Trigger"; break;
                case 1: buttonName = "A"; break;
                case 2: buttonName = "B"; break;
                case 3: buttonName = "C"; break;
                case 4: buttonName = "D"; break;
                case 5: buttonName = "Pinky"; break;
                case 6: buttonName = "H1 Up"; break;
                case 7: buttonName = "H1 Right"; break;
                case 8: buttonName = "H1 Down"; break;
                case 9: buttonName = "H1 Left"; break;
                case 10: buttonName = "H2 Up"; break;
                case 11: buttonName = "H2 Right"; break;
                case 12: buttonName = "H2 Down"; break;
                case 13: buttonName = "H2 Left"; break;
                case 14: buttonName = "M1"; break;
                case 15: buttonName = "M2"; break;
                case 16: buttonName = "S1"; break;
                case 17: buttonName = "POV Up"; break;
                case 18: buttonName = "POV Right"; break;
                case 19: buttonName = "POV Down"; break;
                case 20: buttonName = "POV Left"; break;
                case 21: buttonName = "Left"; break;
                case 22: buttonName = "Right"; break;
                case 23: buttonName = "Up"; break;
                case 24: buttonName = "Down"; break;
                case 25: buttonName = "C Left"; break;
                case 26: buttonName = "C Right"; break;
                case 27: buttonName = "C Up"; break;
                case 28: buttonName = "C Down"; break;
                case 29: buttonName = "Twist Left"; break;
                case 30: buttonName = "Twist Right"; break;
            }
        }
        else if (pJoystick->getType() == OJoystick::Type::Throttle)
        {
            switch (button)
            {
                case 0: buttonName = "E"; break;
                case 1: buttonName = "F"; break;
                case 2: buttonName = "G"; break;
                case 3: buttonName = "I"; break;
                case 4: buttonName = "H"; break;
                case 5: buttonName = "SW 1"; break;
                case 6: buttonName = "SW 2"; break;
                case 7: buttonName = "SW 3"; break;
                case 8: buttonName = "SW 4"; break;
                case 9: buttonName = "SW 5"; break;
                case 10: buttonName = "SW 6"; break;
                case 11: buttonName = "TGL 1 Up"; break;
                case 12: buttonName = "TGL 1 Down"; break;
                case 13: buttonName = "TGL 2 Up"; break;
                case 14: buttonName = "TGL 2 Down"; break;
                case 15: buttonName = "TGL 3 Up"; break;
                case 16: buttonName = "TGL 3 Down"; break;
                case 17: buttonName = "TGL 4 Up"; break;
                case 18: buttonName = "TGL 4 Down"; break;
                case 19: buttonName = "H3 Up"; break;
                case 20: buttonName = "H3 Right"; break;
                case 21: buttonName = "H3 Down"; break;
                case 22: buttonName = "H3 Left"; break;
                case 23: buttonName = "H4 Up"; break;
                case 24: buttonName = "H4 Right"; break;
                case 25: buttonName = "H4 Down"; break;
                case 26: buttonName = "H4 Left"; break;
                case 27: buttonName = "K1 Up"; break;
                case 28: buttonName = "K1 Down"; break;
                case 29: buttonName = "Scroll FWD"; break;
                case 30: buttonName = "Scroll BCK"; break;
                case 31: buttonName = "Ministick"; break;
                case 32: buttonName = "SLD"; break;
                case 33: buttonName = "M1"; break;
                case 34: buttonName = "M2"; break;
                case 35: buttonName = "S1"; break;
                case 36: buttonName = "L Throttle FWD"; break;
                case 37: buttonName = "L Throttle BCK"; break;
                case 38: buttonName = "R Throttle FWD"; break;
                case 39: buttonName = "R Throttle BCK"; break;
                case 40: buttonName = "RTY F FWD"; break;
                case 41: buttonName = "RTY F BCK"; break;
                case 42: buttonName = "Ministick Left"; break;
                case 43: buttonName = "Ministick Right"; break;
                case 44: buttonName = "Ministick Up"; break;
                case 45: buttonName = "Ministick Down"; break;
                case 46: buttonName = "RTY G FWD"; break;
                case 47: buttonName = "RTY G BCK"; break;
                case 48: buttonName = "RTY 3 CCW"; break;
                case 49: buttonName = "RTY 3 CW"; break;
                case 50: buttonName = "RTY 4 CCW"; break;
                case 51: buttonName = "RTY 4 CW"; break;
            }
        }
        if (!hasIndex) joystickPrettyname.clear();

        return joystickPrettyname + buttonName;
    }

    if (button < hatBase)
    {
        return stickName + " " + std::to_string(button + 1);
    }
    else if (button < axisBase)
    {
        auto hatIndex = (button - hatBase) / 4;
        auto hatCount = (axisBase - hatBase) / 4;
        auto dir = (button - hatBase) % 4;
        auto hatName = std::string("POV") + ((hatCount == 1 || hatIndex == 0) ? "" : std::to_string(hatIndex + 1));
        switch (dir)
        {
            case 0: return stickName + " " + hatName + "-Up";
            case 1: return stickName + " " + hatName + "-Right";
            case 2: return stickName + " " + hatName + "-Down";
            case 3: return stickName + " " + hatName + "-Left";
        }
    }
    else
    {
        auto axis = (button - axisBase) / 2;
        return stickName + " " + (((button - axisBase) % 2 == 0) ? "-" : "+") + onut::getAxisPrettyName(axis, joystickIndex);
    }

    return "N/A";
}
