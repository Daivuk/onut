#ifndef INPUTDEVICE_H_INCLUDED
#define INPUTDEVICE_H_INCLUDED

// Forward
#include <onut/ForwardDeclaration.h>
OForwardDeclare(Input);
OForwardDeclare(InputDevice);

namespace onut
{
    class InputDevice
    {
    public:
        static OInputDeviceRef create(OInput* pInput = nullptr);
        ~InputDevice();

        void update();

    protected:
        InputDevice(OInput* pInput);
        virtual void readKeyboard() = 0;
        virtual void readMouse() = 0;

        OInput* m_pInput;
    };
}

extern OInputDeviceRef oInputDevice;

#endif
