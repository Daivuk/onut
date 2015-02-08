#pragma once
#include <Windows.h>

namespace onut
{
    class Window
    {
    public:
        Window(const POINT& resolution, bool isResizable = false);
        HWND getHandle();
        void setCursor(HCURSOR cursor);

        std::function<void(char)>       onWrite;
        std::function<void(uintptr_t)>  onKey;
        std::function<void(UINT)>       onMenu;

    private:
        friend LRESULT CALLBACK WinProc(HWND handle, UINT msg, WPARAM wparam, LPARAM lparam);

        HWND    m_handle;
        HCURSOR m_cursor;
    };
}
