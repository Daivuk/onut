// Onut
#include <onut/Files.h>
#include <onut/Input.h>
#include <onut/Log.h>

// Private
#include "InputDeviceRPI.h"

// STL
#include <cassert>
#include <fcntl.h>
#include <unistd.h>

namespace onut
{
    OInputDeviceRef InputDevice::create(OInput* pInput)
    {
        return std::shared_ptr<InputDeviceRPI>(new InputDeviceRPI(pInput));
    }

    InputDeviceRPI::InputDeviceRPI(OInput* pInput)
        : InputDevice(pInput)
    {
        memset(m_keyMap, 0, sizeof(m_keyMap));
        memset(m_states, 0, sizeof(m_states));
        memset(m_previousStates, 0, sizeof(m_previousStates));
        
        // Delay the initialization so keyup events can be sent in the OS.
        // I know this is silly, but once I switch to SDL2 it won't be an 
        // issue anymore
        m_initTimer.start(std::chrono::milliseconds(500), [this]
        {
            auto files = onut::findAllFiles("/dev/input/by-path/", "*", false);
            for (const auto& file : files)
            {
                if (file.find("event-kbd") != std::string::npos)
                {
                    int fd = open(file.c_str(), O_RDONLY | O_NONBLOCK);
                    if (fd != -1)
                    {
                        int grab = 1;
                        ioctl(fd, EVIOCGRAB, &grab);
                        m_devices.push_back(fd);
                    }
                }
            }
            assert(!m_devices.empty());
        });
    }

    InputDeviceRPI::~InputDeviceRPI()
    {
        m_initTimer.stop(false);
        for (auto fd : m_devices)
        {
            if (fd != -1)
            {
                ioctl(fd, EVIOCGRAB, NULL);
                close(fd);
            }
        }
    }

    void InputDeviceRPI::readKeyboard()
    {
        memset(m_states, 0, sizeof(m_states));
        
        for (auto fd : m_devices)
        {
            ioctl(fd, EVIOCGKEY(sizeof(m_keyMap)), m_keyMap);

            for (int i = 0; i < KEY_MAX; ++i)
            {
                int keyb = m_keyMap[i / 8];
                int mask = 1 << (i % 8);
                m_states[i] |= (keyb & mask);
            }
        }

        for (int i = 0; i < KEY_MAX; ++i)
        {
            auto isDown = m_states[i];
            auto isPreviousDown = m_previousStates[i];
            if (!isPreviousDown && isDown)
            {
                m_pInput->setStateDown(static_cast<onut::Input::State>(i));
            }
            else if (isPreviousDown && !isDown)
            {
                m_pInput->setStateUp(static_cast<onut::Input::State>(i));
            }
        }
        
        memcpy(m_previousStates, m_states, sizeof(m_states));
    }

    void InputDeviceRPI::readMouse()
    {
    }
}
