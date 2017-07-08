#ifndef WINDOWSDL2_H_INCLUDED
#define WINDOWSDL2_H_INCLUDED

// Onut
#include <onut/Window.h>

// Third party
#include <SDL2/SDL.h>

// Forward Declaration
#include <onut/ForwardDeclaration.h>
OForwardDeclare(WindowSDL2)

namespace onut
{
    class WindowSDL2 final : public Window
    {
    public:
        WindowSDL2();
        ~WindowSDL2();

        void setCaption(const std::string& newName) override;
        bool pollEvents() override;

        SDL_Window* getSDLWindow() const;

    private:
        SDL_Window* m_pWindow = nullptr;
    };
}

#endif
