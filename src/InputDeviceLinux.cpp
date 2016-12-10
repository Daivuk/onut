#if defined(__unix__)
// Onut
#include <onut/Files.h>
#include <onut/Input.h>
#include <onut/Log.h>

// Private
#include "InputDeviceLinux.h"

// STL
#include <cassert>
#include <fcntl.h>
#include <unistd.h>

namespace onut
{
    static bool isKeyDown(char* pKeyMap, int key)
    {
        int keyb = pKeyMap[key / 8];
        int mask = 1 << (key % 8);
        return (keyb & mask);
    }
    
    OInputDeviceRef InputDevice::create(OInput* pInput)
    {
        return std::shared_ptr<InputDeviceLinux>(new InputDeviceLinux(pInput));
    }

    InputDeviceLinux::InputDeviceLinux(OInput* pInput)
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
                        ioctl(m_fd, EVIOCGRAB, &grab);
                        m_devices.push_back(fd);
                    }
                }
            }
            assert(!m_devices.empty());
        });
    }

    InputDeviceLinux::~InputDeviceLinux()
    {
        m_initTimer.stop(false);
        if (m_fd != -1)
        {
            
            ioctl(m_fd, EVIOCGRAB, NULL);
            close(m_fd);
        }
    }

    void InputDeviceLinux::readKeyboard()
    {
        if (m_fd != -1)
        {
            for (auto fd : m_devices)
            {
                ioctl(fd, EVIOCGKEY(sizeof(m_keyMap)), m_keyMap);

                for (int i = 0; i < KEY_MAX; ++i)
                {
                    m_states[i] = isKeyDown(m_keyMap, i);
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
            }
            
            memcpy(m_previousStates, m_states, sizeof(m_states));
        }
    }

    void InputDeviceLinux::readMouse()
    {
    }
}

#endif
