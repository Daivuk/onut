#ifndef WINDOWWIN32_H_INCLUDED
#define WINDOWWIN32_H_INCLUDED

// Onut
#include <onut/Window.h>

// Forward Declaration
#include <onut/ForwardDeclaration.h>
OForwardDeclare(WindowWIN32)

namespace onut
{
    class WindowWIN32 final : public Window
    {
    public:
        WindowWIN32();
        ~WindowWIN32();

        HWND getHandle();
        void setCaption(const std::string& newName) override;
        void setFullscreen(bool isFullscreen) override;
        bool pollEvents() override;

    private:
        HWND m_handle;
        bool m_isFullScreen = false;
        Point m_resSetting;
    };
}

#endif
