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

    private:
        friend LRESULT CALLBACK WinProc(HWND handle, UINT msg, WPARAM wparam, LPARAM lparam);

        HWND    m_handle;
        HCURSOR m_cursor;
    };
}
