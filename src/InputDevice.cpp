// Onut
#include <onut/Input.h>
#include <onut/UIContext.h>
#include <onut/UITextBox.h>

// Private
#include "InputDevice.h"

OInputDeviceRef oInputDevice;

namespace onut
{
    InputDevice::InputDevice(OInput* pInput)
    {
        m_pInput = pInput;
        if (!m_pInput) m_pInput = oInput.get();
    }

    InputDevice::~InputDevice()
    {
    }

    void InputDevice::update()
    {
        // We only update the input device if the main UI system doesn't have a text box in focus
        auto& pFocusControl = oUIContext->getFocusControl();
        if (pFocusControl)
        {
            auto pFocusTextBox = ODynamicCast<OUITextBox>(pFocusControl);
            if (pFocusTextBox)
            {
                readMouse();
                return;
            }
        }
        readKeyboard();
        readMouse();
    }
}
