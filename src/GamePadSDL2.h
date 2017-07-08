#ifndef GAMEPADSDL2_H_INCLUDED
#define GAMEPADSDL2_H_INCLUDED

// onut
#include <onut/GamePad.h>

// Third party
#include <SDL2/SDL.h>

// STL
#include <vector>

// Forward
#include <onut/ForwardDeclaration.h>
OForwardDeclare(GamePadSDL2);

namespace onut
{
    class GamePadSDL2 final : public GamePad
    {
    public:
        GamePadSDL2(int index);
        ~GamePadSDL2();

        void update() override;
        bool isConnected() const override;
        bool isPressed(Button button) const override;
        bool isJustPressed(Button button) const override;
        bool isJustReleased(Button button) const override;

        void onAdded();
        void onRemoved();

        SDL_GameController* getSDLController() const;
        void setSDLController(SDL_GameController* pSDLController);
        void updateSDL2();

    private:
        bool isPressed(Button button, const std::vector<Uint8>& state) const;

        std::vector<Uint8> m_previousState;
        std::vector<Uint8> m_state;
        SDL_GameController* m_pSDLGamePad = nullptr;
        float m_lTrigger = 0;
        float m_rTrigger = 0;

        bool bSwaped = true;
    };
}

#endif
