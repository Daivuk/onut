#ifndef WINDOWOSX_H_INCLUDED
#define WINDOWOSX_H_INCLUDED

// Onut
#include <onut/Window.h>

// Forward Declaration
#include <onut/ForwardDeclaration.h>
OForwardDeclare(WindowOSX)

namespace onut
{
    class WindowOSX final : public Window
    {
    public:
        WindowOSX();
        ~WindowOSX();

        void setCaption(const std::string& newName) override;

    private:
    };
}

#endif
