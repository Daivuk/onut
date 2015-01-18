#pragma once
#include <Windows.h>

namespace onut
{
    class Window
    {
    public:
        Window(const POINT& resolution, bool isResizable = false);
        HWND getHandle();

    private:
        HWND m_handle;
    };
}
