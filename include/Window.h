#pragma once
#include <functional>
#include <Windows.h>
#include <string>

namespace onut
{
    class Window
    {
    public:
        Window(const POINT& resolution, bool isResizable = false);
        virtual ~Window();

        HWND getHandle();
        void setCursor(HCURSOR cursor);
        void setCaption(const std::string& newName);

        std::function<void(char)>                   onWrite;
        std::function<void(uintptr_t)>              onKey;
        std::function<void(UINT)>                   onMenu;
        std::function<void(const std::string&)>     onDrop;
        std::function<void(const POINT& newSize)>   onResize;

    private:
        friend LRESULT CALLBACK WinProc(HWND handle, UINT msg, WPARAM wparam, LPARAM lparam);

        HWND                                    m_handle;
        HCURSOR                                 m_cursor;
    };
}
