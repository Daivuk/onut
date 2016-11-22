#pragma once
#if defined(WIN32)
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
        void setCursor(HCURSOR cursor);
        HCURSOR getCursor() const;
        void setCaption(const std::string& newName);

    private:
        HWND m_handle;
        HCURSOR m_cursor;
    };
}

#endif
