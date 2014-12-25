#pragma once
#include <Windows.h>

namespace onut
{
    class Window
    {
    public:
        Window(const POINT& resolution);
        HWND getHandle();

    private:
        HWND m_handle;
    };
}
