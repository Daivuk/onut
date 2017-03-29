#ifndef WINDOWRPI_H_INCLUDED
#define WINDOWRPI_H_INCLUDED

// Onut
#include <onut/Window.h>

// Forward Declaration
#include <onut/ForwardDeclaration.h>
OForwardDeclare(WindowRPI)

namespace onut
{
    class WindowRPI final : public Window
    {
    public:
        WindowRPI();
        ~WindowRPI();

        void setCaption(const std::string& newName) override;
        bool pollEvents() override;

    private:
    };
}

#endif
