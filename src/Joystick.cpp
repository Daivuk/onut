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
        return "btn" + std::to_string(button);
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

    return "joy" + std::to_string(joystickIndex) + " " + pJoystick->getAxisName(axis);
}

std::string OGetJoystickButtonName(int button, int joystickIndex)
{
    auto pJoystick = OGetJoystick(joystickIndex);
    if (!pJoystick) return "N/A";

    return "joy" + std::to_string(joystickIndex) + " " + pJoystick->getButtonName(button);
}

std::string OGetJoystickPrettyAxisName(int axis, int joystickIndex)
{
    return "N/A";
}

std::string OGetJoystickPrettyButtonName(int button, int joystickIndex)
{
    return "N/A";
}
