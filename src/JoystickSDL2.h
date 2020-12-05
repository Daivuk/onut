#ifndef JOYSTICKSDL2_H_INCLUDED
#define JOYSTICKSDL2_H_INCLUDED

// onut
#include <onut/Joystick.h>

// Third party
#include <SDL.h>

// STL
#include <vector>

// Forward
#include <onut/ForwardDeclaration.h>
OForwardDeclare(JoystickSDL2);

namespace onut
{
    class JoystickSDL2 final : public Joystick
    {
    public:
        JoystickSDL2(int index);
        ~JoystickSDL2();

        void update() override;
        void updateSDL2();

        bool isPressed(int button) const override;
        bool isJustPressed(int button) const override;
        bool isJustReleased(int button) const override;
        float getAxis(int id) const override;
        int getAxisCount() const override;
        int getButtonCount() const override;
        int getHatButtonBase() const override;
        int getAxisButtonBase() const override;
        std::string getButtonName(int button) const override;

    private:
        bool isPressed(int button, const Uint8* state) const;

        SDL_Joystick* m_pSDLJoystick;
        int m_buttonCount = 0;
        int m_axisCount = 0;
        int m_hatCount = 0;
        float* m_axisStates;
        Uint8* m_state;
        Uint8* m_previousState;

        bool bSwaped = true;
    };
}

#endif
