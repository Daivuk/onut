#ifndef WINDOWSDL2_H_INCLUDED
#define WINDOWSDL2_H_INCLUDED

// Onut
#include <onut/Window.h>

// Third party
#include <SDL.h>

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
        void setClipboard(const std::string& text) override;
        std::string getClipboard() override;

        SDL_Window* getSDLWindow() const;
        
#if defined(WIN32)
        HWND getHandle() override;
#endif
    private:
        SDL_Window* m_pWindow = nullptr;
    };
}

#endif
