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
        return !(keyb & mask);
    }
    
    OInputDeviceRef InputDevice::create(OInput* pInput)
    {
        return std::shared_ptr<InputDeviceLinux>(new InputDeviceLinux(pInput));
    }

    InputDeviceLinux::InputDeviceLinux(OInput* pInput)
        : InputDevice(pInput)
    {
        memset(m_previousKeyMap, 0, sizeof(m_keyMap));
        memset(m_previousKeyMap, 0, sizeof(m_previousKeyMap));
        
        m_initTimer.start(std::chrono::seconds(1), [this]
        {
            auto files = onut::findAllFiles("/dev/input/by-path/", "*", false);
            for (const auto& file : files)
            {
                if (file.find("event-kbd") != std::string::npos)
                {
                    m_fd = open(file.c_str(), O_RDONLY | O_NONBLOCK);
                    if (m_fd != -1)
                    {
                        // Make sure we send up event for keys that were down before grabbing exclusivity
                     /*   ioctl(m_fd, EVIOCGKEY(sizeof(m_keyMap)), m_keyMap);
                        for (int i = 0; i < KEY_MAX; ++i)
                        {
                            if (isKeyDown(m_keyMap, i))
                            {
                            }
                        }*/
                        
                        int grab = 1;
                        ioctl(m_fd, EVIOCGRAB, &grab);
                        
                        break;
                    }
                }
            }
            assert(m_fd != -1);
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
            ioctl(m_fd, EVIOCGKEY(sizeof(m_keyMap)), m_keyMap);
            
            for (int i = 0; i < KEY_MAX; ++i)
            {
                auto isDown = isKeyDown(m_keyMap, i);
                auto isPreviousDown = isKeyDown(m_previousKeyMap, i);
                
                if (!isPreviousDown && isDown)
                {
                    m_pInput->setStateDown(static_cast<onut::Input::State>(i));
                }
                else if (isPreviousDown && !isDown)
                {
                    m_pInput->setStateUp(static_cast<onut::Input::State>(i));
                }
            }
            
            memcpy(m_previousKeyMap, m_keyMap, sizeof(m_keyMap));
        }
    }

    void InputDeviceLinux::readMouse()
    {
    }
}

#endif
