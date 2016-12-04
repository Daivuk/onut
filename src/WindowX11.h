#ifndef WINDOWX11_H_INCLUDED
#define WINDOWX11_H_INCLUDED

#if defined(__unix__)
// Onut
#include <onut/Window.h>

// Forward Declaration
#include <onut/ForwardDeclaration.h>
OForwardDeclare(WindowX11)

namespace onut
{
    class WindowX11 final : public Window
    {
    public:
        WindowX11();
        ~WindowX11();

        void setCaption(const std::string& newName) override;

    private:
    };
}

#endif

#endif
